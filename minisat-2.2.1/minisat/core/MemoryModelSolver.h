// __SZH_ADD_BEGIN__

#ifndef MEMORYMODELSOLVER_H
#define MEMORYMODELSOLVER_H

#include <map>

#include "../../minisat/mtl/Queue.h"
#include "../../minisat/core/Solver.h"
#include "MemoryModelSolverStruct.h"

#include <cat/cat_module.h>

typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oc_edge_tablet;
typedef std::vector<std::pair<std::string, std::pair<Minisat::Lit, std::string> > > oc_label_tablet;
typedef std::map<std::string, int> oc_location_mapt;
typedef std::map<std::string, unsigned> oc_thread_mapt;

namespace Minisat
{

class MemoryModelSolver : public Solver
{
    oc_edge_tablet oc_edge_table;
    oc_label_tablet oc_label_table;
    oc_location_mapt oc_location_map;
    oc_thread_mapt oc_thread_map;
    cat_modulet raw_cat_module;

    std::vector<int> thread_map;

    class mm_linkt
    {
    public:
        rel_opt link_type;
        link_post link_position;
        int another_kind;
        int subtract_id; //only for link_type == MINUS
        mm_linkt(rel_opt _type) : 
            link_type(_type) {}
        mm_linkt(rel_opt _type, link_post _pos) : 
            link_type(_type), link_position(_pos) {}
        mm_linkt(rel_opt _type, int _rel) :
            link_type(_type), another_kind(_rel) {}
        mm_linkt(rel_opt _type, link_post _pos, int _rel) :
            link_type(_type), link_position(_pos), another_kind(_rel), subtract_id(-1) {}
        
        std::string link_type_name()
        {
            static std::string link_type_name_table[]{"terminate", "|", ";", "&", "\\", "*", "[]", "+", "?", "^-1", "rot", "int", "ext", "grd"};
            return link_type_name_table[int(link_type)];
        }
    };

    class mm_edget // like (co, link, fr)
    {
    public:
        int from;
        mm_linkt link;
        int to;
        mm_edget(int _from, mm_linkt _link, int _to) :
            from(_from), link(_link), to(_to) {}
    };

    int numbered = 0;
    std::map<std::string, int> kind_str2id;
    std::set<std::string> assigned_kinds;
    std::vector<std::string> kind_id2str;
    std::vector<int> kind_arities;
    std::vector<unary_sett> unary_sets;
    std::vector<binary_sett> binary_sets;
    std::vector<std::vector<mm_edget>> propagate_forward;
    std::vector<std::vector<mm_edget>> propagate_backward;
    std::vector<rel_axiomt> axioms;
    std::vector<std::vector<int>> graph_updates; // for relations with ACYCLIC axiom

    std::vector<unary_sett> dangerous_unary_sets;
    std::vector<binary_sett> dangerous_binary_sets;

    struct trailt
    {
        int arity, kind, node1, node2;
        trailt(int _a = 0, int _k = -1, int _n1 = -1, int _n2 = -1) : 
            arity(_a), kind(_k), node1(_n1), node2(_n2) {}
    };

    trailt unary_trail(int k, int n) {trailt ret(1, k, n, -1); return ret;}
    trailt binary_trail(int k, int n1, int n2) {trailt ret(2, k, n1, n2); return ret;}

    std::vector<trailt> mm_trail;
    std::vector<int> mm_trail_lim;
    std::vector<trailt> mm_dangerous_trail;
    std::vector<int> mm_dangerous_trail_lim;
    void push_scope();
    void pop_scope(int new_level);

    int theory_propagation = 0;
    int conflict_cycle = 0;

    void show_model();

    vec<int> bind; // a variable is bound to what kind of label/relation?
    std::vector<int> num_bind_var; // each kind is bound to how many variables?
    std::vector<int> num_assigned_bind_var; // each kind is bound to how many already assigned variables?

    struct subtractt
    {
        int difference, minuend, subtrahend; // let difference = minuend - subtrahend
        std::vector<int> subtrahend_bases; // how many 
        subtractt(int _d, int _m, int _s) : 
            difference(_d), minuend(_m), subtrahend(_s) {}
    };

