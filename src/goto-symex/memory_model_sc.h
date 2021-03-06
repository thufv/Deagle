/*******************************************************************\

Module: Memory models for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#ifndef CPROVER_MEMORY_MODEL_SC_H
#define CPROVER_MEMORY_MODEL_SC_H

#include "memory_model.h"

class memory_model_sct:public memory_model_baset
{
public:
  inline explicit memory_model_sct(const namespacet &_ns):
    memory_model_baset(_ns)
  {
  }

  virtual void operator()(symex_target_equationt &equation);
  
protected:
  virtual exprt before(event_it e1, event_it e2);
  virtual symbol_exprt clock(event_it e1);
  virtual bool program_order_is_relaxed(
    partial_order_concurrencyt::event_it e1,
    partial_order_concurrencyt::event_it e2) const;

  void build_per_thread_map(
    const symex_target_equationt &equation,
    per_thread_mapt &dest) const;
  void thread_spawn(
    symex_target_equationt &equation,
    const per_thread_mapt &per_thread_map);

  std::string fill_spawn(event_it& event);

  void program_order(symex_target_equationt &equation);
  void from_read(symex_target_equationt &equation);
  void write_serialization_external(symex_target_equationt &equation);

  // added by szh
  void abstract_write_guard(symex_target_equationt &equation);

  // added by ylz08
  void get_symbols(const exprt &expr, std::vector<symbol_exprt>& symbols);
  void set_events_ssa_id(symex_target_equationt &equation);
  unsigned set_single_event_ssa_id(symex_target_equationt &equation, symbol_exprt event, int id);
};

#endif

