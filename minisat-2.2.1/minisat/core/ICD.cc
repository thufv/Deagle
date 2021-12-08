// __SZH_ADD_BEGIN__

#include "ICDSolver.h"
#include <deque>
#include <algorithm>
#include <iostream>

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

const int OC_VERBOSITY = 0;

#define push_need(lv, l) {if(l.x != -1) lv.push_back(l);}

#define DFS 0
#define BFS 1
#define PFS 2 //the best
#define SEARCH_METHOD PFS

using namespace Minisat;

template<typename T> bool contains(std::set<T> s, T elem)
{
    return s.find(elem) != s.end();
}

lbool ICD::get_assignment(Lit l)
{
    return solver->value(l);
}

int ICD::union_find_parent(int n)
{
    if(nodes[n].atomic_parent == n)
        return n;
    else
        return nodes[n].atomic_parent = union_find_parent(nodes[n].atomic_parent);
}

void ICD::union_join(int m, int n)
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
}

ICD::ICD() {}

void ICD::init(ICDSolver* _solver)
{
    solver = _solver;
    nodes = std::vector<ICD_nodet>();
}

int ICD::get_node(std::string name) //create node if not exist
{
    for(int i = 0; i < nodes.size(); i++)
        if(nodes[i].name == name)
            return i;

    int new_node_id = nodes.size();
    nodes.push_back(ICD_nodet(name, new_node_id));

    if(OC_VERBOSITY >= 1)
        std::cout << "creating " << name << "\n";

    return new_node_id;
}

void ICD::init_reasonable_edge(int u, int v, edge_kindt kind, Lit l)
{
    lit_to_edge[l] = std::make_pair(std::make_pair(u, v), kind);
    inactive_edge_t inactive_edge = std::make_pair(std::make_pair(u, v), l);
    tail_to_inactive_edges[u].push_back(inactive_edge);
}

bool ICD::activate_edge(int u, int v, edge_kindt kind, ICD_reasont reason)
{
    if(OC_VERBOSITY >= 1)
        std::cout << "activating (" << u << ", " << v << ", " << kind_to_str(kind) << ")\n";

    ICD_edget edge(u, v, kind, reason);
    return activate_directed_edge(edge);
}

bool ICD::activate_epo(int u, int v) //always before activating other edges
{
    // if(union_check(u, v))
    //     std::cout << "WARNING: " << u << " and " << v << " are already together\n";

    union_join(u, v);

    ICD_edget edge_forward(u, v, OCLT_PO);
    ICD_edget edge_backward(u, v, OCLT_PO);

    return activate_directed_edge(edge_forward) || activate_directed_edge(edge_backward);
}

void ICD::push_scope()
{
    trail_edge_lim.push(trail_edge.size());
}

void ICD::pop_scope(int new_level)
{
    for (int c = trail_edge.size() - 1; c >= trail_edge_lim[new_level]; c--)
    {
        auto e = trail_edge[c];
        nodes[e.first].out.pop_back();
        nodes[e.second].in.pop_back();
    }
    trail_edge.shrink(trail_edge.size() - trail_edge_lim[new_level]);
    trail_edge_lim.shrink(trail_edge_lim.size() - new_level);
}

void ICD::show_edges()
{
    if(OC_VERBOSITY < 1)
        return;

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

decide_entryt ICD::get_decide_entry(Lit l)
{
    if(lit_to_edge.find(l) != lit_to_edge.end())
        return lit_to_edge[l];
    return std::make_pair(std::make_pair(-1, -1), OCLT_PO);
}

bool ICD::use_available_info()
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
            ICD_reasont r(lit);
            ICD_edget edge(e.first.first, e.first.second, e.second, r);
            if(activate_edge(edge.from, edge.to, edge.kind, edge.reason))
                return true;

            it = lit_to_edge.erase(it);

            for(auto it2 = tail_to_inactive_edges[edge.from].begin(); it2 != tail_to_inactive_edges[edge.from].end(); it2++)
            {
                if(it2->second == lit)
                {
                    tail_to_inactive_edges[edge.from].erase(it2);
                    break;
                }
            }
        }
        else
            it++;
    }

    return false;
}

int ICD::get_decision_level()
{
    return trail_edge_lim.size();
}

bool ICD::remove_inactive_edges(int from, int to, Lit p)
{
    lit_to_edge.erase(p);

    for(auto it = tail_to_inactive_edges[from].begin(); it != tail_to_inactive_edges[from].end(); it++)
    {
        if(it->second == p)
        {
            tail_to_inactive_edges[from].erase(it);
            return true;
        }
    }

    return true;
}

bool oclt_access(edge_kindt kind)
{
    return kind == OCLT_COE || kind == OCLT_RFE || kind == OCLT_FR;
}

