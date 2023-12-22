// __SZH_ADD_BEGIN__

#include "ClosureSolver.h"
#include <algorithm>
#include <iostream>
#include <regex>

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

const int OC_VERBOSITY = 0;

#define append(lv1, lv2) {for(auto l : lv2) if(l.x != -1) lv1.push_back(l);}

#define push_need(lv, l) {if(l.x != -1) lv.push_back(l);}

#define neg(lv) {for(auto& l : lv) l = ~l;}

#define PROPAGATE_METHOD 1 //-1: nothing; 0: only rf; 1: and ws, fr. Seems that -1 is just the same as 0. And 1 is the only choice

using namespace Minisat;

literal_vector empty_lv;

void process_reason(literal_vector& lv)
{
    std::sort(lv.begin(), lv.end());
    //auto it = std::unique(lv.begin(), lv.end());
    //lv.erase(it, lv.end());
}

template<typename T> bool contains(std::set<T> s, T elem)
{
    return s.find(elem) != s.end();
}

lbool closure::get_assignment(Lit l)
{
    if(l.x == -1)
        return l_True;

    return solver->value(l);
}

int closure::union_find_parent(int n)
{
    if(nodes[n].atomic_parent == n)
        return n;
    else
        return nodes[n].atomic_parent = union_find_parent(nodes[n].atomic_parent);
}

void closure::refine_atomic_atoms()
{
    for(int rep = 0; rep < int(nodes.size()); rep++)
        if(is_representative(rep))
        {
            for(auto item: nodes[rep].atomic_items)
            {
                if(nodes[item].is_write)
                    nodes[rep].atomic_writes.insert(item);
                if(nodes[item].is_read)
                    nodes[rep].atomic_reads.insert(item);
            }
        }
}

void closure::union_join(int m, int n)
{
    for(auto p: nodes[m].atomic_in)
        nodes[p].atomic_out.insert(nodes[n].atomic_out.begin(), nodes[n].atomic_out.end());

    for(auto s: nodes[n].atomic_out)
        nodes[s].atomic_in.insert(nodes[m].atomic_in.begin(), nodes[m].atomic_in.end());

    //union operation
    int m_parent = union_find_parent(m);
    int n_parent = union_find_parent(n);
    if(m_parent != n_parent)
        nodes[m_parent].atomic_parent = n_parent;

    nodes[n_parent].atomic_items.insert(nodes[m_parent].atomic_items.begin(), nodes[m_parent].atomic_items.end());

}

void closure::filter_writes(std::set<int> &src, std::vector<int> &dst)
{
    for(auto item: src)
        if(nodes[item].guard_lighted)
            dst.push_back(item);
}

void closure::filter_writes_reasons(std::set<int> &src, std::vector<std::pair<int, literal_vector>> &dst, literal_vector& reason)
{
    for(auto item: src)
        if(nodes[item].guard_lighted)
            dst.push_back(std::make_pair(item, reason));
}

void closure::filter_reads(std::set<int> &src, std::vector<int> &dst)
{
    for(auto item: src)
        if(!nodes[item].in_rf.empty())
            dst.push_back(item);
}

bool closure::has_edge(int u, int v)
{
    return nodes[union_find_parent(u)].out_bitset[union_find_parent(v)] != -1;
}

closure_edget& closure::find_edge(int u, int v)
{
    int index = nodes[union_find_parent(u)].out_bitset[union_find_parent(v)];
    return nodes[union_find_parent(u)].out[index];
}

Lit closure::check_tail_head_to_inactive_lit(int a, int b)
{
    if(int(tail_head_to_inactive_lit.size()) <= a)
        return lit_Error;
    if(int(tail_head_to_inactive_lit[a].size()) <= b)
        return lit_Error;
    return tail_head_to_inactive_lit[a][b];
}

closure::closure() {}

void closure::init(ClosureSolver* _solver)
{
    solver = _solver;
    nodes = std::vector<closure_nodet>();
}

std::string get_address(std::string name)
{
    // std::regex element_pattern("([^\\#]*)#\\d+(\\[\\[[a-zA-Z0-9]+\\]\\])");
    // std::smatch element_results;
    // if(regex_match(name, element_results, element_pattern))
    //     return element_results[1].str() + element_results[2].str();

    // std::regex member_pattern("([^\\#]*)#\\d+(\\.\\.[a-zA-Z0-9_]+)");
    // std::smatch member_results;
    // if(regex_match(name, member_results, member_pattern))
    //     return member_results[1].str() + member_results[2].str();

    std::regex pattern("([^\\#]*)#\\d+([^\\d].*)");
    std::smatch results;
    if(regex_match(name, results, pattern))
        return results[1].str() + results[2].str();

    return name.substr(0, name.find_first_of('#'));
}

int closure::get_node(std::string name)
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

    nodes.push_back(closure_nodet(name, address_id, new_node_id));

    if(OC_VERBOSITY >= 1)
        std::cout << "creating " << name << " " << address << "(" << address_id << ")\n";

    for(auto& node: nodes)
        node.out_bitset.resize(nodes.size(), -1);

    //if(OC_VERBOSITY >= 1)
    //    std::cout << "create node " << new_node_id << ": " << nodes[new_node_id].name << " " << nodes[new_node_id].address << "\n";

    return new_node_id;
}

