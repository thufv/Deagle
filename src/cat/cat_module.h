#ifndef CAT_MODULE_H
#define CAT_MODULE_H

#include "cat_type.h"

#include <map>
#include <set>

class cat_modulet
{
public:
    void registerID(std::string id, cat_relationt relation);
    void addConstraint(cat_axiomt axiom);
    void addBase(std::string name, int arity);
    void show();

    std::set<std::string> advance;
    void calc_advance();
    void guard_extension();
    void simplify_graph();
    void remove_dummy_set(std::set<std::string>& s);
    void remove_dummy();
    void known_real_spread();

    rel_axiomt get_axiom(std::string real_name);

    std::set<std::string> base_relations;

    std::set<std::string> unary_relations;
    std::set<std::string> binary_relations;
private:
    std::map<std::string, cat_relationt> relation_map; //use real_name
    std::map<std::string, std::string> known_to_real;
    std::map<std::string, std::string> real_to_known;

    std::set<std::string> empty_axioms; // all use real_name
    std::set<std::string> acyclic_axioms;
    std::set<std::string> irreflexive_axioms;
    std::map<std::string, rel_axiomt> all_axioms;

    bool is_unary(std::string name) {return unary_relations.find(name) != unary_relations.end();}
    bool is_binary(std::string name) {return binary_relations.find(name) != binary_relations.end();}
public:
    int get_arity(std::string name)
    {
        if(is_unary(name)) return 1;
        if(is_binary(name)) return 2;
        return 0;
    }
private:
    void set_arity(std::string name, int arity)
    {
        if(arity == 1) unary_relations.insert(name);
        if(arity == 2) binary_relations.insert(name);
    }

    void buildPropagateMap(cat_relationt relation);
    std::string get_real_name(cat_relationt& relation);

public:
    std::map<std::string, std::vector<cat_edget>> propagate_forward; // a relation can lead to what relations?
    std::map<std::string, std::vector<cat_edget>> propagate_backward; // a relation is derived from what relations?

};

#endif // CAT_MODULE_H