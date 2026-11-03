// __SZH_ADD_BEGIN__

#include <regex>

#include "SegmentSolver.h"

const int OC_VERBOSITY = 0;

using namespace Minisat;

#define RETURN_IF_TRUE(statement) {if(statement) return true;}

#define PROPAGATE_INTENSITY 1 //-1: nothing; 0: only rf; 1: and ws, fr

void SegmentSolver::save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_guard_mapt& _oc_guard_map, oc_location_mapt& _oc_location_map, std::map<std::string, int>& _oc_result_order)
{
    oc_edge_table = _oc_edge_table;
    oc_guard_map = _oc_guard_map;
    oc_location_map = _oc_location_map;
    oc_result_order = &_oc_result_order;

    set_graph();
}

int SegmentSolver::get_node(std::string name)
{
    for(int i = 0; i < int(nodes.size()); i++)
        if(nodes[i].name == name)
            return i;

    int new_node_id = nodes.size();

    std::string address = get_address(name);
    int address_id = -1;

    for(int i = 0; i < int(id_to_address.size()); i++)
    {
        if(id_to_address[i] == address)
        {
            address_id = i;
            break;
        }
    }

    if(address_id == -1)
    {
        address_id = id_to_address.size();
        id_to_address.push_back(address);
    }

    nodes.push_back(segment_nodet(name, address_id, new_node_id));

    if(OC_VERBOSITY >= 1)
        std::cout << "creating " << name << " " << address << "(" << address_id << ")\n";

    return new_node_id;
}

void SegmentSolver::set_graph()
{
    //init nodes
    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;
        get_node(pair.first.first);
        get_node(pair.first.second);
    }

    for(auto pair: oc_location_map)
    {
        auto write_name = pair.first;
        auto write_location = pair.second;
        int id = get_node(write_name);
        nodes[id].location = write_location;
    }

    // set chains
    std::vector<std::pair<int, int>> cross_edges = set_skeleton();
    set_atomic();

    // init from raw data
    init_races();
    init_reasonable_edges();
    init_guards();

    init_vital_edges();
    check_races_in_skeleton();

    // add cross edges
    for(auto cross_edge : cross_edges)
    {
        if(add_edge(cross_edge.first, cross_edge.second, -1))
        {
            std::cout << "WARNING: find conflict at the very start\n";
            ok = false;
        }
    }

    apply_literal_assignment();
}

int get_head(std::vector<int>& skeleton_in_num, std::set<int>& unsettled_nodes)
{
    for(auto node : unsettled_nodes)
        if(skeleton_in_num[node] == 0)
            return node;
    assert(false); // Unreachable
}

std::vector<std::pair<int, int>> SegmentSolver::set_skeleton()
{
    std::vector<std::vector<int>> skeleton_out(nodes.size());
    std::vector<int> skeleton_in_num(nodes.size(), 0);
    std::set<int> unsettled_nodes;

    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int node1 = get_node(pair.first.first);
        int node2 = get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind != OC_APO && kind != OC_PO)
            continue;

        if(OC_VERBOSITY >= 1)
            std::cout << "initing skeleton " << node1 << "(" << pair.first.first << ") " << node2 << "(" << pair.first.second << ") " << kind_to_str(kind) << "\n";

        skeleton_out[node1].push_back(node2);
        skeleton_in_num[node2]++;
        unsettled_nodes.insert(node1);
        unsettled_nodes.insert(node2);
    }

    std::vector<std::pair<int, int>> cross_edges;

    while(!unsettled_nodes.empty())
    {
        int head = get_head(skeleton_in_num, unsettled_nodes);

        int chain_id = chains.size();
        chains.push_back(std::vector<int>());
        chains_atomic_last.push_back(std::vector<int>());
        chains_atomic_first.push_back(std::vector<int>());
        auto& chain = chains.back();
        auto& chain_atomic_last = chains_atomic_last.back();
        auto& chain_atomic_first = chains_atomic_first.back();
        for(int curr = head; ; )//for(int curr = head; !skeleton_out[curr].empty(); curr = skeleton_out[curr][0])
        {
            int serial = chain.size();
            chain.push_back(curr);
            chain_atomic_last.push_back(serial);
            chain_atomic_first.push_back(serial);

            nodes[curr].chain = chain_id;
            nodes[curr].serial = serial;
            unsettled_nodes.erase(curr);

            int direct_succ = -1;
            for(int j = 0; j < int(skeleton_out[curr].size()); j++)
            {
                int succ = skeleton_out[curr][j];
                skeleton_in_num[succ]--;
                if(direct_succ == -1 && unsettled_nodes.find(succ) != unsettled_nodes.end())
                    direct_succ = succ;
                else
                    cross_edges.push_back(std::make_pair(curr, succ));
            }
            curr = direct_succ;
            if(curr == -1)
                break;
        }
    }

    // init tree
    trees.resize(chains.size());
    for(int chain1 = 0; chain1 < int(chains.size()); chain1++)
    {
        trees[chain1].resize(chains.size(), segment_treet(chains[chain1].size()));
    }

    if(OC_VERBOSITY >= 1)
    {
        std::cout << "num of chains: " << chains.size() << "\n";
        for(int i = 0; i < int(chains.size()); i++)
        {
            std::cout << "chain " << i << ":\n";
            for(auto node_id : chains[i])
                std::cout << "\t" << node_id << " " << nodes[node_id].name << "\n";
        }
        std::cout << "num of cross edges: " << cross_edges.size() << "\n";
        for(auto cross_edge : cross_edges)
            std::cout << "\t" << nodes[cross_edge.first].name << " " << nodes[cross_edge.second].name << "\n";
    }

    return cross_edges;
}

void SegmentSolver::set_atomic()
{
    std::vector<std::vector<std::pair<int, int>>> atomic_per_chain(chains.size());

    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        edge_kindt kind = str_to_kind(pair.second.second);
        if(kind != OC_APO)
            continue;

        int node1 = get_node(pair.first.first);
        int node2 = get_node(pair.first.second);
        auto node1_chain = nodes[node1].chain;
        auto node2_chain = nodes[node2].chain;
        auto node1_serial = nodes[node1].serial;
        auto node2_serial = nodes[node2].serial;
        if(node1_chain != node2_chain)
        {
            std::cout << "FATAL ERROR: apo " << nodes[node1].name << " " << nodes[node2].name << " cross chains\n";
            std::exit(1);
        }
        atomic_per_chain[node1_chain].push_back(std::make_pair(node1_serial, node2_serial));
    }

    for(int i = 0; i < int(atomic_per_chain.size()); i++)
    {
        auto& atomics = atomic_per_chain[i];
        std::sort(atomics.begin(), atomics.end());
        for(auto it = atomics.rbegin(); it != atomics.rend(); it++)
        {
            auto from = it->first;
            auto to = it->second;
            chains_atomic_last[i][from] = chains_atomic_last[i][to];
        }
    }

    for(int i = 0; i < int(atomic_per_chain.size()); i++)
    {
        auto& atomics = atomic_per_chain[i];
        std::sort(atomics.begin(), atomics.end());
        for(auto it = atomics.begin(); it != atomics.end(); it++)
        {
            auto from = it->first;
            auto to = it->second;
            chains_atomic_first[i][to] = chains_atomic_first[i][from];
        }
    }

    if(OC_VERBOSITY >= 1)
    {
        for(int i = 0; i < int(atomic_per_chain.size()); i++)
        {
            std::cout << "chain " << i << "'s atomic: ";
            for(auto& pair : atomic_per_chain[i])
                std::cout << "(" << pair.first << ", " << pair.second << ") ";
            std::cout << "\n\t";
            for(auto& serial : chains_atomic_last[i])
                std::cout << serial << " ";
            std::cout << "\n\t";
            for(auto& serial : chains_atomic_first[i])
                std::cout << serial << " ";
            std::cout << "\n";
        }
    }
}

