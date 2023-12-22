/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

/// \file
/// Memory model for partial order concurrency

#include "memory_model.h"

#include <util/std_expr.h>

memory_model_baset::memory_model_baset(const namespacet &_ns)
  : partial_order_concurrencyt(_ns), use_deagle(false), enable_datarace(false), var_cnt(0)
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

void memory_model_baset::read_from(symex_target_equationt &equation)
{
  // We iterate over all the reads, and
  // make them match at least one
  // (internal or external) write.

  for(const auto &address : address_map)
  {
    for(const auto &read_event : address.second.reads)
    {
      exprt::operandst rf_choice_symbols;
      rf_choice_symbols.reserve(address.second.writes.size());

      // this is quadratic in #events per address
      for(const auto &write_event : address.second.writes)
      {
        // rf cannot contradict program order
        if(!po(read_event, write_event))
        {
          rf_choice_symbols.push_back(register_read_from_choice_symbol(
            read_event, write_event, equation));
        }
      }

      // uninitialised global symbol like symex_dynamic::dynamic_object*
      // or *$object
      if(!rf_choice_symbols.empty())
      {
        // Add the read's guard, each of the writes' guards is implied
        // by each entry in rf_some

        // __SZH_ADD_BEGIN__ add dirty
        exprt rf_some_cond;
        if(equation.read_dirties.find(read_event) != equation.read_dirties.end())
          rf_some_cond = and_exprt(read_event->guard, not_exprt(equation.read_dirties[read_event]));
        else
          rf_some_cond = read_event->guard;

        add_constraint(
          equation,
          implies_exprt{rf_some_cond, disjunction(rf_choice_symbols)},
          "rf-undirty-some",
          read_event->source);
        // __SZH_ADD_END__

        // add_constraint(
        //   equation,
        //   implies_exprt{read_event->guard, disjunction(rf_choice_symbols)},
        //   "rf-some",
        //   read_event->source);
      }
    }
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
    add_constraint(
      equation,
      implies_exprt{s, and_exprt{w->guard, r->guard, equal_exprt{r->ssa_lhs, w->ssa_lhs}}},
      "rf",
      r->source);
    add_oc_edge(equation, w, r, "rf", s);
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
      equation.oc_guard_map.insert(e_it);
}

void memory_model_baset::build_guard_map_all(symex_target_equationt &equation)
{
  for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    if(e_it->is_shared_write() || e_it->is_shared_read() || e_it->is_memory_barrier())
      equation.oc_guard_map.insert(e_it);
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

std::string memory_model_baset::fill_name(event_it& event)
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

void memory_model_baset::add_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr)
{
  add_oc_edge(equation, get_name(e1), get_name(e2), kind, guard_expr);
}

void memory_model_baset::add_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr)
{
  equation.oc_edges.push_back(oc_edget(e1_str, e2_str, kind, guard_expr));
  // std::cout << "add relation: " << e1_str << " " << e2_str << " has " << kind << "\n";
}

void memory_model_baset::add_oc_label(symex_target_equationt &equation, event_it e, std::string label)
{
  std::string e_str = id2string(id(e));
  if(e_str == "")
    e_str = fill_name(e);
  
  equation.oc_labels.push_back(oc_labelt(e_str, label));

  // std::cout << "add label: " << e_str << " is a " << label << "\n";
}