void closure::init_reasonable_edge(int u, int v, edge_kindt kind, Lit l)
{
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

void closure::init_race(int u, int v, Lit l)
{
    if(u > v)
        std::swap(u, v); //make u < v

    if(OC_VERBOSITY >= 1)
        std::cout << "initing race " << u << " " << v << " with literal " << var(l) << " (" << sign(l) << ")\n";

    pair_to_inactive_races[std::make_pair(u, v)] = l;
}

bool closure::add_guard_literal(Lit guard_literal, int u)
{
    nodes[u].is_write = true;
    nodes[u].guard = guard_literal;

    if(guard_literal.x == -1)
        return light_guard(u);
    else
    {
        guard_lit_to_node.insert(std::make_pair(guard_literal, u));
        return false;
    }
}

std::vector<int> closure::check_guard_literal(Lit guard_literal)
{
    std::vector<int> ret;

    auto range = guard_lit_to_node.equal_range(guard_literal);
    bool has = false;
    while(range.first != range.second)
    {
        has = true;

        // if(range.first->first != guard_literal)
        //     std::cout << "ERROR: Found wrong guard literal\n";

        ret.push_back(range.first->second);

        range.first++;
    }

    if(has && get_decision_level() == 0)
        guard_lit_to_node.erase(guard_literal);

    return ret;
}

void closure::add_trinary_pattern(int w1, int w2, int r)
{
    // if(OC_VERBOSITY >= 1)
    //     std::cout << "checking pattern " << w1 << " " << w2 << " " << r << "\n";

    auto rf_lit = check_tail_head_to_inactive_lit(w1, r);
    if(rf_lit == lit_Error)
        return;
    
    auto guard_lit = nodes[w2].guard;

    auto rf_hold = get_assignment(rf_lit);
    auto guard_hold = get_assignment(guard_lit);

    if(rf_hold == l_False || guard_hold == l_False)
        return;

    if(rf_hold == l_True && guard_hold == l_True)
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "shit! we cannot save it\n";
        return;
    }

    triplets_to_check.push_back(triplett(w1, w2, r, rf_lit, guard_lit));
}

void closure::check_trinary_pattern()
{
    while(!triplets_to_check.empty())
    {
        auto triplet = triplets_to_check.back();
        triplets_to_check.pop_back();

        auto& w1 = triplet.w1;
        auto& w2 = triplet.w2;
        auto& r = triplet.r;
        auto& rf_lit = triplet.rf_lit;
        auto& guard_lit = triplet.guard_lit;
        
        auto rf_hold = get_assignment(rf_lit);
        auto guard_hold = get_assignment(guard_lit);

        auto& reason_w1w2 = find_edge(w1, w2).reason;
        auto& reason_w2r = find_edge(w2, r).reason;

        if(rf_hold == l_True) //just add guard false
        {
            auto unit_lv = reason_w1w2;
            append(unit_lv, reason_w2r);
            push_need(unit_lv, rf_lit);
            push_need(unit_lv, guard_lit);
            neg(unit_lv);

            solver->assign_literal(~guard_lit, unit_lv);

            if(OC_VERBOSITY >= 1)
                std::cout << "using pattern, propagate guard " << w2 << " false\n";
        }
        else if(guard_hold == l_True) //just add rf false
        {
            auto unit_lv = reason_w1w2;
            append(unit_lv, reason_w2r);
            push_need(unit_lv, rf_lit);
            push_need(unit_lv, guard_lit);
            neg(unit_lv);

            solver->assign_literal(~rf_lit, unit_lv);

            if(OC_VERBOSITY >= 1)
                std::cout << "using pattern, propagate rf " << w1 << " " << r << " false\n";
        }
        else
        {
            //add "binary clause"
            triplett triplet(w1, w2, r, rf_lit, guard_lit);
            rf_to_triplets[rf_lit].push_back(triplet);
            guard_to_triplets[guard_lit].push_back(triplet);

            trail_triplet.push(std::make_pair(rf_lit, guard_lit));
        }
    }
}

bool closure::activate_edge(int vi, int wi, edge_kindt kind, literal_vector& reason)
{
    if(OC_VERBOSITY >= 1)
        std::cout << "activating (" << vi << ", " << wi << ", " << kind_to_str(kind) << ")\n";

    closure_edget edge(vi, wi, kind, reason);
    if(activate_directed_edge(edge))
        return true;
    
    //revisit triplets
    if(kind == OC_RF)
    {
        auto rf_lit = reason[0];
        for(auto& triplet : rf_to_triplets[rf_lit])
        {
            if(get_assignment(triplet.guard_lit) == l_Undef)
            {
                auto& w1 = triplet.w1;
                auto& w2 = triplet.w2;
                auto& r = triplet.r;
                auto& guard_lit = triplet.guard_lit;

                auto& reason_w1w2 = find_edge(w1, w2).reason;
                auto& reason_w2r = find_edge(w2, r).reason;

                auto unit_lv = reason_w1w2;
                append(unit_lv, reason_w2r);
                push_need(unit_lv, rf_lit);
                push_need(unit_lv, guard_lit);
                neg(unit_lv);

                solver->assign_literal(~guard_lit, unit_lv);

                if(OC_VERBOSITY >= 1)
                    std::cout << "revisiting pattern, propagate guard " << w2 << " false\n";
            }
        }
    }

    //visit new triplets
    check_trinary_pattern();

    show_race();

    return false;
}

