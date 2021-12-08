// __SZH_ADD_BEGIN__

#ifndef ICDSOLVER_H
#define ICDSOLVER_H

#include <map>

#include "../../minisat/mtl/Queue.h"
#include "../../minisat/core/Solver.h"

#include "ICDTypes.h"
#include "ICD.h"



typedef std::vector<std::pair<std::pair<std::string, std::string>, std::pair<Minisat::Lit, std::string> > > oclt_convert_tablet;

namespace Minisat
{

class ICDSolver : public Solver
{
    ICD graph;

    oclt_convert_tablet oclt_convert_table;

    std::vector<std::pair<Lit, literal_set>> literals_to_assign;

    int conflict_cycle;

protected:
    CRef propagate();
    void cancelUntil(int level);
    void newDecisionLevel();
    lbool search(int nof_conflicts);
    void analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel);
public:
    ICDSolver();
    void init();
    void save_raw_graph(oclt_convert_tablet& _oclt_convert_table);
    void set_graph();

    lbool solve_();
    bool solve() { budgetOff(); assumptions.clear(); return solve_() == l_True; }
    bool solve(const vec<Lit>& assumps) { budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }

    void assign_literal(Lit l, literal_set& lv);
    void assign_literal(Lit l, literal_vector& lv);
};

}


#endif //ICDSOLVER_H

// __SZH_ADD_END__