void memory_model_baset::data_race(symex_target_equationt &equation)
{
  for(int id = 0; id < int(equation.numbered_dataraces.size()); id++)
  {
    auto& race = equation.numbered_dataraces[id];
    auto& first_event = race.first;
    auto& second_event = race.second;

    std::string race_id_str = "race" + std::to_string(id);
    irep_idt race_id(race_id_str);
    symbol_exprt race_var(race_id, bool_typet());

    if(use_deagle)
      add_oc_edge(equation, first_event, second_event, "race", race_var);
    else
    {
      implies_exprt race_order(race_var, before(first_event, second_event));
      add_constraint(equation, race_order, "race", first_event->source);
    }
  }

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

    exprt::operandst dirties;

    if(first_event->is_shared_read())
    {
      if(equation.read_dirties.find(first_event) == equation.read_dirties.end())
        equation.read_dirties[first_event] = symbol_exprt(nondet_bool_symbol("dirty"));
      symbol_exprt& dirty_var = to_symbol_expr(equation.read_dirties[first_event]);
      dirties.push_back(dirty_var);
      dirty_to_races[dirty_var].push_back(race_var);
      dirty_to_source[dirty_var] = &(first_event->source);
    }

    if(second_event->is_shared_read())
    {
      if(equation.read_dirties.find(second_event) == equation.read_dirties.end())
        equation.read_dirties[second_event] = symbol_exprt(nondet_bool_symbol("dirty"));
      symbol_exprt& dirty_var = to_symbol_expr(equation.read_dirties[second_event]);
      dirties.push_back(dirty_var);
      dirty_to_races[dirty_var].push_back(race_var);
      dirty_to_source[dirty_var] = &(second_event->source);
    }

    if(!dirties.empty())
      add_constraint(equation, implies_exprt(race_var, conjunction(dirties)), "race-dirty", first_event->source);
  }

  for(auto& pair : dirty_to_races)
  {
    const symbol_exprt& dirty_var = pair.first;
    auto& races = pair.second;
    add_constraint(equation, implies_exprt(dirty_var, disjunction(races)), "dirty-some", *(dirty_to_source[dirty_var]));
  }

    // std::map<event_it, exprt::operandst> read_to_races;

    // exprt::operandst all_races;

    // event_it any_event;

    // for(auto& datarace_pair: equation.datarace_pairs)
    // {
    //   auto first_event = datarace_pair.first;
    //   auto second_event = datarace_pair.second;

    //   auto e1_str = id2string(id(first_event));
    //   auto e2_str = id2string(id(second_event));
      
    //   symbol_exprt race = symbol_exprt(nondet_bool_symbol("race"));
    //   std::string race_lit = race.get_identifier().c_str();

    //   implies_exprt race_guard(race, and_exprt(first_event->guard, second_event->guard));
    //   add_constraint(equation, race_guard, "race", first_event->source);

    //   if(use_deagle)
    //     add_oc_edge(equation, first_event, second_event, "race", race);
    //   else
    //   {
    //     implies_exprt race_order(race, before(first_event, second_event));
    //     add_constraint(equation, race_order, "race", first_event->source);
    //   }

    //   any_event = first_event;

    //   if(first_event->is_shared_read())
    //   {
    //     read_to_races[first_event].push_back(race);
    //     // if(read_disorders.find(first_event) == read_disorders.end())
    //     //   read_disorders[first_event] = symbol_exprt(nondet_bool_symbol("disorder"));
    //   }

    //   if(second_event->is_shared_read())
    //   {
    //     read_to_races[second_event].push_back(race);
    //     // if(read_disorders.find(second_event) == read_disorders.end())
    //     //   read_disorders[second_event] = symbol_exprt(nondet_bool_symbol("disorder"));
    //   }

    //   all_races.push_back(race);

    //   //if arrays race, we make them access the same index
    //   if(first_event->ssa_lhs.type().id() == ID_array)
    //   {
    //     std::cout << "racing arrays!\n";

    //     exprt first_event_index = array_find_index(equation, first_event);
    //     exprt second_event_index = array_find_index(equation, second_event);

    //     if(first_event_index.type() == second_event_index.type() && first_event_index != nil_exprt())
    //     {
    //       std::cout << "find their indices!!!\n";
    //       //std::cout << "\t" << first_event_index << " \n\t and " << second_event_index << "\n";

    //       implies_exprt race_array_index(race, equal_exprt(first_event_index, second_event_index));
    //       add_constraint(equation, race_array_index, "race", first_event->source);
    //     }
    //   }
    // }

    // //Constraint: race_some
    // if(all_races.empty())
    // {
    //   std::cout << "Error: No race to detect.\n";
    //   std::exit(1);
    // }
    // else
    //   add_constraint(equation, disjunction(all_races), "race_some", any_event->source);

    // //Constraint: disorder_guard
    // for(auto& affected_read : read_disorders)
    // {
    //     auto& read = affected_read.first;
    //     auto& disorder = affected_read.second;

    //     implies_exprt disorder_guard(disorder, read->guard);
    //     add_constraint(equation, disorder_guard, "disorder_guard", read->source);
    // }

    // //Constraint: disorder_some
    // for(auto& affected_read : read_to_races)
    // {
    //     auto& read = affected_read.first;
    //     auto& initial_races = affected_read.second;
    //     exprt races;

    //     if(initial_races.empty())
    //     {
    //         std::cout << "Error: Read " << id2string(id(read)) << " has no races\n";
    //         std::exit(1);
    //     }
    //     else if(initial_races.empty())
    //     {
    //         races = initial_races[0];
    //     }
    //     else
    //     {
    //         races = or_exprt();
    //         races.operands().swap(initial_races);
    //     }

    //     equal_exprt disorder_some(read_disorders[read], races);
    //     add_constraint(equation, disorder_some, "disorder_some", read->source);
    // }

    // std::cout << "see the equation after race!\n";
    // equation.output(std::cout);
    // std::cout << "finish seeing!\n";
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
  per_thread_mapt &dest) const
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
    std::string original_name = lhs.get_original_name().c_str();

    dest[original_name].push_back(e_it);
  }
}

// __SZH_ADD_END__