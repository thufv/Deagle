#include "cat_module.h"

#include <iostream>

#define contains(container, key) ((container).find(key) != (container).end())

void cat_modulet::registerID(std::string id, cat_relationt relation)
{
    relation.known_name = id;
    relation_map[relation.real_name] = relation;
    known_to_real[relation.known_name] = relation.real_name;
    real_to_known[relation.real_name] = relation.known_name;
}

void cat_modulet::addConstraint(cat_axiomt axiom)
{
    switch(axiom.axiom_type)
    {
    case rel_axiomt::EMPTY:
        empty_axioms.insert(axiom.relation.real_name);
        break;
    case rel_axiomt::ACYCLIC:
        acyclic_axioms.insert(axiom.relation.real_name);
        break;
    case rel_axiomt::IRREFLEXIVE:
        irreflexive_axioms.insert(axiom.relation.real_name);
        break;
    case rel_axiomt::NO_AXIOM:
        return;
    }
    all_axioms[axiom.relation.real_name] = axiom.axiom_type;
    buildPropagateMap(axiom.relation);
}

rel_axiomt cat_modulet::get_axiom(std::string real_name)
{
    if(!contains(all_axioms, real_name))
        return NO_AXIOM;
    return all_axioms[real_name];
}

void cat_modulet::addBase(std::string name, int arity)
{
    base_relations.insert(name);
    set_arity(name, arity);
}

std::string cat_modulet::get_real_name(cat_relationt& relation)
{
    std::string real_name = relation.real_name;
    while(relation.op == rel_opt::TERMINATE && contains(known_to_real, real_name))
        real_name = known_to_real[real_name];
    return real_name;
}