bool closure::activate_directed_edge(closure_edget &edge, bool need_closure)
{
    auto kind = edge.kind;
    auto& reason = edge.reason;

    if(kind == OC_RF)
    {
        nodes[edge.from].out_rf.push_back(edge);
        nodes[edge.to].in_rf.push_back(edge);
        trail_rf.push(std::make_pair(edge.from, edge.to));

        int w1 = edge.from;
        int w1_rep = union_find_parent(w1);
        int r = edge.to;
        int r_rep = union_find_parent(r);
        int address = nodes[w1].address;

        //co propagation
        if(nodes[w1].guard_lighted)
        {
            std::vector<closure_edget> edges_w2_r;

            for(auto& edge_w2_r_rep_cand: nodes[r_rep].in)
            {
                int w2_rep = edge_w2_r_rep_cand.from;
                for(auto w2_cand: nodes[w2_rep].atomic_writes)
                    if (w2_cand != w1 && nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, edge_w2_r_rep_cand.reason));
            }

            for(int w2_cand: nodes[r].atomic_in)
                if(w2_cand != w1 && nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                    edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, empty_lv));

            for(auto& edge_w2_r: edges_w2_r)
            {
                auto w2 = edge_w2_r.from;

                if(has_edge(w2, w1))
                    continue;

                literal_vector co_reason = edge.reason;
                co_reason.insert(co_reason.end(), edge_w2_r.reason.begin(), edge_w2_r.reason.end());
                push_need(co_reason, nodes[w1].guard);
                push_need(co_reason, nodes[w2].guard);

                process_reason(co_reason);

                closure_edget co(w2, w1, OC_NA, co_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << edge.from << " " << edge.to << " propagate " << co.from << " " << co.to
                              << " by co during rf\n";
                    std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                }

                if(activate_directed_edge(co))
                    return true;
            }
        }

        //fr propagation
        if(nodes[w1].guard_lighted)
        {
            std::vector<closure_edget> edges_w1_w2;

            for(auto& edge_w1_w2_rep_cand: nodes[w1_rep].out)
            {
                int w2_rep = edge_w1_w2_rep_cand.to;
                for(auto w2_cand: nodes[w2_rep].atomic_writes)
                    if (nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                        edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, edge_w1_w2_rep_cand.reason));
            }

            for(int w2_cand: nodes[w1].atomic_out)
                if(nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                    edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, empty_lv));

            for(auto& edge_w1_w2: edges_w1_w2)
            {
                auto w2 = edge_w1_w2.to;

                if(has_edge(r, w2))
                    continue;

                literal_vector fr_reason = edge.reason;
                fr_reason.insert(fr_reason.end(), edge_w1_w2.reason.begin(), edge_w1_w2.reason.end());
                push_need(fr_reason, nodes[w1].guard);
                push_need(fr_reason, nodes[w2].guard);

                process_reason(fr_reason);

                closure_edget fr(r, w2, OC_NA, fr_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << edge.from << " " << edge.to << " propagate " << fr.from << " " << fr.to
                              << " by fr during rf\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if(activate_directed_edge(fr))
                    return true;
            }
        }
    }

    int from_rep = union_find_parent(edge.from);
    int to_rep = union_find_parent(edge.to);
    closure_edget edge_rep(from_rep, to_rep, edge.kind, edge.reason);

    if(from_rep == to_rep)
    {
        //whether is a cycle
        if(edge.from == edge.to)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << "adding " << edge.from << " " << edge.to << " a cycle\n";

            conflict_lv = edge.reason;
            return true;
        }
        //or an intra-atomic edge contradicting po
        if(nodes[edge.from].atomic_in.find(edge.to) != nodes[edge.from].atomic_in.end())
        {
            if(OC_VERBOSITY >= 1)
                std::cout << "adding " << edge.from << " " << edge.to << " a cycle due to apo\n";

            conflict_lv = edge.reason;
            return true;
        }
        //an intra-atomic edge obeying po is not considered
    }
    else if(!has_edge(from_rep, to_rep)) //inter-atomic edge
    {
        //add edge
        nodes[from_rep].out_bitset[to_rep] = nodes[from_rep].out.size();
        nodes[from_rep].out.push_back(edge_rep);
        nodes[to_rep].in.push_back(edge_rep);
        trail_edge.push(std::make_pair(from_rep, to_rep));

        //propagate race
        for(auto original_from: nodes[from_rep].atomic_items)
            for(auto original_to: nodes[to_rep].atomic_items)
            {
                int from, to;
                if(original_from > original_to)
                {
                    from = original_to;
                    to = original_from;
                }
                else
                {
                    from = original_from;
                    to = original_to;
                }

                if(pair_to_inactive_races.find(std::make_pair(from, to)) == pair_to_inactive_races.end())
                    continue;

                auto race_lit = pair_to_inactive_races[std::make_pair(from, to)];
                auto race_lit_status = get_assignment(race_lit);

                if(race_lit_status == l_False)
                    continue;

                auto conflict_reason = edge.reason;
                conflict_reason.push_back(race_lit);

                if(race_lit_status == l_True)
                {
                    if(OC_VERBOSITY >= 1)
                        std::cout << "race" << from << " " << to << " should be set false but already true\n";

                    conflict_lv = conflict_reason;
                    return true;
                }
                else if(race_lit_status == l_Undef)
                {
                    neg(conflict_reason);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "set race" << from << " " << to << " false\n";

                    solver->assign_literal(~race_lit, conflict_reason);
                }
                else
                {
                    std::cout << "shit! a race_lit_status is not l_True, l_False, or l_Undef\n";
                }

            }

#if (PROPAGATE_METHOD >= 0)
        //add vital edges, w-w and w-r, and for r-w, just disable rf
        for(auto to: nodes[to_rep].atomic_items)
            for(auto from: nodes[from_rep].atomic_items)
            {
                if(nodes[to].address != nodes[from].address)
                    continue;

#if (PROPAGATE_METHOD >= 1)
                if(nodes[from].is_write && nodes[to].is_write) //w-w
                {
                    if(OC_VERBOSITY >= 1)
                        std::cout << "new w-w vital edge " << from << " " << to << "\n";
                    
                    nodes[from].out_vital.push_back(to);
                    nodes[to].in_vital.push_back(from);
                    trail_vital.push(std::make_pair(from, to));

                    auto& w1 = from;
                    auto& w2 = to;
                    for(auto r : nodes[w2].out_vital)
                        add_trinary_pattern(w1, w2, r);
                }

                if(nodes[from].is_write && nodes[to].is_read) //w-r
                {
                    if(OC_VERBOSITY >= 1)
                        std::cout << "new w-r vital edge " << from << " " << to << "\n";
                    
                    nodes[from].out_vital.push_back(to);
                    nodes[to].in_vital.push_back(from);
                    trail_vital.push(std::make_pair(from, to));

                    auto& w2 = from;
                    auto& r = to;
                    for(auto w1 : nodes[w2].in_vital)
                        add_trinary_pattern(w1, w2, r);
                }
#endif // (PROPAGATE_METHOD >= 1)
                if(nodes[from].is_read && nodes[to].is_write) //special for rf, just set its rf_variable false
                {
                    auto rf_lit_cand = check_tail_head_to_inactive_lit(to, from);
                    if(rf_lit_cand == lit_Error)
                        continue;

                    if (get_assignment(rf_lit_cand) != l_Undef)
                        continue;

                    literal_set unit_lv;
                    for(auto l: edge.reason)
                        unit_lv.insert(~l);
                    unit_lv.insert(~rf_lit_cand);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "before preventive propagation, set rf" << to << " " << from << " false\n";

                    solver->assign_literal(~rf_lit_cand, unit_lv);
                }
            }
#endif // (PROPAGATE_METHOD >= 0)

        if(need_closure)
        {
            auto back_edges = nodes[from_rep].in;
            back_edges.push_back(closure_edget(from_rep, from_rep, OC_NA, empty_lv));
            auto front_edges = nodes[to_rep].out;
            front_edges.push_back(closure_edget(to_rep, to_rep, OC_NA, empty_lv));

            for (auto &back_edge: back_edges)
                for (auto &front_edge: front_edges)
                {
                    if (has_edge(back_edge.from, front_edge.to)) //edge exists
                        continue;

                    literal_vector na_reason = back_edge.reason;
                    na_reason.insert(na_reason.end(), reason.begin(), reason.end());
                    na_reason.insert(na_reason.end(), front_edge.reason.begin(), front_edge.reason.end());

                    process_reason(na_reason);

                    closure_edget closure_edge(back_edge.from, front_edge.to, OC_NA, na_reason);

                    if(OC_VERBOSITY >= 1)
                        std::cout << edge.from << " " << edge.to << " propagate " << closure_edge.from << " " << closure_edge.to << " by transitivity\n";

                    if(activate_directed_edge(closure_edge, false))
                        return true;

                }
        }

        //co propagation
        {
            std::vector<int> w2s;
            filter_writes(nodes[from_rep].atomic_writes, w2s);
            std::vector<int> rs;
            filter_reads(nodes[to_rep].atomic_reads, rs);

            for(auto w2: w2s)
                for(auto r: rs)
                {
                    if(nodes[w2].address != nodes[r].address)
                        continue;

                    for (auto &edge_w1_r: nodes[r].in_rf)
                    {
                        auto w1 = edge_w1_r.from;

                        if (w1 == w2 || !nodes[w1].guard_lighted || has_edge(w2, w1))
                            continue;

                        if (nodes[w1].guard_lighted)
                        {
                            literal_vector co_reason = edge.reason;
                            co_reason.insert(co_reason.end(), edge_w1_r.reason.begin(), edge_w1_r.reason.end());
                            push_need(co_reason, nodes[w1].guard);
                            push_need(co_reason, nodes[w2].guard);

                            process_reason(co_reason);

                            closure_edget co(w2, w1, OC_NA, co_reason);

                            if (OC_VERBOSITY >= 1)
                            {
                                std::cout << edge.from << " " << edge.to << " propagate " << w2 << " " << w1
                                          << " by co during na\n";
                                std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                            }

                            if (activate_directed_edge(co))
                                return true;
                        }
                    }
                }
        }

        //fr propagation
        {
            std::vector<int> w1s;
            filter_writes(nodes[from_rep].atomic_writes, w1s);
            std::vector<int> w2s;
            filter_writes(nodes[to_rep].atomic_writes, w2s);



            for(auto w1: w1s)
                for(auto w2: w2s)
                {
                    if(nodes[w1].address != nodes[w2].address)
                        continue;

                    for (auto &edge_w1_r: nodes[w1].out_rf)
                    {
                        auto r = edge_w1_r.to;

                        if(has_edge(r, w2))
                            continue;

                        literal_vector fr_reason = edge.reason;
                        fr_reason.insert(fr_reason.end(), edge_w1_r.reason.begin(), edge_w1_r.reason.end());
                        push_need(fr_reason, nodes[w1].guard);
                        push_need(fr_reason, nodes[w2].guard);

                        process_reason(fr_reason);

                        closure_edget fr(r, w2, OC_NA, fr_reason);

                        //process_reason(fr_reason);

                        if (OC_VERBOSITY >= 1)
                        {
                            std::cout << edge.from << " " << edge.to << " propagate " << fr.from << " " << fr.to
                                      << " by fr during na\n";
                            std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                        }

                        if (activate_directed_edge(fr))
                            return true;
                    }
                }
        }
    }

    return false;
}

