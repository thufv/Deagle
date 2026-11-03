/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

/// \file
/// Memory model for partial order concurrency

#include "memory_model.h"
#include <iostream>

#include <util/std_expr.h>

memory_model_baset::memory_model_baset(const namespacet &_ns)
  : partial_order_concurrencyt(_ns),
    use_deagle(false),
    use_zord(false),
    enable_datarace(false),
    enable_deadlock(false),
    simplified_rf_val_count(0),
    total_rf_val_count(0),
    var_cnt(0)
{
}

memory_model_baset::~memory_model_baset()
{
}

symbol_exprt memory_model_baset::nondet_bool_symbol(const std::string &prefix)
{
  return symbol_exprt(
    "memory_model::choice_" + prefix + std::to_string(var_cnt++), bool_typet());
}

bool memory_model_baset::po(event_it e1, event_it e2)
{
  // within same thread
  if(e1->source.thread_nr == e2->source.thread_nr)
    return numbering[e1] < numbering[e2];
  else
  {
    // in general un-ordered, with exception of thread-spawning
    return false;
  }
}

// __SZH_ADD_BEGIN__
exprt memory_model_baset::zord_pred(irep_idt pred, event_it e1, event_it e2, symbol_exprt reason)
{
  assert(pred == ID_pred_rf || pred == ID_pred_ws || pred == ID_pred_race);

  symbol_exprt e1_clock(get_name_with_clk(e1), typet(ID_event));
  symbol_exprt e2_clock(get_name_with_clk(e2), typet(ID_event));
  return ternary_exprt(pred, e1_clock, e2_clock, reason, bool_typet());
}

exprt memory_model_baset::zord_pred(irep_idt pred, event_it e1, event_it e2)
{
  assert(pred == ID_pred_po || pred == ID_pred_atom);

  symbol_exprt e1_clock(get_name_with_clk(e1), typet(ID_event));
  symbol_exprt e2_clock(get_name_with_clk(e2), typet(ID_event));
  return binary_exprt(e1_clock, pred, e2_clock, bool_typet());
}
// __SZH_ADD_END__

void memory_model_baset::read_from(symex_target_equationt &equation)
{
  // We iterate over all the reads, and
  // make them match at least one
  // (internal or external) write.

  for(const auto &address : address_map)
  {
    std::map<event_it, exprt::operandst> lock_w2unlock_rf;
    std::map<event_it, exprt::operandst> lock_w2lock_rf;
    std::set<event_it> lock_w;

    for(const auto &read_event : address.second.reads)
    {
      exprt::operandst rf_choice_symbols;
      rf_choice_symbols.reserve(address.second.writes.size());

      // this is quadratic in #events per address
      for(const auto &write_event : address.second.writes)
      {
        // rf cannot contradict program order
        if(assume_local_consistency && po(read_event, write_event))
          continue;

        rf_choice_symbols.push_back(register_read_from_choice_symbol(
          read_event, write_event, equation));
        
        // __WP_ADD_BEGIN__ for deadlock
        if (enable_deadlock) {
          if (id2string(write_event->source.function_id) == "pthread_mutex_lock") 
          {
            lock_w.insert(write_event);

            if (id2string(read_event->source.function_id) == "pthread_mutex_unlock")
              lock_w2unlock_rf[write_event].push_back(rf_choice_symbols.back());
            
            if (id2string(read_event->source.function_id) == "pthread_mutex_lock")
              lock_w2lock_rf[write_event].push_back(rf_choice_symbols.back());
          }
        }
        // __WP_ADD_END__ for deadlock
      }

      // uninitialised global symbol like symex_dynamic::dynamic_object*
      // or *$object
      if(!rf_choice_symbols.empty())
      {
        // Add the read's guard, each of the writes' guards is implied
        // by each entry in rf_some

        exprt rf_some_implications;
        if(equation.r2race_vars.find(read_event) != equation.r2race_vars.end())
          rf_some_implications = or_exprt(disjunction(rf_choice_symbols), disjunction(equation.r2race_vars[read_event])); // // __WP_ADD_ for datarace
        else
          rf_some_implications = disjunction(rf_choice_symbols);

        add_constraint(
          equation,
          implies_exprt{read_event->guard, rf_some_implications},
          "rf-some",
          read_event->source);
      }
    }

    // __WP_ADD_BEGIN__ for deadlock
    if (enable_deadlock) {
      std::cout << "Add the lock-read-from exclusive constraints!\n";
      for(auto& write_event: lock_w)
      {
        add_constraint(
          equation,
          implies_exprt{disjunction(lock_w2unlock_rf[write_event]), not_exprt(disjunction(lock_w2lock_rf[write_event]))},
          "rf-lock-exclusive",
          write_event->source);
      }
    }
    // __WP_ADD_END__ for deadlock
  }

  if(enable_datarace) {
    std::cout << "The number of simplified rf-val constraints: " << simplified_rf_val_count << "\n";
    std::cout << "The total number of original rf-val constraints: " << total_rf_val_count << "\n";
  }
}

