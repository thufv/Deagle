// __SZH_ADD_BEGIN__

#ifndef CLOSURESOLVER_H
#define CLOSURESOLVER_H

#include <map>

#include "../../minisat/mtl/Queue.h"
#include "../../minisat/core/Solver.h"

#include "ClosureTypes.h"
#include "Closure.h"



typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oclt_convert_tablet;
typedef std::map<std::string, Minisat::Lit> oclt_guard_mapt;
typedef std::map<std::string, int> oclt_location_mapt;

namespace Minisat
{

class ClosureSolver : public Solver
{
    closure graph;

    oclt_convert_tablet oclt_convert_table;
    oclt_guard_mapt oclt_write_guard;
    oclt_location_mapt oclt_write_location;

    std::vector<std::pair<Lit, literal_set>> literals_to_assign;

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
    void save_raw_graph(oclt_convert_tablet& _oclt_convert_table, oclt_guard_mapt& _oclt_write_guard, oclt_location_mapt& _oclt_write_location);
    void set_graph();

    lbool solve_();
    bool solve() { budgetOff(); assumptions.clear(); return solve_() == l_True; }
    bool solve(const vec<Lit>& assumps) { budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }

    void show_rf();

    void assign_literal(Lit l, literal_set& lv);
    void assign_literal(Lit l, literal_vector& lv);

    std::vector<std::string> get_write_order();
};

}


#endif //CLOSURESOLVER_H

// __SZH_ADD_END__