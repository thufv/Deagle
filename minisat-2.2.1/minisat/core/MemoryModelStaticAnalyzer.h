#ifndef MEMORY_MODEL_STATIC_ANALYZER_H
#define MEMORY_MODEL_STATIC_ANALYZER_H

#include <cat/cat_module.h>
#include "MemoryModelSolverStruct.h"

namespace Minisat
{

class MemoryModelStaticAnalyzert
{
    cat_modulet cat;

public:

    std::map<std::string, int> node_str2id;
    std::map<std::string, int> kind_str2id;
    std::vector<std::string> kind_id2str;

    std::vector<unary_may_sett> unary_musts;
    std::vector<unary_may_sett> unary_mays;
    std::vector<binary_may_sett> binary_musts;
    std::vector<binary_may_sett> binary_mays;

    MemoryModelStaticAnalyzert() {}
    MemoryModelStaticAnalyzert(cat_modulet& _cat, std::map<std::string, int>& _node_str2id, std::map<std::string, int>& _kind_str2id, std::vector<std::string>& _kind_id2str, oc_edge_tablet& _oc_edge_table, oc_label_tablet& _oc_label_table) : 
        cat(_cat), node_str2id(_node_str2id), kind_str2id(_kind_str2id), kind_id2str(_kind_id2str)
    {
        unary_musts.resize(kind_id2str.size());
        unary_mays.resize(kind_id2str.size());
        binary_musts.resize(kind_id2str.size());
        binary_mays.resize(kind_id2str.size());

        for(auto& edge : _oc_edge_table)
        {
            auto node1 = node_str2id[edge.first.first];
            auto node2 = node_str2id[edge.first.second];
            auto kind = kind_str2id[edge.second.second];
            auto lit = edge.second.first;
            add_must_may_edge(node1, node2, kind, lit);
        }
        
        for(auto& label : _oc_label_table)
        {
            auto node = node_str2id[label.first];
            auto kind = kind_str2id[label.second.second];
            auto lit = label.second.first;
            add_must_may_label(node, kind, lit);
        }
    }

    void add_must_may_label(int node, int kind, Lit lit)
    {
        unary_mays[kind].set(node);
        // if lit.x == -1, the label is always there
        // for non-rf and non-co relations, the label is also always there (don't consider guard condition here)
        if((kind_id2str[kind] != "rf" && kind_id2str[kind] != "co") || lit.x == -1)
            unary_musts[kind].set(node);
    }

    void add_must_may_edge(int node1, int node2, int kind, Lit lit)
    {
        binary_mays[kind].set(node1, node2);
        // if lit.x == -1, the edge is always there
        // for non-rf and non-co relations, the edge is also always there (don't consider guard condition here)
        if((kind_id2str[kind] != "rf" && kind_id2str[kind] != "co") || lit.x == -1)
            binary_musts[kind].set(node1, node2);
    }

    void analyze();
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

  // for lazy cutting

//   struct int_reasont
//   {
//     int node1;
//     int node2;
//     std::string kind;
//     int arity;
//     int_reasont(int _node, std::string _kind):
//       node1(_node), kind(_kind), arity(1) {}
//     int_reasont(int _node1, int _node2, std::string _kind):
//       node1(_node1), node2(_node2), kind(_kind), arity(2) {}
//   };
//   typedef std::vector<int_reasont> int_reasonst;
//   typedef std::vector<int_reasonst> int_reasonsst;
//   // each reasonsst in the form of
//   // (reason \wedge reason ...) \vee (reason \wedge reason ...) \vee ...

//   typedef std::map<int_nodet, int_reasonsst> int_unary_rule_mapt;
//   typedef std::map<int_edget, int_reasonsst> int_binary_rule_mapt;

//   std::map<std::string, int_unary_rule_mapt> int_unary_rule_maps;
//   std::map<std::string, int_binary_rule_mapt> int_binary_rule_maps;

//   void add_unary_reasons(int_reasonst& reasons, std::string kind, int_nodet node, bool replace);

  // void calc_rules_union_unary(std::string left_kind, bool left_replace, std::string right_kind, bool right_replace, std::string kind);
  // void calc_rules_union_binary(std::string left_kind, const int_binary_sett& left, std::string right_kind, const int_binary_sett& right, int_binary_rule_mapt& result);

  // void calc_rules_single_kind(std::string kind, int arity);
  // void calc_rules();
};
}

#endif // MEMORY_MODEL_STATIC_ANALYZER_H