symbol_exprt memory_model_baset::register_read_from_choice_symbol(
  const event_it &r,
  const event_it &w,
  symex_target_equationt &equation)
{
  symbol_exprt s = nondet_bool_symbol("rf");

  // record the symbol
  choice_symbols.emplace(std::make_pair(r, w), s);

  // __SZH_ADD_BEGIN__ : completely different when using deagle
  if(use_deagle)
  {
    // __WP_ADD_BEGIN__ for simplify_assignment_encoding

    total_rf_val_count++;
    // std::cout << "It's the rf constraint between a read event " << id2string(r->ssa_lhs.get_identifier()) << " and a write event " << id2string(w->ssa_lhs.get_identifier()) << "!\n";
    
    if (enable_datarace && key_identifiers.find(r->ssa_lhs.get_identifier()) == key_identifiers.end() && key_identifiers.find(w->ssa_lhs.get_identifier()) == key_identifiers.end()) {
      add_constraint(
        equation,
        implies_exprt{s, and_exprt{w->guard, r->guard}},
        "rf-val-simplified",
        r->source);
      
      simplified_rf_val_count++;
      // std::cout << "We simplify the rf-val constraint between a read event " << id2string(r->ssa_lhs.get_identifier()) << " and a write event " << id2string(w->ssa_lhs.get_identifier()) << "!\n";
    }

    // __WP_ADD_END__ for simplify_assignment_encoding

    else {
      add_constraint(
        equation,
        implies_exprt{s, and_exprt{w->guard, r->guard, equal_exprt{r->ssa_lhs, w->ssa_lhs}}},
        "rf-val",
        r->source);
    }
    add_oc_edge(equation, w, r, "rf", s); // rf-order constraints
  }
  else if(use_zord)
  {
    add_constraint(
      equation,
      implies_exprt{s, and_exprt{w->guard, r->guard, equal_exprt{r->ssa_lhs, w->ssa_lhs}}},
      "rf",
      r->source);

    add_constraint(equation, zord_pred(ID_pred_rf, w, r, s), "rf-order", r->source);
  }
  // __SZH_ADD_END__
  else
  {
    bool is_rfi = w->source.thread_nr == r->source.thread_nr;
    // Uses only the write's guard as precondition, read's guard
    // follows from rf_some
    add_constraint(
      equation,
      // We rely on the fact that there is at least
      // one write event that has guard 'true'.
      implies_exprt{s, and_exprt{w->guard, equal_exprt{r->ssa_lhs, w->ssa_lhs}}},
      is_rfi ? "rfi" : "rf",
      r->source);

    if(!is_rfi)
    {
      // if r reads from w, then w must have happened before r
      add_constraint(
        equation, implies_exprt{s, before(w, r)}, "rf-order", r->source);
    }
  }
  return s;
}

// __SZH_ADD_BEGIN__

#include <iostream>

void memory_model_baset::build_guard_map_write(symex_target_equationt &equation)
{
  for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    if(e_it->is_shared_write())
    {
      equation.oc_guard_map.insert(e_it);
      id_to_guard[id(e_it).c_str()] = e_it->guard;
    }
}

void memory_model_baset::build_guard_map_all(symex_target_equationt &equation)
{
  for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    if(e_it->is_shared_write() || e_it->is_shared_read() || e_it->is_memory_barrier())
    {
      equation.oc_guard_map.insert(e_it);
      id_to_guard[id(e_it).c_str()] = e_it->guard;
    }
}