bool closure::activate_apo(int u, int v) //always before activating other edges
{
    // if(union_check(u, v))
    //     std::cout << "WARNING: " << u << " and " << v << " are already together\n";

    union_join(u, v);

    return false;
}

void closure::atomic_remove_self()
{
    for(int i = 0; i < int(nodes.size()); i++)
    {
        nodes[i].atomic_in.erase(i);
        nodes[i].atomic_out.erase(i);
    }
}

bool closure::light_guard(int u)
{
   if(nodes[u].guard_lighted) //this circumstance exist?
   {
       //std::cout << "WARNING: " << u << " is lit twice\n";
       return false;
   }

    nodes[u].guard_lighted = true;
    trail_light_guard.push(u);

    if(OC_VERBOSITY >= 1)
        std::cout << "guard of " << u << " is lit\n";

    //co_propagation: this is w1
    {
        int w1 = u;
        for (auto &edge_w1_r: nodes[w1].out_rf)
        {
            auto r = edge_w1_r.to;
            int r_rep = union_find_parent(r);

            std::vector<closure_edget> edges_w2_r;

            for(auto& edge_w2_r_rep_cand: nodes[r_rep].in)
            {
                int w2_rep = edge_w2_r_rep_cand.from;
                for(auto w2_cand: nodes[w2_rep].atomic_writes)
                    if (nodes[w2_cand].guard_lighted)
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, edge_w2_r_rep_cand.reason));
            }

            for(int w2_cand: nodes[r].atomic_in)
                if(nodes[w2_cand].guard_lighted)
                    edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, empty_lv));

            for (auto &edge_w2_r: edges_w2_r)
            {
                auto w2 = edge_w2_r.from;

                if (w2 == w1 || nodes[w2].address != nodes[w1].address || has_edge(w2, w1))
                    continue;

                literal_vector co_reason = edge_w1_r.reason;
                co_reason.insert(co_reason.end(), edge_w2_r.reason.begin(), edge_w2_r.reason.end());
                push_need(co_reason, nodes[w1].guard);
                push_need(co_reason, nodes[w2].guard);

                process_reason(co_reason);

                closure_edget co(w2, w1, OC_NA, co_reason);

                if (OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << co.from << " " << co.to
                              << " by co during light(this is w1)\n";
                    std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                }

                if (activate_directed_edge(co))
                    return true;
            }
        }
    }

    //co_propagation: this is w2
    {
        int w2 = u;
        int w2_rep = union_find_parent(w2);

        std::vector<closure_edget> edges_w2_r;

        for(auto& edge_w2_r_rep_cand: nodes[w2_rep].out)
        {
            int r_rep = edge_w2_r_rep_cand.to;
            for(auto r_cand: nodes[r_rep].atomic_reads)
                if (!nodes[r_cand].in_rf.empty())
                    edges_w2_r.push_back(closure_edget(w2, r_cand, OC_NA, edge_w2_r_rep_cand.reason));
        }

        for(int r_cand: nodes[w2].atomic_out)
            if(!nodes[r_cand].in_rf.empty())
                edges_w2_r.push_back(closure_edget(w2, r_cand, OC_NA, empty_lv));

        for(auto& edge_w2_r: edges_w2_r)
        {
            auto r = edge_w2_r.to;

            if(nodes[w2].address != nodes[r].address)
                continue;

            for(auto& edge_w1_r: nodes[r].in_rf)
            {
                auto w1 = edge_w1_r.from;

                if(w2 == w1 || has_edge(w2, w1))
                    continue;

                if(nodes[w1].guard_lighted)
                {
                    literal_vector co_reason = edge_w1_r.reason;
                    co_reason.insert(co_reason.end(), edge_w2_r.reason.begin(), edge_w2_r.reason.end());
                    push_need(co_reason, nodes[w1].guard);
                    push_need(co_reason, nodes[w2].guard);

                    process_reason(co_reason);

                    closure_edget co(w2, w1, OC_NA, co_reason);

                    if(OC_VERBOSITY >= 1)
                    {
                        std::cout << "light " << u << " propagate " << co.from << " " << co.to << " by co during light(this is w2)\n";
                        std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                    }

                    if(activate_directed_edge(co))
                        return true;
                }
            }
        }
    }

    //fr_propagation: this is w1
    {
        int w1 = u;
        int w1_rep = union_find_parent(w1);

        std::vector<closure_edget> edges_w1_w2;

        for(auto& edge_w1_w2_rep_cand: nodes[w1_rep].out)
        {
            int w2_rep = edge_w1_w2_rep_cand.to;
            for(auto w2_cand: nodes[w2_rep].atomic_writes)
                if (nodes[w2_cand].guard_lighted)
                    edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, edge_w1_w2_rep_cand.reason));
        }

        for(int w2_cand: nodes[w1].atomic_out)
            if(nodes[w2_cand].guard_lighted)
                edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, empty_lv));

        std::vector<closure_edget>& edges_w1_r = nodes[w1].out_rf;

        for (auto &edge_w1_r: edges_w1_r)
            for(auto& edge_w1_w2: edges_w1_w2)
            {
                auto r = edge_w1_r.to;
                auto w2 = edge_w1_w2.to;

                if(nodes[w2].address != nodes[r].address || has_edge(r, w2))
                    continue;

                literal_vector fr_reason = edge_w1_r.reason;
                fr_reason.insert(fr_reason.end(), edge_w1_w2.reason.begin(), edge_w1_w2.reason.end());
                push_need(fr_reason, nodes[w1].guard);
                push_need(fr_reason, nodes[w2].guard);

                process_reason(fr_reason);

                closure_edget fr(r, w2, OC_NA, fr_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << fr.from << " " << fr.to
                              << " by fr during light(this is w1)\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if(activate_directed_edge(fr))
                    return true;
            }
    }


    //fr_propagation: this is w2
    {
        int w2 = u;
        int w2_rep = union_find_parent(w2);

        std::vector<closure_edget> edges_w1_w2;

        for(auto& edge_w1_w2_rep_cand: nodes[w2_rep].in)
        {
            int w1_rep = edge_w1_w2_rep_cand.from;
            for(auto w1_cand: nodes[w1_rep].atomic_writes)
                if (nodes[w1_cand].guard_lighted)
                    edges_w1_w2.push_back(closure_edget(w1_cand, w2, OC_NA, edge_w1_w2_rep_cand.reason));
        }

        for(int w1_cand: nodes[w2].atomic_in)
            if(nodes[w1_cand].guard_lighted)
                edges_w1_w2.push_back(closure_edget(w1_cand, w2, OC_NA, empty_lv));

        for (auto &edge_w1_w2: edges_w1_w2)
        {
            auto w1 = edge_w1_w2.from;
            if (nodes[w1].address != nodes[u].address)
                continue;

            for (auto &edge_w1_r: nodes[w1].out_rf)
            {
                auto r = edge_w1_r.to;

                if(has_edge(r, w2))
                    continue;

                literal_vector fr_reason = edge_w1_r.reason;
                fr_reason.insert(fr_reason.end(), edge_w1_w2.reason.begin(), edge_w1_w2.reason.end());
                push_need(fr_reason, nodes[w1].guard);
                push_need(fr_reason, nodes[w2].guard);

                process_reason(fr_reason);

                closure_edget fr(r, w2, OC_NA, fr_reason);

                if (OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << fr.from << " " << fr.to
                              << " by fr during light(this is w2)\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if (activate_directed_edge(fr))
                    return true;
            }
        }
    }

    //revisit triplets
    {
        auto guard_lit = nodes[u].guard;
        for(auto& triplet : guard_to_triplets[guard_lit])
        {
            if(get_assignment(triplet.rf_lit) == l_Undef)
            {
                auto& w1 = triplet.w1;
                auto& w2 = triplet.w2;
                auto& r = triplet.r;
                auto& rf_lit = triplet.rf_lit;

                auto& reason_w1w2 = find_edge(w1, w2).reason;
                auto& reason_w2r = find_edge(w2, r).reason;

                auto unit_lv = reason_w1w2;
                append(unit_lv, reason_w2r);
                push_need(unit_lv, rf_lit);
                push_need(unit_lv, guard_lit);
                neg(unit_lv);

                solver->assign_literal(~rf_lit, unit_lv);

                if(OC_VERBOSITY >= 1)
                    std::cout << "revisiting pattern, propagate rf " << w1 << " " << r << " false\n";
            }
        }
    }

    //visit new triplets
    check_trinary_pattern();

    return false;
}

