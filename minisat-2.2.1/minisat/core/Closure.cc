// __SZH_ADD_BEGIN__

#include "ClosureSolver.h"
#include <deque>
#include <algorithm>
#include <iostream>

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

#define RENEW_DANGEROUS_EDGE 1
#define DANGEROUS_EDGE_CLOSURE 1

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
    for(int rep = 0; rep < nodes.size(); rep++)
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
    if(tail_head_to_inactive_lit.size() <= a)
        return lit_Error;
    if(tail_head_to_inactive_lit[a].size() <= b)
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
    return name.substr(0, name.find_first_of('#'));
}

int closure::get_node(std::string name)
{
    for(int i = 0; i < nodes.size(); i++)
        if(nodes[i].name == name)
            return i;

    int new_node_id = nodes.size();

    std::string address = get_address(name);
    int address_id = -1;

    for(int i = 0; i < id_to_address.size(); i++)
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

    if(tail_to_inactive_edges.size() <= u)
        tail_to_inactive_edges.resize(u + 1, std::vector<inactive_edge_t>());
    tail_to_inactive_edges[u].push_back(inactive_edge);

    if(head_to_inactive_edges.size() <= v)
        head_to_inactive_edges.resize(v + 1, std::vector<inactive_edge_t>());
    head_to_inactive_edges[v].push_back(inactive_edge);

    if(tail_head_to_inactive_lit.size() <= u)
        tail_head_to_inactive_lit.resize(u + 1, std::vector<Lit>());
    if(tail_head_to_inactive_lit[u].size() <= v)
        tail_head_to_inactive_lit[u].resize(v + 1, lit_Error);
    tail_head_to_inactive_lit[u][v] = l;

    if(kind == OCLT_RFI || kind == OCLT_RFE)
    {
        nodes[u].is_write = true;
        nodes[v].is_read = true;
    }
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

void closure::preventive_propagation(std::set<Lit>& assigned_lit)
{
    while(!unvisited_dangerous_edges.empty())
    {
        auto& unvisited_dangerous_edge = get_dangerous_edge(unvisited_dangerous_edges.back());
        unvisited_dangerous_edges.pop_back();
        unvisited_dangerous_edge.visited = true;

        auto from = unvisited_dangerous_edge.from;
        auto to = unvisited_dangerous_edge.to;

        if(OC_VERBOSITY >= 1)
            std::cout << "visiting dangerous edge " << from << " " << to << " " << unvisited_dangerous_edge.unvisited_reason << ", NOW!\n";

#if DANGEROUS_EDGE_CLOSURE
        if(unvisited_dangerous_edge.unvisited_reason == unvisited_reasont::newly_added_prop)
        {
            //transitive closure, only for newly added dangerous edges obtained from preventive propagation
            auto from_rep = union_find_parent(from);
            auto to_rep = union_find_parent(to);
            if(!has_edge(to, from))
            {
                for (auto& from_out_edge: nodes[from_rep].out)
                    for (auto& to_in_edge: nodes[to_rep].in)
                    {
                        auto from_out_rep = from_out_edge.to;
                        auto to_in_rep = to_in_edge.from;
                        if (has_edge(to_in_rep, from_out_rep))
                            continue;

                        for (auto from_out : nodes[from_out_rep].atomic_items)
                            for (auto to_in : nodes[to_in_rep].atomic_items) {
                                if (nodes[to_in].address != nodes[from_out].address)
                                    continue;

                                if (nodes[to_in].is_write) //ws, fr
                                {
                                    auto new_dangerous_reason = unvisited_dangerous_edge.dangerous_reason;
                                    append(new_dangerous_reason, from_out_edge.reason);
                                    append(new_dangerous_reason, to_in_edge.reason);

                                    if (!dangerous_edge_exist(from_out, to_in))
                                    {
                                        if (OC_VERBOSITY >= 1)
                                            std::cout << "new dangerous edge: " << from_out << " " << to_in
                                                      << ", by transitive closure\n";

                                        auto new_dangerous_edge_index = new_dangerous_edge(from_out, to_in, new_dangerous_reason, newly_added_closure);
                                        unvisited_dangerous_edges.push_back(new_dangerous_edge_index);
                                    }
                                }
                                else if (nodes[from_out].is_write) //rf, just set its rf_variable false
                                {
                                    auto rf_lit_cand = check_tail_head_to_inactive_lit(from_out,to_in);
                                    if(rf_lit_cand == lit_Error)
                                        continue;

                                    if (get_assignment(rf_lit_cand) != l_Undef)
                                        continue;
                                    if (contains(assigned_lit, rf_lit_cand));
                                    continue;

                                    auto unit_lv = unvisited_dangerous_edge.dangerous_reason;
                                    append(unit_lv, find_edge(from, from_out).reason);
                                    append(unit_lv, find_edge(to_in, to).reason);
                                    push_need(unit_lv, rf_lit_cand);

                                    neg(unit_lv);

                                    if (OC_VERBOSITY >= 1)
                                        std::cout << "during preventive propagation, set rf" << from_out << " " << to_in << " false\n";

                                    solver->assign_literal(~rf_lit_cand, unit_lv);
                                    assigned_lit.insert(rf_lit_cand);
                                }
                            }
                    }
            }
        }
#endif //DANGEROUS_EDGE_CLOSURE

        auto unvisited_reason = unvisited_dangerous_edge.unvisited_reason;
        if(nodes[from].is_write && nodes[to].is_write)
        {
            int w1 = from;
            int w2 = to;
            int w1_rep = union_find_parent(w1);
            int w2_rep = union_find_parent(w2);

            //only no rf
            if(nodes[w1].guard_lighted && unvisited_reason != rf_activated && tail_to_inactive_edges.size() > w2)
            {
                for(auto& inactive_rf: tail_to_inactive_edges[w2])
                {
                    if(get_assignment(inactive_rf.second) != l_Undef)
                        continue;

                    auto r = inactive_rf.first.second;
                    int r_rep = union_find_parent(r);

                    if(!has_edge(w1, r))
                        continue;

                    auto inactive_rf_reason = inactive_rf.second;

                    auto unit_lv = unvisited_dangerous_edge.dangerous_reason;
                    push_need(unit_lv, inactive_rf_reason);
                    push_need(unit_lv, nodes[w1].guard);
                    append(unit_lv, find_edge(w1, r).reason);
                    
                    neg(unit_lv);
                    
                    if(OC_VERBOSITY >= 1)
                        std::cout << "during preventive propagation, set rf" << inactive_rf.first.first << " " << inactive_rf.first.second << " false by ws\n";

                    solver->assign_literal(~inactive_rf_reason, unit_lv);
                }
            }

            closure_edget* active_rfs;
            int active_rf_num;
            if(unvisited_reason != rf_activated)
            {
                active_rfs = &(nodes[w2].out_rf[0]);
                active_rf_num = nodes[w2].out_rf.size();
            }
            else
            {
                for(auto& rf : nodes[w2].out_rf)
                    if(rf.to == unvisited_dangerous_edge.unvisited_rf_node)
                    {
                        active_rfs = &rf;
                        active_rf_num = 1;
                        break;
                    }
            }

            //only no guard(w1)
            if(!nodes[w1].guard_lighted && get_assignment(nodes[w1].guard) == l_Undef)
            {
                for(int i = 0; i < active_rf_num; i++)
                {
                    auto& active_rf = *(active_rfs + i);
                    auto r = active_rf.to;
                    int r_rep = union_find_parent(r);
                    
                    if(!has_edge(w1, r))
                        continue;
                    
                    auto unit_lv = unvisited_dangerous_edge.dangerous_reason;
                    append(unit_lv, active_rf.reason);
                    push_need(unit_lv, nodes[w1].guard);
                    append(unit_lv, find_edge(w1, r).reason);

                    neg(unit_lv);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "during preventive propagation, set guard of " << w1 << " false by ws\n";

                    solver->assign_literal(~nodes[w1].guard, unit_lv);
                }
            }

            //only no star edge
            if(nodes[w1].guard_lighted)
            {
                for(int i = 0; i < active_rf_num; i++)
                {
                    auto& active_rf = *(active_rfs + i);
                    auto r = active_rf.to;
                    
                    if(has_edge(w1, r))
                    {
                        //std::cout << "WARNING: During preventive propagation, w1 = " << w1 << ", w2 = " << w2 << ", r = " << r << "\n";
                        continue;
                    }
                    
                    auto new_dangerous_reason = unvisited_dangerous_edge.dangerous_reason;
                    new_dangerous_reason.push_back(nodes[w1].guard);
                    append(new_dangerous_reason, active_rf.reason);

                    if(!dangerous_edge_exist(w1, r))
                    {
                        if(OC_VERBOSITY >= 1)
                            std::cout << "new dangerous edge: " << w1 << " " << r << ", by ws\n";

                        auto new_dangerous_edge_index = new_dangerous_edge(w1, r, new_dangerous_reason, newly_added_prop);
                        unvisited_dangerous_edges.push_back(new_dangerous_edge_index);
                        //unvisited_dangerous_edge.implies.push_back(new_dangerous_edge_index);
                    }
                }
            }
        }

        if(nodes[from].is_read && nodes[to].is_write)
        {
            int r = from;
            int w2 = to;
            int r_rep = union_find_parent(r);
            int w2_rep = union_find_parent(w2);

            //only no rf
            if(nodes[w2].guard_lighted && unvisited_reason != rf_activated && head_to_inactive_edges.size() > r)
            {
                for(auto& inactive_rf: head_to_inactive_edges[r])
                {
                    if(get_assignment(inactive_rf.second) != l_Undef)
                        continue;
                    
                    auto w1 = inactive_rf.first.first;
                    int w1_rep = union_find_parent(w1);

                    if(w1 == w2 || !has_edge(w1, w2))
                        continue;

                    auto inactive_rf_reason = inactive_rf.second;

                    auto unit_lv = unvisited_dangerous_edge.dangerous_reason;
                    push_need(unit_lv, inactive_rf_reason);
                    push_need(unit_lv, nodes[w2].guard);
                    append(unit_lv, find_edge(w1, w2).reason);
                    
                    neg(unit_lv);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "during preventive propagation, set rf" << inactive_rf.first.first << " " << inactive_rf.first.second << " false by fr\n";

                    solver->assign_literal(~inactive_rf_reason, unit_lv);
                }
            }

            closure_edget* active_rfs;
            int active_rf_num;
            if(unvisited_reason != rf_activated)
            {
                active_rfs = &(nodes[r].in_rf[0]);
                active_rf_num = nodes[r].in_rf.size();
            }
            else
            {
                for(auto& rf : nodes[r].in_rf)
                    if(rf.from == unvisited_dangerous_edge.unvisited_rf_node)
                    {
                        active_rfs = &rf;
                        active_rf_num = 1;
                        break;
                    }
            }

            //only no guard(w2)
            if(!nodes[w2].guard_lighted && get_assignment(nodes[w2].guard) == l_Undef)
            {
                for(int i = 0; i < active_rf_num; i++)
                {
                    auto& active_rf = *(active_rfs + i);
                    auto w1 = active_rf.from;
                    int w1_rep = union_find_parent(w1);
                    
                    if(w1 == w2 || !has_edge(w1, w2))
                        continue;
                    
                    auto active_rf_reason = active_rf.reason;

                    auto unit_lv = unvisited_dangerous_edge.dangerous_reason;
                    append(unit_lv, active_rf_reason);
                    push_need(unit_lv, nodes[w2].guard);
                    append(unit_lv, find_edge(w1, w2).reason);
                    
                    neg(unit_lv);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "during preventive propagation, set guard of " << w2 << " false by fr\n";

                    solver->assign_literal(~nodes[w2].guard, unit_lv);
                }
            }

            //only no star edge
            if(nodes[w2].guard_lighted)
            {
                for(int i = 0; i < active_rf_num; i++)
                {
                    auto& active_rf = *(active_rfs + i);
                    auto w1 = active_rf.from;
                    
                    if(w1 == w2)
                        continue;

                    if(has_edge(w1, w2))
                    {
                        //std::cout << "WARNING: During preventive propagation, w1 = " << w1 << ", w2 = " << w2 << ", r = " << r << "\n";
                        continue;
                    }
                    
                    auto new_dangerous_reason = unvisited_dangerous_edge.dangerous_reason;
                    new_dangerous_reason.push_back(nodes[w2].guard);
                    append(new_dangerous_reason, active_rf.reason);

                    if(!dangerous_edge_exist(w1, w2))
                    {
                        if(OC_VERBOSITY >= 1)
                            std::cout << "new dangerous edge: " << w1 << " " << w2 << ", by fr\n";

                        auto new_dangerous_edge_index = new_dangerous_edge(w1, w2, new_dangerous_reason, newly_added_prop);
                        unvisited_dangerous_edges.push_back(new_dangerous_edge_index);
                        //unvisited_dangerous_edge.implies.push_back(new_dangerous_edge_index);
                    }
                }
            }
        }
    }
}

bool closure::activate_edge(int vi, int wi, edge_kindt kind, literal_vector& reason)
{
    if(OC_VERBOSITY >= 1)
        std::cout << "activating (" << vi << ", " << wi << ", " << kind_to_str(kind) << ")\n";

    std::set<Lit> assigned_lit;

    closure_edget edge(vi, wi, kind, reason);
    if(activate_directed_edge(edge, assigned_lit))
        return true;

    preventive_propagation(assigned_lit);
    
    return false;
}

bool closure::activate_directed_edge(closure_edget &edge, std::set<Lit>& assigned_lit, bool need_closure)
{
    auto begin = &(nodes[0]);
    auto kind = edge.kind;
    auto& reason = edge.reason;

    if(kind == OCLT_RFE || kind == OCLT_RFI)
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
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, edge_w2_r_rep_cand.reason));
            }

            for(int w2_cand: nodes[r].atomic_in)
                if(w2_cand != w1 && nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                    edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, empty_lv));

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

                closure_edget co(w2, w1, OCLT_NA, co_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << edge.from << " " << edge.to << " propagate " << co.from << " " << co.to
                              << " by co during rf\n";
                    std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                }

                if(activate_directed_edge(co, assigned_lit))
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
                        edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, edge_w1_w2_rep_cand.reason));
            }

            for(int w2_cand: nodes[w1].atomic_out)
                if(nodes[w2_cand].guard_lighted && nodes[w2_cand].address == address)
                    edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, empty_lv));

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

                closure_edget fr(r, w2, OCLT_NA, fr_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << edge.from << " " << edge.to << " propagate " << fr.from << " " << fr.to
                              << " by fr during rf\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if(activate_directed_edge(fr, assigned_lit))
                    return true;
            }
        }

