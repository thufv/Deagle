#include "cat_module.h"

#include <iostream>

#define contains(container, key) ((container).find(key) != (container).end())

std::string op_to_str(rel_opt op)
{
    switch(op)
    {
    case TERMINAL:
        return "";
    case ALT:
        return "|";
    case SEQ:
        return ";";
    case AND:
        return "&";
    case SUB:
        return "\\";
    case PROD:
        return "*";
    case BRACKET:
        return "[]";
    case FLIP:
        return "^-1";
    case PLUS:
        return "+";
    default:
        return "";
    }
}

std::string get_relation_str(rel_opt op_type, std::vector<std::string> operands)
{
    if(operands.size() == 1)
        return operands[0] + op_to_str(op_type);
    if(operands.size() == 2)
        return operands[0] + op_to_str(op_type) + operands[1];
    return "unsupported";
}

cat_relationt cat_modulet::make_relation(rel_opt op_type, std::vector<std::string> operands)
{
    static std::map<std::string, cat_relationt> existing_relations;

    auto relation_str = get_relation_str(op_type, operands);
    if(contains(existing_relations, relation_str))
        return existing_relations[relation_str];

    std::string name = "p" + std::to_string(rel_number++);
    cat_relationt relation(name, op_type, operands);
    relation_map[name] = relation;
    existing_relations[relation_str] = relation;

    std::cout << "make_relation: let " << name << " = " << relation_str << "\n";

    return relation;
}

cat_relationt cat_modulet::make_star(std::string operand)
{
    auto plus = make_relation(rel_opt::PLUS, std::vector<std::string>{operand});
    make_base("id");
    return make_relation(rel_opt::ALT, std::vector<std::string>{plus.name, "id"});
}

cat_relationt cat_modulet::make_qmark(std::string operand)
{
    make_base("id");
    return make_relation(rel_opt::ALT, std::vector<std::string>{operand, "id"});
}

cat_relationt cat_modulet::make_fencerel(std::string operand)
{
    auto bracket_f = make_relation(rel_opt::BRACKET, std::vector<std::string>{operand});
    make_base("po");
    auto po_bracket_f = make_relation(rel_opt::SEQ, std::vector<std::string>{"po", bracket_f.name});
    return make_relation(rel_opt::SEQ, std::vector<std::string>{po_bracket_f.name, "po"});
}

cat_relationt cat_modulet::make_base(std::string name)
{
    cat_relationt relation(name, TERMINAL, std::vector<std::string>{});
    relation_map[name] = relation;
    return relation;
}

cat_relationt cat_modulet::make_free()
{
    std::string name = "free" + std::to_string(rel_number++);
    return make_base(name);
}

void cat_modulet::rename_relation(cat_relationt relation, std::string new_name, bool is_rec)
{
    auto old_name = relation.name;
    relation.name = new_name;
    relation_map[new_name] = relation;
    // relation_map.erase(old_name);

    if(is_rec || recursive_relations.find(old_name) != recursive_relations.end())
        recursive_relations.insert(new_name);

    if(old_name.find("free") != std::string::npos)
    {
        std::cout << new_name << " is free\n";
        free_relations.insert(new_name);
        set_arity(new_name, 2);
    }

    for(auto& relation_pair : relation_map)
        for(auto& operand : relation_pair.second.operands)
            if(operand == old_name)
                operand = new_name;
}

void cat_modulet::addConstraint(rel_axiomt axiom_type, cat_relationt relation)
{
    switch(axiom_type)
    {
    case rel_axiomt::EMPTY:
        empty_axioms.insert(relation.name);
        break;
    case rel_axiomt::NOT_EMPTY:
        not_empty_axioms.insert(relation.name);
        break;
    case rel_axiomt::IRREFLEXIVE:
        irreflexive_axioms.insert(relation.name);
        break;
    case rel_axiomt::ACYCLIC:
    {
        cat_relationt plus_relation = make_relation(PLUS, std::vector<std::string>{relation.name});
        addConstraint(rel_axiomt::IRREFLEXIVE, plus_relation);
    }
    // todo: later we may have NOT_EMPTY, TOTAL, NOT_TOTAL ...
    case rel_axiomt::NO_AXIOM:
    default:
        return;
    }

    bool positivity = (axiom_type != rel_axiomt::NOT_EMPTY);
    build_arity(relation.name);
    build_positivity(relation.name, positivity);
    build_need_must_may_set(relation.name, false);
    build_necessary(relation.name);
}