void SegmentSolver::init_races()
{
    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        edge_kindt kind = str_to_kind(pair.second.second);
        if(kind != OC_RACE)
            continue;

        int u = get_node(pair.first.first);
        int v = get_node(pair.first.second);
        if(u > v)
            std::swap(u, v); //make u < v

        Lit& l = pair.second.first;
        pair_to_inactive_races[std::make_pair(u, v)] = l;

        if(OC_VERBOSITY >= 1)
            std::cout << "initing race " << u << " " << v << " with literal " << var(l) << " (" << sign(l) << ")\n";
    }

}

void SegmentSolver::init_guards()
{
    for(auto pair: oc_guard_map)
    {
        auto write_name = pair.first;
        int w = get_node(write_name);
        auto& guard_lit = pair.second;

        nodes[w].is_write = true;
        nodes[w].guard_lit = guard_lit;

        if(guard_lit.x == -1) // directly add this!
        {
            if(enable_guard(w))
            {
                std::cout << "WARNING: inconsistency during set graph\n";
                ok = false;
            }
        }
        else
            guard_lit_to_node.insert(std::make_pair(guard_lit, w));
    }
}

void SegmentSolver::init_reasonable_edges()
{
    for(auto pair: oc_edge_table) // others last
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int u = get_node(pair.first.first);
        int v = get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind != OC_RF)
            continue;

        if(OC_VERBOSITY >= 1)
            std::cout << "initing " << u << "(" << pair.first.first << ") " << v << "(" << pair.first.second << ") " << kind_to_str(kind) << "\n";

        Lit& l = pair.second.first;
        lit_to_edge[l] = std::make_pair(std::make_pair(u, v), kind);
        inactive_edge_t inactive_edge = std::make_pair(std::make_pair(u, v), l);

        if(int(tail_to_inactive_edges.size()) <= u)
            tail_to_inactive_edges.resize(u + 1, std::vector<inactive_edge_t>());
        tail_to_inactive_edges[u].push_back(inactive_edge);

        if(int(head_to_inactive_edges.size()) <= v)
            head_to_inactive_edges.resize(v + 1, std::vector<inactive_edge_t>());
        head_to_inactive_edges[v].push_back(inactive_edge);

        if(int(tail_head_to_inactive_lit.size()) <= u)
            tail_head_to_inactive_lit.resize(u + 1, std::vector<Lit>());
        if(int(tail_head_to_inactive_lit[u].size()) <= v)
            tail_head_to_inactive_lit[u].resize(v + 1, lit_Error);
        tail_head_to_inactive_lit[u][v] = l;

        if(kind == OC_RF)
        {
            nodes[u].is_write = true;
            nodes[v].is_read = true;
        }
    }
}

void SegmentSolver::init_vital_edges()
{
    // init vital edges
    for(int c = 0; c < int(chains.size()); c++)
        for(int s1 = 0; s1 < int(chains[c].size()); s1++)
            for(int s2 = s1 + 1; s2 < int(chains[c].size()); s2++)
            {
                node_idt node1 = chains[c][s1];
                node_idt node2 = chains[c][s2];
                if(nodes[node1].address != nodes[node2].address)
                    continue;
                if(nodes[node1].is_write && nodes[node2].is_write)
                    add_w_w_vital_edge(node1, node2, -1);
                if(nodes[node1].is_write && nodes[node2].is_read)
                    add_w_r_vital_edge(node1, node2, -1);
            }
}

void SegmentSolver::check_races_in_skeleton()
{
    for(int c = 0; c < int(chains.size()); c++)
        for(int s1 = 0; s1 < int(chains[c].size()); s1++)
            for(int s2 = s1 + 1; s2 < int(chains[c].size()); s2++)
            {
                node_idt node1 = chains[c][s1];
                node_idt node2 = chains[c][s2];
                if(nodes[node1].address != nodes[node2].address)
                    continue;
                if((nodes[node1].is_write && nodes[node2].is_read) ||
                    (nodes[node1].is_write && nodes[node2].is_write) ||
                    (nodes[node1].is_read && nodes[node2].is_write))
                {
                    if(check_pair(node1, node2, -1, -1, -1))
                    {
                        std::cout << "WARNING: find races in skeleton\n";
                        ok = false;
                    }
                }
            }
}

std::vector<digit_reasont> SegmentSolver::get_first_successors(node_idt node)
{
    std::vector<digit_reasont> first_successor_per_chain(chains.size(), digit_reasont());

    int chain = nodes[node].chain;
    int serial = nodes[node].serial;
    int first_atomic_serial = get_atomic_first(chain, serial);
    int first_atomic_node = chains[chain][first_atomic_serial];

    std::vector<digit_reasont> tovisit { digit_reasont(first_atomic_node, -1) };
    while(!tovisit.empty())
    {
        digit_reasont visiting = tovisit.back();
        tovisit.pop_back();

        int current_node = visiting.digit;
        int reason_id = visiting.reason_id;
        int current_chain = nodes[current_node].chain;
        int current_serial = nodes[current_node].serial;

        for(int next_chain = 0; next_chain < int(chains.size()); next_chain++)
        {
            if(next_chain == current_chain)
                continue;
            auto next_min = trees[current_chain][next_chain].get_min(current_serial);
            auto next_serial = next_min.digit;
            auto next_atomic_serial = get_atomic_first(next_chain, next_serial);
            auto next_reason_id = next_min.reason_id;
            if(next_atomic_serial >= first_successor_per_chain[next_chain].digit)
                continue;

            node_idt next_node = chains[next_chain][next_atomic_serial];
            pooled_reasont new_reason({}, {reason_id, next_reason_id});
            int new_reason_id = register_reason(new_reason);
            first_successor_per_chain[next_chain] = digit_reasont(next_atomic_serial, new_reason_id);
            tovisit.push_back(digit_reasont(next_node, new_reason_id));
        }
    }
    return first_successor_per_chain;
}

std::vector<digit_reasont> SegmentSolver::get_last_predecessors(node_idt node)
{
    std::vector<digit_reasont> last_predecessors_per_chain(chains.size(), digit_reasont(-1, -1));

    int chain = nodes[node].chain;
    int serial = nodes[node].serial;
    int last_atomic_serial = get_atomic_last(chain, serial);
    int last_atomic_node = chains[chain][last_atomic_serial];

    std::vector<digit_reasont> tovisit { digit_reasont(last_atomic_node, -1) };
    while(!tovisit.empty())
    {
        digit_reasont visiting = tovisit.back();
        tovisit.pop_back();

        int current_node = visiting.digit;
        int reason_id = visiting.reason_id;
        int current_chain = nodes[current_node].chain;
        int current_serial = nodes[current_node].serial;

        for(int last_chain = 0; last_chain < int(chains.size()); last_chain++)
        {
            if(last_chain == current_chain)
                continue;
            auto last_max = trees[last_chain][current_chain].get_argleq(current_serial);
            auto last_serial = last_max.digit;
            auto last_atomic_serial = get_atomic_last(last_chain, last_serial);
            auto last_reason_id = last_max.reason_id;
            if(last_atomic_serial <= last_predecessors_per_chain[last_chain].digit)
                continue;

            node_idt last_node = chains[last_chain][last_atomic_serial];
            pooled_reasont new_reason({}, {reason_id, last_reason_id});
            int new_reason_id = register_reason(new_reason);
            last_predecessors_per_chain[last_chain] = digit_reasont(last_atomic_serial, new_reason_id);
            tovisit.push_back(digit_reasont(last_node, new_reason_id));
        }
    }
    return last_predecessors_per_chain;
}