bool memory_model_baset::is_apo(event_it e1, event_it e2)
{
  return e1->atomic_section_id != 0 && e1->atomic_section_id == e2->atomic_section_id;
}

bool memory_model_baset::is_apo(std::set<std::pair<std::string, std::string>>& array_update_set, event_it e1, event_it e2)
{
  std::string e1_str = id2string(id(e1));
  std::string e2_str = id2string(id(e2));
  if(array_update_set.find(std::make_pair(e1_str, e2_str)) != array_update_set.end())
    return true;
  return is_apo(e1, e2);
}

std::string memory_model_baset::fill_name(const event_it& event)
{
    if(event->is_spawn())
        return "t"+std::to_string(event->source.thread_nr+1)+"$"+ std::to_string(numbering[event])+"$spwnclk";
    else if(event->is_memory_barrier())
        return "t"+std::to_string(event->source.thread_nr+1)+"$"+ std::to_string(numbering[event])+"$fenceclk";
    
    return "t"+std::to_string(event->source.thread_nr+1)+"$"+ std::to_string(numbering[event])+"$unknownclk";
}

std::string memory_model_baset::get_name(event_it e)
{
  std::string e_str = id2string(id(e));
  if(e_str == "")
    e_str = fill_name(e);
  return e_str;
}

std::string memory_model_baset::get_name_with_clk(event_it e)
{
  auto basename = get_name(e);
  if(e->is_shared_read())
    return basename + "$rclk";
  if(e->is_shared_write())
    return basename + "$wclk";
  return basename;
}

void memory_model_baset::add_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr)
{
  add_oc_edge(equation, get_name(e1), get_name(e2), kind, guard_expr);
}

// #include <util/format_expr.h>

void memory_model_baset::add_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr)
{
  if(guard_expr == false_exprt())
    return;
  equation.oc_edges.push_back(oc_edget(e1_str, e2_str, kind, guard_expr));

  // std::cout << "add " << kind << " edge between " << e1_str << " and " << e2_str << ", with guard " << format(guard_expr) << "\n";
}

void memory_model_baset::add_oc_label(symex_target_equationt &equation, event_it e, std::string label, exprt guard_expr)
{
  add_oc_label(equation, get_name(e), label, guard_expr);
}

void memory_model_baset::add_oc_label(symex_target_equationt &equation, std::string e_str, std::string label, exprt guard_expr)
{
  if(guard_expr == false_exprt())
    return;
  equation.oc_labels.push_back(oc_labelt(e_str, label, guard_expr));

  // std::cout << "add " << label << " label to " << e_str << ", with guard " << format(guard_expr) << "\n";
}

void memory_model_baset::data_race(symex_target_equationt &equation)
{
  std::map<symbol_exprt, exprt::operandst> dirty_to_races;
  std::map<symbol_exprt, const symex_targett::sourcet*> dirty_to_source;

  for(int i = 0; i < int(equation.numbered_dataraces.size()); i++)
  {
    auto& race = equation.numbered_dataraces[i];
    auto& first_event = race.first;
    auto& second_event =race.second;

    std::string race_id_str = "race" + std::to_string(i);
    irep_idt race_id(race_id_str);
    symbol_exprt race_var(race_id, bool_typet());

    if(use_deagle)
      add_oc_edge(equation, first_event, second_event, "race", race_var); // rc-order constraints
    else if(use_zord)
      add_constraint(equation, zord_pred(ID_pred_race, first_event, second_event, race_var), "race", first_event->source);
    else
    {
      implies_exprt race_order(race_var, before(first_event, second_event));
      add_constraint(equation, race_order, "race", first_event->source);
    }

    exprt::operandst dirties;

    if(first_event->is_shared_read())
      equation.r2race_vars[first_event].push_back(race_var);
    
    if(second_event->is_shared_read())
      equation.r2race_vars[second_event].push_back(race_var);
  }
}

#include <algorithm>

void find_indices(
  const exprt &src,
  std::vector<index_exprt> &dest)
{
  if(src.id()==ID_index)
    dest.push_back(to_index_expr(src));
  else
  {
    forall_operands(it, src)
      find_indices(*it, dest);
  }
}