void cat_modulet::build_arity(std::string relation_name)
{
    auto& relation = relation_map[relation_name];
    if(relation.op_type == rel_opt::TERMINAL) // base relations
        return;

    static std::set<std::string> built_relations;
    if(built_relations.find(relation_name) != built_relations.end()) //recursive
    {
        if(get_arity(relation_name) == 0) // arity-undetermined relation, assume binary
        {
            std::cout << "WARNING: arity of " << relation_name << " undetermined, assuming 2.\n";
            set_arity(relation_name, 2);
        }
        return;
    }
    built_relations.insert(relation_name);

    for(auto& operand : relation.operands)
        build_arity(operand);

    auto arity0 = get_arity(relation.operands[0]);
    switch(relation.op_type)
    {
    case rel_opt::ALT:
    case rel_opt::AND:
    case rel_opt::SUB:
    {
        auto arity1 = get_arity(relation.operands[1]);
        if(arity0 == 1 && arity1 == 1)
            set_arity(relation_name, 1);
        else if(arity0 == 2 && arity1 == 2)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (ALT/AND/SUB)\n";
            std::cout << "left: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::cout << "right: " << relation.operands[1] << " has arity " << arity1 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::SEQ:
    {
        auto arity1 = get_arity(relation.operands[1]);
        if(arity0 == 2 && arity1 == 2)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (SEQ)\n";
            std::cout << "left: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::cout << "right: " << relation.operands[1] << " has arity " << arity1 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::PROD:
    {
        auto arity1 = get_arity(relation.operands[1]);
        if(arity0 == 1 && arity1 == 1)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (PROD)\n";
            std::cout << "left: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::cout << "right: " << relation.operands[1] << " has arity " << arity1 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::BRACKET:
    {
        if(arity0 == 1)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (BRACKET)\n";
            std::cout << "child: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::FLIP:
    {
        if(arity0 == 2)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (FLIP)\n";
            std::cout << "child: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::TERMINAL:
    {
        if(arity0 != 1 && arity0 != 2)
        {
            std::cout << "ERROR on arity of " << relation_name << " (TERMINAL)\n";
            std::cout << "it has arity " << arity0 << "\n";
            std::exit(1);
        }
    }
    case rel_opt::PLUS:
    {
        if(arity0 == 2)
            set_arity(relation_name, 2);
        else
        {
            std::cout << "ERROR on arity of " << relation_name << " (PLUS)\n";
            std::cout << "child: " << relation.operands[0] << " has arity " << arity0 << "\n";
            std::exit(1);
        }
    }
    default:
        break;
    }
}

void cat_modulet::build_positivity(std::string relation_name, bool positivity)
{
    auto& relations = positivity ? positive_relations : negative_relations;
    if(relations.find(relation_name) != relations.end())
        return;
    relations.insert(relation_name);

    auto relation = relation_map[relation_name];
    if(relation.op_type == rel_opt::SUB)
    {
        build_positivity(relation.operands[0], positivity);
        build_positivity(relation.operands[1], !positivity);
    }
    else
    {
        for(auto& operand : relation.operands)
            build_positivity(operand, positivity);
    }
}

void cat_modulet::build_need_must_may_set(std::string relation_name, bool need_must_may_set)
{
    if(contains(negative_relations, relation_name))
        need_must_may_set = true;

    static std::set<std::string> need_relations;
    static std::set<std::string> need_not_relations;
    auto& visited_relations = need_must_may_set ? need_relations : need_not_relations;
    if(contains(visited_relations, relation_name))
        return;
    visited_relations.insert(relation_name);

    if(need_must_may_set)
        need_must_may_set_relations.insert(relation_name);

    auto relation = relation_map[relation_name];
    for(auto& operand : relation.operands)
        build_need_must_may_set(operand, need_must_may_set);
}

void cat_modulet::build_necessary(std::string relation_name)
{
    if(contains(necessary_relations, relation_name))
        return;
    necessary_relations.insert(relation_name);
    auto& relation = relation_map[relation_name];
    for(auto& operand : relation.operands)
        build_necessary(operand);
}

rel_axiomt cat_modulet::get_axiom(std::string real_name)
{
    if(contains(empty_axioms, real_name))
        return rel_axiomt::EMPTY;
    if(contains(not_empty_axioms, real_name))
        return rel_axiomt::NOT_EMPTY;
    if(contains(irreflexive_axioms, real_name))
        return rel_axiomt::IRREFLEXIVE;
    return NO_AXIOM;
}

void cat_modulet::addBase(std::string name, int arity)
{
    make_base(name);
    set_arity(name, arity);
}

void cat_modulet::build_propagate_map_all()
{
    for(auto& relation : empty_axioms)
        buildPropagateMap(relation);
    for(auto& relation : irreflexive_axioms)
        buildPropagateMap(relation);
    for(auto& relation : not_empty_axioms)
        buildPropagateMap(relation);
    remove_duplicate_propagate_edges();
}

void cat_modulet::buildPropagateMap(std::string relation_name)
{
    auto relation = relation_map[relation_name];
    auto name = relation.name;

    static std::set<std::string> built_relations;
    if(built_relations.find(name) != built_relations.end()) //recursive
    {
        if(get_arity(name) == 0) // arity-undetermined relation, assume binary
        {
            std::cout << "WARNING: arity of " << name << " undetermined, assuming 2.\n";
            set_arity(name, 2);
        }
        return;
    }
    built_relations.insert(name);

    if(relation.op_type == rel_opt::TERMINAL)
        return;

    auto name0 = relation.operands[0];
    switch(relation.op_type)
    {
    case rel_opt::ALT:
    {
        auto name1 = relation.operands[1];
        cat_linkt alt_link0(ALT, name1);
        cat_edget edge0(name0, alt_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt alt_link1(ALT, name0);
        cat_edget edge1(name1, alt_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::SEQ:
    {
        auto name1 = relation.operands[1];
        cat_linkt seq_link0(SEQ, LEFT, name1);
        cat_edget edge0(name0, seq_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt seq_link1(SEQ, RIGHT, name0);
        cat_edget edge1(name1, seq_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::AND:
    {
        auto name1 = relation.operands[1];
        cat_linkt and_link0(AND, name1);
        cat_edget edge0(name0, and_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt and_link1(AND, name0);
        cat_edget edge1(name1, and_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::SUB:
    {
        auto name1 = relation.operands[1];
        cat_linkt minus_link0(SUB, LEFT, name1);
        cat_edget edge0(name0, minus_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt minus_link1(SUB, RIGHT, name0);
        cat_edget edge1(name1, minus_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::PROD:
    {
        auto name1 = relation.operands[1];
        cat_linkt times_link0(PROD, LEFT, name1);
        cat_edget edge0(name0, times_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt times_link1(PROD, RIGHT, name0);
        cat_edget edge1(name1, times_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::BRACKET:
    {
        cat_linkt bracket_link(BRACKET);
        cat_edget edge(name0, bracket_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::FLIP:
    {
        cat_linkt flip_link(FLIP);
        cat_edget edge(name0, flip_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::PLUS:
    {
        cat_linkt plus_link(PLUS);
        cat_edget edge(name0, plus_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
    }
    case rel_opt::TERMINAL:
    default:
        break;
    }

    for(auto& operand : relation.operands)
        buildPropagateMap(operand);
}

void cat_modulet::show()
{
    for(auto& pair : propagate_forward)
    {
        if(pair.second.empty())
            continue;
        std::cout << pair.first << " has the following edges:\n";
        for(auto& edge : pair.second)
        {
            std::cout << "\t" << edge.from << " ";

            auto link = edge.link;
            auto pos = link.link_position;
            auto another = link.another_kind;
            switch(link.link_type)
            {
            case ALT:
                std::cout << "(alt)";
                break;
            case SEQ:
                if(pos == LEFT)
                    std::cout << "(;" << another << ")";
                else if(pos == RIGHT)
                    std::cout << "(" << another << ";)";
                break;
            case AND:
                std::cout << "(and " << another << ")";
                break;
            case SUB:
                if(pos == LEFT)
                    std::cout << "(/" << another << ")";
                else if(pos == RIGHT)
                    std::cout << "(" << another << "/)";
                break;
            case PROD:
                if(pos == LEFT)
                    std::cout << "(*" << another << ")";
                else if(pos == RIGHT)
                    std::cout << "(" << another << "*)";
                break;
            case BRACKET:
                std::cout << "([])";
                break;
            case FLIP:
                std::cout << "(^-1)";
                break;
            case PLUS:
                std::cout << "+";
                break;
            case TERMINAL:
            default:
                break;
            }

            std::cout << " " << edge.to << "\n";
        }
    }

    for(auto& relation : empty_axioms)
        std::cout << "empty " << relation << "\n";
    for(auto& relation : irreflexive_axioms)
        std::cout << "irreflexive " << relation << "\n";
    for(auto& relation : not_empty_axioms)
        std::cout << "not-empty " << relation << "\n";
}

void cat_modulet::remove_unnecessary()
{
    for(auto it = unary_relations.begin(); it != unary_relations.end();)
        if(!contains(necessary_relations, *it))
            it = unary_relations.erase(it);
        else
            it++;
    for(auto it = binary_relations.begin(); it != binary_relations.end();)
        if(!contains(necessary_relations, *it))
            it = binary_relations.erase(it);
        else
            it++;
    for(auto it = relation_map.begin(); it != relation_map.end();)
        if(!contains(necessary_relations, it->first))
            it = relation_map.erase(it);
        else
            it++;
}

void cat_modulet::remove_duplicate_propagate_edges()
{
    for(auto& pair : propagate_forward)
    {
        auto& edges = pair.second;
        std::set<cat_edget> edges_set(edges.begin(), edges.end());
        edges.assign(edges_set.begin(), edges_set.end());
    }
    for(auto& pair : propagate_backward)
    {
        auto& edges = pair.second;
        std::set<cat_edget> edges_set(edges.begin(), edges.end());
        edges.assign(edges_set.begin(), edges_set.end());
    }
}

void cat_modulet::replace_plus()
{
    for(auto& relation_pair : relation_map)
    {
        auto relation_name = relation_pair.first;
        auto relation = relation_pair.second;
        if(relation.op_type != PLUS)
            continue;

        auto operand = relation.operands[0];
        auto seq_relation = make_relation(rel_opt::SEQ, std::vector<std::string>{relation_name, relation_name});
        auto new_relation = cat_relationt(relation_name, rel_opt::ALT, std::vector<std::string>{operand, seq_relation.name});
        relation_map[relation_name] = new_relation;
        recursive_relations.insert(relation_name);

        std::cout << "replace plus: " << get_relation_str(relation.op_type, relation.operands) << " becomes " << get_relation_str(new_relation.op_type, new_relation.operands) << "\n";

        if(contains(positive_relations, relation_name))
            positive_relations.insert(seq_relation.name);
        if(contains(negative_relations, relation_name))
            negative_relations.insert(seq_relation.name);
        if(contains(necessary_relations, relation_name))
            necessary_relations.insert(seq_relation.name);
        if(contains(unary_relations, relation_name))
            unary_relations.insert(seq_relation.name);
        if(contains(binary_relations, relation_name))
            binary_relations.insert(seq_relation.name);
    }
}

void cat_modulet::recursion_simulation()
{  
    for(auto relation : recursive_relations)
        if(negative_relations.find(relation) != negative_relations.end())
            recursion_simulation_per_relation(relation);
}

void cat_modulet::recursion_simulation_per_relation(std::string relation)
{
    std::string new_relation = relation + "_prime";
    auto new_free = make_free();
    rename_relation(new_free, new_relation, false);

    for(auto& relation_pair : relation_map)
    {
        auto& operands = relation_pair.second.operands;
        for(auto& operand_relation : operands)
            if(operand_relation == relation)
                operand_relation = new_relation;
    }

    auto new_bigger_than_old = make_relation(rel_opt::SUB, {new_relation, relation});
    addConstraint(rel_axiomt::EMPTY, new_bigger_than_old);
    positive_relations.insert(new_bigger_than_old.name);
    necessary_relations.insert(new_bigger_than_old.name);
    binary_relations.insert(new_bigger_than_old.name);

    auto old_bigger_than_new = make_relation(rel_opt::SUB, {relation, new_relation});
    addConstraint(rel_axiomt::EMPTY, old_bigger_than_new);
    positive_relations.insert(old_bigger_than_new.name);
    necessary_relations.insert(old_bigger_than_new.name);
    binary_relations.insert(old_bigger_than_new.name);

    // both positive and negative
    positive_relations.insert(relation);
    negative_relations.insert(relation);

    positive_relations.insert(new_relation);
    negative_relations.insert(new_relation);
    necessary_relations.insert(new_relation);
    binary_relations.insert(new_relation);

    binary_musts[new_relation] = binary_musts[relation];
    binary_mays[new_relation] = binary_mays[relation];
}