void SegmentSolver::extract_writes_reads_per_loc(node_idt node, std::vector<digit_reasont>& serials, bool is_successor, std::vector<std::vector<digit_reasont>>& writes_per_loc, std::vector<std::vector<digit_reasont>>& reads_per_loc, bool include_self)
{
    writes_per_loc.resize(id_to_address.size());
    reads_per_loc.resize(id_to_address.size());
    for(int chain = 0; chain < int(chains.size()); chain++)
    {
        if(chain == nodes[node].chain) // We specifically handle intra-threaded successors elsewhere
            continue;
        int serial = serials[chain].digit;
        int reason_id = serials[chain].reason_id;
        if(is_successor)
            for(int s = serial; s < int(chains[chain].size()); s++)
            {
                node_idt n = chains[chain][s];
                if(nodes[n].is_write)
                    writes_per_loc[nodes[n].address].push_back(digit_reasont(n, reason_id));
                if(nodes[n].is_read)
                    reads_per_loc[nodes[n].address].push_back(digit_reasont(n, reason_id));
            }
        else
            for(int s = 0; s <= serial; s++)
            {
                node_idt n = chains[chain][s];
                if(nodes[n].is_write)
                    writes_per_loc[nodes[n].address].push_back(digit_reasont(n, reason_id));
                if(nodes[n].is_read)
                    reads_per_loc[nodes[n].address].push_back(digit_reasont(n, reason_id));
            }
    }
    int self_chain = nodes[node].chain;
    int self_serial = nodes[node].serial;
    int serial_begin, serial_end;
    if(is_successor)
    {
        serial_begin = include_self ? get_atomic_first(self_chain, self_serial) : self_serial + 1;
        serial_end = int(chains[self_chain].size());
    }
    else
    {
        serial_begin = 0;
        serial_end = include_self ? get_atomic_last(self_chain, self_serial) + 1 : self_serial;
    }
    for(int s = serial_begin; s < serial_end; s++)
    {
        node_idt n = chains[self_chain][s];
        if(nodes[n].is_write)
            writes_per_loc[nodes[n].address].push_back(digit_reasont(n, -1));
        if(nodes[n].is_read)
            reads_per_loc[nodes[n].address].push_back(digit_reasont(n, -1));
    }
}

void SegmentSolver::get_successors(node_idt node, std::vector<std::vector<digit_reasont>>& writes_per_loc, std::vector<std::vector<digit_reasont>>& reads_per_loc, bool include_self)
{
    auto first_successors = get_first_successors(node);
    extract_writes_reads_per_loc(node, first_successors, true, writes_per_loc, reads_per_loc, include_self);
}

void SegmentSolver::get_predecessors(node_idt node, std::vector<std::vector<digit_reasont>>& writes_per_loc, std::vector<std::vector<digit_reasont>>& reads_per_loc, bool include_self)
{
    auto last_predecessors = get_last_predecessors(node);
    extract_writes_reads_per_loc(node, last_predecessors, false, writes_per_loc, reads_per_loc, include_self);
}

bool SegmentSolver::need_add_edge(node_idt node1, node_idt node2)
{
    int node1_chain = nodes[node1].chain;
    int node1_serial = nodes[node1].serial;
    int node2_chain = nodes[node2].chain;
    int node2_serial = nodes[node2].serial;
    if(node1_chain == node2_chain && node1_serial < node2_serial)
        return false;
    if(node1_chain != node2_chain && trees[node1_chain][node2_chain].get_min(node1_serial).digit <= node2_serial)
        return false;
    return true;
}

bool SegmentSolver::check_pair(node_idt node1, node_idt node2, int reason_id1, int reason_id2, int reason_id3)
{
    if(node1 > node2)
        std::swap(node1, node2);

    auto it = pair_to_inactive_races.find(std::make_pair(node1, node2));
    if(it == pair_to_inactive_races.end())
        return false;
    auto race_lit  = it->second;
    auto race_lit_status = get_assignment(race_lit);
    if(race_lit_status == l_False)
        return false;

    pooled_reasont final_reason({race_lit}, {reason_id1, reason_id2, reason_id3});

    if(race_lit_status == l_True)
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "race" << node1 << " " << node2 << " should be set false but already true\n";

        conflict_clause = final_reason.get_full_reason(reason_pool);
        return true;
    }
    else // race_lit_status == l_Undef
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "set race" << node1 << " " << node2 << " false\n";

        assign_literal(~race_lit, register_reason(final_reason));
        return false;
    }
}

#define output_lv(lv) {for (auto l: (lv)) std::cout << var(l) << "(" << sign(l) << ") ";}

