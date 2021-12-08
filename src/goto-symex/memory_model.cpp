/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#include <util/std_expr.h>
#include <util/i2string.h>
#include <util/prefix.h>

#include "../goto-symex/symex_target_equation.h"

#include "memory_model.h"
#include <iostream>
#include <string>
#include <map>

/*******************************************************************\

Function: memory_model_baset::memory_model_baset

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

memory_model_baset::memory_model_baset(const namespacet &_ns):
		partial_order_concurrencyt(_ns),
		var_cnt(0)
{
}

/*******************************************************************\

Function: memory_model_baset::~memory_model_baset

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

memory_model_baset::~memory_model_baset()
{
}

/*******************************************************************\

Function: memory_model_baset::nondet_bool_symbol

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

symbol_exprt memory_model_baset::nondet_bool_symbol(
		const std::string &prefix)
{
//  return symbol_exprt(
//    "memory_model::choice_"+prefix+i2string(var_cnt++),
//    bool_typet());
	//// __FHY_ADD_BEGIN__
	return symbol_exprt(
			prefix+i2string(var_cnt++),
			bool_typet());
	//// __FHY_ADD_END__
}

/*******************************************************************\

Function: memory_model_baset::po

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

bool memory_model_baset::po(event_it e1, event_it e2)
{
	// within same thread
	if(e1->source.thread_nr==e2->source.thread_nr)
		return numbering[e1]<numbering[e2];
	else
	{
		// in general un-ordered, with exception of thread-spawning
		return false;
	}
}

bool memory_model_baset::valid_mutex(symex_target_equationt &equation)
{
	int mutex_num = 0;
	for(eventst::const_iterator
				e_it=equation.SSA_steps.begin();
		e_it!=equation.SSA_steps.end();
		e_it++)
	{
		// concurreny-related?
		if(e_it->is_verify_atomic_begin(equation.mutex_flag))
			mutex_num++;
	}
	return (mutex_num != 1);
}

void memory_model_baset::read_from(symex_target_equationt &equation)
{
	per_thread_mapt per_thread_map;
	for(eventst::const_iterator e_it=equation.SSA_steps.begin();
		e_it!=equation.SSA_steps.end(); e_it++)
	{
		if((is_shared_read(e_it) ||
			is_shared_write(e_it) ||
			e_it->is_verify_atomic_begin(equation.mutex_flag) ||
			e_it->is_verify_atomic_end(equation.mutex_flag) ||
			e_it->is_thread_create() ||
			e_it->is_thread_join()))
		{
			per_thread_map[e_it->source.thread_nr].push_back(e_it);
		}
	}
	int thread_num = per_thread_map.size();
	if (thread_num > 10)
		equation.symmetry_flag = true;
	
	// iterate over threads
	for(per_thread_mapt::const_iterator
				t_it=per_thread_map.begin();
		t_it!=per_thread_map.end();
		t_it++)
	{
		const event_listt &events=t_it->second;
		std::map<irep_idt, event_it> event_value_map;
		bool atomic_flag = false;
		
		int curr_threads = (((*(events.begin()))->source.thread_nr == 0) ? 1 : 100);
		
		for(event_listt::const_iterator e_it=events.begin();
			e_it!=events.end(); e_it++)
		{
			event_it e = *e_it;

            if (e->is_thread_create() && e->source.thread_nr == 0) {
				curr_threads++;
				event_value_map.clear();
			}
			
			if (e->is_thread_join() && e->source.thread_nr == 0) {
				curr_threads--;
				event_value_map.clear();
			}
			
			if (e->is_verify_atomic_begin(equation.mutex_flag) && valid_mutex(equation)) {
				atomic_flag = true;
				event_value_map.clear();
			}
			else if (e->is_verify_atomic_end(equation.mutex_flag)) {
				atomic_flag = false;
				event_value_map.clear();
			}
			else if (atomic_flag) {
				if (is_shared_read(e)) {
					if (e->rely)
					{
						if (event_value_map.find(address(e)) == event_value_map.end()) {
							read_from_item(e, equation, thread_num);
							event_value_map[address(e)] = e;
						}
						else {
//                            std::cout << "event: " << id2string(id(e)) << "   " << e->is_shared_read() << e->is_shared_write() <<"\n";
//                            std::cout << e->atomic_section_id << "\n";
                            add_constraint(equation, implies_exprt(e->guard, equal_exprt(e->ssa_lhs, event_value_map[address(e)]->ssa_lhs)), "rfi", e->source);
//                            add_constraint(equation, equal_exprt(e->ssa_lhs, event_value_map[address(e)]->ssa_lhs), "rfi", e->source);

                        }
					}
				}
				else if (is_shared_write(e)) {
					event_value_map[address(e)] = e;
				}
			}
			else {
				assert(!atomic_flag);
				if (is_shared_read(e) && e->rely) {
					read_from_item(e, equation, thread_num);
				}
			}
		}
	}
}

void memory_model_baset::read_from_item(const event_it& r, symex_target_equationt &equation, int thread_num) {
	const a_rect &a_rec=address_map[address(r)];
	event_listt rfwrites;
	
	exprt::operandst rf_some_operands;
	rf_some_operands.reserve(a_rec.writes.size());
	
	// this is quadratic in #events per address
	for(event_listt::const_iterator w_it=a_rec.writes.begin(); w_it!=a_rec.writes.end(); ++w_it)
	{
		const event_it w=*w_it;
		bool is_rfi = (r->source.thread_nr==w->source.thread_nr);
		if(po(r, w))
			continue; // contradicts po
		if (is_rfi && !(equation.aux_enable && w->is_aux_var()))
		{
			rfwrites.push_back(w);
		}
		else
		{
			if (equation.symmetry_flag &&
				equation.same_threads_id[r->source.thread_nr] == equation.same_threads_id[w->source.thread_nr] &&
				r->source.thread_nr < w->source.thread_nr)
			{
				continue;
			}
			
			//// __FHY_ADD_BEGIN__
			symbol_exprt s = symbol_exprt(nondet_bool_symbol("rf"));
			std::string rf_lit = s.get_identifier().c_str();
			//// __FHY_ADD_END__

			// szh
//			std::set<std::string> true_set{};
//			if(true_set.find(rf_lit) != true_set.end())
//            {
//			    std::cout << rf_lit << " is marked as true\n";
//			    add_constraint(equation, s, "mark", r->source);
//            }
			// szh

			// record the symbol
			choice_symbols[std::make_pair(r, w)] = s;
			
			// We rely on the fact that there is at least
			// one write event that has guard 'true'.
			exprt rw = equal_exprt(r->ssa_lhs, w->ssa_lhs);
//			if (equation.order_flag)
//				rw = and_exprt(rw, before(w, r));
//			or_exprt read_from(not_exprt(s), and_exprt(rw, w->guard));

#if origin_frontend_set_partial_fr
            implies_exprt read_from(s, and_exprt(rw, w->guard));
#else
            implies_exprt read_from(s, and_exprt(rw, w->guard, r->guard));
#endif
            // add the rf relation to the amp
			equation.choice_symbol_map[s] = new symex_target_equationt::eq_edge(&(*w), &(*r));
			
			// Uses only the write's guard as precondition, read's guard
			// follows from rf_some
			add_constraint(equation, read_from, "rf", r->source);
			
			//// __FHY_ADD_BEGIN__
			if(solve_method == solve_method_smt2)
			    add_constraint(equation, implies_exprt(s, before(w, r)), "rf-order", r->source);
			std::string e1_str = id2string(id(w));
			std::string e2_str = id2string(id(r));
			if(is_rfi)
			{
				equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(rf_lit, "rfi")));
				equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "rfi", rf_lit, s));
			}
			else
			{
				equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(rf_lit, "rfe")));
				equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "rfe", rf_lit, s));
			}
			//// __FHY_ADD_END__
			
			rf_some_operands.push_back(s);
		}
	}
	
	event_listt::const_iterator w_it, wt_it;
	for(w_it=rfwrites.begin(); w_it!=rfwrites.end(); ++w_it)
	{
		{
			const event_it w=*w_it;
			
			//// __FHY_ADD_BEGIN__
			symbol_exprt s = symbol_exprt(nondet_bool_symbol("rf"));
			std::string rf_lit = s.get_identifier().c_str();
			//// __FHY_ADD_END__

            // szh
//            std::set<std::string> true_set{};
//            if(true_set.find(rf_lit) != true_set.end())
//            {
//                std::cout << rf_lit << " is marked as true\n";
//                add_constraint(equation, s, "mark", r->source);
//            }
            // szh

			// record the symbol
			choice_symbols[std::make_pair(r, w)] = s;
			
			// We rely on the fact that there is at least
			// one write event that has guard 'true'.
			exprt rw = equal_exprt(r->ssa_lhs, w->ssa_lhs);
//			if (equation.order_flag)
//				rw = and_exprt(rw, before(w, r));
#if origin_frontend_set_partial_fr
            implies_exprt read_from(s, and_exprt(rw, w->guard));
#else
            implies_exprt read_from(s, and_exprt(rw, r->guard, w->guard));
#endif

            // add the rf relation to the amp
			equation.choice_symbol_map[s] = new symex_target_equationt::eq_edge(&(*w), &(*r));
			
			// Uses only the write's guard as precondition, read's guard
			// follows from rf_some
			add_constraint(equation, read_from, "rfi", r->source);
			
			//// __FHY_ADD_BEGIN__
			if(solve_method == solve_method_smt2)
			    add_constraint(equation, implies_exprt(s, before(w, r)), "rf-order", r->source);
			
			std::string e1_str = id2string(id(w));
			std::string e2_str = id2string(id(r));
			equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(rf_lit, "rfi")));
			equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "rfi", rf_lit, s));
			//// __FHY_ADD_END__
			
			rf_some_operands.push_back(s);
		}
	}
	
	// value equals the one of some write
	exprt rf_some;
	
	// uninitialised global symbol like symex_dynamic::dynamic_object*
	// or *$object
	if(rf_some_operands.empty())
		return;
	else if(rf_some_operands.size()==1)
		rf_some=rf_some_operands.front();
	else
	{
		rf_some=or_exprt();
		rf_some.operands().swap(rf_some_operands);
	}
	
	// Add the read's guard, each of the writes' guards is implied
	// by each entry in rf_some
	add_constraint(equation, implies_exprt(r->guard, rf_some), "rf-some", r->source);

}
