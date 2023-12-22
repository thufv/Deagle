#ifndef MEMORY_MODEL_SOLVER_STRUCT_H
#define MEMORY_MODEL_SOLVER_STRUCT_H

#include <vector>
#include <set>

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

namespace Minisat
{

typedef std::vector<Lit> literal_vector;
typedef std::pair<int, literal_vector> elementt;

class unary_sett
{
    std::vector<elementt> elements; //1 for in, 0 for not in
    std::set<int> element_set;
public:
    unary_sett(int size = 0) {elements.resize(size);}

    bool contains(int node_id) {return bool(elements[node_id].first);}
    literal_vector& get_reason(int node_id) {return elements[node_id].second;}

    std::set<int>& all_elements() {return element_set;}

    void set(int node_id, literal_vector lv) 
    {
        elements[node_id] = std::make_pair(1, lv);
        element_set.insert(node_id);
    }
    void unset(int node_id) 
    {
        elements[node_id] = std::make_pair(0, literal_vector());
        element_set.erase(node_id);
    }

    int size() {return elements.size();}
};

class binary_sett //also called event graph!
{
    std::vector<std::vector<elementt>> elements; //1 for in, 0 for not in
    std::vector<std::set<int>> outs;
    std::vector<std::set<int>> ins;
public:
    binary_sett(int size = 0) 
    {
        elements.resize(size);
        outs.resize(size);
        ins.resize(size);
        for(auto& row : elements)
            row.resize(size);
    }

    bool contains(int node1_id, int node2_id) {return bool(elements[node1_id][node2_id].first);}
    literal_vector& get_reason(int node1_id, int node2_id) {return elements[node1_id][node2_id].second;}

    void set(int node1_id, int node2_id, literal_vector lv)
    {
        elements[node1_id][node2_id] = std::make_pair(1, lv);
        outs[node1_id].insert(node2_id);
        ins[node2_id].insert(node1_id);
    }
    void unset(int node1_id, int node2_id) 
    {
        elements[node1_id][node2_id] = std::make_pair(0, literal_vector());
        outs[node1_id].erase(node2_id);
        ins[node2_id].erase(node1_id);
    }

    std::set<int>& get_outs(int node_id) {return outs[node_id];}
    std::set<int>& get_ins(int node_id) {return ins[node_id];}

    int size() {return outs.size();}
};
}

#endif // MEMORY_MODEL_SOLVER_STRUCT_H