bool SegmentSolver::add_edge(node_idt node1, node_idt node2, int reason_id)
{
    int node1_chain = nodes[node1].chain;
    int node1_serial = nodes[node1].serial;
    int node2_chain = nodes[node2].chain;
    int node2_serial = nodes[node2].serial;

    if(OC_VERBOSITY >= 1) {
        std::cout << "adding edge " << node1 << "(" << node1_chain << ", " << node1_serial << ") " << node2 << "(" << node2_chain << ", " << node2_serial << ")\n";
        if(reason_id != -1)
        {
            std::cout << "\t reason: ";
            output_lv(reason_pool[reason_id].get_full_reason(reason_pool));
            std::cout << "\n";
        }
        else
            std::cout << "\t no reason\n";
    }
    if(node1_chain == node2_chain)
    {
        if(node1_serial < node2_serial) // Consistent to po, no problem
            return false;
        else
        {
            if(reason_id == -1)
                conflict_clause.clear();
            else
                conflict_clause = reason_pool[reason_id].get_full_reason(reason_pool);
            return true;
        }
    }

    // edge already implied
    if(trees[node1_chain][node2_chain].get_min(node1_serial).digit <= node2_serial)
        return false;

    // consistency checking
    std::vector<digit_reasont> node2_succ_first = get_first_successors(node2);
    if(node2_succ_first[node1_chain].digit <= chains_atomic_last[node1_chain][node1_serial])
    {
        pooled_reasont new_reason({}, {reason_id, node2_succ_first[node1_chain].reason_id});
        conflict_clause = new_reason.get_full_reason(reason_pool);
        return true;
    }

    // add edge
    auto previous_serial_reason = trees[node1_chain][node2_chain].update(node1_serial, digit_reasont(node2_serial, reason_id));
    trail_updatet update(node1_chain, node2_chain, node1_serial, previous_serial_reason);
    trail_update.push_back(update);

    // propagation
    std::vector<std::vector<digit_reasont>> node1_pred_writes_per_loc, node1_pred_reads_per_loc, node2_succ_writes_per_loc, node2_succ_reads_per_loc;
    get_predecessors(node1, node1_pred_writes_per_loc, node1_pred_reads_per_loc, true);
    extract_writes_reads_per_loc(node2, node2_succ_first, true, node2_succ_writes_per_loc, node2_succ_reads_per_loc, true);

    for(int addr = 0; addr < int(id_to_address.size()); addr++)
    {
        auto& node1_pred_writes = node1_pred_writes_per_loc[addr];
        auto& node1_pred_reads = node1_pred_reads_per_loc[addr];
        auto& node2_succ_writes = node2_succ_writes_per_loc[addr];
        auto& node2_succ_reads = node2_succ_reads_per_loc[addr];

        // fr derivation
        for(auto& node1_pred_write_pair : node1_pred_writes)
        {
            auto node1_pred_write = node1_pred_write_pair.digit;
            auto pred_reason_id = node1_pred_write_pair.reason_id;
            for(auto& node1_pred_write_out_rf : nodes[node1_pred_write].out_rf)
            {
                node_idt r = node1_pred_write_out_rf.to;
                auto& rf_lit = node1_pred_write_out_rf.lit;
                
                for(auto& node2_succ_write_pair : node2_succ_writes)
                {
                    auto node2_succ_write = node2_succ_write_pair.digit;
                    if(!nodes[node2_succ_write].guard_enabled)
                        continue;
                    if(!need_add_edge(r, node2_succ_write))
                        continue;
                    auto succ_reason_id = node2_succ_write_pair.reason_id;
                    pooled_reasont new_reason({rf_lit, nodes[node2_succ_write].guard_lit}, {pred_reason_id, succ_reason_id, reason_id});
                    
                    if(OC_VERBOSITY >= 1)
                        std::cout << "FR derivation: " << node1_pred_write << " rf " << r << " and " << node1_pred_write << " hb " << node2_succ_write << " and guard " << node2_succ_write << "\n";

                    RETURN_IF_TRUE(add_edge(r, node2_succ_write, register_reason(new_reason)))
                }
            }
        }

        // ws derivation
        for(auto& node2_succ_read_pair : node2_succ_reads)
        {
            auto node2_succ_read = node2_succ_read_pair.digit;
            auto succ_reason_id = node2_succ_read_pair.reason_id;
            for(auto& node2_succ_read_in_rf : nodes[node2_succ_read].in_rf)
            {
                node_idt w = node2_succ_read_in_rf.from;
                auto& rf_lit = node2_succ_read_in_rf.lit;
                
                for(auto& node1_pred_write_pair : node1_pred_writes)
                {
                    auto node1_pred_write = node1_pred_write_pair.digit;
                    if(node1_pred_write == w || !nodes[node1_pred_write].guard_enabled)
                        continue;
                    if(!need_add_edge(node1_pred_write, w))
                        continue;
                    auto& pred_reason_id = node1_pred_write_pair.reason_id;
                    pooled_reasont new_reason({rf_lit, nodes[node1_pred_write].guard_lit}, {pred_reason_id, succ_reason_id, reason_id});

                    if(OC_VERBOSITY >= 1)
                        std::cout << "WS derivation: " << w << " rf " << node2_succ_read << " and " << node1_pred_write << " hb " << node2_succ_read << " and guard " << node1_pred_write << "\n";

                    RETURN_IF_TRUE(add_edge(node1_pred_write, w, register_reason(new_reason)))
                }
            }
        }

        // race propagation: r-w
        for(auto& node1_pred_read_pair : node1_pred_reads)
            for(auto& node2_succ_write_pair : node2_succ_writes)
            {
                node_idt from = node1_pred_read_pair.digit;
                node_idt to = node2_succ_write_pair.digit;
                int reason_id1 = node1_pred_read_pair.reason_id;
                int reason_id2 = node2_succ_write_pair.reason_id;
                RETURN_IF_TRUE(check_pair(from, to, reason_id1, reason_id2, reason_id))
            }
        // race propagation: w-w
        for(auto& node1_pred_write_pair : node1_pred_writes)
            for(auto& node2_succ_write_pair : node2_succ_writes)
            {
                node_idt from = node1_pred_write_pair.digit;
                node_idt to = node2_succ_write_pair.digit;
                int reason_id1 = node1_pred_write_pair.reason_id;
                int reason_id2 = node2_succ_write_pair.reason_id;
                RETURN_IF_TRUE(check_pair(from, to, reason_id1, reason_id2, reason_id))
            }
        // race propagation: w-r
        for(auto& node1_pred_write_pair : node1_pred_writes)
            for(auto& node2_succ_read_pair : node2_succ_reads)
            {
                node_idt from = node1_pred_write_pair.digit;
                node_idt to = node2_succ_read_pair.digit;
                int reason_id1 = node1_pred_write_pair.reason_id;
                int reason_id2 = node2_succ_read_pair.reason_id;
                RETURN_IF_TRUE(check_pair(from, to, reason_id1, reason_id2, reason_id))
            }
    }

#if (PROPAGATE_INTENSITY >= 0)

    // for r-w vital edges, only negate rf
    for(int addr = 0; addr < int(id_to_address.size()); addr++)
    {
        auto& node1_pred_reads = node1_pred_reads_per_loc[addr];
        auto& node2_succ_writes = node2_succ_writes_per_loc[addr];
        for(auto node1_pred_r_pair : node1_pred_reads)
            for(auto node2_succ_w_pair : node2_succ_writes)
            {
                node_idt r = node1_pred_r_pair.digit;
                node_idt w = node2_succ_w_pair.digit;
                int reason1 = node1_pred_r_pair.reason_id;
                int reason2 = node2_succ_w_pair.reason_id;
                auto rf_lit = check_tail_head_to_inactive_lit(w, r);
                if(rf_lit == lit_Error || get_assignment(rf_lit) != l_Undef)
                    continue;

                if(OC_VERBOSITY >= 1)
                    std::cout << "Preventive: set rf " << w << " " << r << " false\n";

                pooled_reasont new_reason({rf_lit}, {reason1, reason2, reason_id});
                assign_literal(~rf_lit, register_reason(new_reason));
            }
    }

#endif // (PROPAGATE_INTENSITY >= 0)

#if (PROPAGATE_INTENSITY >= 1)

    // for w-w vital edges
    for(int addr = 0; addr < int(id_to_address.size()); addr++)
    {
        auto& node1_pred_writes = node1_pred_writes_per_loc[addr];
        auto& node2_succ_writes = node2_succ_writes_per_loc[addr];
        for(auto node1_pred_w_pair : node1_pred_writes)
            for(auto node2_succ_w_pair : node2_succ_writes)
            {
                node_idt w1 = node1_pred_w_pair.digit;
                node_idt w2 = node2_succ_w_pair.digit;

                int reason1 = node1_pred_w_pair.reason_id;
                int reason2 = node2_succ_w_pair.reason_id;
                pooled_reasont new_reason({}, {reason1, reason2, reason_id});
                add_w_w_vital_edge(w1, w2, register_reason(new_reason));
            }
    }

    // for w-r vital edges
    for(int addr = 0; addr < int(id_to_address.size()); addr++)
    {
        auto& node1_pred_writes = node1_pred_writes_per_loc[addr];
        auto& node2_succ_reads = node2_succ_reads_per_loc[addr];
        for(auto node1_pred_w_pair : node1_pred_writes)
            for(auto node2_succ_r_pair : node2_succ_reads)
            {
                node_idt w2 = node1_pred_w_pair.digit;
                node_idt r = node2_succ_r_pair.digit;

                int reason1 = node1_pred_w_pair.reason_id;
                int reason2 = node2_succ_r_pair.reason_id;
                pooled_reasont new_reason({}, {reason1, reason2, reason_id});
                add_w_r_vital_edge(w2, r, register_reason(new_reason));
            }
    }

#endif // (PROPAGATE_INTENSITY >= 1)

    return false;
}

void SegmentSolver::add_w_w_vital_edge(node_idt w1, node_idt w2, int reason_id)
{
    if(nodes[w1].has_out_vital.find(w2) != nodes[w1].has_out_vital.end())
        return;

    if(OC_VERBOSITY >= 1)
        std::cout << "new w-w vital edge " << w1 << " " << w2 << "\n";

    nodes[w1].out_vital.push_back(digit_reasont(w2, reason_id));
    nodes[w2].in_vital.push_back(digit_reasont(w1, reason_id));
    nodes[w1].has_out_vital.insert(w2);
    trail_vital.push_back(std::make_pair(w1, w2));

    for(auto r_pair : nodes[w2].out_vital)
    {
        node_idt r = r_pair.digit;
        int r_reason_id = r_pair.reason_id;
        check_trinary(w1, w2, r, reason_id, r_reason_id);
    }
}