#if RENEW_DANGEROUS_EDGE
        //when rf is added, some visited dangerous edges need to revisit
        for(int w2 : nodes[r].dangerous_out)
        {
            //revisit r->w2
            if(OC_VERBOSITY >= 1)
                std::cout << "old dangerous edge " << r << " " << w2 << " need to revisit, after rf " << w1 << " " << r << "\n";

            unvisited_dangerous_edges.push_back(renew_dangerous_edge(r, w2, rf_activated, w1));
        }

        for(auto w0 : nodes[w1].dangerous_in)
        {
            if(!nodes[w0].is_write)
                continue;

            //revisit w0->w1
            if(OC_VERBOSITY >= 1)
                std::cout << "old dangerous edge " << w0 << " " << w1 << " need to revisit, after rf " << w1 << " " << r << "\n";

            unvisited_dangerous_edges.push_back(renew_dangerous_edge(w0, w1, rf_activated, r));
        }
#endif //RENEW_DANGEROUS_EDGE
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
                std::cout << "adding " << edge.from << " " << edge.to << " a cycle due to epo\n";

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

#if (PROPAGATE_METHOD >= 0)
        //add dangerous edges
        for(auto to: nodes[to_rep].atomic_items)
            for(auto from: nodes[from_rep].atomic_items)
            {
                if(nodes[to].address != nodes[from].address)
                    continue;

                if(nodes[from].is_write) //ws, fr
                {
#if (PROPAGATE_METHOD >= 1)
                    if(!dangerous_edge_exist(to, from))
                    {
                        if(OC_VERBOSITY >= 1)
                            std::cout << "new dangerous edge " << to << " " << from << ", for normal edge addition\n";
                        
                        unvisited_dangerous_edges.push_back(new_dangerous_edge(to, from, reason, newly_added_closure));

                        if(OC_VERBOSITY >= 1)
                            std::cout << "newly added index is " << unvisited_dangerous_edges.back() << "\n" << "node size is " << nodes.size() << "\n";
                    }
#endif // (PROPAGATE_METHOD >= 1)
                }
                else if(nodes[to].is_write) //rf, just set its rf_variable false
                {
                    auto rf_lit_cand = check_tail_head_to_inactive_lit(to, from);
                    if(rf_lit_cand == lit_Error)
                        continue;

                    if (get_assignment(rf_lit_cand) != l_Undef)
                        continue;
                    if (contains(assigned_lit, rf_lit_cand))
                        continue;

                    literal_set unit_lv;
                    for(auto l: edge.reason)
                        unit_lv.insert(~l);
                    unit_lv.insert(~rf_lit_cand);

                    if(OC_VERBOSITY >= 1)
                        std::cout << "before preventive propagation, set rf" << to << " " << from << " false\n";

                    solver->assign_literal(~rf_lit_cand, unit_lv);
                    assigned_lit.insert(rf_lit_cand);
                }
            }