exprt memory_model_baset::array_find_index(symex_target_equationt &equation, event_it event)
{
    exprt event_index = nil_exprt();

    for(auto event_next = event; event_next != equation.SSA_steps.end(); event_next++)
    {
        if(event_next->is_assignment())
        {
            auto& lhs = event_next->ssa_lhs;
            auto& rhs = event_next->ssa_rhs;

            if(rhs.id() != ID_with)
                continue;

            auto& with_original = rhs.op0(); //useless
            auto& with_index = rhs.op1();
            auto& with_new_value = rhs.op2();

            if(event->is_shared_write() && event->ssa_lhs.get_identifier() == lhs.get_identifier())
            {
                event_index = with_index;
            }
            if(event->is_shared_read() && event->ssa_lhs.get_identifier() == to_symbol_expr(with_original).get_identifier())
            {
                std::vector<index_exprt> index_exprs;
                find_indices(with_new_value, index_exprs);
                if(index_exprs.size() == 1)
                {
                    auto& index_expr = index_exprs[0];
                    event_index = index_expr.op1();
                }
            }
        }
    }

    return event_index;
}

void memory_model_baset::build_per_thread_map(
  const symex_target_equationt &equation,
  per_thread_mapt &dest,
  bool skip_init) const
{
  // this orders the events within a thread

  for(eventst::const_iterator
      e_it=equation.SSA_steps.begin();
      e_it!=equation.SSA_steps.end();
      e_it++)
  {
    // concurrency-related?
    if(!e_it->is_shared_read() &&
       !e_it->is_shared_write() &&
       !e_it->is_spawn() &&
       !e_it->is_memory_barrier()) continue;

    if(skip_init && e_it->is_init())
      continue;

    dest[e_it->source.thread_nr].push_back(e_it);
  }
}

void memory_model_baset::build_per_loc_map(
  const symex_target_equationt &equation,
  per_loc_mapt &dest) const
{
  // this orders the events with the same location

  for(eventst::const_iterator
      e_it=equation.SSA_steps.begin();
      e_it!=equation.SSA_steps.end();
      e_it++)
  {
    // concurrency-related?
    if(!e_it->is_shared_read() &&
       !e_it->is_shared_write()) continue;

    auto lhs = e_it->ssa_lhs;
    std::string original_name = remove_level_2(lhs).get_identifier().c_str();

    dest[original_name].push_back(e_it);
  }
}

// __SZH_ADD_END__


// __WP_ADD_BEGIN__
#include <util/find_symbols.h>