void SegmentSolver::add_w_r_vital_edge(node_idt w2, node_idt r, int reason_id)
{
    if(nodes[w2].has_out_vital.find(r) != nodes[w2].has_out_vital.end())
        return;

    if(OC_VERBOSITY >= 1)
        std::cout << "new w-r vital edge " << w2 << " " << r << "\n";

    nodes[w2].out_vital.push_back(digit_reasont(r, reason_id));
    nodes[r].in_vital.push_back(digit_reasont(w2, reason_id));
    nodes[w2].has_out_vital.insert(r);
    trail_vital.push_back(std::make_pair(w2, r));

    for(auto w1_pair : nodes[w2].in_vital)
    {
        node_idt w1 = w1_pair.digit;
        int w1_reason_id = w1_pair.reason_id;
        check_trinary(w1, w2, r, reason_id, w1_reason_id);
    }
}

void SegmentSolver::check_trinary(node_idt w1, node_idt w2, node_idt r, int reason_id1, int reason_id2)
{
    auto rf_lit = check_tail_head_to_inactive_lit(w1, r);
    if(rf_lit == lit_Error)
        return;

    auto guard_lit = nodes[w2].guard_lit;

    auto rf_hold = get_assignment(rf_lit);
    auto guard_hold = get_assignment(guard_lit);

    if(rf_hold == l_True && guard_hold == l_Undef)
    {
        pooled_reasont final_reason({guard_lit, rf_lit}, {reason_id1, reason_id2});
        assign_literal(~guard_lit, register_reason(final_reason));

        if(OC_VERBOSITY >= 1)
            std::cout << "In pattern " << w1 << " " << w2 << " " << r << ", propagate guard of " << w2 << " false\n";
    }
    if(rf_hold == l_Undef && guard_hold == l_True)
    {
        pooled_reasont final_reason({rf_lit, guard_lit}, {reason_id1, reason_id2});
        assign_literal(~rf_lit, register_reason(final_reason));

        if(OC_VERBOSITY >= 1)
            std::cout << "In pattern " << w1 << " " << w2 << " " << r << ", propagate rf " << w1 << " " << r << " false\n";
    }
    if(rf_hold == l_Undef && guard_hold == l_Undef)
    {
        // add "binary clause"
        triplett triplet(w1, w2, r, rf_lit, guard_lit, reason_id1, reason_id2);
        rf_to_triplets[rf_lit].push_back(triplet);
        guard_to_triplets[guard_lit].push_back(triplet);

        trail_triplet.push_back(std::make_pair(rf_lit, guard_lit));
    }
}

void SegmentSolver::assign_literal(Lit l, int reason_id)
{
    if(assigned_literals.find(l) != assigned_literals.end())
        return;

    literals_to_assign.push_back(std::make_pair(l, reason_id));
    assigned_literals.insert(l);
}

void SegmentSolver::apply_literal_assignment()
{
    for(auto p : literals_to_assign)
    {
        if(OC_VERBOSITY >= 1)
        {
            std::cout << var(p.first) << "(" << sign(p.first) << ") is now assigned\n";
            std::cout << "its value was " << toInt(value(p.first)) << "\n";
        }

        int reason_id = p.second;
        auto full_reason = reason_pool[reason_id].get_full_reason(reason_pool);

        vec<Lit> learnt_clause;
        for(auto lit : full_reason)
            learnt_clause.push(~lit);

        if (learnt_clause.size() == 1)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << learnt_clause[0].x << " is really assigned\n";
            uncheckedEnqueue(learnt_clause[0]);
            theory_propagation++;
        }
        else
        {
            CRef cr = ca.alloc(learnt_clause, true);
            learnts.push(cr);
            attachClause(cr);
            claBumpActivity(ca[cr]);
            if(value(learnt_clause[0]) == l_Undef)
            {
                if(OC_VERBOSITY >= 1)
                    std::cout << learnt_clause[0].x << " is really assigned\n";

                uncheckedEnqueue(learnt_clause[0], cr);
                theory_propagation++;
            }
        }

    }

    literals_to_assign.clear();
    assigned_literals.clear();
}

bool SegmentSolver::add_rf(node_idt w, node_idt r, Lit lit)
{
    if(OC_VERBOSITY >= 1)
    {
        std::cout << "adding rf " << w << " " << r << "\n";
        std::cout << "\t lit: " << var(lit) << "(" << sign(lit) << ")\n";
    }

    segment_edget rf_edge(w, r, OC_RF, lit);
    nodes[w].out_rf.push_back(rf_edge);
    nodes[r].in_rf.push_back(rf_edge);
    trail_rf.push_back(std::make_pair(w, r));

    // Revisiting triplets
    for(auto& triplet : rf_to_triplets[lit])
    {
        if(get_assignment(triplet.guard_lit) == l_Undef)
        {
            Lit guard_lit = triplet.guard_lit;
            int reason_id1 = triplet.reason_id1;
            int reason_id2 = triplet.reason_id2;
            pooled_reasont final_reason({guard_lit, lit}, {reason_id1, reason_id2});
            assign_literal(~guard_lit, register_reason(final_reason));

            if(OC_VERBOSITY >= 1)
            {
                auto& w1 = triplet.w1;
                auto& w2 = triplet.w2;
                auto& r = triplet.r;
                std::cout << "revisiting pattern " << w1 << " " << w2 << " " << r << ", propagate guard of " << w2 << " false\n";
            }
        }
    }

    // WS derivation
    std::vector<std::vector<digit_reasont>> r_pred_writes_per_loc, r_pred_reads_per_loc;
    get_predecessors(r, r_pred_writes_per_loc, r_pred_reads_per_loc, false);
    auto& r_pred_writes = r_pred_writes_per_loc[nodes[r].address];
    for(auto& another_w_pair : r_pred_writes)
    {
        auto another_w = another_w_pair.digit;
        if(another_w == w || !nodes[another_w].guard_enabled)
            continue;
        if(!need_add_edge(another_w, w))
            continue;
        auto another_w_reason_id = another_w_pair.reason_id;
        pooled_reasont new_reason({lit, nodes[another_w].guard_lit}, {another_w_reason_id});

        if(OC_VERBOSITY >= 1)
            std::cout << "WS derivation: " << w << " rf " << r << " and " << another_w << " hb " << r << " and guard " << another_w << "\n";

        RETURN_IF_TRUE(add_edge(another_w, w, register_reason(new_reason)))
    }

    // FR derivation
    std::vector<std::vector<digit_reasont>> w_succ_writes_per_loc, w_succ_reads_per_loc;
    get_successors(w, w_succ_writes_per_loc, w_succ_reads_per_loc, false);
    auto& w_succ_writes = w_succ_writes_per_loc[nodes[w].address];
    for(auto& another_w_pair : w_succ_writes)
    {
        auto& another_w = another_w_pair.digit;
        if(!nodes[another_w].guard_enabled)
            continue;
        if(!need_add_edge(r, another_w))
            continue;
        auto& another_w_reason_id = another_w_pair.reason_id;
        pooled_reasont new_reason({lit, nodes[another_w].guard_lit}, {another_w_reason_id});

        if(OC_VERBOSITY >= 1)
            std::cout << "FR derivation: " << w << " rf " << r << " and " << w << " hb " << another_w << " and guard " << another_w << "\n";

        RETURN_IF_TRUE(add_edge(r, another_w, register_reason(new_reason)))
    }

    return false;
}

