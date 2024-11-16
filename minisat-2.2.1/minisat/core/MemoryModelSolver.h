// __SZH_ADD_BEGIN__

#ifndef MEMORYMODELSOLVER_H
#define MEMORYMODELSOLVER_H

#include <map>
#include <iostream>

#include "../../minisat/mtl/Queue.h"
#include "../../minisat/core/Solver.h"
#include "MemoryModelSolverStruct.h"

#include <cat/cat_module.h>

typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oc_edge_tablet;
typedef std::vector<std::pair<std::string, std::pair<Minisat::Lit, std::string> > > oc_label_tablet;
typedef std::map<std::string, std::set<std::string>> oc_unary_mayst;
typedef std::map<std::string, std::set<std::pair<std::string, std::string>>> oc_binary_mayst;

namespace Minisat
{

class MemoryModelSolver : public Solver
{
    oc_edge_tablet oc_edge_table;
    oc_label_tablet oc_label_table;
    cat_modulet raw_cat_module;

    class mm_linkt
    {
    public:
        rel_opt link_type;
        link_post link_position;
        int another_kind;
        mm_linkt(rel_opt _type) : 
            link_type(_type) {}
        mm_linkt(rel_opt _type, link_post _pos) : 
            link_type(_type), link_position(_pos) {}
        mm_linkt(rel_opt _type, int _rel) :
            link_type(_type), another_kind(_rel) {}
        mm_linkt(rel_opt _type, link_post _pos, int _rel) :
            link_type(_type), link_position(_pos), another_kind(_rel) {}
        
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

    std::vector<bool> need_negation;
    std::vector<unary_sett> neg_unary_sets;
    std::vector<binary_sett> neg_binary_sets;

    std::vector<unary_may_sett> unary_may_sets;
    std::vector<binary_may_sett> binary_may_sets;

    std::vector<unary_sett> dangerous_unary_sets;
    std::vector<binary_sett> dangerous_binary_sets;

    std::vector<binary_sett> neg_seq_components;
    std::vector<std::vector<std::vector<int>>> seq_derivation_num;
    std::vector<std::vector<std::vector<std::vector<int>>>> left_may_seq;
    std::vector<std::vector<std::vector<std::vector<int>>>> right_may_seq;

    struct trailt
    {
        int arity, kind, node1, node2;
        bool pos; // false means negated
        trailt(int _a = 0, int _k = -1, int _n1 = -1, int _n2 = -1, bool _pos = true) : 
            arity(_a), kind(_k), node1(_n1), node2(_n2), pos(_pos) {}
    };

    struct neg_seq_trailt
    {
        int kind, node1, node2;
        bool non_trivial;
        neg_seq_trailt(int _k = -1, int _n1 = -1, int _n2 = -1, bool _nt = true) : 
            kind(_k), node1(_n1), node2(_n2), non_trivial(_nt) {}
    };

    trailt unary_trail(int k, int n, bool pos = true) {trailt ret(1, k, n, -1, pos); return ret;}
    trailt binary_trail(int k, int n1, int n2, bool pos = true) {trailt ret(2, k, n1, n2, pos); return ret;}

    std::vector<trailt> mm_trail;
    std::vector<int> mm_trail_lim;
    std::vector<trailt> mm_dangerous_trail;
    std::vector<int> mm_dangerous_trail_lim;
    std::vector<neg_seq_trailt> mm_neg_seq_trail;
    std::vector<int> mm_neg_seq_trail_lim;
    void push_scope();
    void pop_scope(int new_level);

    int theory_propagation = 0;
    int conflict_cycle = 0;

    void show_model();
    void negation_check();

protected:
    CRef propagate();
    void cancelUntil(int level);
    void newDecisionLevel();
    lbool search(int nof_conflicts);
    void analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel);
    bool simplify();
public:
    MemoryModelSolver();

    void save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_label_tablet& _oc_label_table, cat_modulet& _cat_module);
    
    std::set<int> base_kinds;
    void build_sets(int arity, std::string kind, bool is_base);
    void set_nodes_and_kinds();
    void set_may_sets();
    void set_graph();
    void set_neg_graph();

    lbool solve_();
    bool solve() { budgetOff(); assumptions.clear(); return solve_() == l_True; }
    inline bool solve(const vec<Lit>& assumps){ budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }
    inline lbool solveLimited(const vec<Lit>& assumps){ assumps.copyTo(assumptions); return solve_(); }

    std::vector<std::string> node_names;
    int node_num() { return int(node_names.size()); }
    int get_node(std::string name)
    {
        for(int i = 0; i < node_num(); i++)
            if(node_names[i] == name)
                return i;
        int new_node = node_names.size();
        node_names.push_back(name);
        std::cout << new_node << " is " << name << "\n";
        return new_node;
    }

    struct labelt
    {
        int node;
        int kind;
        mm_reasont reason;
        labelt(int _n, int _k, Lit _l) : node(_n), kind(_k), reason(mm_reasont(_l)) {}
    };