bool oclt_can_be_ignored(edge_kindt kind)
{
    return kind == OCLT_RFI || kind == OCLT_COI;
}

std::vector<ICD_nodet*> po_trace(ICD_nodet* n)
{
    std::vector<ICD_nodet*> tovisit;
    std::vector<ICD_nodet*> ret;
    tovisit.push_back(n);

    while(!tovisit.empty())
    {
        auto curr = tovisit.back();
        tovisit.pop_back();

        bool is_terminal = true;
        for(auto father : curr->valid_out)
        {
            if(!oclt_access(father.kind) && father.n != curr)
            {
                is_terminal = false;
                tovisit.push_back(father.n);
            }
        }
        if(is_terminal)
            ret.push_back(curr);
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

    bool subset(const std::vector<ICD_nodet*>& r1, const std::vector<ICD_nodet*>& r2)
    {
        if(r1.size() > r2.size())
            return false;
        unsigned i = 0, j = 0;
        for (i = 0; i < r1.size(); i++) {
            for (; j < r2.size(); j++) {
                if (r2[j] == r1[i]) {
                    break;
                }
            }
            if (j == r2.size())
                return false;
            else
                j++;
        }
        return true;
    }

void ICD::more_fathers(ICD_nodet* child, ICD_fathert father)
{
    bool should_add = false;

    if(oclt_access(father.kind))
        should_add = true;
    else
    {
        auto father_po = po_trace(father.n);
        auto child_po = po_trace(child);
        should_add = !subset(father_po, child_po);
    }

    if(should_add)
    {
        child->valid_out.push_back(father);

        father.n->valid_in.push_back(father);
        father.n->valid_in.back().n = child;
    }
}

void ICD::all_cycles_backward_search(bool* visited, ICD_nodet* v, ICD_nodet* w)
{
    ICD_nodet* begin = &(nodes[0]);

    std::deque<ICD_nodet*> tovisit;
    tovisit.push_front(v);

    std::deque<ICD_fathert> tovisit_father;
    tovisit_father.push_front(ICD_fathert(nullptr, ICD_reasont(), OCLT_PO));

    while(!tovisit.empty())
    {
        ICD_nodet* curr = tovisit.front();
        tovisit.pop_front();
        ICD_fathert father = tovisit_father.front();
        tovisit_father.pop_front();

        int cost;
        if(father.n)
            cost = oclt_access(father.kind) ? father.n->cost + 1 : father.n->cost;
        else
            cost = 0;

        if (!visited[curr - begin])
        {
            visited[curr - begin] = true;
            curr->cost = cost;

            curr->valid_out.clear();
            curr->valid_in.clear();

            if(father.n)
            {
                curr->valid_out.push_back(father);
                father.n->valid_in.push_back(father);
                father.n->valid_in.back().n = curr;
            }

            if(curr == w)
                continue;

            for (auto pred : curr->in)
            {
                auto pred_node = pred.from + begin;
                if(!oclt_access(pred.kind))
                {
                    tovisit.push_front(pred_node);
                    tovisit_father.push_front(ICD_fathert(curr, oclt_can_be_ignored(pred.kind) ? ICD_reasont() : pred.reason, pred.kind));
                }
                else
                {
                    tovisit.push_back(pred_node);
                    tovisit_father.push_back(ICD_fathert(curr, oclt_can_be_ignored(pred.kind) ? ICD_reasont() : pred.reason, pred.kind));
                }
            }
        }
        else
        {
            if(cost > curr->cost)
                continue;
            
            if(cost < curr->cost)
                std::cout << "ERROR: An edge cost lower and is visited later!\n";

            more_fathers(curr, father);
        }
    }
}

void ICD::all_cycles_forward_search(bool* visited, ICD_nodet* v, ICD_nodet* w)
{
    ICD_nodet* begin = &(nodes[0]);

    std::vector<ICD_nodet*> tovisit;
    tovisit.push_back(w);
    visited[w - begin] = true;

    while(!tovisit.empty())
    {
        auto curr = tovisit.back();
        tovisit.pop_back();

        curr->all_reasons.clear();
        for(auto succ: curr->valid_out)
        {
            if(!visited[succ.n - begin])
            {
                visited[succ.n - begin] = true;
                tovisit.push_back(succ.n);
            }
        }
    }
}

void ICD::refine_valid_in(bool* visited_backward, bool* visited_forward)
{
    ICD_nodet* begin = &(nodes[0]);
    auto node_num = nodes.size();

    for(int i = 0; i < node_num; i++)
    {
        if(!visited_backward[i] || !visited_forward[i])
            continue;

        std::vector<ICD_fathert> refined_valid_in;
        for(auto in: nodes[i].valid_in)
            if(visited_forward[in.n - begin])
                refined_valid_in.push_back(in);

        nodes[i].valid_in = refined_valid_in;
        nodes[i].valid_in_num = refined_valid_in.size();
    }
}

void ICD::generate_all_reasons(ICD_nodet* v, ICD_nodet* w)
{
    ICD_nodet* begin = &(nodes[0]);

    std::vector<ICD_nodet*> tovisit;
    tovisit.push_back(w);

    if(w->valid_in_num != 0)
        std::cout << "ERROR: valid_in_num of w " << w - begin << " is " << w->valid_in_num << "\n";

    while(!tovisit.empty())
    {
        auto curr = tovisit.back();
        tovisit.pop_back();

        for(auto in: curr->valid_in)
        {
            if(in.n->all_reasons.empty())
            {
                if(in.reason.num > 0)
                {
                    literal_vector new_lv;
                    new_lv << in.reason;
                    curr->all_reasons.push_back(new_lv);
                }
            }
            else
            {
                for(auto reason: in.n->all_reasons)
                {
                    auto new_reason = reason;
                    new_reason << in.reason;
                    curr->all_reasons.push_back(new_reason);
                }
            }
        }

        for(auto out: curr->valid_out)
        {
            out.n->valid_in_num--;
            if(out.n->valid_in_num == 0)
                tovisit.push_back(out.n);
        }
    }
}

void ICD::trace_father(std::vector<int>& related_nodes, literal_vector& related_reasons, ICD_nodet* start, ICD_nodet* end, bool is_forward)
{
    ICD_nodet* begin = &(nodes[0]);

    std::vector<int> order;

    ICD_nodet* curr = start;
    ICD_fathert father = is_forward ? curr->forward_father : curr->backward_father;
    order.push_back(curr - begin);
    
    while(curr != end)
    {
        related_reasons << father.reason;
        curr = father.n;
        father = is_forward ? curr->forward_father : curr->backward_father;
        order.push_back(curr - begin);
    }

    if(is_forward)
    {
        for(auto it = order.rbegin(); it != order.rend(); it++)
            related_nodes.push_back(*it);
    }
    else
    {
        for(auto it = order.begin(); it != order.end(); it++)
            related_nodes.push_back(*it);
    }
}

void ICD::get_nodes_reasons(std::vector<int>& related_nodes, literal_vector& related_reasons, ICD_nodet* backward_link, ICD_nodet* forward_link, ICD_nodet* v, ICD_nodet* w)
{
    trace_father(related_nodes, related_reasons, backward_link, v, false); 
    trace_father(related_nodes, related_reasons, forward_link, w, true);
}

bool ICD::activate_directed_edge(ICD_edget edge) //true: cycle detected
{
    ICD_nodet* begin = &(nodes[0]);

    bool cycle = false;

    //test order
    auto u = edge.from;
    auto v = edge.to;
    auto u_node = u + begin;
    auto v_node = v + begin;

    u_node->backward_father = ICD_fathert(u_node, ICD_reasont(), OCLT_PO);
    v_node->forward_father = ICD_fathert(v_node, ICD_reasont(), OCLT_PO);

    //search backward
    static bool visited[MAX_NODES];
    memset(visited, false, sizeof(bool) * nodes.size());

    std::deque<ICD_nodet*> tovisit;
    tovisit.push_front(u_node);

    std::deque<ICD_fathert> tovisit_father;
    tovisit_father.push_front(ICD_fathert(u_node, ICD_reasont(), OCLT_PO));

    while (!tovisit.empty())
    {
        ICD_nodet* curr = tovisit.front();
        tovisit.pop_front();
        ICD_fathert father = tovisit_father.front();
        tovisit_father.pop_front();

        if (!visited[curr - begin])
        {
            visited[curr - begin] = true;

            curr->backward_father = father;

            bool found_cycle = false;

            for (auto pred : curr->in)
            {
                if (pred.from == v)
                {
                    cycle = true;
                    found_cycle = true;
                    break; //new
                }
                ICD_nodet* pred_node = begin + pred.from;
                auto is_access = oclt_access(pred.kind);

                if(SEARCH_METHOD == DFS || (SEARCH_METHOD == PFS && !is_access))
                {
                    tovisit.push_front(pred_node);
                    tovisit_father.push_front(ICD_fathert(curr, oclt_can_be_ignored(pred.kind) ? ICD_reasont() : pred.reason, pred.kind));
                }
                if(SEARCH_METHOD == BFS || (SEARCH_METHOD == PFS && is_access))
                {
                    tovisit.push_back(pred_node);
                    tovisit_father.push_back(ICD_fathert(curr, oclt_can_be_ignored(pred.kind) ? ICD_reasont() : pred.reason, pred.kind));
                }
            }

            if(found_cycle)
                break;
        }
    }

    if(cycle) //set conflict
    {
        if(union_check(u, v) && (edge.kind == OCLT_PO || u_node->atomic_out.find(v) != u_node->atomic_out.end()))
        {
            cycle = false;
        }
        else
        {
            if(OC_VERBOSITY >= 1)
                std::cout << "true cycle\n";

            bool visited_backward[MAX_NODES];
            memset(visited_backward, 0, sizeof(bool) * nodes.size());
            all_cycles_backward_search(visited_backward, u_node, v_node);

            bool visited_forward[MAX_NODES];
            memset(visited_forward, 0, sizeof(bool) * nodes.size());
            all_cycles_forward_search(visited_forward, u_node, v_node);

            refine_valid_in(visited_backward, visited_forward);

            generate_all_reasons(u_node, v_node);

            auto& all_reasons = u_node->all_reasons;

            if(all_reasons.empty())
            {
                all_reasons.push_back(literal_vector());
            }

            for(auto reasons: all_reasons)
            {
                reasons << edge.reason;
                conflict_literals_vector.push_back(reasons);

                if(OC_VERBOSITY >= 1)
                {
                    for(auto lit: reasons)
                        std::cout << lit.x << " ";
                    std::cout << "is one of the cycle\n";
                }
            }
        }
    }

    if (!cycle) //enclosure
    {
        //propagate
        std::vector<literal_vector> literals_to_add;

        //false propagation
        auto& inactive_edges = tail_to_inactive_edges[v];
        for(auto& inactive_edge: inactive_edges)
        {
            Lit inactive_l = inactive_edge.second;
            if(get_assignment(inactive_l) != l_Undef)
                continue;

            int inactive_u = inactive_edge.first.second;
            int inactive_v = inactive_edge.first.first;

            if(!visited[inactive_u])
                continue;

            std::vector<int> related_nodes;
            literal_vector related_reasons;
            get_nodes_reasons(related_nodes, related_reasons, &(nodes[inactive_u]), &(nodes[inactive_v]), u_node, v_node);

            for(auto& l: related_reasons)
                l = ~l;

            if(!oclt_can_be_ignored(edge.kind))
            {
                ICD_reasont neg_reason = ~edge.reason;
                for(auto& lv: literals_to_add)
                    lv << neg_reason;
            }

            related_reasons.push_back(~inactive_l);

            literals_to_add.push_back(related_reasons);

            if(OC_VERBOSITY >= 1)
                std::cout << "propagate " << inactive_v << " " << inactive_u << " is not ok";
        }

        for(auto& lv: literals_to_add)
        {
            solver->assign_literal(lv.back(), lv);
        }
    }

    trail_edge.push(std::make_pair(u, v));
    u_node->out.push_back(edge);
    v_node->in.push_back(edge);

    if(!cycle)
    {
        std::vector<std::pair<int, int>> from_reads;
        std::vector<ICD_reasont> from_reads_reason;
        if(edge.kind == OCLT_RFI || edge.kind == OCLT_RFE)
        {
            for(auto& succ: u_node->out)
                if(succ.kind == OCLT_COI || succ.kind == OCLT_COE)
                {
                    from_reads.push_back(std::make_pair(v, succ.to));
                    from_reads_reason.push_back(ICD_reasont(edge.reason, succ.reason));
                    
                    if(OC_VERBOSITY >= 1)
                        std::cout << "found a fr, related reasons are " << edge.reason << " " << succ.reason << " its reason is " << ICD_reasont(edge.reason, succ.reason) << "\n";
                }
        }
        else if(edge.kind == OCLT_COI || edge.kind == OCLT_COE)
        {
            for(auto& succ: u_node->out)
                if(succ.kind == OCLT_RFI || succ.kind == OCLT_RFE)
                {
                    from_reads.push_back(std::make_pair(succ.to, v));
                    from_reads_reason.push_back(ICD_reasont(edge.reason, succ.reason));

                    if(OC_VERBOSITY >= 1)
                        std::cout << "found a fr, related reasons are " << edge.reason << " " << succ.reason << " its reason is " << ICD_reasont(edge.reason, succ.reason) << "\n";
                }
        }

        for(int i = 0; i < from_reads.size(); i++)
        {
            auto& from_read = from_reads[i];
            auto from_read_reason = from_reads_reason[i];

            if(OC_VERBOSITY >= 1)
                std::cout << "(" << from_read.first << ", " << from_read.second << ", " << from_read_reason << ") is a from read\n";

            ICD_edget fr_edge(from_read.first, from_read.second, OCLT_FR, from_read_reason);
            cycle = activate_directed_edge(edge);

            if(cycle)
            {
                if(OC_VERBOSITY >= 1)
                    std::cout << "Find cycle when adding an fr\n";

                return true;
            }
            else
            {
                if(OC_VERBOSITY >= 1)
                    std::cout << "No cycle when adding an fr\n";
            }
        }
    }
    return cycle;
}

// __SZH_ADD_END__