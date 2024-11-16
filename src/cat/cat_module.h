#ifndef CAT_MODULE_H
#define CAT_MODULE_H

#include "cat_type.h"

#include <map>
#include <set>
#include <stack>

class cat_modulet
{
    unsigned rel_number = 0;
public:
    // relations
    cat_relationt make_relation(rel_opt op_type, std::vector<std::string> operands);
    cat_relationt make_star(std::string operand);
    cat_relationt make_qmark(std::string operand);
    cat_relationt make_fencerel(std::string operand);

    cat_relationt make_base(std::string name);
    cat_relationt make_free();
    void rename_relation(cat_relationt relation, std::string new_name, bool is_rec);
    void addBase(std::string name, int arity);

    // axioms
    void addConstraint(rel_axiomt axiom_type, cat_relationt relation);

    void remove_unnecessary();

    void replace_plus();

    void remove_duplicate_propagate_edges();

    void recursion_simulation();
    void recursion_simulation_per_relation(std::string relation);

    void build_propagate_map_all();

    void show();

    rel_axiomt get_axiom(std::string real_name);

    std::set<std::string> unary_relations;
    std::set<std::string> binary_relations;

    std::set<std::string> positive_relations;
    std::set<std::string> negative_relations;
    std::set<std::string> need_must_may_set_relations;

    std::set<std::string> necessary_relations;

    std::set<std::string> free_relations;
    std::set<std::string> recursive_relations;

    std::map<std::string, std::set<std::string>> unary_musts;
    std::map<std::string, std::set<std::string>> unary_mays;
    std::map<std::string, std::set<std::pair<std::string, std::string>>> binary_musts;
    std::map<std::string, std::set<std::pair<std::string, std::string>>> binary_mays;
private:
    std::map<std::string, cat_relationt> relation_map;

    std::set<std::string> empty_axioms;
    std::set<std::string> not_empty_axioms;
    std::set<std::string> irreflexive_axioms;

    bool is_unary(std::string name) {return unary_relations.find(name) != unary_relations.end();}
    bool is_binary(std::string name) {return binary_relations.find(name) != binary_relations.end();}
public:
    int get_arity(std::string name)
    {
        if(is_unary(name)) return 1;
        if(is_binary(name)) return 2;
        return 0;
    }
    cat_relationt& get_relation_from_name(std::string name) { return relation_map[name]; }
private:
    void set_arity(std::string name, int arity)
    {
        if(arity == 1) unary_relations.insert(name);
        if(arity == 2) binary_relations.insert(name);
    }

    void build_positivity(std::string relation_name, bool positivity);
    void build_need_must_may_set(std::string relation_name, bool need_must_may_set);
    void build_arity(std::string relation_name);
    void build_necessary(std::string relation_name);

    void buildPropagateMap(std::string relation_name);

public:
    std::map<std::string, std::vector<cat_edget>> propagate_forward; // a relation can lead to what relations?
    std::map<std::string, std::vector<cat_edget>> propagate_backward; // a relation is derived from what relations?

};

#endif // CAT_MODULE_H