    struct edget
    {
        int node1;
        int node2;
        int kind;
        mm_reasont reason;
        edget(int _n1, int _n2, int _k, Lit _l) : node1(_n1), node2(_n2), kind(_k), reason(mm_reasont(_l)) {}
        edget(int _n1, int _n2, int _k, mm_reasont _r) : node1(_n1), node2(_n2), kind(_k), reason(_r) {}
    };
    
    std::multimap<Lit, labelt> lit_to_labels;
    std::multimap<Lit, labelt> lit_to_neglabels;
    std::multimap<Lit, edget> lit_to_edges;
    std::multimap<Lit, edget> lit_to_negedges;
    std::vector<std::map<std::pair<int, int>, Lit>> edges_to_lit;
    std::vector<std::map<std::pair<int, int>, Lit>> negedges_to_lit;
    std::vector<std::map<int, Lit>> labels_to_lit;
    std::vector<std::map<int, Lit>> neglabels_to_lit;

    // fast versions of lit_to_labels and lit_to_edges
    std::vector<labelt> empty_labels;
    std::vector<edget> empty_edges;
    vec<std::vector<labelt>> correspond_labels;
    vec<std::vector<labelt>> correspond_neglabels;
    vec<std::vector<edget>> correspond_edges;
    vec<std::vector<edget>> correspond_negedges;
    inline std::vector<labelt>& get_correspond_label(Lit l)
    {
        if(correspond_labels.size() <= l.x)
            return empty_labels;
        return correspond_labels[l.x];
    }
    inline std::vector<labelt>& get_correspond_neglabel(Lit l)
    {
        if(correspond_neglabels.size() <= l.x)
            return empty_labels;
        return correspond_neglabels[l.x];
    }
    inline std::vector<edget>& get_correspond_edge(Lit l)
    {
        if(correspond_edges.size() <= l.x)
            return empty_edges;
        return correspond_edges[l.x];
    }
    inline std::vector<edget>& get_correspond_negedge(Lit l)
    {
        if(correspond_negedges.size() <= l.x)
            return empty_edges;
        return correspond_negedges[l.x];
    }
    inline void set_correspond_label(Lit l, labelt& label)
    {
        if(correspond_labels.size() <= l.x)
            correspond_labels.growTo(l.x + 1);
        correspond_labels[l.x].push_back(label);
    }
    inline void set_correspond_neglabel(Lit l, labelt& label)
    {
        if(correspond_neglabels.size() <= l.x)
            correspond_neglabels.growTo(l.x + 1);
        correspond_neglabels[l.x].push_back(label);
    }
    inline void set_correspond_edge(Lit l, edget& edge)
    {
        if(correspond_edges.size() <= l.x)
            correspond_edges.growTo(l.x + 1);
        correspond_edges[l.x].push_back(edge);
    }
    inline void set_correspond_negedge(Lit l, edget& edge)
    {
        if(correspond_negedges.size() <= l.x)
            correspond_negedges.growTo(l.x + 1);
        correspond_negedges[l.x].push_back(edge);
    }

    void init_label(int node, int kind, Lit l)
    {
        labels_to_lit[kind][node] = l;
        lit_to_labels.insert(std::make_pair(l, labelt(node, kind, l)));
    }
    void init_neglabel(int node, int kind, Lit l)
    {
        neglabels_to_lit[kind][node] = l;
        lit_to_neglabels.insert(std::make_pair(l, labelt(node, kind, l)));
    }

    void init_edge(int node1, int node2, int kind, Lit l)
    {
        edges_to_lit[kind][std::make_pair(node1, node2)] = l;
        lit_to_edges.insert(std::make_pair(l, edget(node1, node2, kind, l)));
    }

    void init_negedge(int node1, int node2, int kind, Lit l)
    {
        negedges_to_lit[kind][std::make_pair(node1, node2)] = l;
        lit_to_negedges.insert(std::make_pair(l, edget(node1, node2, kind, l)));
    }

    bool add_label(int node, int kind, mm_reasont& reason);
    bool add_label(labelt label) { return add_label(label.node, label.kind, label.reason); }

    bool add_edge(int node1, int node2, int kind, mm_reasont& reason);
    bool add_edge(edget edge) { return add_edge(edge.node1, edge.node2, edge.kind, edge.reason); }

    bool add_neglabel(int node, int kind, mm_reasont& reason);
    bool add_neglabel(labelt label) { return add_neglabel(label.node, label.kind, label.reason); }

    bool add_negedge(int node1, int node2, int kind, mm_reasont& reason);
    bool add_negedge(edget elem) { return add_negedge(elem.node1, elem.node2, elem.kind, elem.reason); }

    bool add_dangerous_label(int node, int kind, mm_reasont& reason);
    bool add_dangerous_edge(int node1, int node2, int kind, mm_reasont& reason);

    bool use_available_info(bool is_first);

    literal_vector conflict_clause;

    std::vector<std::pair<Lit, literal_vector>> literals_to_assign;
    std::set<Lit> assigned_literals;
    
    void assign_literal(Lit l, literal_vector& lv);
    void apply_literal_assignment();

    std::string pretty_lit(Minisat::Lit l);
};

}


#endif //MEMORYMODELSOLVER_H

// __SZH_ADD_END__