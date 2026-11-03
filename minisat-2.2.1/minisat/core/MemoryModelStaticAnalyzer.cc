#include "MemoryModelStaticAnalyzer.h"

using namespace Minisat;

void MemoryModelStaticAnalyzert::analyze()
{
    build_scc();
    build_topological();
    calc_must_may_sets();
    // calc_rules();

    // for(auto& kind_pair : kind_str2id)
    // {
    //     auto& kind_str = kind_pair.first;
    //     auto& kind = kind_pair.second;

    //     if(cat.unary_relations.find(kind_str) != cat.unary_relations.end())
    //     {
    //         std::cout << kind_str << " is unary, with must set:\n";
    //         for(auto node : unary_musts[kind].elements)
    //             std::cout << node << " ";
    //         std::cout << "\n";
    //         std::cout << kind_str << " is unary, with may set:\n";
    //         for(auto node : unary_mays[kind].elements)
    //             std::cout << node << " ";
    //         std::cout << "\n";
    //     }
    //     if(cat.binary_relations.find(kind_str) != cat.binary_relations.end())
    //     {
    //         std::cout << kind_str << " is binary, with must set:\n";
    //         for(auto pair : binary_musts[kind].elements)
    //             std::cout << "(" << pair.first << ", " << pair.second << ") ";
    //         std::cout << "\n";
    //         std::cout << kind_str << " is binary, with may set:\n";
    //         for(auto pair : binary_mays[kind].elements)
    //             std::cout << "(" << pair.first << ", " << pair.second << ") ";
    //         std::cout << "\n";
    //     }
    // }
}

void MemoryModelStaticAnalyzert::tarjan(int kind, std::string kind_str, std::stack<int>& stack, std::set<int>& in_stack, std::vector<int>& dfn, std::vector<int>& low, int& count)
{
    count++;
    dfn[kind] = low[kind] = count;
    stack.push(kind);
    in_stack.insert(kind);

    auto& edges = cat.get_relation_from_name(kind_str).operands;
    for(auto next_kind_str : edges)
    {
        int next_kind = kind_str2id[next_kind_str];
        if(!dfn[next_kind])
        {
            tarjan(next_kind, next_kind_str, stack, in_stack, dfn, low, count);
            low[kind] = std::min(low[kind], low[next_kind]);
        }
        else if(in_stack.find(next_kind) != in_stack.end())
            low[kind] = std::min(low[kind], dfn[next_kind]);
    }
    if(dfn[kind] == low[kind])
    {
        std::vector<int> scc;
        int k;
        do {
            k = stack.top();
            scc.push_back(k);
            stack.pop();
            in_stack.erase(k);
        } while(k != kind);
        int scc_id = sccs.size();
        sccs.push_back(scc);
        for(auto kind : scc)
            kind_to_scc_id[kind] = scc_id;
    }
}

void MemoryModelStaticAnalyzert::build_scc()
{
    int count = 0;
    std::stack<int> stack;
    std::set<int> in_stack;

    std::vector<int> dfn(kind_str2id.size(), 0);
    std::vector<int> low(kind_str2id.size(), 0);
    for(auto& kind_pair : kind_str2id)
    {
        auto kind_str = kind_pair.first;
        auto kind = kind_pair.second;
        if(!dfn[kind])
            tarjan(kind, kind_str, stack, in_stack, dfn, low, count);
    }
}

void MemoryModelStaticAnalyzert::build_topological()
{
    std::vector<std::set<int>> forward_edges;
    std::vector<int> in_degrees;
    forward_edges.resize(sccs.size());
    in_degrees.resize(sccs.size());

    for(auto kind_str : cat.necessary_relations)
    {
        auto& relation = cat.get_relation_from_name(kind_str);
        int kind = kind_str2id[kind_str];
        for(auto operand_str : relation.operands)
        {
            int operand = kind_str2id[operand_str];
            int from_scc_id = kind_to_scc_id[operand];
            int to_scc_id = kind_to_scc_id[kind];
            if(from_scc_id == to_scc_id)
                continue;
            auto insert_result = forward_edges[from_scc_id].insert(to_scc_id);
            if(insert_result.second)
                in_degrees[to_scc_id]++;
        }
    }

    std::vector<int> zero_in_degree_items;
    for(int scc_id = 0; scc_id < int(sccs.size()); scc_id++)
        if(in_degrees[scc_id] == 0)
            zero_in_degree_items.push_back(scc_id);

    while(!zero_in_degree_items.empty())
    {
        int from_scc_id = zero_in_degree_items.back();
        sorted_scc_ids.push_back(from_scc_id);
        zero_in_degree_items.pop_back();

        for(int to_scc_id : forward_edges[from_scc_id])
        {
            in_degrees[to_scc_id]--;
            if(in_degrees[to_scc_id] <= 0)
                zero_in_degree_items.push_back(to_scc_id);
        }
    }
}

