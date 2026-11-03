#ifndef MEMORY_MODEL_STATIC_ANALYZER_H
#define MEMORY_MODEL_STATIC_ANALYZER_H

#include <cat/cat_module.h>

#include "symex_target_equation.h"

class cuttert
{
    cat_modulet cat;

    class unary_must_may_sett
    {
    public:
        std::set<int> elements;

        bool contains(int node_id) { return elements.find(node_id) != elements.end(); }
        void set(int node_id) { elements.insert(node_id); }
        int size() { return elements.size(); }
    };
    typedef unary_must_may_sett unary_must_sett;
    typedef unary_must_may_sett unary_may_sett;

    class binary_must_may_sett
    {
    public:
        std::set<std::pair<int, int>> elements;

        bool contains(int node1_id, int node2_id) { return elements.find(std::make_pair(node1_id, node2_id)) != elements.end(); }
        void set(int node1_id, int node2_id) { elements.insert(std::make_pair(node1_id, node2_id)); }
        int size() { return elements.size(); }
    };

    typedef binary_must_may_sett binary_must_sett;
    typedef binary_must_may_sett binary_may_sett;

public:

    std::map<std::string, int> node_str2id;
    std::vector<std::string> node_id2str;
    std::map<std::string, int> kind_str2id;
    std::vector<std::string> kind_id2str;

    std::vector<unary_may_sett> unary_musts;
    std::vector<unary_may_sett> unary_mays;
    std::vector<binary_may_sett> binary_musts;
    std::vector<binary_may_sett> binary_mays;

    int get_node(std::string& str)
    {
        if(node_str2id.find(str) != node_str2id.end())
            return node_str2id[str];
        int new_id = node_id2str.size();
        node_id2str.push_back(str);
        node_str2id[str] = new_id;
        return new_id;
    }

    int get_kind(std::string& str)
    {
        if(kind_str2id.find(str) != kind_str2id.end())
            return kind_str2id[str];
        int new_id = kind_id2str.size();
        kind_id2str.push_back(str);
        kind_str2id[str] = new_id;
        return new_id;
    }

    void build_node_id(std::vector<oc_edget>& oc_edges, std::vector<oc_labelt>& oc_labels)
    {
        for(auto& edge : oc_edges)
        {
            get_node(edge.e1_str);
            get_node(edge.e2_str);
        }
        for(auto& label : oc_labels)
            get_node(label.e_str);
    }

    void build_kind_id(cat_modulet& cat)
    {
        for(auto kind_str : cat.necessary_relations)
            get_kind(kind_str);
    }

    cuttert() {}
    cuttert(cat_modulet& _cat, std::vector<oc_edget>& oc_edges, std::vector<oc_labelt>& oc_labels) : 
        cat(_cat)
    {
        build_node_id(oc_edges, oc_labels);
        build_kind_id(_cat);

        unary_musts.resize(kind_id2str.size());
        unary_mays.resize(kind_id2str.size());
        binary_musts.resize(kind_id2str.size());
        binary_mays.resize(kind_id2str.size());

        for(auto& edge : oc_edges)
        {
            auto node1 = node_str2id[edge.e1_str];
            auto node2 = node_str2id[edge.e2_str];
            auto kind = kind_str2id[edge.kind];
            auto expr = edge.expr;
            add_must_may_edge(node1, node2, kind, expr);
        }
        
        for(auto& label : oc_labels)
        {
            auto node = node_str2id[label.e_str];
            auto kind = kind_str2id[label.label];
            auto expr = label.expr;
            add_must_may_label(node, kind, expr);
        }
    }

    void add_must_may_label(int node, int kind, exprt expr)
    {
        unary_mays[kind].set(node);
        if(expr.is_true())
            unary_musts[kind].set(node);
    }

    void add_must_may_edge(int node1, int node2, int kind, exprt expr)
    {
        binary_mays[kind].set(node1, node2);
        if(expr.is_true())
            binary_musts[kind].set(node1, node2);
    }


    void analyze_sets();
    std::vector<std::vector<int>> sccs;
    std::vector<int> sorted_scc_ids;
    std::map<int, int> kind_to_scc_id;
    void tarjan(int kind, std::string kind_str, std::stack<int>& stack, std::set<int>& in_stack, std::vector<int>& dfn, std::vector<int>& low, int& count);
    void build_scc();
    void build_topological();

    void calc_unary_union(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result);
    void calc_binary_union(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result);
    void calc_unary_intersection(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result);
    void calc_binary_intersection(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result);
    void calc_unary_difference(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result);
    void calc_binary_difference(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result);
    void calc_composition(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result);
    void calc_product(const unary_must_may_sett& left, const unary_must_may_sett& right, binary_must_may_sett& result);
    void calc_bracket(const unary_must_may_sett& source, binary_must_may_sett& result);
    void calc_flip(const binary_must_may_sett& source, binary_must_may_sett& result);
    void calc_transitive_closure(const binary_must_may_sett& source, binary_must_may_sett& result);
    void calc_single_kind(int kind, std::string kind_str, int arity);
    void calc_must_may_sets();


    void generate_cutting_constraints(std::vector<oc_edget>& oc_edges, std::vector<oc_labelt>& oc_labels);

    void cutting_one_edge(cat_edget& propagate_edge);

    void cutting_unary_alt(int from, int to);
    void cutting_binary_alt(int from, int to);
    void cutting_seq(int from, int to,  link_post pos, int another);
    void cutting_unary_and(int from, int to, int another);
    void cutting_binary_and(int from, int to, int another);
    void cutting_unary_sub(int from, int to, link_post pos, int another);
    void cutting_binary_sub(int from, int to, link_post pos, int another);
    void cutting_prod(int from, int to, link_post pos, int another);
    void cutting_bracket(int from, int to);
    void cutting_flip(int from, int to);
    void cutting_plus(int from, int to);

    // input
    std::vector<std::map<std::pair<int, int>, exprt>> binary_bv;
    std::vector<std::map<int, exprt>> unary_bv;

    // output
    std::vector<exprt> cutting_constraints;

    // intermediate information
    std::vector<std::map<std::pair<int, int>, std::vector<exprt>>> binary_bv_conditions;
    std::vector<std::map<int, std::vector<exprt>>> unary_bv_conditions;
};

#endif // MEMORY_MODEL_STATIC_ANALYZER_H