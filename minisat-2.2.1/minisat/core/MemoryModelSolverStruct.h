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

class mm_reasont
{
    Lit terminal;
    std::vector<mm_reasont*> sources;

    void get_full_reason_rec(literal_vector& lv)
    {
        if(terminal.x != -1) lv.push_back(terminal);
        for(auto source : sources)
            source->get_full_reason_rec(lv);
    }

public:
    int real_size = 0; // may be greater than sources.size(), because some sources are too trivial to be maintained. BUT THEY COUNT.

    bool trivial() {return terminal.x == -1 && sources.empty();}

    mm_reasont(Lit _terminal = lit_Error) :
        terminal(_terminal) {}
    mm_reasont(mm_reasont* _source1, mm_reasont* _source2) :
        terminal(lit_Error)
    {
        if(!_source1->trivial())
            sources.push_back(_source1);
        if(!_source2->trivial())
            sources.push_back(_source2);
    }
    
    bool push_source_unless_trivial(mm_reasont* source) // return true if non-trivial so that really pushed
    {
        bool non_trivial = !source->trivial();
        if(non_trivial)
            sources.push_back(source);
        return non_trivial;
    }
    void pop_source() { sources.pop_back(); }
    int source_size() { return int(sources.size()); }

    literal_vector get_full_reason()
    {
        literal_vector lv;
        get_full_reason_rec(lv);
        return lv;
    }
};

typedef std::pair<int, mm_reasont> elementt;

class unary_may_sett
{
    std::vector<int> exists;
    std::vector<int> elements;
public:
    unary_may_sett(int size = 0) {exists = std::vector<int>(size, 0);}
    bool contains(int node_id)
    {
        if(node_id >= int(exists.size()))
            return false;
        return bool(exists[node_id]);
    }
    void set(int node_id)
    {
        exists[node_id] = 1;
        elements.push_back(node_id);
    }
    std::vector<int>& get_elements() {return elements;}
};

class binary_may_sett
{
    std::vector<std::vector<int>> exists;
    std::vector<std::pair<int, int>> elements;
public:
    binary_may_sett(int size = 0)
    {
        exists.resize(size);
        for(auto& row : exists)
            row = std::vector<int>(size, 0);
    }
    bool contains(int node1_id, int node2_id)
    {
        if(node1_id >= int(exists.size()))
            return false;
        if(node2_id >= int(exists[node1_id].size()))
            return false;
        return bool(exists[node1_id][node2_id]);
    }
    void set(int node1_id, int node2_id)
    {
        exists[node1_id][node2_id] = 1;
        elements.push_back(std::make_pair(node1_id, node2_id));
    }
    std::vector<std::pair<int, int>> get_elements() {return elements;}
};

class unary_sett
{
    std::vector<elementt> elements; //1 for in, 0 for not in
    std::set<int> element_set;
public:
    unary_sett(int size = 0) {elements.resize(size);}

    bool contains(int node_id) {return bool(elements[node_id].first);}
    mm_reasont& get_reason(int node_id) {return elements[node_id].second;}

    std::set<int>& all_elements() {return element_set;}
    int get_element_num() {return element_set.size();}

    void set(int node_id, mm_reasont reason)
    {
        elements[node_id] = std::make_pair(1, reason);
        element_set.insert(node_id);
    }
    void unset(int node_id) 
    {
        elements[node_id] = std::make_pair(0, mm_reasont());
        element_set.erase(node_id);
    }

    int size() {return elements.size();}
};

class binary_sett //also called event graph!
{
    std::vector<std::vector<elementt>> elements; //1 for in, 0 for not in
    std::vector<std::set<int>> outs;
    std::vector<std::set<int>> ins;
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
    mm_reasont& get_reason(int node1_id, int node2_id) {return elements[node1_id][node2_id].second;}

    void set(int node1_id, int node2_id, mm_reasont reason)
    {
        elements[node1_id][node2_id] = std::make_pair(1, reason);
        outs[node1_id].insert(node2_id);
        ins[node2_id].insert(node1_id);
        element_num++;
    }
    void unset(int node1_id, int node2_id) 
    {
        elements[node1_id][node2_id] = std::make_pair(0, mm_reasont());
        outs[node1_id].erase(node2_id);
        ins[node2_id].erase(node1_id);
        element_num--;
    }

    std::set<int>& get_outs(int node_id) {return outs[node_id];}
    std::set<int>& get_ins(int node_id) {return ins[node_id];}

    int size() {return outs.size();}
    int get_element_num() {return element_num;}
};
}

#endif // MEMORY_MODEL_SOLVER_STRUCT_H