void cat_modulet::buildPropagateMap(cat_relationt relation)
{
    if(relation.op == rel_opt::TERMINATE)
    {
        //try to find its definition
        auto name = get_real_name(relation);
        relation = relation_map[name];
        if(relation.op == rel_opt::TERMINATE)
            return;
    }

    auto name = get_real_name(relation);

    static std::set<std::string> built_relation;
    if(built_relation.find(name) != built_relation.end()) //recursive
    {
        if(get_arity(name) == 0) // arity-undetermined relation, assume binary
        {
            std::cout << "WARNING: arity of " << name << " undetermined, assuming 2.\n";
            set_arity(name, 2);
        }
        return;
    }
    built_relation.insert(name);

    auto name0 = get_real_name(relation.operands[0]);
    switch(relation.op)
    {
    case rel_opt::ALT:
    {
        auto name1 = get_real_name(relation.operands[1]);
        cat_linkt alt_link(ALT);
        cat_edget edge0(name0, alt_link, name);
        propagate_forward[name0].push_back(edge0);
        cat_edget edge1(name1, alt_link, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::SEQ:
    {
        auto name1 = get_real_name(relation.operands[1]);
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
        auto name1 = get_real_name(relation.operands[1]);
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
    case rel_opt::MINUS:
    {
        auto name1 = get_real_name(relation.operands[1]);
        cat_linkt minus_link0(MINUS, LEFT, name1);
        cat_edget edge0(name0, minus_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt minus_link1(MINUS, RIGHT, name0);
        cat_edget edge1(name1, minus_link1, name);
        propagate_forward[name1].push_back(edge1);

        propagate_backward[name].push_back(edge0);
        propagate_backward[name].push_back(edge1);
        break;
    }
    case rel_opt::TIMES:
    {
        auto name1 = get_real_name(relation.operands[1]);
        cat_linkt times_link0(TIMES, LEFT, name1);
        cat_edget edge0(name0, times_link0, name);
        propagate_forward[name0].push_back(edge0);
        cat_linkt times_link1(TIMES, RIGHT, name0);
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
    case rel_opt::PLUS:
    {
        cat_linkt plus_link(PLUS);
        cat_edget edge(name0, plus_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::STAR:
    {
        cat_linkt star_link(STAR);
        cat_edget edge(name0, star_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::QMARK:
    {
        cat_linkt qmark_link(QMARK);
        cat_edget edge(name0, qmark_link, name);
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
    case rel_opt::ROT:
    {
        cat_linkt rot_link(ROT);
        cat_edget edge(name0, rot_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::INT:
    {
        cat_linkt int_link(INT);
        cat_edget edge(name0, int_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::EXT:
    {
        cat_linkt ext_link(EXT);
        cat_edget edge(name0, ext_link, name);
        propagate_forward[name0].push_back(edge);

        propagate_backward[name].push_back(edge);
        break;
    }
    case rel_opt::TERMINATE:
    case rel_opt::GUARD_ENABLEMENT: //temp
    default:
        break;
    }

    for(auto& operand : relation.operands)
        buildPropagateMap(operand);

    //calc arity
    auto arity0 = get_arity(name0);
    switch(relation.op)
    {
    case rel_opt::ALT:
    case rel_opt::AND:
    case rel_opt::MINUS:
    {
        auto name1 = get_real_name(relation.operands[1]);
        auto arity1 = get_arity(name1);
        if(arity0 == 1 && arity1 == 1)
            set_arity(name, 1);
        else if(arity0 == 2 && arity1 == 2)
            set_arity(name, 2);
        else
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::cout << "left: " << name0 << " has arity " << arity0 << "\n";
            std::cout << "right: " << name1 << " has arity " << arity1 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::SEQ:
    {
        auto name1 = get_real_name(relation.operands[1]);
        auto arity1 = get_arity(name1);
        if(arity0 == 2 && arity1 == 2)
            set_arity(name, 2);
        else
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::cout << "left: " << name0 << " has arity " << arity0 << "\n";
            std::cout << "right: " << name1 << " has arity " << arity1 << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::TIMES:
    {
        auto name1 = get_real_name(relation.operands[1]);
        auto arity1 = get_arity(name1);
        if(arity0 == 1 && arity1 == 1)
            set_arity(name, 2);
        else
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::BRACKET:
    {
        if(arity0 == 1)
            set_arity(name, 2);
        else
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::PLUS:
    case rel_opt::STAR:
    case rel_opt::QMARK:
    case rel_opt::FLIP:
    case rel_opt::ROT:
    case rel_opt::INT:
    case rel_opt::EXT:
    {
        if(arity0 == 2)
            set_arity(name, 2);
        else
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::exit(1);
        }
        break;
    }
    case rel_opt::TERMINATE:
    {
        if(arity0 != 1 && arity0 != 2)
        {
            std::cout << "ERROR on arity of " << name << "\n";
            std::exit(1);
        }
    }
    case rel_opt::GUARD_ENABLEMENT:
    default:
        break;
    }
}

void cat_modulet::show()
{
    for(auto& pair : propagate_forward)
    {
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
            case MINUS:
                if(pos == LEFT)
                    std::cout << "(/" << another << ")";
                else if(pos == RIGHT)
                    std::cout << "(" << another << "/)";
                break;
            case TIMES:
                if(pos == LEFT)
                    std::cout << "(*" << another << ")";
                else if(pos == RIGHT)
                    std::cout << "(" << another << "*)";
                break;
            case BRACKET:
                std::cout << "([])";
                break;
            case PLUS:
                std::cout << "(+)";
                break;
            case STAR:
                std::cout << "(*)";
                break;
            case QMARK:
                std::cout << "(?)";
                break;
            case FLIP:
                std::cout << "(^-1)";
                break;
            case ROT:
                std::cout << "(rot)";
                break;
            case INT:
                std::cout << "(int)";
                break;
            case EXT:
                std::cout << "(ext)";
                break;
            case rel_opt::TERMINATE:
            case rel_opt::GUARD_ENABLEMENT:
            default:
                break;
            }

            std::cout << " " << edge.to << "\n";
        }
    }
}

void cat_modulet::calc_advance()
{
    bool need_repeat = true;
    while(need_repeat)
    {
        need_repeat = false;
        for(auto& propagation_pair : propagate_forward)
            for(auto& edge : propagation_pair.second)
            {
                if((edge.link.link_type == rel_opt::MINUS && edge.link.link_position == link_post::RIGHT) || contains(advance, edge.to))
                {
                    if(!contains(advance, edge.from))
                    {
                        advance.insert(edge.from);
                        need_repeat = true;
                    }
                }
            }
    }
    for(auto advance_kind : advance)
        std::cout << advance_kind << " should be determined in advance\n";
}

// szh: BINARY relations such as rf are enabled only if guards on both sides hold
// firstly, rf-raw is modeled into the formula
// then axiom rf-raw(e1, e2) \wedge guard(e1) \wedge guard(e2) \rightarrow rf(e1, e2) is used

void cat_modulet::guard_extension()
{
    std::string guard = "guard";

    for(auto& propagation_pair : propagate_forward)
    {
        auto& from = propagation_pair.first;
        std::string from_raw = from + "-raw";
        if(contains(base_relations, from))
        {
            // if(contains(unary_relations, from))
            // {
            //     cat_edget from_raw_to_from(from_raw, cat_linkt(rel_opt::AND, guard), from);
            //     propagate_forward[from_raw].push_back(from_raw_to_from);
            //     cat_edget guard_to_from(guard, cat_linkt(rel_opt::AND, from_raw), from);
            //     propagate_forward[guard].push_back(guard_to_from);
            //     set_arity(from_raw, 1);
            // }
            if(contains(binary_relations, from))
            {
                cat_edget from_raw_to_from(from_raw, cat_linkt(rel_opt::GUARD_ENABLEMENT, guard), from);
                propagate_forward[from_raw].push_back(from_raw_to_from);
                cat_edget guard_to_from(guard, cat_linkt(rel_opt::GUARD_ENABLEMENT, from_raw), from);
                propagate_forward[guard].push_back(guard_to_from);
                set_arity(from_raw, 2);
            }
        }
    }

    set_arity(guard, 1);
}

void cat_modulet::known_real_spread()
{
    for(auto& axiom : all_axioms)
    {
        if(contains(known_to_real, axiom.first))
            all_axioms[known_to_real[axiom.first]] = axiom.second;
        if(contains(real_to_known, axiom.first))
            all_axioms[real_to_known[axiom.first]] = axiom.second;
    }
    for(auto& name : empty_axioms)
    {
        if(contains(known_to_real, name))
            empty_axioms.insert(known_to_real[name]);
        if(contains(real_to_known, name))
            empty_axioms.insert(real_to_known[name]);
    }
    for(auto& name : acyclic_axioms)
    {
        if(contains(known_to_real, name))
            acyclic_axioms.insert(known_to_real[name]);
        if(contains(real_to_known, name))
            acyclic_axioms.insert(real_to_known[name]);
    }
    for(auto& name : irreflexive_axioms)
    {
        if(contains(known_to_real, name))
            irreflexive_axioms.insert(known_to_real[name]);
        if(contains(real_to_known, name))
            irreflexive_axioms.insert(real_to_known[name]);
    }
}

void cat_modulet::remove_dummy_set(std::set<std::string>& s)
{
    for(auto it = s.begin(); it != s.end();)
    {
        auto& relation = *it;
        if(propagate_forward[relation].empty() && all_axioms.find(relation) == all_axioms.end())
        {
            std::cout << relation << " is removed\n";
            it = s.erase(it);
        }
        else
            it++;
    }
}

void cat_modulet::simplify_graph()
{
    std::set<std::string> all_relations;
    all_relations.insert(unary_relations.begin(), unary_relations.end());
    all_relations.insert(binary_relations.begin(), binary_relations.end());

    bool repeat = true;
    while(repeat)
    {
        repeat = false;
        for(auto relation : all_relations)
        {
            // we cannot remove relations with axioms
            if(contains(all_axioms, relation))
                continue;

            // we do not remove base or final relations
            auto& forward_vec = propagate_forward[relation];
            auto& backward_vec = propagate_backward[relation];
            if(forward_vec.empty() || backward_vec.empty())
                continue;

            // we only simplify trivial relations
            bool can_remove = true;
            for(auto forward_edge : forward_vec)
                if(forward_edge.link.link_type != rel_opt::ALT)
                    can_remove = false;
            for(auto backward_edge : backward_vec)
                if(backward_edge.link.link_type != rel_opt::ALT)
                    can_remove = false;
            if(!can_remove)
                continue;

            std::cout << relation << " should be removed\n";
            std::cout << relation << " had forward ";
            for(auto forward_edge : forward_vec)
                std::cout << forward_edge.to << " ";
            std::cout << "\n";
            std::cout << relation << " had backward ";
            for(auto backward_edge : backward_vec)
                std::cout << backward_edge.from << " ";
            std::cout << "\n";
            
            for(auto forward_edge : forward_vec)
                for(auto backward_edge : backward_vec)
                {
                    auto backward_relation = backward_edge.from;
                    auto forward_relation = forward_edge.to;
                    cat_linkt alt_link(ALT);
                    cat_edget edge(backward_relation, alt_link, forward_relation);
                    propagate_forward[backward_relation].push_back(edge);
                    propagate_backward[forward_relation].push_back(edge);
                    repeat = true;
                }
            
            for(auto backward_edge : backward_vec)
                for(auto it = propagate_forward[backward_edge.from].begin(); it != propagate_forward[backward_edge.from].end();)
                    if(it->to == relation && it->link.link_type == rel_opt::ALT)
                        it = propagate_forward[backward_edge.from].erase(it);
                    else
                        it++;

            for(auto forward_edge : forward_vec)
                for(auto it = propagate_backward[forward_edge.to].begin(); it != propagate_backward[forward_edge.to].end();)
                    if(it->from == relation && it->link.link_type == rel_opt::ALT)
                        it = propagate_backward[forward_edge.to].erase(it);
                    else
                        it++;
            
            propagate_forward[relation].clear();
            propagate_backward[relation].clear();
        }
    }
}

void cat_modulet::remove_dummy()
{
    remove_dummy_set(base_relations);
    remove_dummy_set(unary_relations);
    remove_dummy_set(binary_relations);
}