bool SegmentSolver::enable_guard(node_idt w)
{
    if(nodes[w].guard_enabled) //this circumstance exist?
    {
        //std::cout << "WARNING: " << w << "'s guard is enabled twice\n";
        return false;
    }

    auto lit = nodes[w].guard_lit;
    nodes[w].guard_enabled = true;
    trail_guard.push_back(w);

    if(OC_VERBOSITY >= 1)
        std::cout << "guard of " << w << " is enabled\n";

    // Revisiting triplets
    for(auto& triplet : guard_to_triplets[lit])
    {
        if(get_assignment(triplet.rf_lit) == l_Undef)
        {
            Lit rf_lit = triplet.rf_lit;
            int reason_id1 = triplet.reason_id1;
            int reason_id2 = triplet.reason_id2;
            pooled_reasont final_reason({rf_lit, lit}, {reason_id1, reason_id2});
            assign_literal(~rf_lit, register_reason(final_reason));

            if(OC_VERBOSITY >= 1)
            {
                auto& w1 = triplet.w1;
                auto& w2 = triplet.w2;
                auto& r = triplet.r;
                std::cout << "revisiting pattern " << w1 << " " << w2 << " " << r << ", propagate rf " << w1 << " " << r << " false\n";
            }
        }
    }

    // WS derivation
    std::vector<std::vector<digit_reasont>> w_succ_writes_per_loc, w_succ_reads_per_loc;
    get_successors(w, w_succ_writes_per_loc, w_succ_reads_per_loc, false);
    auto& w_succ_reads = w_succ_reads_per_loc[nodes[w].address];
    for(auto& r_pair : w_succ_reads)
    {
        auto& r = r_pair.digit;
        auto r_reason_id = r_pair.reason_id;
        for(auto& r_in_rf : nodes[r].in_rf)
        {
            auto another_w = r_in_rf.from;
            if(w == another_w)
                continue;
            if(!need_add_edge(w, another_w))
                continue;
            auto rf_lit = r_in_rf.lit;
            pooled_reasont new_reason({nodes[w].guard_lit, rf_lit}, {r_reason_id});
            RETURN_IF_TRUE(add_edge(w, another_w, register_reason(new_reason)))
        }
    }

    // FR derivation
    std::vector<std::vector<digit_reasont>> w_pred_writes_per_loc, w_pred_reads_per_loc;
    get_predecessors(w, w_pred_writes_per_loc, w_pred_reads_per_loc, false);
    auto& w_pred_writes = w_pred_writes_per_loc[nodes[w].address];
    for(auto& another_w_pair : w_pred_writes)
    {
        auto& another_w = another_w_pair.digit;
        auto another_w_reason_id = another_w_pair.reason_id;
        for(auto& another_w_out_rf : nodes[another_w].out_rf)
        {
            auto r = another_w_out_rf.to;
            if(!need_add_edge(r, w))
                continue;
            auto rf_lit = another_w_out_rf.lit;
            pooled_reasont new_reason({nodes[w].guard_lit, rf_lit}, {another_w_reason_id});
            RETURN_IF_TRUE(add_edge(r, w, register_reason(new_reason)))
        }
    }

    return false;
}

int SegmentSolver::register_reason(pooled_reasont reason)
{
    if(reason.trivial())
        return -1;

    int reason_id = reason_pool.size();
    reason_pool.push_back(reason);
    return reason_id;
}

void SegmentSolver::push_scope()
{
    trail_update_lim.push_back(trail_update.size());
    trail_rf_lim.push_back(trail_rf.size());
    trail_guard_lim.push_back(trail_guard.size());
    trail_vital_lim.push_back(trail_vital.size());
    trail_triplet_lim.push_back(trail_triplet.size());
    reason_pool_lim.push_back(reason_pool.size());
}

void SegmentSolver::pop_scope(int new_level)
{
    for (int c = trail_rf.size() - 1; c >= trail_rf_lim[new_level]; c--)
    {
        auto e = trail_rf[c];
        nodes[e.first].out_rf.pop_back();
        nodes[e.second].in_rf.pop_back();
    }
    trail_rf.resize(trail_rf_lim[new_level]);
    trail_rf_lim.resize(new_level);

    for(int c = trail_guard.size() - 1; c >= trail_guard_lim[new_level]; c--)
    {
        auto n = trail_guard[c];
        nodes[n].guard_enabled = false;
    }
    trail_guard.resize(trail_guard_lim[new_level]);
    trail_guard_lim.resize(new_level);

    for(int c = trail_update.size() - 1; c >= trail_update_lim[new_level]; c--)
    {
        auto& update = trail_update[c];
        trees[update.chain1][update.chain2].update(update.serial1, update.old_entry);
    }
    trail_update.resize(trail_update_lim[new_level]);
    trail_update_lim.resize(new_level);

    for (int c = trail_vital.size() - 1; c >= trail_vital_lim[new_level]; c--)
    {
        auto e = trail_vital[c];
        nodes[e.first].out_vital.pop_back();
        nodes[e.second].in_vital.pop_back();
        nodes[e.first].has_out_vital.erase(e.second);
    }
    trail_vital.resize(trail_vital_lim[new_level]);
    trail_vital_lim.resize(new_level);

    for (int c = trail_triplet.size() - 1; c >= trail_triplet_lim[new_level]; c--)
    {
        auto lits = trail_triplet[c];
        rf_to_triplets[lits.first].pop_back();
        guard_to_triplets[lits.second].pop_back();
    }
    trail_triplet.resize(trail_triplet_lim[new_level]);
    trail_triplet_lim.resize(new_level);

    reason_pool.resize(reason_pool_lim[new_level]);
    reason_pool_lim.resize(new_level);
}

lbool SegmentSolver::get_assignment(Lit lit)
{
    if(lit.x == -1)
        return l_True;
    if(lit.x == -2)
        return l_False;
    return value(lit);
}

bool SegmentSolver::use_available_info()
{
    for(auto it = lit_to_edge.begin(); it != lit_to_edge.end();)
    {
        auto& pair = *it;
        auto& lit = pair.first;
        auto& e = pair.second;
        if(get_assignment(lit) == l_True)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << e.first.first << " " << e.first.second << " " << e.second << " should be initially added\n";
            segment_edget edge(e.first.first, e.first.second, e.second, lit);
            RETURN_IF_TRUE(add_edge(edge.from, edge.to, -1))
            if(edge.kind == OC_RF)
                RETURN_IF_TRUE(add_rf(edge.from, edge.to, lit))

            it = lit_to_edge.erase(it);

            remove_rf(edge.from, edge.to, lit);
        }
        else
            it++;
    }

    for(auto it = guard_lit_to_node.begin(); it != guard_lit_to_node.end();)
    {
        auto& pair = *it;
        auto& lit = pair.first;
        auto& n = pair.second;
        if(get_assignment(lit) == l_True)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << n << " should be initially lighted\n";
            RETURN_IF_TRUE(enable_guard(n))

            it = guard_lit_to_node.erase(it);
        }
        else
            it++;
    }

    return false;
}