#endif // (PROPAGATE_METHOD >= 0)

        trail_edge.push(std::make_pair(from_rep, to_rep));

        if(need_closure)
        {
            auto back_edges = nodes[from_rep].in;
            back_edges.push_back(closure_edget(from_rep, from_rep, OCLT_NA, empty_lv));
            auto front_edges = nodes[to_rep].out;
            front_edges.push_back(closure_edget(to_rep, to_rep, OCLT_NA, empty_lv));

            for (auto &back_edge: back_edges)
                for (auto &front_edge: front_edges)
                {
                    if (has_edge(back_edge.from, front_edge.to)) //edge exists
                        continue;

                    literal_vector na_reason = back_edge.reason;
                    na_reason.insert(na_reason.end(), reason.begin(), reason.end());
                    na_reason.insert(na_reason.end(), front_edge.reason.begin(), front_edge.reason.end());

                    process_reason(na_reason);

                    closure_edget closure_edge(back_edge.from, front_edge.to, OCLT_NA, na_reason);

                    if(OC_VERBOSITY >= 1)
                        std::cout << edge.from << " " << edge.to << " propagate " << closure_edge.from << " " << closure_edge.to << " by transitivity\n";

                    if(activate_directed_edge(closure_edge, assigned_lit, false))
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

                            closure_edget co(w2, w1, OCLT_NA, co_reason);

                            if (OC_VERBOSITY >= 1)
                            {
                                std::cout << edge.from << " " << edge.to << " propagate " << w2 << " " << w1
                                          << " by co during na\n";
                                std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                            }

                            if (activate_directed_edge(co, assigned_lit))
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

                        closure_edget fr(r, w2, OCLT_NA, fr_reason);

                        //process_reason(fr_reason);

                        if (OC_VERBOSITY >= 1)
                        {
                            std::cout << edge.from << " " << edge.to << " propagate " << fr.from << " " << fr.to
                                      << " by fr during na\n";
                            std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                        }

                        if (activate_directed_edge(fr, assigned_lit))
                            return true;
                    }
                }
        }
    }

    return false;
}

