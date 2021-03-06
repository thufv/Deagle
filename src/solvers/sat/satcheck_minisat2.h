/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SATCHECK_MINISAT2_H
#define CPROVER_SATCHECK_MINISAT2_H

#include "cnf.h"

#include "../../../minisat-2.2.1/minisat/core/ClosureSolver.h"
#include "../../../minisat-2.2.1/minisat/core/ICDSolver.h"

// Select one: basic solver or with simplification.
// Note that the solver with simplifier isn't really robust
// when used incrementally, as variables may disappear
// unless set to 'frozen'.

namespace Minisat
{
  class Solver;
  class SimpSolver;
}

template<typename T>
class satcheck_minisat2_baset:public cnf_solvert
{
public:
  explicit satcheck_minisat2_baset(T *);
  virtual ~satcheck_minisat2_baset();
  
  virtual resultt prop_solve();
  virtual tvt l_get(literalt a) const;

  virtual void lcnf(const bvt &bv);
  virtual void set_assignment(literalt a, bool value);

  // extra MiniSat feature: solve with assumptions
  virtual void set_assumptions(const bvt &_assumptions);

  virtual bool is_in_conflict(literalt a) const;
  virtual bool has_set_assumptions() const { return true; }
  virtual bool has_is_in_conflict() const { return true; }
  
// __SZH_ADD_BEGIN__
  //our method need to access solver directly!
  T* get_solver() {return solver;}
// __SZH_ADD_END__

protected:
  T *solver;
  
  void add_variables();
  bvt assumptions;
};

class satcheck_minisat_no_simplifiert:
  public satcheck_minisat2_baset<Minisat::Solver>
{
public:
  satcheck_minisat_no_simplifiert();
  virtual const std::string solver_text();
};

class satcheck_minisat_simplifiert:
  public satcheck_minisat2_baset<Minisat::SimpSolver>
{
public:
  satcheck_minisat_simplifiert();
  virtual const std::string solver_text();
  virtual void set_frozen(literalt a);
  bool is_eliminated(literalt a) const;
};

// __SZH_ADD_BEGIN__
class satcheck_minisat_closuret:
  public satcheck_minisat2_baset<Minisat::ClosureSolver>
{
public:
  satcheck_minisat_closuret();
  virtual const std::string solver_text();
};

class satcheck_minisat_ICDt:
  public satcheck_minisat2_baset<Minisat::ICDSolver>
{
public:
  satcheck_minisat_ICDt();
  virtual const std::string solver_text();
};
// __SZH_ADD_END__

#endif