lbool SegmentSolver::search(int nof_conflicts)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    //initialize nodes

    if(use_available_info())
    {
        std::cout << "WARNING: use_available_info find cycle at the very start\n";
        return l_False;
    }

    for (;;){
        CRef confl = propagate();
        if (confl != CRef_Undef){
            // CONFLICT
            conflicts++; conflictC++;
            if (decisionLevel() == 0) return l_False;

            learnt_clause.clear();
            analyze(confl, learnt_clause, backtrack_level);

            cancelUntil(backtrack_level);

            if (learnt_clause.size() == 1){
                uncheckedEnqueue(learnt_clause[0]);
            }else{
                CRef cr = ca.alloc(learnt_clause, true);
                learnts.push(cr);
                attachClause(cr);
                claBumpActivity(ca[cr]);
                uncheckedEnqueue(learnt_clause[0], cr);
            }

            varDecayActivity();
            claDecayActivity();

            if (--learntsize_adjust_cnt == 0){
                learntsize_adjust_confl *= learntsize_adjust_inc;
                learntsize_adjust_cnt    = (int)learntsize_adjust_confl;
                max_learnts             *= learntsize_inc;

                if (verbosity >= 1)
                    printf("| %9d | %7d %8d %8d | %8d %8d %6.0f | %6.3f %% |\n",
                           (int)conflicts,
                           (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]), nClauses(), (int)clauses_literals,
                           (int)max_learnts, nLearnts(), (double)learnts_literals/nLearnts(), progressEstimate()*100);
            }

        }else{
            // NO CONFLICT
            if ((nof_conflicts >= 0 && conflictC >= nof_conflicts) || !withinBudget()){
                // Reached bound on number of conflicts:
                progress_estimate = progressEstimate();
                cancelUntil(0);
                return l_Undef; }

            // Simplify the set of problem clauses:
            if (decisionLevel() == 0 && !simplify())
                return l_False;

            if (learnts.size()-nAssigns() >= max_learnts)
                // Reduce the set of learnt clauses:
                reduceDB();

            Lit next = lit_Undef;
            while (decisionLevel() < assumptions.size()){
                // Perform user provided assumption:
                Lit p = assumptions[decisionLevel()];
                if (value(p) == l_True){
                    // Dummy decision level:
                    newDecisionLevel();
                }else if (value(p) == l_False){
                    analyzeFinal(~p, conflict);
                    return l_False;
                }else{
                    next = p;
                    break;
                }
            }

            if (next == lit_Undef){
                // New variable decision:
                decisions++;
                next = pickBranchLit();

                if (next == lit_Undef)
                    // Model found:
                    return l_True;
            }

            // Increase decision level and enqueue 'next'
            newDecisionLevel();
            uncheckedEnqueue(next);
        }
    }
}

static double luby(double y, int x){

    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for (size = 1, seq = 0; size < x+1; seq++, size = 2*size+1);

    while (size-1 != x){
        size = (size-1)>>1;
        seq--;
        x = x % size;
    }

    return pow(y, seq);
}

lbool SegmentSolver::solve_()
{
    model.clear();
    conflict.clear();
    if (!ok) return l_False;

    solves++;

    max_learnts               = nClauses() * learntsize_factor;
    learntsize_adjust_confl   = learntsize_adjust_start_confl;
    learntsize_adjust_cnt     = (int)learntsize_adjust_confl;
    lbool   status            = l_Undef;

    if (verbosity >= 1){
        printf("============================[ Search Statistics ]==============================\n");
        printf("| Conflicts |          ORIGINAL         |          LEARNT          | Progress |\n");
        printf("|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |\n");
        printf("===============================================================================\n");
    }

    // Search:
    int curr_restarts = 0;
    while (status == l_Undef){
        double rest_base = luby_restart ? luby(restart_inc, curr_restarts) : pow(restart_inc, curr_restarts);
        status = search(rest_base * restart_first);
        if (!withinBudget()) break;
        curr_restarts++;
    }

    if (verbosity >= 1)
        printf("===============================================================================\n");


    if (status == l_True){
        // Extend & copy model:
        model.growTo(nVars());
        for (int i = 0; i < nVars(); i++) model[i] = value(i);

        show_model();
    }
    else if (status == l_False && conflict.size() == 0)
        ok = false;

    std::cout << "SegmentSolver finishes with " << conflicts << " conflicts, " << decisions << " decisions, and " << propagations << " propagations. " << theory_propagation << " theory propagations and " << conflict_cycle << " cycles included.\n";

    cancelUntil(0);
    return status;
}

SegmentSolver::SegmentSolver()
{
    Solver();
}

SegmentSolver::decide_entryt SegmentSolver::get_decide_entry(Lit l)
{
    if(lit_to_edge.find(l) != lit_to_edge.end())
        return lit_to_edge[l];
    return std::make_pair(std::make_pair(-1, -1), OC_NA);
}

std::vector<int> SegmentSolver::check_guard_literal(Lit guard_lit)
{
    std::vector<int> ret;

    auto range = guard_lit_to_node.equal_range(guard_lit);
    bool has = false;
    while(range.first != range.second)
    {
        has = true;
        ret.push_back(range.first->second);
        range.first++;
    }

    if(has && decisionLevel() == 0)
        guard_lit_to_node.erase(guard_lit);

    return ret;
}

CRef SegmentSolver::propagate()
{
    literals_to_assign.clear();
    assigned_literals.clear();

    CRef confl = CRef_Undef;
    int num_props = 0;
    watches.cleanAll();

    std::vector<segment_edget> edges_to_add;
    std::vector<int> guards_to_enable;

    while (qhead < trail.size()){
        Lit            p   = trail[qhead++];     // 'p' is enqueued fact to propagate.
        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        //our method
        auto decide_entry = get_decide_entry(p);
        if(decide_entry.first.first != -1)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << var(p) << "(" << sign(p) << ") is related to an edge (" << toInt(assigns[var(p)]) << ")\n";

            edges_to_add.push_back(segment_edget(decide_entry.first.first, decide_entry.first.second, decide_entry.second, p));
        }

        auto guard_nodes = check_guard_literal(p);
        for(auto guard_node: guard_nodes)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << var(p) << "(" << sign(p) << ") is related to a guard of " << guard_node << "\n";

            guards_to_enable.push_back(guard_node);
        }
        //out method ends

        for (i = j = (Watcher*)ws, end = i + ws.size();  i != end;){
            // Try to avoid inspecting the clause:
            Lit blocker = i->blocker;
            if (value(blocker) == l_True){
                *j++ = *i++; continue; }

            // Make sure the false literal is data[1]:
            CRef     cr        = i->cref;

            Clause&  c         = ca[cr];

            Lit      false_lit = ~p;
            if (c[0] == false_lit)
                c[0] = c[1], c[1] = false_lit;
            assert(c[1] == false_lit);
            i++;

            // If 0th watch is true, then clause is already satisfied.
            Lit     first = c[0];
            Watcher w     = Watcher(cr, first);
            if (first != blocker && value(first) == l_True){
                *j++ = w; continue; }

            // Look for new watch:
            for (int k = 2; k < c.size(); k++)
                if (value(c[k]) != l_False){
                    c[1] = c[k]; c[k] = false_lit;
                    watches[~c[1]].push(w);
                    goto NextClause; }

            // Did not find watch -- clause is unit under assignment:
            *j++ = w;
            if (value(first) == l_False){
                confl = cr;
                qhead = trail.size();
                // Copy the remaining watches:
                while (i < end)
                    *j++ = *i++;
            }else
                uncheckedEnqueue(first, cr);

        NextClause:;
        }
        ws.shrink(i - j);
    }

    bool one_more_time = false;
    //our method
    if(confl == CRef_Undef)
    {
        bool cycle = false;

        for(auto& edge: edges_to_add)
        {
            cycle = add_edge(edge.from, edge.to, register_reason(pooled_reasont({edge.lit})));

            if(!cycle && edge.kind == OC_RF)
            {
                cycle = add_rf(edge.from, edge.to, edge.lit);
                if(decisionLevel() == 0)
                    remove_rf(edge.from, edge.to, edge.lit);
            }

            if(cycle)
                break;
        }

        if(!cycle)
        {
            for(auto& n: guards_to_enable)
            {
                cycle = enable_guard(n);
                if(cycle)
                    break;
            }
        }

        if(cycle)
        {
            // graph.triplets_to_check.clear();

            conflict_cycle++;
            if (OC_VERBOSITY >= 1)
            {
                std::cout << "found cycle! conflict clause is: \n";
                for (auto l: conflict_clause)
                    std::cout << var(l) << "(" << sign(l) << ") ";
                std::cout << "\n";
            }

            auto &lv = conflict_clause;

            vec<Lit> minisat_lv;
            for (auto l: lv)
                minisat_lv.push(~l);

            confl = ca.alloc(minisat_lv, true);
        }
        else if(!literals_to_assign.empty())//add literals propagated by unit edge
        {
            one_more_time = true;
            apply_literal_assignment();
        }

        literals_to_assign.clear();
        assigned_literals.clear();
    }
    //our method ends

    propagations += num_props;
    simpDB_props -= num_props;

    if(one_more_time)
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "one more time\n";
        assert(confl == CRef_Undef);
        return propagate();
    }

    return confl;
}

