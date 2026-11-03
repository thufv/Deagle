/*******************************************************************\

Module: Memory models for C11 memory model

(would be later extended to all memory models)

Author: Zhihang Sun, sunzh20@mails.tsinghua.edu.cn

\*******************************************************************/

#ifndef CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H
#define CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H

#include "memory_model.h"

class memory_model_generalt:public memory_model_baset
{
public:
  explicit memory_model_generalt(const namespacet &_ns, cat_modulet& _cat, bool _enable_cutting, std::string _compile_target) : 
    memory_model_baset(_ns), cat(_cat), enable_cutting(_enable_cutting), compile_target(_compile_target) {}

  virtual void operator()(symex_target_equationt &equation, message_handlert &);

  exprt get_guard(event_it e) { return simplify_expr(e->guard, ns); }
  exprt get_two_guards(event_it e1, event_it e2) { return simplify_expr(and_exprt(e1->guard, e2->guard), ns); }
  exprt get_guard(std::string e_str)
  {
    return id_to_guard.find(e_str) != id_to_guard.end() ? simplify_expr(id_to_guard[e_str], ns) : true_exprt();
  }
  exprt get_two_guards(std::string e1_str, std::string e2_str)
  {
    exprt guard1 = id_to_guard.find(e1_str) != id_to_guard.end() ? id_to_guard[e1_str] : true_exprt();
    exprt guard2 = id_to_guard.find(e2_str) != id_to_guard.end() ? id_to_guard[e2_str] : true_exprt();
    return simplify_expr(and_exprt(guard1, guard2), ns);
  }

  void add_necessary_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr);
  void add_necessary_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr);
  void add_necessary_oc_label(symex_target_equationt &equation, event_it e, std::string label, exprt guard_expr);
  void add_necessary_oc_label(symex_target_equationt &equation, std::string e_str, std::string label, exprt guard_expr);
  void add_apo(symex_target_equationt &equation, event_it e1, event_it e2, std::map<event_it, event_it>& rmw_counterparts);

  void build_labels(symex_target_equationt& equation, std::map<event_it, std::string>& event_mo);
  void build_lkmm_nesting(symex_target_equationt &equation);
  void build_loc(symex_target_equationt &equation);
  void build_thd(symex_target_equationt &equation);
  void build_id(symex_target_equationt &equation);

  bool is_mm_apo(event_it e1, event_it e2);

  void build_po(symex_target_equationt &equation, std::map<event_it, event_it>& rmw_counterparts);
  void build_rf(symex_target_equationt &equation);
  void build_co(symex_target_equationt &equation);

  void build_shared_reads(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_guard_map(symex_target_equationt& equation, std::map<symbol_exprt, exprt>& guard_map);
  void build_data(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_addr(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_ctrl(symex_target_equationt &equation, std::set<std::string>& reads);

  std::pair<std::string, std::string> get_compiled_fence(std::string compile_target, event_it e_it, std::map<event_it, std::string>& event_mo);
  std::pair<std::string, std::string> get_compiled_fence_rmw(std::string compile_target, event_it e_it, std::map<event_it, std::string>& event_mo, event_it rmw_counterpart);
  std::string relabel_fence(std::string compile_target, event_it e_it, std::map<event_it, std::string>& event_mo);
  void build_compiled_fences(symex_target_equationt &equation, std::map<event_it, std::string>& event_mo, std::map<event_it, event_it>& rmw_counterparts);

  std::vector<event_it> lkmm_locks;
  std::vector<event_it> lkmm_unlocks;
  void build_lkmm_crit(symex_target_equationt &equation);

  // for cutting

  bool really_need_cutting();
  void build_cutting(symex_target_equationt &equation);

  void remove_atomic_typecast(symex_target_equationt &equation);
  
protected:
  // Useless, but must override
  virtual exprt before(event_it e1, event_it e2) { return partial_order_concurrencyt::before(e1, e2, AX_PROPAGATION); }

private:
  cat_modulet cat;
  bool enable_cutting;
  std::string compile_target;
};
#endif // CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H