bool closure::activate_epo(int u, int v) //always before activating other edges
{
    // if(union_check(u, v))
    //     std::cout << "WARNING: " << u << " and " << v << " are already together\n";

    union_join(u, v);

    return false;
}

void closure::atomic_remove_self()
{
    for(int i = 0; i < nodes.size(); i++)
    {
        nodes[i].atomic_in.erase(i);
        nodes[i].atomic_out.erase(i);
    }
}

bool closure::light_guard(int u)
{
    std::set<Lit> assigned_lit;

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
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, edge_w2_r_rep_cand.reason));
            }

            for(int w2_cand: nodes[r].atomic_in)
                if(nodes[w2_cand].guard_lighted)
                    edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, empty_lv));

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

                closure_edget co(w2, w1, OCLT_NA, co_reason);

                if (OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << co.from << " " << co.to
                              << " by co during light(this is w1)\n";
                    std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                }

                if (activate_directed_edge(co, assigned_lit))
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
                    edges_w2_r.push_back(closure_edget(w2, r_cand, OCLT_NA, edge_w2_r_rep_cand.reason));
        }

        for(int r_cand: nodes[w2].atomic_out)
            if(!nodes[r_cand].in_rf.empty())
                edges_w2_r.push_back(closure_edget(w2, r_cand, OCLT_NA, empty_lv));

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

                    closure_edget co(w2, w1, OCLT_NA, co_reason);

                    if(OC_VERBOSITY >= 1)
                    {
                        std::cout << "light " << u << " propagate " << co.from << " " << co.to << " by co during light(this is w2)\n";
                        std::cout << nodes[co.from].name << " " << nodes[co.to].name << "\n";
                    }

                    if(activate_directed_edge(co, assigned_lit))
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
                    edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, edge_w1_w2_rep_cand.reason));
        }

        for(int w2_cand: nodes[w1].atomic_out)
            if(nodes[w2_cand].guard_lighted)
                edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, empty_lv));

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

                closure_edget fr(r, w2, OCLT_NA, fr_reason);

                if(OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << fr.from << " " << fr.to
                              << " by fr during light(this is w1)\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if(activate_directed_edge(fr, assigned_lit))
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
                    edges_w1_w2.push_back(closure_edget(w1_cand, w2, OCLT_NA, edge_w1_w2_rep_cand.reason));
        }

        for(int w1_cand: nodes[w2].atomic_in)
            if(nodes[w1_cand].guard_lighted)
                edges_w1_w2.push_back(closure_edget(w1_cand, w2, OCLT_NA, empty_lv));

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

                closure_edget fr(r, w2, OCLT_NA, fr_reason);

                if (OC_VERBOSITY >= 1)
                {
                    std::cout << "light " << u << " propagate " << fr.from << " " << fr.to
                              << " by fr during light(this is w2)\n";
                    std::cout << nodes[fr.from].name << " " << nodes[fr.to].name << "\n";
                }

                if (activate_directed_edge(fr, assigned_lit))
                    return true;
            }
        }
    }

