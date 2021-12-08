/*******************************************************************\

Module: Memory models for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#ifndef CPROVER_MEMORY_MODEL_H
#define CPROVER_MEMORY_MODEL_H

#include "partial_order_concurrency.h"
#include "../cbmc/eog.h"
//// __FHY_ADD_BEGIN__
/*
 * Encoding Method:
 * Origin:
 *      po -> clk(e1) < clk(e2)
 *
 *      rfi/rfe -> value(w) = value(r) & w.guard &
 *      rfi/rfe -> clk(w) < clk(r)
 *      r.guard -> || rf(i), where read event of rf(i) is r

 *      coe -> clk(w1) < clk(w2)
 *      !coe -> clk(w2) < clk(w1)
 *
 *      rf & coe & w2.guard & r.guard -> fr
 * Now:
 *      po -> clk(e1) < clk(e2)
 *
 *      rfi/rfe -> value(w) = value(r) & w.guard & r.guard
 *      rfi/rfe -> clk(w) < clk(r)
 *      r.guard -> || rf(i), where read event of rf(i) is r
 *
 *      coi -> w1.guard & w2.guard
 *      coi -> clk(w1) < clk(w2)
 *      w1.guard & w2.guard -> coi
 *
 *      coe1 -> w1.guard & w2.guard
 *      coe1 -> clk(w1) < clk(w2)
 *      coe2 -> w2.guard & w1.guard
 *      coe2 -> clk(w2) < clk(w1)
 *      w1.guard & w2.guard -> coe1 || coe2
 *
 */
#define origin_frontend_set_partial_fr 0
#define front_deduce_fr_by_coi 0
#define front_deduce_all_fr 0

//// __FHY_ADD_BEGIN_210409__
#define TRADITIONAL 0
#define PARTIAL_PPO 0
#define ALL_PPO 1
/*
 * PARTIAL_PPO:
 *      + just delete some po in SC and leave the derivation of ppo to backend
 * ALL_PPO:
 *      + generate all the ppo order in front-end.
 */
//// __FHY_ADD_BEGIN_210409__
//// __FHY_ADD_END__

class memory_model_baset:public partial_order_concurrencyt
{
public:
	// __SZH_ADD_BEGIN__
	enum solve_methodt {solve_method_sat, solve_method_smt2, solve_method_sat_closure, solve_method_sat_icd};
	solve_methodt solve_method;
	// __SZH_ADD_END__

	explicit memory_model_baset(const namespacet &_ns);
	virtual ~memory_model_baset();
	
	virtual void operator()(symex_target_equationt &)=0;

protected:
	// program order
	bool po(event_it e1, event_it e2);
	
	// produce fresh symbols
	unsigned var_cnt;
	symbol_exprt nondet_bool_symbol(const std::string &prefix);
	
	// This gives us the choice symbol for an R-W pair;
	// built by the method below.
	typedef std::map<
			std::pair<event_it, event_it>, exprt> choice_symbolst;
	choice_symbolst choice_symbols;
	
	//// __FHY_ADD_BEGIN__
	choice_symbolst wse_symbols;
	//// __FHY_ADD_END__
	
	// maps thread numbers to an event list
	typedef std::map<unsigned, event_listt> per_thread_mapt;
	
	bool valid_mutex(symex_target_equationt &equation);
	
	void read_from(symex_target_equationt &equation);
	
	void read_from_item(const event_it& r, symex_target_equationt &equation, int thread_num);
};

#endif

