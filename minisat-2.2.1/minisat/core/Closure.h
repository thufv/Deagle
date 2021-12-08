// __SZH_ADD_BEGIN__

#include "ClosureTypes.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

#include <map>
#include <list>

namespace Minisat
{
class ClosureSolver;

typedef std::pair<std::pair<int, int>, edge_kindt> decide_entryt;

class closure
{
private:
    //interact with solver
    ClosureSolver* solver;
    lbool get_assignment(Lit l);

    //basic information
public:
    std::vector<closure_nodet> nodes;
private:
    std::map<Lit, decide_entryt> lit_to_edge;
    std::multimap<Lit, int> guard_lit_to_node;

    std::vector<std::string> id_to_address;

    //for backtracking
    vec<std::pair<int, int>> trail_edge;
    vec<int> trail_edge_lim;
    vec<std::pair<int, int>> trail_rf;
    vec<int> trail_rf_lim;
    vec<int> trail_light_guard;
    vec<int> trail_light_guard_lim;
    vec<std::pair<int, int>> trail_dangerous_edge;
    vec<int> trail_dangerous_edge_lim;

    //unvisited dangerous edges
    enum unvisited_reasont {newly_added_closure, newly_added_prop, guard_activated, rf_activated};
    typedef int dangerous_edge_indext;

    struct dangerous_edget
    {
        int from; //if from == -1, this dangerous edge does not exist
        int to;
        literal_vector dangerous_reason;
        unvisited_reasont unvisited_reason; //only for unvisited_reason
        int unvisited_rf_node; //only when unvisited_reason == rf_activated

        bool visited;

        dangerous_edget(int _from = -1, int _to = -1, literal_vector _dangerous_reason = literal_vector(), unvisited_reasont _unvisited_reason = newly_added_closure):
            from(_from), to(_to), dangerous_reason(_dangerous_reason), unvisited_reason(_unvisited_reason), unvisited_rf_node(-1), visited(false) {}
    };

    std::vector<dangerous_edget> dangerous_edge_pool;

    dangerous_edge_indext to_dangerous_edge_index(int from, int to)
    {
        return from * nodes.size() + to;
    }

    bool dangerous_edge_exist(int from, int to)
    {
        dangerous_edge_indext index = to_dangerous_edge_index(from, to);

        if(dangerous_edge_pool.size() <= index)
            dangerous_edge_pool.resize(index + 1);

        return dangerous_edge_pool[index].from != -1;
    }

    dangerous_edge_indext new_dangerous_edge(int from, int to, literal_vector& dangerous_reason, unvisited_reasont unvisited_reason)
    {
        dangerous_edge_indext index = to_dangerous_edge_index(from, to);

        if(dangerous_edge_pool.size() <= index)
            dangerous_edge_pool.resize(index + 1);

        dangerous_edge_pool[index] = dangerous_edget(from, to, dangerous_reason, unvisited_reason);

        trail_dangerous_edge.push(std::make_pair(from, to));

        nodes[from].dangerous_out.push_back(to);
        nodes[to].dangerous_in.push_back(from);

        return index;
    }

    dangerous_edge_indext renew_dangerous_edge(int from, int to, unvisited_reasont unvisited_reason, int unvisited_rf_node = -1)
    {
        dangerous_edge_indext index = to_dangerous_edge_index(from, to);
        if(dangerous_edge_pool.size() <= index)
            dangerous_edge_pool.resize(index + 1);

        dangerous_edge_pool[index].unvisited_reason = unvisited_reason;
        dangerous_edge_pool[index].unvisited_rf_node = unvisited_rf_node;
        dangerous_edge_pool[index].visited = false;

        return index;
    }

    dangerous_edget& get_dangerous_edge(int from, int to)
    {
        dangerous_edge_indext index = to_dangerous_edge_index(from, to);
        return dangerous_edge_pool[index];
    }

    dangerous_edget& get_dangerous_edge(dangerous_edge_indext index)
    {
        return dangerous_edge_pool[index];
    }

    void remove_dangerous_edge(int from, int to)
    {
        dangerous_edge_indext index = to_dangerous_edge_index(from, to);
        dangerous_edge_pool[index].from = -1;
        nodes[from].dangerous_out.pop_back();
        nodes[to].dangerous_in.pop_back();
    }

    std::vector<dangerous_edge_indext> unvisited_dangerous_edges;

    //union operations
    void union_join(int m, int n);

    //methods to add/erase an edge
    bool has_edge(int u, int v);
    closure_edget& find_edge(int u, int v);

    typedef std::pair<std::pair<int, int>, Lit> inactive_edge_t;

    std::vector<std::vector<inactive_edge_t>> tail_to_inactive_edges;
    std::vector<std::vector<inactive_edge_t>> head_to_inactive_edges;
    std::vector<std::vector<Lit>> tail_head_to_inactive_lit;

    Lit check_tail_head_to_inactive_lit(int a, int b);

    //for generating model
    struct simple_nodet
    {
        std::string name;
        bool is_write;
        int in_degree;
        std::vector<int> out;
    };
    simple_nodet simplify_node(int node_id);

public:
    closure();
    void init(ClosureSolver* _solver);

    int get_node(std::string name); //get node, if the node does not exist, create it

    void init_reasonable_edge(int u, int v, edge_kindt kind, Lit l);

    bool add_guard_literal(Lit guard_literal, int u);
    std::vector<int> check_guard_literal(Lit guard_literal);

    bool activate_edge(int u, int v, edge_kindt kind, literal_vector& reason);
    bool activate_directed_edge(closure_edget &edge, std::set<Lit>& assigned_lit, bool need_closure = true);
    bool activate_epo(int u, int v);

    void preventive_propagation(std::set<Lit>& assigned_lit);

    void atomic_remove_self();

    bool light_guard(int u);

    void push_scope();
    void pop_scope(int new_level);

    void show_edges();
    void show_rf();
    void final_check();
    literal_vector conflict_lv;

    int union_find_parent(int n);
    bool union_check(int m, int n) { return union_find_parent(m) == union_find_parent(n); }
    bool is_representative(int n) {return union_find_parent(n) == n;}
    void refine_atomic_atoms();

    void filter_writes(std::set<int>& src, std::vector<int>& dst); //filter those guard_lighted
    void filter_reads(std::set<int>& src, std::vector<int>& dst); //filter those have rf_in
    void filter_writes_reasons(std::set<int> &src, std::vector<std::pair<int, literal_vector>> &dst, literal_vector& reason);

    decide_entryt get_decide_entry(Lit l);
    bool use_available_info();
    int get_decision_level();
    bool remove_rf(int from, int to, Lit p);

    //std::vector<std::string> write_order;
};
}

// __SZH_ADD_END__