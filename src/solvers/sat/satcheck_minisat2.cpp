/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef _MSC_VER
#include <inttypes.h>
#endif

#include <cassert>
#include <stack>

#include <util/i2string.h>
#include <util/threeval.h>

#include "satcheck_minisat2.h"

#include <core/Solver.h>
#include <simp/SimpSolver.h>
#include <core/ClosureSolver.h>
#include <core/ICDSolver.h>

#ifndef HAVE_MINISAT2
//#error "Expected HAVE_MINISAT2"
#endif
#include <iostream>
/*******************************************************************\

Function: convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void convert(const bvt &bv, Minisat::vec<Minisat::Lit> &dest)
{
  dest.capacity(bv.size());

  forall_literals(it, bv)
    if(!it->is_false())
      dest.push(Minisat::mkLit(it->var_no(), it->sign()));
}

/*******************************************************************\

Function: satcheck_minisat2_baset::l_get

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
tvt satcheck_minisat2_baset<T>::l_get(literalt a) const
{
  if(a.is_true())
    return tvt(true);
  else if(a.is_false())
    return tvt(false);

  tvt result;

  if(a.var_no()>=(unsigned)solver->model.size())
    return tvt(tvt::TV_UNKNOWN);

  using Minisat::lbool;

  if(solver->model[a.var_no()]==l_True)
    result=tvt(true);
  else if(solver->model[a.var_no()]==l_False)
    result=tvt(false);
  else
    return tvt(tvt::TV_UNKNOWN);
  
  if(a.sign()) result=!result;

  return result;
}

/*******************************************************************\

Function: satcheck_minisat_no_simplifiert::solver_text

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const std::string satcheck_minisat_no_simplifiert::solver_text()
{
  return "MiniSAT 2.2.0 without simplifier";
}

/*******************************************************************\

Function: satcheck_minisat_simplifiert::solver_text

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const std::string satcheck_minisat_simplifiert::solver_text()
{
  return "MiniSAT 2.2.0 with simplifier";
}

// __SZH_ADD_BEGIN__
const std::string satcheck_minisat_closuret::solver_text()
{
  return "A magical closure modication of MiniSAT by super noob SZH";
}

const std::string satcheck_minisat_ICDt::solver_text()
{
  return "A magical ICD modication of MiniSAT by super noob SZH";
}
// __SZH_ADD_END__

/*******************************************************************\

Function: satcheck_minisat2_baset::add_variables

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
void satcheck_minisat2_baset<T>::add_variables()
{
  while((unsigned)solver->nVars()<no_variables())
    solver->newVar();
}

/*******************************************************************\

Function: satcheck_minisat2_baset::lcnf

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
void satcheck_minisat2_baset<T>::lcnf(const bvt &bv)
{
//	unsigned i;
//	for (i = 0; i < bv.size(); i++)
//	{
//		if (bv[i].is_true())
//			break;
//	}
//	if (i == bv.size())
//	{
//		for (i = 0; i < bv.size(); i++)
//		{
//			if (!bv[i].is_false())
//				std::cout << bv[i].dimacs() << " ";
//		}
//		std::cout << "\n";
//	}

  add_variables();

  forall_literals(it, bv)
  {
    if(it->is_true())
      return;
    else if(!it->is_false())
      assert(it->var_no()<(unsigned)solver->nVars());
  }
    
  Minisat::vec<Minisat::Lit> c;

  convert(bv, c);

  // Note the underscore.
  // Add a clause to the solver without making superflous internal copy.

  solver->addClause_(c);

  clause_counter++;
//  std::cout << clause_counter << "\n";
}

/*******************************************************************\

Function: satcheck_minisat2_baset::prop_solve

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
propt::resultt satcheck_minisat2_baset<T>::prop_solve()
{
  assert(status!=ERROR);

  {
    std::string msg=
      i2string(_no_variables)+" variables, "+
      i2string(solver->nClauses())+" clauses";
    messaget::status(msg);
      std::cout << msg << "\n";
  }
  
  add_variables();
  
  std::string msg;

  if(!solver->okay())
  {
    msg="SAT checker inconsistent: negated claim is UNSATISFIABLE, i.e., holds";
    messaget::status(msg);
  }
  else
  {
    Minisat::vec<Minisat::Lit> MiniSat_assumptions;
    convert(assumptions, MiniSat_assumptions);

    if(solver->solve(MiniSat_assumptions))
    {
      msg="SAT checker: negated claim is SATISFIABLE, i.e., does not hold";
      messaget::status(msg);
      assert(solver->model.size()!=0);
      status=SAT;
      return P_SATISFIABLE;
    }
    else
    {
      msg="SAT checker: negated claim is UNSATISFIABLE, i.e., holds";
      messaget::status(msg);
    }
  }

  status=UNSAT;
  return P_UNSATISFIABLE;
}

// __SZH_ADD_BEGIN__: my specialized version!
template<>
propt::resultt satcheck_minisat2_baset<Minisat::ClosureSolver>::prop_solve()
{
    std::cout << "call our own prop_solve of satcheck_minisat2_baset!\n";

    assert(status!=ERROR);

    {
        std::string msg=
                i2string(_no_variables)+" variables, "+
                i2string(solver->nClauses())+" clauses";
        messaget::status(msg);
        std::cout << msg << "\n";
    }

    add_variables();

    std::string msg;

    solver->init();
    solver->set_graph();

    if(!solver->okay())
    {
        msg="SAT checker inconsistent: negated claim is UNSATISFIABLE, i.e., holds";
        messaget::status(msg);
    }
    else
    {
        std::cout << "start to solve\n";
        Minisat::vec<Minisat::Lit> MiniSat_assumptions;
        convert(assumptions, MiniSat_assumptions);

        if(solver->solve(MiniSat_assumptions))
        {
            msg="SAT checker: negated claim is SATISFIABLE, i.e., does not hold";
            messaget::status(msg);
            std::cout << msg << "\n";
            assert(solver->model.size()!=0);
            status=SAT;
            return P_SATISFIABLE;
        }
        else
        {
            msg="SAT checker: negated claim is UNSATISFIABLE, i.e., holds";
            messaget::status(msg);
        }
    }

    status=UNSAT;
    return P_UNSATISFIABLE;
}

template<>
propt::resultt satcheck_minisat2_baset<Minisat::ICDSolver>::prop_solve()
{
    std::cout << "call our own prop_solve of satcheck_minisat2_baset!\n";

    assert(status!=ERROR);

    {
        std::string msg=
                i2string(_no_variables)+" variables, "+
                i2string(solver->nClauses())+" clauses";
        messaget::status(msg);
        std::cout << msg << "\n";
    }

    add_variables();

    std::string msg;

    solver->init();
    solver->set_graph();

    if(!solver->okay())
    {
        msg="SAT checker inconsistent: negated claim is UNSATISFIABLE, i.e., holds";
        messaget::status(msg);
    }
    else
    {
        std::cout << "start to solve\n";
        Minisat::vec<Minisat::Lit> MiniSat_assumptions;
        convert(assumptions, MiniSat_assumptions);

        if(solver->solve(MiniSat_assumptions))
        {
            msg="SAT checker: negated claim is SATISFIABLE, i.e., does not hold";
            messaget::status(msg);
            std::cout << msg << "\n";
            assert(solver->model.size()!=0);
            status=SAT;
            return P_SATISFIABLE;
        }
        else
        {
            msg="SAT checker: negated claim is UNSATISFIABLE, i.e., holds";
            messaget::status(msg);
        }
    }

    status=UNSAT;
    return P_UNSATISFIABLE;
}
// __SZH_ADD_END__

/*******************************************************************\

Function: satcheck_minisat2_baset::set_assignment

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
void satcheck_minisat2_baset<T>::set_assignment(literalt a, bool value)
{
  assert(!a.is_constant());

  unsigned v=a.var_no();
  bool sign=a.sign();

  // MiniSat2 kills the model in case of UNSAT
  solver->model.growTo(v+1);
  value^=sign;
  solver->model[v]=Minisat::lbool(value);
}

/*******************************************************************\

Function: satcheck_minisat2_baset::satcheck_minisat2_baset

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
satcheck_minisat2_baset<T>::satcheck_minisat2_baset(T *_solver):
  solver(_solver)
{
}

/*******************************************************************\

Function: satcheck_minisat2_baset::~satcheck_minisat2_baset

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<>
satcheck_minisat2_baset<Minisat::Solver>::~satcheck_minisat2_baset()
{
  delete solver;
}

template<>
satcheck_minisat2_baset<Minisat::SimpSolver>::~satcheck_minisat2_baset()
{
  delete solver;
}

template<>
satcheck_minisat2_baset<Minisat::ClosureSolver>::~satcheck_minisat2_baset()
{
    delete solver;
}

template<>
satcheck_minisat2_baset<Minisat::ICDSolver>::~satcheck_minisat2_baset()
{
    delete solver;
}

/*******************************************************************\

Function: satcheck_minisat2_baset::is_in_conflict

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
bool satcheck_minisat2_baset<T>::is_in_conflict(literalt a) const
{
  int v=a.var_no();

  for(int i=0; i<solver->conflict.size(); i++)
    if(var(solver->conflict[i])==v)
      return true;

  return false;
}

/*******************************************************************\

Function: satcheck_minisat2_baset::set_assumptions

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<typename T>
void satcheck_minisat2_baset<T>::set_assumptions(const bvt &bv)
{
  assumptions=bv;

  forall_literals(it, assumptions)
    assert(!it->is_constant());
}

/*******************************************************************\

Function: satcheck_minisat_no_simplifiert::satcheck_minisat_no_simplifiert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisat_no_simplifiert::satcheck_minisat_no_simplifiert():
  satcheck_minisat2_baset<Minisat::Solver>(new Minisat::Solver)
{
}

/*******************************************************************\

Function: satcheck_minisat_simplifiert::satcheck_minisat_simplifiert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisat_simplifiert::satcheck_minisat_simplifiert():
  satcheck_minisat2_baset<Minisat::SimpSolver>(new Minisat::SimpSolver)
{
}

// __SZH_ADD_BEGIN__
satcheck_minisat_closuret::satcheck_minisat_closuret():
  satcheck_minisat2_baset<Minisat::ClosureSolver>(new Minisat::ClosureSolver)
{
}

satcheck_minisat_ICDt::satcheck_minisat_ICDt():
  satcheck_minisat2_baset<Minisat::ICDSolver>(new Minisat::ICDSolver)
{
}
// __SZH_ADD_END__

/*******************************************************************\

Function: satcheck_minisat_simplifiert::set_frozen

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void satcheck_minisat_simplifiert::set_frozen(literalt a)
{
  assert(!a.is_constant());
  add_variables();
  solver->setFrozen(a.var_no(), true);
}

/*******************************************************************\

Function: satcheck_minisat_simplifiert::is_eliminated

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool satcheck_minisat_simplifiert::is_eliminated(literalt a) const
{
  assert(!a.is_constant());

  return solver->isEliminated(a.var_no());
}

