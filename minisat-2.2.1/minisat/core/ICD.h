// __SZH_ADD_BEGIN__

#include "ICDTypes.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

#include <map>
#include <list>

namespace Minisat
{
class ICDSolver;

typedef std::pair<std::pair<int, int>, edge_kindt> decide_entryt;

class ICD
{
private:
    //interact with solver
    ICDSolver* solver;
    lbool get_assignment(Lit l);

    //basic information
public:
    std::vector<ICD_nodet> nodes;
private:
    std::map<Lit, decide_entryt> lit_to_edge;

    //for backtracking
    vec<std::pair<int, int>> trail_edge;
    vec<int> trail_edge_lim;

    //union operations
    void union_join(int m, int n);

    //methods to add/erase an edge
    bool has_edge(int u, int v);

    typedef std::pair<std::pair<int, int>, Lit> inactive_edge_t;

    std::map<int, std::list<inactive_edge_t>> tail_to_inactive_edges;

public:
    ICD();
    void init(ICDSolver* _solver);

    int get_node(std::string name); //get node, if the node does not exist, create it

    void init_reasonable_edge(int u, int v, edge_kindt kind, Lit l);

    bool add_guard_literal(Lit guard_literal, int u);
    std::vector<int> check_guard_literal(Lit guard_literal);

    // Auxiliary functions for activate_directed_edge
    void more_fathers(ICD_nodet* child, ICD_fathert father);
    void all_cycles_backward_search(bool* visited, ICD_nodet* v, ICD_nodet* w);
    void all_cycles_forward_search(bool* visited, ICD_nodet* v, ICD_nodet* w);
    void refine_valid_in(bool* visited_backward, bool* visited_forward);
    void generate_all_reasons(ICD_nodet* v, ICD_nodet* w);
    void trace_father(std::vector<int>& related_nodes, literal_vector& related_reasons, ICD_nodet* start, ICD_nodet* end, bool is_forward);
    void get_nodes_reasons(std::vector<int>& related_nodes, literal_vector& related_reasons, ICD_nodet* backward_link, ICD_nodet* forward_link, ICD_nodet* v, ICD_nodet* w);

    bool activate_directed_edge(ICD_edget edge);

    bool activate_edge(int u, int v, edge_kindt kind, ICD_reasont reason);
    bool activate_epo(int u, int v);

    void atomic_remove_self();

    bool light_guard(int u);

    void push_scope();
    void pop_scope(int new_level);

    void show_edges();
    void final_check();

    std::vector<literal_vector> conflict_literals_vector;

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
    bool remove_inactive_edges(int from, int to, Lit p);
};
}

// __SZH_ADD_END__