#include <algorithm>

void MemoryModelStaticAnalyzert::calc_unary_union(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_union(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void MemoryModelStaticAnalyzert::calc_binary_union(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    result.outs.clear();
    for(int node1 = 0; node1 < std::max(int(left.outs.size()), int(right.outs.size())); node1++)
    {
        result.outs.push_back(std::set<int>());
        if(node1 < int(left.outs.size()) && node1 < int(right.outs.size()))
            std::set_union(left.outs[node1].begin(), left.outs[node1].end(), right.outs[node1].begin(), right.outs[node1].end(), std::inserter(result.outs[node1], result.outs[node1].begin()));
        else if(node1 < int(left.outs.size()))
            result.outs[node1] = left.outs[node1];
        else if(node1 < int(right.outs.size()))
            result.outs[node1] = right.outs[node1];
    }
}

void MemoryModelStaticAnalyzert::calc_unary_intersection(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_intersection(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void MemoryModelStaticAnalyzert::calc_binary_intersection(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    result.outs.clear();
    for(int node1 = 0; node1 < std::min(int(left.outs.size()), int(right.outs.size())); node1++)
    {
        result.outs.push_back(std::set<int>());
        std::set_intersection(left.outs[node1].begin(), left.outs[node1].end(), right.outs[node1].begin(), right.outs[node1].end(), std::inserter(result.outs[node1], result.outs[node1].begin()));
    }
}

void MemoryModelStaticAnalyzert::calc_unary_difference(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_difference(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void MemoryModelStaticAnalyzert::calc_binary_difference(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    result.outs.clear();
    for(int node1 = 0; node1 < int(left.outs.size()); node1++)
    {
        result.outs.push_back(std::set<int>());
        if(node1 < int(right.outs.size()))
            std::set_difference(left.outs[node1].begin(), left.outs[node1].end(), right.outs[node1].begin(), right.outs[node1].end(), std::inserter(result.outs[node1], result.outs[node1].begin()));
        else
            result.outs[node1] = left.outs[node1];
    }
}

void MemoryModelStaticAnalyzert::calc_composition(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    for(int node1 = 0; node1 < int(left.outs.size()); node1++)
        for(auto node2 : left.outs[node1])
        {
            if(node2 >= int(right.outs.size()))
                continue;
            for(auto node3 : right.outs[node2])
                result.set(node1, node3);
        }
}

void MemoryModelStaticAnalyzert::calc_product(const unary_must_may_sett& left, const unary_must_may_sett& right, binary_must_may_sett& result)
{
    for(auto left_node : left.elements)
        for(auto right_node : right.elements)
            result.set(left_node, right_node);
}

void MemoryModelStaticAnalyzert::calc_bracket(const unary_must_may_sett& source, binary_must_may_sett& result)
{
    for(auto node : source.elements)
        result.set(node, node);
}

void MemoryModelStaticAnalyzert::calc_flip(const binary_must_may_sett& source, binary_must_may_sett& result)
{
    for(int node1 = 0; node1 < int(source.outs.size()); node1++)
        for(auto node2 : source.outs[node1])
            result.set(node2, node1);
}

void MemoryModelStaticAnalyzert::calc_transitive_closure(const binary_must_may_sett& source, binary_must_may_sett& result)
{
    std::vector<std::pair<int, int>> waiting_list;
    for(int node1 = 0; node1 < int(source.outs.size()); node1++)
        for(auto node2 : source.outs[node1])
            waiting_list.push_back(std::make_pair(node1, node2));

    std::vector<std::set<int>> ins;
    std::vector<std::set<int>> outs;

    while(!waiting_list.empty())
    {
        auto edge = waiting_list.back();
        waiting_list.pop_back();
        int from = edge.first, to = edge.second;
        if(int(outs.size()) <= from)
            outs.resize(from + 1);
        if(int(ins.size()) <= to)
            ins.resize(to + 1);
        if(outs[from].find(to) == outs[from].end())
        {
            outs[from].insert(to);
            ins[to].insert(from);
            if(to < int(outs.size()))
                for(auto next : source.outs[to])
                    waiting_list.push_back(std::make_pair(from, next));
            if(from < int(ins.size()))
                for(auto prev : ins[from])
                    waiting_list.push_back(std::make_pair(prev, to));
        }
    }
    result.outs = outs;
}

void MemoryModelStaticAnalyzert::calc_single_kind(int kind, std::string kind_str, int arity)
{
    std::cout << "calculate the must may set of " << kind_str << "\n";

    auto relation = cat.get_relation_from_name(kind_str);
    switch(relation.op_type)
    {
    case ALT:
    {
        int op0 = kind_str2id[relation.operands[0]], op1 = kind_str2id[relation.operands[1]];
        if(arity == 1)
        {
            calc_unary_union(unary_musts[op0], unary_musts[op1], unary_musts[kind]);
            calc_unary_union(unary_mays[op0], unary_mays[op1], unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_binary_union(binary_musts[op0], binary_musts[op1], binary_musts[kind]);
            calc_binary_union(binary_mays[op0], binary_mays[op1], binary_mays[kind]);
        }
        break;
    }
    case SEQ:
    {
        int op0 = kind_str2id[relation.operands[0]], op1 = kind_str2id[relation.operands[1]];
        calc_composition(binary_musts[op0], binary_musts[op1], binary_musts[kind]);
        calc_composition(binary_mays[op0], binary_mays[op1], binary_mays[kind]);
        break;
    }
    case AND:
    {
        int op0 = kind_str2id[relation.operands[0]], op1 = kind_str2id[relation.operands[1]];
        if(arity == 1)
        {
            calc_unary_intersection(unary_musts[op0], unary_musts[op1], unary_musts[kind]);
            calc_unary_intersection(unary_mays[op0], unary_mays[op1], unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_binary_intersection(binary_musts[op0], binary_musts[op1], binary_musts[kind]);
            calc_binary_intersection(binary_mays[op0], binary_mays[op1], binary_mays[kind]);
        }
        break;
    }
    case SUB:
    {
        int op0 = kind_str2id[relation.operands[0]], op1 = kind_str2id[relation.operands[1]];
        if(arity == 1)
        {
            calc_unary_difference(unary_musts[op0], unary_mays[op1], unary_musts[kind]);
            calc_unary_difference(unary_mays[op0], unary_musts[op1], unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_binary_difference(binary_musts[op0], binary_mays[op1], binary_musts[kind]);
            calc_binary_difference(binary_mays[op0], binary_musts[op1], binary_mays[kind]);
        }
        break;
    }
    case PROD:
    {
        int op0 = kind_str2id[relation.operands[0]], op1 = kind_str2id[relation.operands[1]];
        calc_product(unary_musts[op0], unary_musts[op1], binary_musts[kind]);
        calc_product(unary_mays[op0], unary_mays[op1], binary_mays[kind]);
        break;
    }
    case BRACKET:
    {
        int op = kind_str2id[relation.operands[0]];
        calc_bracket(unary_musts[op], binary_musts[kind]);
        calc_bracket(unary_mays[op], binary_mays[kind]);
        break;
    }
    case FLIP:
    {
        int op = kind_str2id[relation.operands[0]];
        calc_flip(binary_musts[op], binary_musts[kind]);
        calc_flip(binary_mays[op], binary_mays[kind]);
        break;
    }
    case PLUS:
    {
        int op = kind_str2id[relation.operands[0]];
        calc_transitive_closure(binary_musts[op], binary_musts[kind]);
        calc_transitive_closure(binary_mays[op], binary_mays[kind]);
        break;
    }
    case TERMINAL:
    {
        if(arity == 1)
        {
            if(int(unary_musts.size()) <= kind)
                unary_musts.resize(kind + 1);
            if(int(unary_mays.size()) <= kind)
                unary_mays.resize(kind + 1);
        }
        if(arity == 2)
        {
            if(int(binary_musts.size()) <= kind)
                binary_musts.resize(kind + 1);
            if(int(binary_mays.size()) <= kind)
                binary_mays.resize(kind + 1);
        }
    }
    default:
        break;
    }
}

void MemoryModelStaticAnalyzert::calc_must_may_sets()
{
    for(auto scc_id : sorted_scc_ids)
    {
        auto& scc = sccs[scc_id];

        // bool need_must_may_set = false;
        // for(auto kind : scc)
        //     if(cat.need_must_may_set_relations.find(kind_id2str[kind]) != cat.need_must_may_set_relations.end())
        //     {
        //         need_must_may_set = true;
        //         break;
        //     }

        bool need_must_may_set = true;

        if(!need_must_may_set)
        {
            for(auto kind : scc)
            {
                auto kind_str = kind_id2str[kind];
                if(cat.get_arity(kind_str) == 1)
                {
                    unary_mays[kind].trivial = true;
                    unary_musts[kind].trivial = true;
                }
                if(cat.get_arity(kind_str) == 2)
                {
                    binary_mays[kind].trivial = true;
                    binary_musts[kind].trivial = true;
                }
            }
            continue;
        }

        if(scc.size() == 1) // not recursive
        {
            auto kind = scc[0];
            auto kind_str = kind_id2str[kind];
            calc_single_kind(kind, kind_str, cat.get_arity(kind_str));
        }
        else
        {
            bool changed = false;
            do
            {
                changed = false;
                for(auto& kind : scc)
                {
                    auto kind_str = kind_id2str[kind];
                    int original_total_size = unary_musts[kind].size() + unary_mays[kind].size() + binary_musts[kind].size() + binary_mays[kind].size();
                    calc_single_kind(kind, kind_str, cat.get_arity(kind_str));
                    int new_total_size = unary_musts[kind].size() + unary_mays[kind].size() + binary_musts[kind].size() + binary_mays[kind].size();
                    if(new_total_size > original_total_size)
                        changed = true;
                }
            } while (changed);
            
        }
    }
}

// void MemoryModelStaticAnalyzert::add_unary_reasons(int_reasonst& reasons, std::string kind, int_nodet node, bool replace)
// {

// }

// void MemoryModelStaticAnalyzert::calc_rules_union_unary(std::string left_kind, bool left_replace, std::string right_kind, bool right_replace, std::string kind)
// {
//     auto& result_map = int_unary_rule_maps[kind];
//     for(auto left_node : unary_mays[left_kind])
//     {
//         int_reasonst new_reasons;
//         add_unary_reasons(new_reasons, left_kind, left_node, left_replace);
//         result_map[left_node].push_back(new_reasons);
//     }
//     for(auto right_node : int_unary_rule_maps[right_kind])
//         result_map[right_node].push_back(int_reasonst{int_reasont(right_node, right_kind)});
// }

// void MemoryModelStaticAnalyzert::calc_rules_union_binary(std::string left_kind, const int_binary_sett& left, std::string right_kind, const int_binary_sett& right, int_binary_rule_mapt& result)
// {
//     for(auto left_edge : left)
//         result[left_edge].push_back(int_reasonst{int_reasont(left_edge.first, left_edge.second, left_kind)});
//     for(auto right_edge : right)
//         result[right_edge].push_back(int_reasonst{int_reasont(right_edge.first, right_edge.second, left_kind)});
// }

// void MemoryModelStaticAnalyzert::calc_rules_union_unary(std::string left_kind, const int_unary_sett& left, std::string right_kind, const int_unary_sett& right, int_unary_rule_mapt& result)
// {
//     for(auto left_node : left)
//         result[left_node].push_back(int_reasonst{int_reasont(left_node, left_kind)});
//     for(auto right_node : right)
//         result[right_node].push_back(int_reasonst{int_reasont(right_node, right_kind)});
// }

// void MemoryModelStaticAnalyzert::calc_rules_single_kind(std::string kind, int arity)
// {
//     std::cout << "calculate the rules of " << kind << "\n";

//     auto relation = cat.get_relation_from_name(kind);
//     switch(relation.op_type)
//     {
//     case ALT:
//         if(arity == 1)
//         {
//             calc_union(unary_musts[relation.operands[0]], unary_musts[relation.operands[1]], unary_musts[kind]);
//             calc_union(unary_mays[relation.operands[0]], unary_mays[relation.operands[1]], unary_mays[kind]);
//         }
//         if(arity == 2)
//         {
//             calc_union(binary_musts[relation.operands[0]], binary_musts[relation.operands[1]], binary_musts[kind]);
//             calc_union(binary_mays[relation.operands[0]], binary_mays[relation.operands[1]], binary_mays[kind]);
//         }
//         break;
//     case SEQ:
//         calc_composition(binary_musts[relation.operands[0]], binary_musts[relation.operands[1]], binary_musts[kind]);
//         calc_composition(binary_mays[relation.operands[0]], binary_mays[relation.operands[1]], binary_mays[kind]);
//         break;
//     case AND:
//         if(arity == 1)
//         {
//             calc_intersection(unary_musts[relation.operands[0]], unary_musts[relation.operands[1]], unary_musts[kind]);
//             calc_intersection(unary_mays[relation.operands[0]], unary_mays[relation.operands[1]], unary_mays[kind]);
//         }
//         if(arity == 2)
//         {
//             calc_intersection(binary_musts[relation.operands[0]], binary_musts[relation.operands[1]], binary_musts[kind]);
//             calc_intersection(binary_mays[relation.operands[0]], binary_mays[relation.operands[1]], binary_mays[kind]);
//         }
//         break;
//     case SUB:
//         if(arity == 1)
//         {
//             calc_difference(unary_musts[relation.operands[0]], unary_mays[relation.operands[1]], unary_musts[kind]);
//             calc_difference(unary_mays[relation.operands[0]], unary_musts[relation.operands[1]], unary_mays[kind]);
//         }
//         if(arity == 2)
//         {
//             calc_difference(binary_musts[relation.operands[0]], binary_mays[relation.operands[1]], binary_musts[kind]);
//             calc_difference(binary_mays[relation.operands[0]], binary_musts[relation.operands[1]], binary_mays[kind]);
//         }
//         break;
//     case PROD:
//         calc_product(unary_musts[relation.operands[0]], unary_musts[relation.operands[1]], binary_musts[kind]);
//         calc_product(unary_mays[relation.operands[0]], unary_mays[relation.operands[1]], binary_mays[kind]);
//         break;
//     case BRACKET:
//         calc_bracket(unary_musts[relation.operands[0]], binary_musts[kind]);
//         calc_bracket(unary_mays[relation.operands[0]], binary_mays[kind]);
//         break;
//     case FLIP:
//         calc_flip(binary_musts[relation.operands[0]], binary_musts[kind]);
//         calc_flip(binary_mays[relation.operands[0]], binary_mays[kind]);
//         break;
//     case PLUS:
//         calc_transitive_closure(binary_musts[relation.operands[0]], binary_musts[kind]);
//         calc_transitive_closure(binary_mays[relation.operands[0]], binary_mays[kind]);
//         break;
//     case TERMINAL:
//         if(arity == 1)
//         {
//             if(unary_musts.find(kind) == unary_musts.end())
//                 unary_musts[kind] = int_unary_sett();
//             if(unary_mays.find(kind) == unary_mays.end())
//                 unary_mays[kind] = int_unary_sett();
//         }
//         if(arity == 2)
//         {
//             if(binary_musts.find(kind) == binary_musts.end())
//                 binary_musts[kind] = int_binary_sett();
//             if(binary_mays.find(kind) == binary_mays.end())
//                 binary_mays[kind] = int_binary_sett();
//         }
//     default:
//         break;
//     }
// }

// void MemoryModelStaticAnalyzert::calc_rules()
// {
//     for(auto scc_id : sorted_scc_ids)
//     {
//         auto& scc = sccs[scc_id];

//         bool need_negation = false;
//         for(auto kind : scc)
//             if(cat.negative_relations.find(kind) != cat.negative_relations.end())
//             {
//                 need_negation = true;
//                 break;
//             }
//         if(!need_negation)
//             continue;

//         bool need_detail = false;
//         if(scc.size() > 1) // recursive
//             need_detail = true;
//         else // defined with let d := r0 ; r1
//         {
//             auto kind = scc[0];
//             auto relation = cat.get_relation_from_name(kind);
//             if(relation.op_type == rel_opt::SEQ)
//             need_detail = true;
//         }
//         if(!need_detail)
//             continue;

//         if(scc.size() == 1) // not recursive
//         {
//             auto kind = scc[0];
//             calc_rules_single_kind(kind, cat.get_arity(kind));
//         }
//         else
//         {
//             bool changed = false;
//             do
//             {
//                 changed = false;
//                 for(auto& kind : scc)
//                 {
//                     int original_total_size = unary_musts[kind].size() + unary_mays[kind].size() + binary_musts[kind].size() + binary_mays[kind].size();
//                     calc_rules_single_kind(kind, cat.get_arity(kind));
//                     int new_total_size = unary_musts[kind].size() + unary_mays[kind].size() + binary_musts[kind].size() + binary_mays[kind].size();
//                     if(new_total_size > original_total_size)
//                         changed = true;
//                 }
//             } while (changed);
            
//         }
//     }
// }