void closure::push_scope()
{
    trail_rf_lim.push(trail_rf.size());
    trail_edge_lim.push(trail_edge.size());
    trail_vital_lim.push(trail_vital.size());
    trail_light_guard_lim.push(trail_light_guard.size());
    trail_triplet_lim.push(trail_triplet.size());
}

void closure::pop_scope(int new_level)
{
    for (int c = trail_edge.size() - 1; c >= trail_edge_lim[new_level]; c--)
    {
        auto e = trail_edge[c];
        nodes[e.first].out_bitset[e.second] = -1;
        nodes[e.first].out.pop_back();
        nodes[e.second].in.pop_back();
    }
    trail_edge.shrink(trail_edge.size() - trail_edge_lim[new_level]);
    trail_edge_lim.shrink(trail_edge_lim.size() - new_level);

    for (int c = trail_rf.size() - 1; c >= trail_rf_lim[new_level]; c--)
    {
        auto e = trail_rf[c];
        nodes[e.first].out_rf.pop_back();
        nodes[e.second].in_rf.pop_back();
    }
    trail_rf.shrink(trail_rf.size() - trail_rf_lim[new_level]);
    trail_rf_lim.shrink(trail_rf_lim.size() - new_level);

    for (int c = trail_vital.size() - 1; c >= trail_vital_lim[new_level]; c--)
    {
        auto e = trail_vital[c];
        nodes[e.first].out_vital.pop_back();
        nodes[e.second].in_vital.pop_back();
    }
    trail_vital.shrink(trail_vital.size() - trail_vital_lim[new_level]);
    trail_vital_lim.shrink(trail_vital_lim.size() - new_level);

    for (int c = trail_light_guard.size() - 1; c >= trail_light_guard_lim[new_level]; c--)
    {
        auto u = trail_light_guard[c];
        nodes[u].guard_lighted = false;
    }
    trail_light_guard.shrink(trail_light_guard.size() - trail_light_guard_lim[new_level]);
    trail_light_guard_lim.shrink(trail_light_guard_lim.size() - new_level);

    for (int c = trail_triplet.size() - 1; c >= trail_triplet_lim[new_level]; c--)
    {
        auto lits = trail_triplet[c];
        rf_to_triplets[lits.first].pop_back();
        guard_to_triplets[lits.second].pop_back();
    }
    trail_triplet.shrink(trail_triplet.size() - trail_triplet_lim[new_level]);
    trail_triplet_lim.shrink(trail_triplet_lim.size() - new_level);
}

