#ifndef MEMORY_MODEL_SOLVER_STRUCT_H
#define MEMORY_MODEL_SOLVER_STRUCT_H

#include <vector>
#include <set>
#include <unordered_set>

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"
#include "GraphTypes.h"

typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oc_edge_tablet;
typedef std::vector<std::pair<std::string, std::pair<Minisat::Lit, std::string> > > oc_label_tablet;

namespace Minisat
{

class unary_must_may_sett
{
public:
    bool trivial = false; // if true, the must/may set's calculation is skipped, so that it's treated as a total set
    std::set<int> elements;

    bool contains(int node_id)
    {
        if(trivial)
            return true; 
        return elements.find(node_id) != elements.end();
    }
    void set(int node_id) { elements.insert(node_id); }
    int size() { return elements.size(); }
};
typedef unary_must_may_sett unary_must_sett;
typedef unary_must_may_sett unary_may_sett;

class binary_must_may_sett
{
public:
    bool trivial = false; // if true, the must/may set's calculation is skipped, so that it's treated as a total set
    std::vector<std::set<int>> outs;

    bool contains(int node1_id, int node2_id)
    {
        if(trivial)
            return true;
        if(int(outs.size()) <= node1_id)
            return false;
        return outs[node1_id].find(node2_id) != outs[node1_id].end();
    }
    void set(int node1_id, int node2_id)
    {
        if(int(outs.size()) <= node1_id)
            outs.resize(node1_id + 1);
        outs[node1_id].insert(node2_id);
    }
    int size()
    {
        int total_size = 0;
        for(auto& out_set : outs)
            total_size += out_set.size();
        return total_size;
    }
    std::vector<std::pair<int, int>> get_elements()
    {
        std::vector<std::pair<int, int>> elements;
        for(int node1 = 0; node1 < int(outs.size()); node1++)
            for(auto node2 : outs[node1])
                elements.push_back(std::make_pair(node1, node2));
        return elements;
    }
};

typedef binary_must_may_sett binary_must_sett;
typedef binary_must_may_sett binary_may_sett;

class unary_sett
{
    std::vector<elementt> elements; //1 for in, 0 for not in
    std::vector<int> element_set;
public:
    unary_sett(int size = 0) {elements.resize(size);}

    bool contains(int node_id) {return bool(elements[node_id].first);}
    reasont& get_reason(int node_id) {return elements[node_id].second;}

    std::vector<int>& all_elements() {return element_set;}
    int get_element_num() {return element_set.size();}

    void set(int node_id, reasont reason)
    {
        // assert(elements[node_id].first == 0);
        elements[node_id] = std::make_pair(1, reason);
        element_set.push_back(node_id);
    }
    void unset(int node_id) 
    {
        // assert(elements[node_id].first == 1);
        elements[node_id] = std::make_pair(0, reasont());
        // assert(element_set.back() == node_id);
        element_set.pop_back();
    }

    int size() {return elements.size();}
};

class binary_sett //also called event graph!
{
    std::vector<std::vector<elementt>> elements; //1 for in, 0 for not in
    std::vector<std::vector<int>> outs;
    std::vector<std::vector<int>> ins;
    int element_num = 0;
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
    reasont& get_reason(int node1_id, int node2_id) {return elements[node1_id][node2_id].second;}

    void set(int node1_id, int node2_id, reasont reason)
    {
        // assert(elements[node1_id][node2_id].first == 0);
        elements[node1_id][node2_id] = std::make_pair(1, reason);
        outs[node1_id].push_back(node2_id);
        ins[node2_id].push_back(node1_id);
        element_num++;
    }
    void unset(int node1_id, int node2_id) 
    {
        // assert(elements[node1_id][node2_id].first == 1);
        elements[node1_id][node2_id] = std::make_pair(0, reasont());
        // assert(outs[node1_id].back() == node2_id);
        outs[node1_id].pop_back();
        // assert(ins[node2_id].back() == node1_id);
        ins[node2_id].pop_back();
        element_num--;
    }

    std::vector<int>& get_outs(int node_id) {return outs[node_id];}
    std::vector<int>& get_ins(int node_id) {return ins[node_id];}

    int size() {return outs.size();}
    int get_element_num() {return element_num;}
};
}

#endif // MEMORY_MODEL_SOLVER_STRUCT_H