    std::vector<subtractt> subtract_pool;
    std::vector<std::vector<int>> subtrahend_to_subtract_ids;
    std::vector<std::vector<int>> base_to_subtract_ids;

    std::vector<int> finished_kinds; //kinds that fully determined in this decision level

    void build_base_to_subtract_ids();

protected:
    CRef propagate();
    void cancelUntil(int level);
    void newDecisionLevel();
    lbool search(int nof_conflicts);
    void analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel);
    void uncheckedEnqueue(Lit p, CRef from = CRef_Undef);
public:
    MemoryModelSolver();

    Var newVar (lbool upol = l_Undef, bool dvar = true);

    void save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_label_tablet& _oc_label_table, oc_location_mapt& _oc_location_map, std::map<std::string, int>& _oc_result_order, cat_modulet& _cat_module, std::map<std::string, unsigned>& _oc_thread_map);
    
    std::set<int> base_kinds;
    void build_sets(int arity, std::string kind, bool is_base);
    void set_graph();

    lbool solve_();
    bool solve() { budgetOff(); assumptions.clear(); return solve_() == l_True; }
    inline bool solve(const vec<Lit>& assumps){ budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }
    inline lbool solveLimited(const vec<Lit>& assumps){ assumps.copyTo(assumptions); return solve_(); }

    std::vector<std::string> get_write_order();
    std::map<std::string, int>* oc_result_order;

    std::vector<std::string> node_names;
    int get_node(std::string name)
    {
        for(int i = 0; i < int(node_names.size()); i++)
            if(node_names[i] == name)
                return i;
        int new_node = node_names.size();
        node_names.push_back(name);
        thread_map.push_back(oc_thread_map[name]);
        return new_node;
    }

    struct unary_elementt
    {
        int node;
        int kind;
        literal_vector reason;
        unary_elementt(int _n, int _k, Lit _l) : node(_n), kind(_k), reason(literal_vector{_l}) {}
    };

    struct binary_elementt
    {
        int node1;
        int node2;
        int kind;
        literal_vector reason;
        binary_elementt(int _n1, int _n2, int _k, Lit _l) : node1(_n1), node2(_n2), kind(_k), reason(literal_vector{_l}) {}
    };
    
    std::multimap<Lit, unary_elementt> lit_to_unary_elements;
    std::multimap<Lit, binary_elementt> lit_to_binary_elements;
    std::vector<std::map<std::pair<int, int>, Lit>> binary_elements_to_lit;
    std::vector<std::map<int, Lit>> unary_elements_to_lit;

    void add_bind(int kind, Lit l)
    {
        while (var(l) >= nVars()) newVar();
        if(bind[var(l)] == -1)
        {
            bind[var(l)] = kind;
            num_bind_var[kind]++;
        }
    }

    void init_unary_element(int node, int kind, Lit l)
    {
        unary_elements_to_lit[kind][node] = l;

        lit_to_unary_elements.insert(std::make_pair(l, unary_elementt(node, kind, l)));
        add_bind(kind, l);
    }
    void init_binary_element(int node1, int node2, int kind, Lit l)
    {
        binary_elements_to_lit[kind][std::make_pair(node1, node2)] = l;

        lit_to_binary_elements.insert(std::make_pair(l, binary_elementt(node1, node2, kind, l)));
        add_bind(kind, l);
    }

    bool add_unary_element(unary_elementt elem);
    bool add_unary_element(int node, int kind, literal_vector& reason);
    bool add_binary_element(binary_elementt elem);
    bool add_binary_element(int node1, int node2, int kind, literal_vector& reason);
    bool add_dangerous_unary_element(int node, int kind, literal_vector& reason);
    bool add_dangerous_binary_element(int node1, int node2, int kind, literal_vector& reason);
    bool dfs(int kind, int node);
    bool cycle_detection();
    bool cycle_detection(int kind);

    bool can_propagate_subtract(subtractt& subtract);
    bool propagate_subtract(subtractt& subtract);

    bool use_available_info();

    literal_vector conflict_clause;

    std::vector<std::pair<Lit, literal_vector>> literals_to_assign;
    void assign_literal(Lit l, literal_vector& lv);
    void apply_literal_assignment();
};

}


#endif //MEMORYMODELSOLVER_H

// __SZH_ADD_END__