void memory_model_baset::simplify_assignment_encoding(symex_target_equationt &equation)
{
  std::cout << "simplify assignment_encoding!\n";

  std::map<irep_idt, std::set<irep_idt>> address2identifiers;

  // key_identifiers is initalized to all identifiers in guard condtions
  std::set<symbol_exprt> reads;

  for(eventst::const_iterator
    e_it=equation.SSA_steps.begin();
    e_it!=equation.SSA_steps.end();
    e_it++) {

      if(e_it->ssa_rhs.id() == ID_if && e_it->ssa_rhs.operands().size() == 3) {

        // std::cout << "This is a ternary expression with condition: " << e_it->ssa_rhs.operands()[0].pretty() << std::endl;

        reads.clear();
        find_symbols(e_it->ssa_rhs.operands()[0], reads);
        for (auto& read : reads) {
          key_identifiers.insert(read.get_identifier());
          address2identifiers[remove_level_2(to_ssa_expr(read)).get_identifier()].insert(read.get_identifier());

          // std::cout << "read.get_identifier():" << id2string(read.get_identifier()) << "\n";
          // std::cout << "remove_level_2(to_ssa_expr(read)).get_identifier():" << id2string(remove_level_2(to_ssa_expr(read)).get_identifier()) << "\n";
        }
      }

      reads.clear();
      find_symbols(e_it->guard, reads);
      for (auto& read : reads) {
        key_identifiers.insert(read.get_identifier());
        address2identifiers[remove_level_2(to_ssa_expr(read)).get_identifier()].insert(read.get_identifier());

        // std::cout << "read.get_identifier():" << id2string(read.get_identifier()) << "\n";
        // std::cout << "remove_level_2(to_ssa_expr(read)).get_identifier():" << id2string(remove_level_2(to_ssa_expr(read)).get_identifier()) << "\n";
      }
    }

  // new_identifiers is initalized to all identifiers that may affect the value of the key_identifiers
  std::set<irep_idt> parent_identifiers(key_identifiers.begin(), key_identifiers.end());
  std::set<irep_idt> new_identifiers;

  do {
    new_identifiers.clear();
    
    for(eventst::const_iterator
        e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end();
        e_it++) {

          if (!e_it->is_assignment())
            continue;

          // std::cout << "id2string(e_it->ssa_lhs.get_identifier()):" << id2string(e_it->ssa_lhs.get_identifier()) << "\n";
          // std::cout << "id2string(address(e_it)):" << id2string(address(e_it)) << "\n";

          if (address2identifiers.find(address(e_it)) != address2identifiers.end())
          {
            // std::cout << "id2string(e_it->ssa_lhs.get_identifier()):" << id2string(e_it->ssa_lhs.get_identifier()) << "\n";
            // std::cout << "id2string(address(e_it)):" << id2string(address(e_it)) << "\n";

            if (key_identifiers.find(e_it->ssa_lhs.get_identifier()) == key_identifiers.end()) {
                new_identifiers.insert(e_it->ssa_lhs.get_identifier());
                address2identifiers[address(e_it)].insert(e_it->ssa_lhs.get_identifier());
            }

            std::set<symbol_exprt> reads;
            find_symbols(e_it->ssa_rhs, reads);
            for(auto& read : reads) {
              if (key_identifiers.find(read.get_identifier()) == key_identifiers.end()) {
                new_identifiers.insert(read.get_identifier());
                address2identifiers[remove_level_2(to_ssa_expr(read)).get_identifier()].insert(read.get_identifier());
              }
            }

          }

        }
    key_identifiers.insert(new_identifiers.begin(), new_identifiers.end());
    parent_identifiers = std::move(new_identifiers);

    // std::cout << "parent_identifiers:\n";
    // for(auto& parent_identifier : parent_identifiers)
    //   std::cout << id2string(parent_identifier) << "\n";
    
    // std::cout << "key_identifiers:\n";
    // for(auto& key_identifier : key_identifiers)
    //   std::cout << id2string(key_identifier) << "\n";
    
    // std::cout << "address2identifiers:";
    // for(auto& p : address2identifiers) {
    //   std::cout << "\naddress:" << id2string(p.first) << "\nidentifiers:";
    //   for(auto& id : p.second)
    //     std::cout << id2string(id) << " ";
    // }

  } while (!parent_identifiers.empty());

  for(auto& key_identifier : key_identifiers)
    std::cout << "key_identifier:" << id2string(key_identifier) << "\n";
  
  // for(auto& p : address2identifiers) {
  //   std::cout << "\naddress:" << id2string(p.first) << "\nidentifiers:";
  //   for(auto& id : p.second)
  //     std::cout << id2string(id) << " ";
  // }

  
  // igore all non-key ssa
  int removed_ssa_count = 0, ssa_total_count = 0;
  for(eventst::iterator
      e_it=equation.SSA_steps.begin();
      e_it!=equation.SSA_steps.end();
      e_it++) {
        if (!e_it->is_assignment())
          continue;
        if (e_it->assignment_type != symex_targett::assignment_typet::STATE)
          continue;
        ssa_total_count++;

  //       if (
  //         id2string(e_it->ssa_lhs.get_identifier()) == "main::1::p!0@1#2"
  //   || id2string(e_it->ssa_lhs.get_identifier()) == "main::1::p!0@1#3"
  //   || id2string(e_it->ssa_lhs.get_identifier()) == "main::1::p!0@1#4"
  //   || id2string(e_it->ssa_lhs.get_identifier()) == "main::1::p!0@1#5"
  // ) {
  //         std::cout << "e_it->output: \n";
  //         e_it->output(std::cout);
  //         continue;
  //       }
        
        if (key_identifiers.find(e_it->ssa_lhs.get_identifier()) == key_identifiers.end()) {
          e_it->ignore = true;
          removed_ssa_count++;

          // std::cout << "e_it->ssa_lhs.pretty(): " << e_it->ssa_lhs.pretty() << "\n";
        }
      }
  std::cout << "The number of removed ssa encoding: " << removed_ssa_count << "\n";
  std::cout << "The total number of original ssa encoding: " << ssa_total_count << "\n";
  
}
// __WP_ADD_END__