void SegmentSolver::cancelUntil(int level)
{
    if (decisionLevel() > level)
    {
        for (int c = trail.size()-1; c >= trail_lim[level]; c--)
        {
            Var x  = var(trail[c]);
            assigns [x] = l_Undef;
            if (phase_saving > 1 || ((phase_saving == 1) && c > trail_lim.last()))
                polarity[x] = sign(trail[c]);
            insertVarOrder(x); 
        }
        qhead = trail_lim[level];
        trail.shrink(trail.size() - trail_lim[level]);
        trail_lim.shrink(trail_lim.size() - level);

        //Our new operation
        pop_scope(level);
    }
}

void SegmentSolver::newDecisionLevel()
{
    trail_lim.push(trail.size());
    push_scope();
}

void SegmentSolver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel)
{
    int pathC = 0;
    Lit p     = lit_Undef;

    // Generate conflict clause:
    //
    out_learnt.push();      // (leave room for the asserting literal)
    int index   = trail.size() - 1;

    do{
        assert(confl != CRef_Undef); // (otherwise should be UIP)
        Clause& c = ca[confl];

        if (c.learnt())
            claBumpActivity(c);

        for (int j = (p == lit_Undef) ? 0 : 1; j < c.size(); j++){
            Lit q = c[j];

            if (!seen[var(q)] && level(var(q)) > 0){
                varBumpActivity(var(q));
                seen[var(q)] = 1;
                if (level(var(q)) >= decisionLevel())
                    pathC++;
                else
                    out_learnt.push(q);
            }
        }

        // Select next clause to look at:
        while (!seen[var(trail[index--])]);
        p     = trail[index+1];
        confl = reason(var(p));
        seen[var(p)] = 0;
        pathC--;

    }while (pathC > 0);
    out_learnt[0] = ~p;

    // Simplify conflict clause:
    //
    int i, j;
    out_learnt.copyTo(analyze_toclear);
    if (ccmin_mode == 2){
        uint32_t abstract_level = 0;
        for (i = 1; i < out_learnt.size(); i++)
            abstract_level |= abstractLevel(var(out_learnt[i])); // (maintain an abstraction of levels involved in conflict)

        for (i = j = 1; i < out_learnt.size(); i++)
            if (reason(var(out_learnt[i])) == CRef_Undef || !litRedundant(out_learnt[i], abstract_level))
                out_learnt[j++] = out_learnt[i];

    }else if (ccmin_mode == 1){
        for (i = j = 1; i < out_learnt.size(); i++){
            Var x = var(out_learnt[i]);

            if (reason(x) == CRef_Undef)
                out_learnt[j++] = out_learnt[i];
            else{
                Clause& c = ca[reason(var(out_learnt[i]))];
                for (int k = 1; k < c.size(); k++)
                    if (!seen[var(c[k])] && level(var(c[k])) > 0){
                        out_learnt[j++] = out_learnt[i];
                        break; }
            }
        }
    }else
        i = j = out_learnt.size();

    max_literals += out_learnt.size();
    out_learnt.shrink(i - j);
    tot_literals += out_learnt.size();

    // Find correct backtrack level:
    //
    if (out_learnt.size() == 1)
        out_btlevel = 0;
    else{
        int max_i = 1;
        // Find the first literal assigned at the next-highest level:
        for (int i = 2; i < out_learnt.size(); i++)
            if (level(var(out_learnt[i])) > level(var(out_learnt[max_i])))
                max_i = i;
        // Swap-in this literal at index 1:
        Lit p             = out_learnt[max_i];
        out_learnt[max_i] = out_learnt[1];
        out_learnt[1]     = p;
        out_btlevel       = level(var(p));
    }

    for (int j = 0; j < analyze_toclear.size(); j++) seen[var(analyze_toclear[j])] = 0;    // ('seen[]' is now cleared)
}

void SegmentSolver::remove_rf(node_idt node1, node_idt node2, Lit lit)
{
    lit_to_edge.erase(lit);

    for(auto it = tail_to_inactive_edges[node1].begin(); it != tail_to_inactive_edges[node1].end(); it++)
        if(it->second == lit)
        {
            tail_to_inactive_edges[node1].erase(it);
            break;
        }
    for(auto it = head_to_inactive_edges[node2].begin(); it != head_to_inactive_edges[node2].end(); it++)
        if(it->second == lit)
        {
            head_to_inactive_edges[node2].erase(it);
            break;
        }
    tail_head_to_inactive_lit[node1][node2] = lit_Error;
}

void SegmentSolver::show_model()
{
    if(OC_VERBOSITY < 1)
        return;

    std::cout << "Happened events:\n";
    for(int i = 0; i < int(nodes.size()); i++)
        if(nodes[i].guard_enabled || !nodes[i].in_rf.empty())
            std::cout << "\t" << nodes[i] << "\n";
    std::cout << "\n";

    std::cout << "Unhappened events:\n";
    for(int i = 0; i < int(nodes.size()); i++)
        if(!nodes[i].guard_enabled && nodes[i].in_rf.empty())
            std::cout << "\t" << nodes[i] << "\n";
    std::cout << "\n";

    std::cout << "Read-from relations:\n";
    for(auto& node: nodes)
        for(auto& rf: node.out_rf)
            std::cout << "\t" << nodes[rf.from] << " " << nodes[rf.to] << "\n";
    std::cout << "\n";

    std::cout << "Inter-chain edges:\n";
    for(int c1 = 0; c1 < int(chains.size()); c1++)
        for(int c2 = 0; c2 < int(chains.size()); c2++)
        {
            if(c1 == c2)
                continue;
            auto& tree = trees[c1][c2];
            for(int s1 = 0; s1 < int(tree.raw_data.size()); s1++)
            {
                int s2 = tree.raw_data[s1].digit;
                if(s2 < int(chains[c2].size()))
                    std::cout << "\t" << nodes[chains[c1][s1]] << " " << nodes[chains[c2][s2]] << "\n";
            }
        }

    std::cout << "Races:\n";
    for(auto race_pair: pair_to_inactive_races)
    {
        int from = race_pair.first.first;
        int to = race_pair.first.second;
        auto race_lit = race_pair.second;
        if(get_assignment(race_lit) == l_True)
            std::cout << "\t" << nodes[from] << " " << nodes[to] << "\n";
    }
}

// __SZH_ADD_END__