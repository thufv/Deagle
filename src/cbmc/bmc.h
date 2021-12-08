/*******************************************************************\

Module: Bounded Model Checking for ANSI-C + HDL

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_CBMC_BMC_H
#define CPROVER_CBMC_BMC_H

#include <list>
#include <map>
#include <memory>

#include <util/hash_cont.h>
#include <util/options.h>

#include <solvers/prop/prop.h>
#include <solvers/prop/prop_conv.h>
#include <solvers/sat/cnf.h>
#include <solvers/sat/satcheck.h>
#include <solvers/smt1/smt1_dec.h>
#include <solvers/smt2/smt2_dec.h>
#include <langapi/language_ui.h>
#include <goto-symex/symex_target_equation.h>
#include <goto-symex/memory_model_sc.h>

#include "symex_bmc.h"
#include "bv_cbmc.h"
#include "eog.h"

struct EPO {
	event_it begin;
	event_it end;
	std::vector<event_it> atomic_events;
};

struct shared_write_order {
	event_it event;
	int line;
	int clock;
};


class bmct:public messaget
{
	typedef std::vector<event_it> event_listt;
	typedef std::map<unsigned, event_listt> per_thread_mapt;
	typedef symex_target_equationt::choice_symbol_mapt cs_mapt;

	struct a_rect {
	event_listt reads, writes;
	};

	typedef std::map<irep_idt, a_rect> address_mapt;
	typedef std::map<irep_idt, nodevt> address_r_w_mapt;

public:
	bmct(
	  optionst &_options,
	  symbol_tablet &_symbol_table,
	  message_handlert &_message_handler):
	  messaget(_message_handler),
	  options(_options),
	  ns(_symbol_table, new_symbol_table),
	  equation(ns),
	  symex(ns, new_symbol_table, equation),
	  gf_ptr(0),
	  ui(ui_message_handlert::PLAIN),
	  memory_model(0)
	{
	  symex.constant_propagation=options.get_bool_option("propagation");
	}

	virtual bool run(goto_functionst& goto_functions);

	virtual ~bmct() {
	}

	expr_listt bmc_constraints;
	friend class cbmc_satt;
	friend class hw_cbmc_satt;
	friend class counterexample_beautification_greedyt;

	void set_ui(language_uit::uit _ui) {
		ui = _ui;
	}

protected:
	optionst& options;
	symbol_tablet new_symbol_table;
	namespacet ns;
	symex_target_equationt equation;
	symex_bmct symex;
	const goto_functionst* gf_ptr;
	std::auto_ptr<memory_model_baset> memory_model;
	language_uit::uit ui;
	virtual decision_proceduret::resultt run_decision_procedure(
			prop_convt& prop_conv);
	virtual bool decide(prop_convt& prop_conv);
	virtual bool decide_bv_refinement();
	virtual bool decide_aig();
	virtual bool decide_smt1();
	virtual bool decide_smt2();
	smt1_dect::solvert get_smt1_solver_type() const;
	smt2_dect::solvert get_smt2_solver_type() const;
	virtual void smt1_convert(smt1_dect::solvert solver, std::ostream& out);
	virtual void smt2_convert(smt2_dect::solvert solver, std::ostream& out);
	virtual bool write_dimacs();
	virtual bool write_dimacs(std::ostream& out);
	virtual void setup_unwind();
	virtual void do_unwind_module(decision_proceduret& decision_procedure);
	void do_conversion(prop_convt& solver);
	prop_convt* solver_factory();
	virtual void show_vcc();
	virtual bool all_properties(const goto_functionst& goto_functions,
			prop_convt& solver);
	virtual void show_vcc(std::ostream& out);
	virtual void show_program();
	virtual void report_success();
	virtual void report_failure();
	virtual void error_trace(const prop_convt& prop_conv);
	void cover_assertions(const goto_functionst& goto_functions,
			prop_convt& solver);

  // Functions for dynamically set the loop bound
  void set_unwind_limit(const goto_functionst& goto_functions, value_sett& value_set);

  // Functions for reduce threads number
  void reduce_threads_num(std::auto_ptr<memory_model_baset>& memory_model, std::vector<int>& valid_threads);
  bool identify_same_threads(goto_functionst &goto_functions, std::vector<int>& valid_threads);
  bool is_same_thread(goto_functionst &goto_functions, int id1, int id2);
  bool has_pthread_join();

  // Functions for reduce loop bound
  void reduce_loop_bound(const goto_functionst &goto_functions, std::auto_ptr<memory_model_baset>& memory_model);
  void compute_reduced_unwind();

  // Functions for writting witness.graphml
  void write_true_witness();
  virtual void report_success_witness();
  void write_false_witness(const prop_convt &prop_conv, std::vector<irep_idt>& write_order);
  virtual void report_failure_witness(const prop_convt &prop_conv, std::vector<irep_idt>& write_order);

  // functions for initial refinement computation
  void compute_init_refinement(prop_convt &prop_conv, exprt& constraint);
  void single_rf_refinement_generation(DEOG& graph, exprt::operandst& constraint_operands);
  void two_rf_refinement_generation(DEOG& graph, exprt::operandst& constraint_operands);
  void redundant_reason_remove( std::vector<exprt::operandst>& reasons,
  		std::vector < exprt::operandst >& effective_reasons);
  const eventt* compute_mid_write(address_mapt& address_map, DEOG& graph, const eventt* dst1, const eventt* dst2);
  void compute_before_write(std::map<const eventt*, eventt*>& before_write_map);
  void compute_EPO_write(DEOG& graph, std::map<const eventt*, eventt*>& EPO_write_map);
  void compute_address_map(address_mapt& address_map);

  // Functions for EOG-based CEGAR
  virtual int decide_default(bool threads_reduced = false);
  void EOG_extraction(prop_convt &prop_conv, EOG& graph, bool trace_flag = true);
  bool CE_evaluation_graph(EOG& eog, DEOG& edog);
  bool CE_evaluation_constraint(EOG& eog, std::vector<symbol_exprt>& unsat_core, std::vector<irep_idt>& write_order);
  void refinement_generation_graph(DEOG& graph, exprt& constraint);
  void refinement_generation_constraint(std::vector<symbol_exprt>& unsat_core, exprt& refinement);
  void true_CE_generation(bv_cbmct &prop_conv, std::vector<irep_idt>& write_order);
  virtual decision_proceduret::resultt incremental_solve(prop_convt& prop_conv, exprt& constraint);

  // __SZH_ADD_BEGIN__
  // Functions for closure and ICD
  bool decide_closure();
  bool decide_ICD();
  // __SZH_ADD_END__

  // Functions for EOG_extraction
  void nodes_extraction(EOG& eog, prop_convt &prop_conv, bool trace_flag = true);
  void program_order_extraction(EOG& eog, prop_convt &prop_conv, bool trace_flag = true);
  void read_from_extraction(EOG& eog, prop_convt &prop_conv);
  void thread_spawn(symex_target_equationt &equation, const per_thread_mapt &per_thread_map,
		  prop_convt &prop_conv,EOG& eog, bool trace_flag = true);

  // Functions for CE_evaluation_graph
  void add_all_nodes(DEOG& graph);
  void deog_generation(DEOG& deog, EOG& eog, bool with_rf = true);
  bool valid_mutex(symex_target_equationt &equation);

  // Functions for CE_evaluation_constraint
  void encode_program_order(EOG& eog, exprt& constraint, std::map<event_it, EPO>& epo_map);
  void encode_read_from(EOG& eog, prop_convt &solver, bvt& assumptions,
  		std::map<literalt, symbol_exprt>& lit_choice_map,
  		std::map<event_it, EPO>& epo_map);
  exprt before(event_it e1, event_it e2, std::map<event_it, EPO>& epo_map);
};

#endif
