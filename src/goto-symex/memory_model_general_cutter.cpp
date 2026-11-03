#include "memory_model_general_cutter.h"

#include <iostream>

void cuttert::analyze_sets()
{
    build_scc();
    build_topological();
    calc_must_may_sets();
}

void cuttert::tarjan(int kind, std::string kind_str, std::stack<int>& stack, std::set<int>& in_stack, std::vector<int>& dfn, std::vector<int>& low, int& count)
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

void cuttert::build_scc()
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

void cuttert::build_topological()
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

void cuttert::calc_unary_union(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_union(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_binary_union(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    std::set_union(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_unary_intersection(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_intersection(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_binary_intersection(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    std::set_intersection(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_unary_difference(const unary_must_may_sett& left, const unary_must_may_sett& right, unary_must_may_sett& result)
{
    std::set_difference(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_binary_difference(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    std::set_difference(left.elements.begin(), left.elements.end(), right.elements.begin(), right.elements.end(), std::inserter(result.elements, result.elements.begin()));
}

void cuttert::calc_composition(const binary_must_may_sett& left, const binary_must_may_sett& right, binary_must_may_sett& result)
{
    for(auto left_edge : left.elements)
        for(auto right_edge : right.elements)
            if(left_edge.second == right_edge.first)
                result.set(left_edge.first, right_edge.second);
}

void cuttert::calc_product(const unary_must_may_sett& left, const unary_must_may_sett& right, binary_must_may_sett& result)
{
    for(auto left_node : left.elements)
        for(auto right_node : right.elements)
            result.set(left_node, right_node);
}

void cuttert::calc_bracket(const unary_must_may_sett& source, binary_must_may_sett& result)
{
    for(auto node : source.elements)
        result.set(node, node);
}

void cuttert::calc_flip(const binary_must_may_sett& source, binary_must_may_sett& result)
{
    for(auto edge : source.elements)
        result.set(edge.second, edge.first);
}

void cuttert::calc_transitive_closure(const binary_must_may_sett& source, binary_must_may_sett& result)
{
    int result_size_before = 0;
    result.elements = source.elements;
    do
    {
        result_size_before = result.elements.size();
        binary_must_may_sett double_result;
        calc_composition(result, result, double_result);
        calc_binary_union(result, double_result, result);
    } while (result_size_before < int(result.elements.size()));
}

void cuttert::calc_single_kind(int kind, std::string kind_str, int arity)
{
    std::cout << "in the front end calculate the must may set of " << kind_str << "\n";

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

void cuttert::calc_must_may_sets()
{
    for(auto scc_id : sorted_scc_ids)
    {
        auto& scc = sccs[scc_id];

        bool need_cutting = false;
        for(auto kind : scc)
            if(cat.need_cutting_relations.find(kind_id2str[kind]) != cat.need_cutting_relations.end())
            {
                need_cutting = true;
                break;
            }
        if(!need_cutting)
            continue;

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

void cuttert::generate_cutting_constraints(std::vector<oc_edget>& oc_edges, std::vector<oc_labelt>& oc_labels)
{
    unary_bv.resize(kind_id2str.size());
    binary_bv.resize(kind_id2str.size());
    unary_bv_conditions.resize(kind_id2str.size());
    binary_bv_conditions.resize(kind_id2str.size());

    for(auto& edge : oc_edges)
    {
        int kind = kind_str2id[edge.kind];
        int node1 = node_str2id[edge.e1_str];
        int node2 = node_str2id[edge.e2_str];
        binary_bv[kind][std::make_pair(node1, node2)] = edge.expr;
    }

    for(auto& label : oc_labels)
    {
        int kind = kind_str2id[label.label];
        int node = node_str2id[label.e_str];
        unary_bv[kind][node] = label.expr;
    }

    for(auto& propagate_pair : cat.propagate_backward)
    {
        auto& target_relation = propagate_pair.first;
        if(cat.need_cutting_relations.find(target_relation) == cat.need_cutting_relations.end())
            continue;

        auto& propagate_edges = propagate_pair.second;
        for(auto& propagate_edge : propagate_edges)
            cutting_one_edge(propagate_edge);
    }

    for(auto relation : cat.need_cutting_relations)
    {
        if(cat.base_relations.find(relation) != cat.base_relations.end())
            continue;

        int kind = kind_str2id[relation];
        if(cat.get_arity(relation) == 1)
        {
            for(auto node : unary_mays[kind].elements)
            {
                if(unary_musts[kind].contains(node))
                    unary_bv_conditions[kind][node] = std::vector<exprt>{true_exprt()};

                auto unary_bv_condition = disjunction(unary_bv_conditions[kind][node]);
                cutting_constraints.push_back(equal_exprt(unary_bv[kind][node], unary_bv_condition));
            }
        }
        if(cat.get_arity(relation) == 2)
        {
            for(auto& edge : binary_mays[kind].elements)
            {
                if(binary_musts[kind].contains(edge.first, edge.second))
                    binary_bv_conditions[kind][edge] = std::vector<exprt>{true_exprt()};

                auto binary_bv_condition = disjunction(binary_bv_conditions[kind][edge]);
                cutting_constraints.push_back(equal_exprt(binary_bv[kind][edge], binary_bv_condition));
            }
        }
    }
}

void cuttert::cutting_one_edge(cat_edget& propagate_edge)
{
    // temporarily does not support recursive situations!
    // but support transitive closure d = r+

    std::string from_relation = propagate_edge.from;
    std::string to_relation = propagate_edge.to;
    int from_relation_id = kind_str2id[from_relation];
    int to_relation_id = kind_str2id[to_relation];
    auto& link = propagate_edge.link;
    switch(link.link_type)
    {
    case ALT:
        if(cat.get_arity(from_relation) == 1)
            cutting_unary_alt(from_relation_id, to_relation_id);
        if(cat.get_arity(from_relation) == 2)
            cutting_binary_alt(from_relation_id, to_relation_id);
        break;
    case SEQ:
        cutting_seq(from_relation_id, to_relation_id, link.link_position, kind_str2id[link.another_kind]);
        break;
    case AND:
        if(cat.get_arity(from_relation) == 1)
            cutting_unary_and(from_relation_id, to_relation_id, kind_str2id[link.another_kind]);
        if(cat.get_arity(from_relation) == 2)
            cutting_binary_and(from_relation_id, to_relation_id, kind_str2id[link.another_kind]);
        break;
    case SUB:
        if(cat.get_arity(from_relation) == 1)
            cutting_unary_sub(from_relation_id, to_relation_id, link.link_position, kind_str2id[link.another_kind]);
        if(cat.get_arity(from_relation) == 2)
            cutting_binary_sub(from_relation_id, to_relation_id, link.link_position, kind_str2id[link.another_kind]);
        break;
    case PROD:
        cutting_prod(from_relation_id, to_relation_id, link.link_position, kind_str2id[link.another_kind]);
        break;
    case BRACKET:
        cutting_bracket(from_relation_id, to_relation_id);
        break;
    case FLIP:
        cutting_flip(from_relation_id, to_relation_id);
        break;
    case PLUS:
        cutting_plus(from_relation_id, to_relation_id);
        break;
    case TERMINAL:
        // do nothing, this should not happen
        break;
    }
}


void cuttert::cutting_unary_alt(int from, int to)
{
    for(auto node : unary_mays[from].elements)
    {
        if(unary_bv_conditions[to].find(node) == unary_bv_conditions[to].end())
            unary_bv_conditions[to][node] = std::vector<exprt>{};
        unary_bv_conditions[to][node].push_back(unary_bv[from][node]);
    }
}

void cuttert::cutting_binary_alt(int from, int to)
{
    for(auto& edge : binary_mays[from].elements)
    {
        if(binary_bv_conditions[to].find(edge) == binary_bv_conditions[to].end())
            binary_bv_conditions[to][edge] = std::vector<exprt>{};
        binary_bv_conditions[to][edge].push_back(binary_bv[from][edge]);
    }
}

void cuttert::cutting_seq(int from, int to,  link_post pos, int another)
{
    if(pos == link_post::LEFT)
    {
        for(auto& ledge : binary_mays[from].elements)
        {
            int node1 = ledge.first;
            int node2 = ledge.second;
            for(int node3 = 0; node3 < int(node_id2str.size()); node3++)
            {
                if(!binary_mays[another].contains(node2, node3))
                    continue;

                auto connected_edge = std::make_pair(node1, node3);
                if(binary_bv_conditions[to].find(connected_edge) == binary_bv_conditions[to].end())
                    binary_bv_conditions[to][connected_edge] = std::vector<exprt>{};
                binary_bv_conditions[to][connected_edge].push_back(and_exprt(binary_bv[from][std::make_pair(node1, node2)], binary_bv[another][std::make_pair(node2, node3)]));
            }
        }
    }
}

void cuttert::cutting_unary_and(int from, int to, int another)
{
    for(auto node : unary_mays[from].elements)
    {
        if(!unary_mays[another].contains(node))
            continue;

        if(unary_bv_conditions[to].find(node) == unary_bv_conditions[to].end())
            unary_bv_conditions[to][node] = std::vector<exprt>{};
        unary_bv_conditions[to][node].push_back(and_exprt(unary_bv[from][node], unary_bv[another][node]));
    }
}

void cuttert::cutting_binary_and(int from, int to, int another)
{
    for(auto& edge : binary_mays[from].elements)
    {
        int node1 = edge.first;
        int node2 = edge.second;

        if(!binary_mays[another].contains(node1, node2))
            continue;

        if(binary_bv_conditions[to].find(edge) == binary_bv_conditions[to].end())
            binary_bv_conditions[to][edge] = std::vector<exprt>{};
        binary_bv_conditions[to][edge].push_back(and_exprt(binary_bv[from][edge], binary_bv[another][edge]));
    }
}

void cuttert::cutting_unary_sub(int from, int to, link_post pos, int another)
{
    if(pos == link_post::LEFT)
    {
        for(auto node : unary_mays[from].elements)
        {
            if(unary_musts[another].contains(node))
                continue;

            if(unary_bv_conditions[to].find(node) == unary_bv_conditions[to].end())
                unary_bv_conditions[to][node] = std::vector<exprt>{};

            if(!unary_mays[another].contains(node))
            {
                unary_bv_conditions[to][node].push_back(unary_bv[from][node]);
                continue;
            }

            unary_bv_conditions[to][node].push_back(and_exprt(unary_bv[from][node], not_exprt(unary_bv[another][node])));
        }
    }
}

void cuttert::cutting_binary_sub(int from, int to, link_post pos, int another)
{
    if(pos == link_post::LEFT)
    {
        for(auto edge : binary_mays[from].elements)
        {
            int node1 = edge.first;
            int node2 = edge.second;

            if(binary_musts[another].contains(node1, node2))
                continue;

            if(binary_bv_conditions[to].find(edge) == binary_bv_conditions[to].end())
                binary_bv_conditions[to][edge] = std::vector<exprt>{};

            if(!binary_mays[another].contains(node1, node2))
            {
                binary_bv_conditions[to][edge].push_back(binary_bv[from][edge]);
                continue;
            }

            binary_bv_conditions[to][edge].push_back(and_exprt(binary_bv[from][edge], not_exprt(binary_bv[another][edge])));
        }
    }
}

void cuttert::cutting_prod(int from, int to, link_post pos, int another)
{
    if(pos == link_post::LEFT)
    {
        for(auto node1 : unary_mays[from].elements)
            for(auto node2 : unary_mays[another].elements)
            {
                auto edge = std::make_pair(node1, node2);
                if(binary_bv_conditions[to].find(edge) == binary_bv_conditions[to].end())
                    binary_bv_conditions[to][edge] = std::vector<exprt>{};
                binary_bv_conditions[to][edge].push_back(and_exprt(unary_bv[from][node1], unary_bv[another][node2]));
            }
    }
}

void cuttert::cutting_bracket(int from, int to)
{
    for(auto node : unary_mays[from].elements)
    {
        auto edge = std::make_pair(node, node);
        if(binary_bv_conditions[to].find(edge) == binary_bv_conditions[to].end())
            binary_bv_conditions[to][edge] = std::vector<exprt>{};
        binary_bv_conditions[to][edge].push_back(unary_bv[from][node]);
    }
}

void cuttert::cutting_flip(int from, int to)
{
    for(auto& edge : binary_mays[from].elements)
    {
        int node1 = edge.first;
        int node2 = edge.second;
        auto flipped_edge = std::make_pair(node2, node1);
        if(binary_bv_conditions[to].find(flipped_edge) == binary_bv_conditions[to].end())
            binary_bv_conditions[to][flipped_edge] = std::vector<exprt>{};
        binary_bv_conditions[to][flipped_edge].push_back(binary_bv[from][edge]);
    }
}

void cuttert::cutting_plus(int from, int to)
{
    cutting_binary_alt(from, to);
    cutting_seq(from, to, link_post::LEFT, to);
}