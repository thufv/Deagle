// __SZH_ADD_BEGIN__

#ifndef CLOSURESOLVER_H
#define CLOSURESOLVER_H

#include <map>

#include "../../minisat/mtl/Queue.h"
#include "../../minisat/core/Solver.h"

#include "ClosureTypes.h"
#include "Closure.h"



typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oc_edge_tablet;
typedef std::map<std::string, Minisat::Lit> oc_guard_mapt;
typedef std::map<std::string, int> oc_location_mapt;

namespace Minisat
{

class ClosureSolver : public Solver
{
    closure graph;

    oc_edge_tablet oc_edge_table;
    oc_guard_mapt oc_guard_map;
    oc_location_mapt oc_location_map;

    std::vector<std::pair<Lit, literal_set>> literals_to_assign;
    std::set<Lit> assigned_literals;

    int conflict_cycle;
    int theory_propagation;

protected:
    CRef propagate();
    void cancelUntil(int level);
    void newDecisionLevel();
    lbool search(int nof_conflicts);
    void analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel);
public:
    ClosureSolver();
    void init();
    void save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_guard_mapt& _oc_guard_map, oc_location_mapt& _oc_location_map, std::map<std::string, int>& _oc_result_order);
    void set_graph();

    lbool solve_();
    bool solve() { budgetOff(); assumptions.clear(); return solve_() == l_True; }
    inline bool solve(const vec<Lit>& assumps){ budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }
    inline lbool solveLimited(const vec<Lit>& assumps){ assumps.copyTo(assumptions); return solve_(); }

    void show_rf();

    void assign_literal(Lit l, literal_set& lv);
    void assign_literal(Lit l, literal_vector& lv);

    void apply_literal_assignment();

    std::vector<std::string> get_write_order();

    //interface
    virtual void addOC(std::string from, std::string to, std::string type, Lit related_lit) override;
    virtual void addGuard(std::string node, Lit guard) override;
    void setRawGraph(oc_edge_tablet& _oc_edge_table, oc_guard_mapt& _oc_guard_map);

    std::map<std::string, int>* oc_result_order;
};

}


#endif //CLOSURESOLVER_H

// __SZH_ADD_END__