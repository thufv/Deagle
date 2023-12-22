/*******************************************************************\

Module: Memory models for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

/// \file
/// Memory models for partial order concurrency

#ifndef CPROVER_GOTO_SYMEX_MEMORY_MODEL_H
#define CPROVER_GOTO_SYMEX_MEMORY_MODEL_H

#include "partial_order_concurrency.h"

class memory_model_baset : public partial_order_concurrencyt
{
public:
  explicit memory_model_baset(const namespacet &_ns);
  virtual ~memory_model_baset();

  virtual void operator()(symex_target_equationt &, message_handlert &) = 0;

  // __SZH_ADD_BEGIN__
  bool use_deagle;
  void build_guard_map_write(symex_target_equationt &equation);
  void build_guard_map_all(symex_target_equationt &equation);
  bool is_apo(event_it e1, event_it e2);
  bool is_apo(std::set<std::pair<std::string, std::string>>& array_update_set, event_it e1, event_it e2); // this apo considers whether e1 and e2 belong to the same array update operation
  std::string get_name(event_it e);
  void add_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr);
  void add_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr);

  void add_oc_label(symex_target_equationt &equation, event_it e, std::string label);
  std::string fill_name(event_it& event);

  bool enable_datarace;
  void data_race(symex_target_equationt &equation);
  exprt array_find_index(symex_target_equationt &equation, event_it event);
  // __SZH_ADD_END__

protected:
  /// In-thread program order
  /// \param e1: preceding event
  /// \param e2: following event
  /// \return true if e1 precedes e2 in program order
  bool po(event_it e1, event_it e2);

  // produce fresh symbols
  unsigned var_cnt;
  symbol_exprt nondet_bool_symbol(const std::string &prefix);

  // This gives us the choice symbol for an R-W pair;
  // built by the method below.
  typedef std::map<std::pair<event_it, event_it>, symbol_exprt> choice_symbolst;
  choice_symbolst choice_symbols;

  /// For each read `r` from every address we collect the choice symbols `S`
  ///   via \ref register_read_from_choice_symbol (for potential read-write
  ///   pairs) and add a constraint r.guard => \/S.
  /// \param equation: symex equation where the new constraint should be added
  void read_from(symex_target_equationt &equation);

  /// Introduce a new choice symbol `s` for the pair (\p r, \p w)
  /// add constraint s => (w.guard /\ r.lhs=w.lhs)
  /// add constraint s => before(w,r) [if \p w is from another thread]
  /// \param r: read event
  /// \param w: write event
  /// \param equation: symex equation where the new constraints should be added
  /// \return the new choice symbol
  symbol_exprt register_read_from_choice_symbol(
    const event_it &r,
    const event_it &w,
    symex_target_equationt &equation);

  // maps thread numbers to an event list
  typedef std::map<unsigned, event_listt> per_thread_mapt;
  typedef std::map<std::string, event_listt> per_loc_mapt;

  void build_per_thread_map(const symex_target_equationt &equation, per_thread_mapt &dest) const;
  void build_per_loc_map(const symex_target_equationt &equation, per_loc_mapt &dest) const;
};

#endif // CPROVER_GOTO_SYMEX_MEMORY_MODEL_H