//void closure::show_edges()
//{
//    if(OC_VERBOSITY < 2)
//        return;
//
//
//    std::cout << "out:";
//    for(auto& n: nodes)
//    {
//        for(auto& s: n.out)
//            std::cout << "(" << &n - &(nodes[0]) << "[" << n.level << "], " << s.n << "[" << nodes[s.n].level << "], " << s.reason << ")";
//        std::cout << "\n";
//    }
//
//    std::cout << "in:";
//    for(auto& n: nodes)
//    {
//        for(auto& s: n.in)
//            std::cout << "(" << s.n << "[" << nodes[s.n].level << "], " << &n - &(nodes[0]) << "[" << n.level << "], " << s.reason << ")";
//        std::cout << "\n";
//    }
//}

void closure::show_race()
{
    if(OC_VERBOSITY < 1)
        return;

    for(auto race_pair: pair_to_inactive_races)
    {
        int from = race_pair.first.first;
        int to = race_pair.first.second;
        auto race_lit = race_pair.second;

        std::cout << "race " << from << " " << to << "'s literal is " << var(race_lit) << "(" << sign(race_lit) << "), whose assignment is " << toInt(get_assignment(race_lit)) << "\n";
    }
}

void closure::show_rf()
{
    if(OC_VERBOSITY < 1)
        return;

    std::cout << "current lighted nodes:\n";
    for(int i = 0; i < int(nodes.size()); i++)
        if(nodes[i].guard_lighted || !nodes[i].in_rf.empty())
            std::cout << i << "(" << nodes[i].name << ")\n";
    std::cout << "\n";

    std::cout << "current rf:\n";

    for(auto& node: nodes)
    {
        for(auto& rf: node.out_rf)
            std::cout << rf.from << "(" << nodes[rf.from].name << ") " << rf.to << "(" << nodes[rf.to].name << ")\n";
    }

    show_race();

    std::cout << "current outs:\n";
    for(int i = 0; i < int(nodes.size()); i++)
    {
        std::cout << i << "has outs: ";
        for(auto& edge: nodes[union_find_parent(i)].out)
            std::cout << edge.to << " ";
        std::cout << " and atomic outs: ";
        for(auto& out: nodes[i].atomic_out)
            std::cout << out << " ";
        std::cout << "\n";
    }

    std::cout << "current ins:\n";
    for(int i = 0; i < int(nodes.size()); i++)
    {
        std::cout << i << "has ins: ";
        for(auto& edge: nodes[union_find_parent(i)].in)
            std::cout << edge.from << " ";
        std::cout << " and atomic ins: ";
        for(auto& in: nodes[i].atomic_in)
            std::cout << in << " ";
        std::cout << "\n";
    }
}