#if RENEW_DANGEROUS_EDGE
    //when guard lighted, some visited dangerous edges need to revisit
    int w = u;

    for(int w2 : nodes[w].dangerous_out)
    {
        //revisit w->w2
        if(OC_VERBOSITY >= 1)
            std::cout << "old dangerous edge " << w << " " << w2 << " need to revisit, after lighting " << w << "\n";

        unvisited_dangerous_edges.push_back(renew_dangerous_edge(w, w2, guard_activated));
    }

    for(int r : nodes[w].dangerous_in)
    {
        if(!nodes[r].is_read)
            continue;

        //revisit r->w
        if(OC_VERBOSITY >= 1)
            std::cout << "old dangerous edge " << r << " " << w << " need to revisit, after lighting " << w << "\n";

        unvisited_dangerous_edges.push_back(renew_dangerous_edge(r, w, guard_activated));
    }
#endif //RENEW_DANGEROUS_EDGE
    preventive_propagation(assigned_lit);

    return false;
}

void closure::push_scope()
{
    trail_rf_lim.push(trail_rf.size());
    trail_edge_lim.push(trail_edge.size());
    trail_light_guard_lim.push(trail_light_guard.size());
    trail_dangerous_edge_lim.push(trail_dangerous_edge.size());
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

    for (int c = trail_light_guard.size() - 1; c >= trail_light_guard_lim[new_level]; c--)
    {
        auto u = trail_light_guard[c];
        nodes[u].guard_lighted = false;
    }
    trail_light_guard.shrink(trail_light_guard.size() - trail_light_guard_lim[new_level]);
    trail_light_guard_lim.shrink(trail_light_guard_lim.size() - new_level);

    unvisited_dangerous_edges.clear();

    for (int c = trail_dangerous_edge.size() - 1; c >= trail_dangerous_edge_lim[new_level]; c--)
    {
        auto dangerous_edge = trail_dangerous_edge[c];
        remove_dangerous_edge(dangerous_edge.first, dangerous_edge.second);
    }
    trail_dangerous_edge.shrink(trail_dangerous_edge.size() - trail_dangerous_edge_lim[new_level]);
    trail_dangerous_edge_lim.shrink(trail_dangerous_edge_lim.size() - new_level);
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

void closure::show_rf()
{
    if(OC_VERBOSITY < 1)
        return;

    std::cout << "current lighted nodes:";
    for(int i = 0; i < nodes.size(); i++)
        if(nodes[i].guard_lighted || !nodes[i].in_rf.empty())
            std::cout << i << " ";
    std::cout << "\n";

    std::cout << "current rf:\n";

    for(auto& node: nodes)
    {
        for(auto& rf: node.out_rf)
            std::cout << rf.from << "(" << nodes[rf.from].name << ") " << rf.to << "(" << nodes[rf.to].name << ")\n";
    }

    std::cout << "current outs:\n";
    for(int i = 0; i < nodes.size(); i++)
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
    for(int i = 0; i < nodes.size(); i++)
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

closure::simple_nodet closure::simplify_node(int node_id)
{
    auto& node = nodes[node_id];
    auto& represent_node = nodes[union_find_parent(node_id)];

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
    for(int i = 0; i < nodes.size(); i++)
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
    for(int i = 0; i < simple_nodes.size(); i++)
        if(simple_nodes[i].in_degree == 0 && is_representative(i))
            zero_degree_nodes.push_back(i);

    int visited_node_num = 0;

    while(!zero_degree_nodes.empty())
    {
        int visiting_node = zero_degree_nodes.back();
        zero_degree_nodes.pop_back();
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
            if(simple_nodes[item].is_write && 
                name.find("__CPROVER") == std::string::npos && 
                name.find("argv'") == std::string::npos)
                write_order.push_back(name);
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
                            edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, edge_w2_r_rep_cand.reason));
                }

                for (int w2_cand: nodes[r].atomic_in)
                    if (nodes[w2_cand].guard_lighted)
                        edges_w2_r.push_back(closure_edget(w2_cand, r, OCLT_NA, empty_lv));

                for (auto &edge_w2_r: edges_w2_r)
                {
                    auto w2 = edge_w2_r.from;

                    if (w2 == w1 || nodes[w2].address != nodes[w1].address || has_edge(w2, w1))
                        continue;

                    literal_vector co_reason = edge.reason;
                    co_reason.insert(co_reason.end(), edge_w2_r.reason.begin(), edge_w2_r.reason.end());
                    push_need(co_reason, nodes[w1].guard);
                    push_need(co_reason, nodes[w2].guard);

                    closure_edget co(w2, w1, OCLT_NA, co_reason);

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
                            edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, edge_w1_w2_rep_cand.reason));
                }

                for (int w2_cand: nodes[w1].atomic_out)
                    if (nodes[w2_cand].guard_lighted)
                        edges_w1_w2.push_back(closure_edget(w1, w2_cand, OCLT_NA, empty_lv));

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

                    closure_edget fr(r, w2, OCLT_NA, fr_reason);

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
    return std::make_pair(std::make_pair(-1, -1), OCLT_PO);
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