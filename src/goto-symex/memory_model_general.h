/*******************************************************************\

Module: Memory models for C11 memory model

(would be later extended to all memory models)

Author: Zhihang Sun, sunzh20@mails.tsinghua.edu.cn

\*******************************************************************/

#ifndef CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H
#define CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H

#include "memory_model.h"

class must_may_calculatort
{
  cat_modulet cat;

public:
  typedef std::set<std::string> unary_sett;
  typedef std::set<std::pair<std::string, std::string>> binary_sett;

  std::map<std::string, unary_sett> unary_musts;
  std::map<std::string, unary_sett> unary_mays;
  std::map<std::string, binary_sett> binary_musts;
  std::map<std::string, binary_sett> binary_mays;

  typedef std::vector<int> int_unary_sett;
  typedef std::vector<std::pair<int, int>> int_binary_sett;

  std::map<std::string, int_unary_sett> int_unary_musts;
  std::map<std::string, int_unary_sett> int_unary_mays;
  std::map<std::string, int_binary_sett> int_binary_musts;
  std::map<std::string, int_binary_sett> int_binary_mays;

  must_may_calculatort() {}
  must_may_calculatort(cat_modulet& _cat, std::vector<oc_edget> oc_edges, std::vector<oc_labelt>& oc_labels) : 
    cat(_cat)
  {
    for(auto& edge : oc_edges)
      add_binary_element(edge.kind, edge.e1_str, edge.e2_str, edge.expr);
    for(auto& label : oc_labels)
      add_unary_element(label.label, label.e_str, label.expr);
  }

  std::vector<std::string> id2str;
  std::map<std::string, int> str2id;

  int get_id(std::string e)
  {
    if(str2id.find(e) != str2id.end())
      return str2id[e];

    int id = id2str.size();
    id2str.push_back(e);
    str2id[e] = id;
    return id;
  }

  void add_unary_element(std::string kind, std::string e, exprt guard)
  {
    int id = get_id(e);
    int_unary_mays[kind].push_back(id);
    if(guard.is_true())
      int_unary_musts[kind].push_back(id);
  }

  void add_binary_element(std::string kind, std::string e1, std::string e2, exprt guard)
  {
    int id1 = get_id(e1);
    int id2 = get_id(e2);
    auto pair = std::make_pair(id1, id2);
    int_binary_mays[kind].push_back(pair);
    if(guard.is_true())
      int_binary_musts[kind].push_back(pair);
  }

  void calculate();
  std::vector<std::vector<std::string>> sccs;
  std::vector<int> sorted_scc_ids;
  std::map<std::string, int> kind_to_scc_id;
  void tarjan(std::string kind, std::stack<std::string>& stack, std::map<std::string, bool>& in_stack, std::map<std::string, int>& dfn, std::map<std::string, int>& low, int& count);
  void build_scc();
  void build_topological();
  void back_to_str();

  template<typename T> void calc_union(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result);
  template<typename T> void calc_intersection(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result);
  template<typename T> void calc_difference(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result);
  void calc_composition(const int_binary_sett& left, const int_binary_sett& right, int_binary_sett& result);
  void calc_product(const int_unary_sett& left, const int_unary_sett& right, int_binary_sett& result);
  void calc_bracket(const int_unary_sett& source, int_binary_sett& result);
  void calc_flip(const int_binary_sett& source, int_binary_sett& result);
  void calc_transitive_closure(const int_binary_sett& source, int_binary_sett& result);
  void calculate_single_kind(std::string kind, int arity);
  void calculate_sets();
};

class memory_model_generalt:public memory_model_baset
{
public:
  explicit memory_model_generalt(const namespacet &_ns, cat_modulet& _cat, bool _strict_guard) : 
    memory_model_baset(_ns), cat(_cat), strict_guard(_strict_guard) {}

  virtual void operator()(symex_target_equationt &equation, message_handlert &);

  void add_necessary_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr);
  void add_necessary_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr);
  void add_necessary_oc_label(symex_target_equationt &equation, event_it e, std::string label);
  void add_apo(symex_target_equationt &equation, event_it e1, event_it e2, std::set<event_it>& added_rmw_labels);

  void build_free(symex_target_equationt &equation);

  void build_labels(symex_target_equationt &equation);
  void build_lkmm_nesting(symex_target_equationt &equation);
  void build_loc(symex_target_equationt &equation);
  void build_thd(symex_target_equationt &equation);
  void build_id(symex_target_equationt &equation);

  bool is_mm_apo(event_it e1, event_it e2);

  void build_po(symex_target_equationt &equation);
  void build_rf(symex_target_equationt &equation);
  void build_co(symex_target_equationt &equation);

  void build_shared_reads(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_guard_map(symex_target_equationt& equation, std::map<symbol_exprt, exprt>& guard_map);
  void build_data(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_addr(symex_target_equationt &equation, std::set<std::string>& reads);
  void build_ctrl(symex_target_equationt &equation, std::set<std::string>& reads);

  std::vector<event_it> lkmm_locks;
  std::vector<event_it> lkmm_unlocks;
  void build_lkmm_crit(symex_target_equationt &equation);

  void remove_atomic_typecast(symex_target_equationt &equation);
  
protected:
  virtual exprt before(event_it e1, event_it e2);

private:
  cat_modulet cat;
  must_may_calculatort must_may_calculator;
  bool strict_guard;
};
#endif // CPROVER_GOTO_SYMEX_MEMORY_MODEL_C11_H