void closure::show_model()
{
    if(OC_VERBOSITY < 1)
        return;

    std::cout << "Happened events:\n";
    for(int i = 0; i < int(nodes.size()); i++)
        if(nodes[i].guard_lighted || !nodes[i].in_rf.empty())
            std::cout << "\t" << nodes[i] << "\n";
    std::cout << "\n";

    std::cout << "Unhappened events:\n";
    for(int i = 0; i < int(nodes.size()); i++)
        if(!nodes[i].guard_lighted && nodes[i].in_rf.empty())
            std::cout << "\t" << nodes[i] << "\n";
    std::cout << "\n";

    std::cout << "Read-from relations:\n";
    for(auto& node: nodes)
        for(auto& rf: node.out_rf)
            std::cout << "\t" << nodes[rf.from] << " " << nodes[rf.to] << "\n";
    std::cout << "\n";

    
}

closure::simple_nodet closure::simplify_node(int node_id)
{
    auto& node = nodes[node_id];

    simple_nodet simple_node;
    simple_node.name = node.name;
    simple_node.is_write = node.is_write;
    
    simple_node.in_degree = node.in.size();

    for(auto& out_edge: node.out)
        simple_node.out.push_back(out_edge.to);

    return simple_node;
}

std::vector<std::string> write_order;

