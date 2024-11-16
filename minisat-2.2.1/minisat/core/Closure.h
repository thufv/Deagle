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
    vec<std::pair<int, int>> trail_vital;
    vec<int> trail_vital_lim;
    vec<int> trail_light_guard;
    vec<int> trail_light_guard_lim;
    vec<std::pair<Lit, Lit>> trail_triplet;
    vec<int> trail_triplet_lim;

public:
    struct triplett
    {
        int w1;
        int w2;
        int r;
        Lit rf_lit;
        Lit guard_lit;

        triplett(int _w1, int _w2, int _r, Lit _rf_lit, Lit _guard_lit) :
            w1(_w1), w2(_w2), r(_r), rf_lit(_rf_lit), guard_lit(_guard_lit) {}
    };

    std::vector<triplett> triplets_to_check;

private:
    std::map<Lit, std::vector<triplett>> rf_to_triplets;
    std::map<Lit, std::vector<triplett>> guard_to_triplets;

    //union operations
    void union_join(int m, int n);

    //methods to add/erase an edge
    bool has_edge(int u, int v);
    closure_edget& find_edge(int u, int v);

    typedef std::pair<std::pair<int, int>, Lit> inactive_edge_t;

    std::vector<std::vector<inactive_edge_t>> tail_to_inactive_edges;
    std::vector<std::vector<inactive_edge_t>> head_to_inactive_edges;
    std::vector<std::vector<Lit>> tail_head_to_inactive_lit;

    typedef std::pair<int, Lit> inactive_race_t;

    std::map<std::pair<int, int>, Lit> pair_to_inactive_races; // pair.first < pair.second

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

    void init_race(int u, int v, Lit l);

    bool add_guard_literal(Lit guard_literal, int u);
    std::vector<int> check_guard_literal(Lit guard_literal);

    bool activate_edge(int u, int v, edge_kindt kind, literal_vector& reason);
    bool activate_directed_edge(closure_edget &edge, bool need_closure = true);
    bool activate_apo(int u, int v);

    void preventive_propagation();

    void add_trinary_pattern(int w1, int w2, int r);
    void check_trinary_pattern();

    void atomic_remove_self();

    bool light_guard(int u);

    void push_scope();
    void pop_scope(int new_level);

    bool co_complete_check();

    void show_edges();
    void show_rf();
    void show_model();
    void show_race();
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