void closure::final_check()
{
    // get model
    std::vector<simple_nodet> simple_nodes;
    for(int i = 0; i < int(nodes.size()); i++)
        simple_nodes.push_back(simplify_node(i));

    // for(int i = 0; i < nodes.size(); i++)
    // {
    //     std::cout << i << "'s name is " << simple_nodes[i].name << ", in_degree is " << simple_nodes[i].in_degree << ", out include: ";
    //     for(auto& out : simple_nodes[i].out)
    //         std::cout << out << " ";
    //     std::cout << "\n";
    // }

    write_order.clear();

    std::vector<int> zero_degree_nodes;
    for(int i = 0; i < int(simple_nodes.size()); i++)
        if(simple_nodes[i].in_degree == 0 && is_representative(i))
            zero_degree_nodes.push_back(i);

    int visited_node_num = 0;

    while(!zero_degree_nodes.empty())
    {
        int visiting_node = zero_degree_nodes.back();
        zero_degree_nodes.pop_back();
        int node_order = visited_node_num;
        visited_node_num++;

        std::vector<std::pair<int, int> > visiting_items_locations;
        for(auto item : nodes[visiting_node].atomic_items)
        {
            visiting_items_locations.push_back(std::make_pair(item, nodes[item].location));
        }
        std::sort(visiting_items_locations.begin(), visiting_items_locations.end(), 
            [](const std::pair<int, int>& a, const std::pair<int, int>& b)
            {
                return a.second < b.second;
            });
        for(auto item_location : visiting_items_locations)
        {
            auto item = item_location.first;
            auto name = simple_nodes[item].name;
            // if(simple_nodes[item].is_write && 
            //     name.find("__CPROVER") == std::string::npos && 
            //     name.find("argv'") == std::string::npos)
            {
                write_order.push_back(name);
                solver->oc_result_order->insert(std::make_pair(name, node_order));
            }
        }

        for(auto out_node : simple_nodes[visiting_node].out)
        {
            simple_nodes[out_node].in_degree--;
            if(simple_nodes[out_node].in_degree == 0)
                zero_degree_nodes.push_back(out_node);

            // if(simple_nodes[out_node].in_degree < 0)
            //     std::cout << "ERROR: negative in_degree, please check final_check\n";
        }
    }

    // std::cout << "see the total order:\n";
    // for(auto& name: write_order)
    //     std::cout << "\t" << name << "\n";

    if(OC_VERBOSITY >= 1)
        for(auto& node: nodes)
        {
            if(node.is_write && node.guard_lighted)
                std::cout << "write " << node.name << " is lighted\n";
        }

    for (auto &node: nodes)
        for (auto &edge: node.out_rf)
        {

            int w1 = edge.from;
            int w1_rep = union_find_parent(w1);
            int r = edge.to;
            int r_rep = union_find_parent(r);

            //co propagation
            if (nodes[w1].guard_lighted)
            {
                std::vector<closure_edget> edges_w2_r;

                for (auto &edge_w2_r_rep_cand: nodes[r_rep].in)
                {
                    int w2_rep = edge_w2_r_rep_cand.from;
                    for (auto w2_cand: nodes[w2_rep].atomic_writes)
                        if (nodes[w2_cand].guard_lighted)
                            edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, edge_w2_r_rep_cand.reason));
                }

                for (int w2_cand: nodes[r].atomic_in)
                    if (nodes[w2_cand].guard_lighted)
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OC_NA, empty_lv));

                for (auto &edge_w2_r: edges_w2_r)
                {
                    auto w2 = edge_w2_r.from;

                    if (w2 == w1 || nodes[w2].address != nodes[w1].address || has_edge(w2, w1))
                        continue;

                    literal_vector co_reason = edge.reason;
                    co_reason.insert(co_reason.end(), edge_w2_r.reason.begin(), edge_w2_r.reason.end());
                    push_need(co_reason, nodes[w1].guard);
                    push_need(co_reason, nodes[w2].guard);

                    closure_edget co(w2, w1, OC_NA, co_reason);

                    int w2_rep = union_find_parent(w2);

                    if(w2_rep == w1_rep)
                    {
                        if(nodes[w2].atomic_in.find(w1) != nodes[w2].atomic_in.end())
                            std::cout << "ERROR: rf" << w1 << " " << r << " and na " << w2_rep << " " << r_rep
                                      << " propagates co " << w2 << " " << w1 << " which contradict atomic_po\n";
                    }
                    else
                    {
                        if (!has_edge(w2_rep, w1_rep))
                            std::cout << "ERROR: rf" << w1 << " " << r << " and na " << w2_rep << " " << r_rep
                                      << " propagates co " << w2_rep << " " << w1_rep << " but not found previously\n";
                    }
                }
            }

            //fr propagation
            if (nodes[w1].guard_lighted)
            {
                std::vector<closure_edget> edges_w1_w2;

                for (auto &edge_w1_w2_rep_cand: nodes[w1_rep].out)
                {
                    int w2_rep = edge_w1_w2_rep_cand.to;
                    for (auto w2_cand: nodes[w2_rep].atomic_writes)
                        if (nodes[w2_cand].guard_lighted)
                            edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, edge_w1_w2_rep_cand.reason));
                }

                for (int w2_cand: nodes[w1].atomic_out)
                    if (nodes[w2_cand].guard_lighted)
                        edges_w1_w2.push_back(closure_edget(w1, w2_cand, OC_NA, empty_lv));

                for (auto &edge_w1_w2: edges_w1_w2)
                {
                    auto w2 = edge_w1_w2.to;

                    if (nodes[w2].address != nodes[w1].address)
                        continue;

                    if(has_edge(r, w2))
                        continue;

                    literal_vector fr_reason = edge.reason;
                    fr_reason.insert(fr_reason.end(), edge_w1_w2.reason.begin(), edge_w1_w2.reason.end());
                    push_need(fr_reason, nodes[w1].guard);
                    push_need(fr_reason, nodes[w2].guard);

                    //newly added
                    process_reason(fr_reason);

                    closure_edget fr(r, w2, OC_NA, fr_reason);

                    int w2_rep = union_find_parent(w2);

                    if(r_rep == w2_rep)
                    {
                        if(nodes[r].atomic_in.find(w2) != nodes[r].atomic_in.end())
                            std::cout << "ERROR: rf" << w1 << " " << r << " and na " << w1_rep << " " << w2_rep << " propagates fr " << r << " " << w2 << " which contradicts atomic_po\n";
                    }
                    else
                    {
                        if (!has_edge(r_rep, w2_rep))
                            std::cout << "ERROR: rf" << w1 << " " << r << " and na " << w1_rep << " " << w2_rep
                                      << " propagates fr " << r_rep << " " << w2_rep << " but not found previously\n";
                    }
                }
            }
        }
}

decide_entryt closure::get_decide_entry(Lit l)
{
    if(lit_to_edge.find(l) != lit_to_edge.end())
        return lit_to_edge[l];
    return std::make_pair(std::make_pair(-1, -1), OC_PO);
}

bool closure::use_available_info()
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
            literal_vector lv(1, lit);
            closure_edget edge(e.first.first, e.first.second, e.second, lv);
            if(activate_edge(edge.from, edge.to, edge.kind, edge.reason))
                return true;

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
            if(light_guard(n))
                return true;

            it = guard_lit_to_node.erase(it);
        }
        else
            it++;
    }

    return false;
}

int closure::get_decision_level()
{
    return trail_edge_lim.size();
}

bool closure::remove_rf(int from, int to, Lit p)
{
    lit_to_edge.erase(p);

    for(auto it = tail_to_inactive_edges[from].begin(); it != tail_to_inactive_edges[from].end(); it++)
    {
        if(it->second == p)
        {
            tail_to_inactive_edges[from].erase(it);
            break;
        }
    }

    for(auto it = head_to_inactive_edges[to].begin(); it != head_to_inactive_edges[to].end(); it++)
    {
        if(it->second == p)
        {
            head_to_inactive_edges[to].erase(it);
            break;
        }
    }

    tail_head_to_inactive_lit[from][to] = lit_Error;

    return true;
}

// __SZH_ADD_END__