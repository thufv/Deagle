/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

/// \file
/// Memory model for partial order concurrency

#include "memory_model_sc.h"

#include <util/std_expr.h>
#include <iostream>

void memory_model_sct::
operator()(symex_target_equationt &equation, message_handlert &message_handler)
{
  std::cout << "see the equation!\n";
  equation.output(std::cout);
  std::cout << "finish seeing!\n";

  messaget log{message_handler};
  log.statistics() << "Adding SC constraints" << messaget::eom;

  build_event_lists(equation, message_handler);
  build_clock_type();

  if(enable_datarace)
  {
    simplify_assignment_encoding(equation);
    data_race(equation);
  }

  read_from(equation);

  if(!use_deagle || equation.use_deagle_icd)
    write_serialization_external(equation);
  
  program_order(equation);

  if(!use_deagle && !use_zord)
    from_read(equation);

  if(use_zord)
  {
    build_guard_of(equation);
    build_addr_of(equation);
  }

  // std::cout << "see the equation!\n";
  // equation.output(std::cout);
  // std::cout << "finish seeing!\n";
}

exprt memory_model_sct::before(event_it e1, event_it e2)
{
  return partial_order_concurrencyt::before(
    e1, e2, AX_PROPAGATION);
}

bool memory_model_sct::program_order_is_relaxed(
  partial_order_concurrencyt::event_it e1,
  partial_order_concurrencyt::event_it e2) const
{
  PRECONDITION(e1->is_shared_read() || e1->is_shared_write());
  PRECONDITION(e2->is_shared_read() || e2->is_shared_write());

  return false;
}

void memory_model_sct::thread_spawn(
  symex_target_equationt &equation,
  const per_thread_mapt &per_thread_map)
{
  // thread spawn: the spawn precedes the first
  // instruction of the new thread in program order

  unsigned next_thread_id=0;
  for(eventst::const_iterator
      e_it=equation.SSA_steps.begin();
      e_it!=equation.SSA_steps.end();
      e_it++)
  {
    if(e_it->is_spawn())
    {
      per_thread_mapt::const_iterator next_thread=
        per_thread_map.find(++next_thread_id);
      if(next_thread==per_thread_map.end())
        continue;

      // Retain the model order of thread creation for YAML witnesses.
      for(auto &spawn : equation.witness_spawn_events)
        if(spawn.child_thread == next_thread_id)
          spawn.clock = use_deagle
            ? symbol_exprt::typeless(get_name(e_it))
            : clock(e_it, AX_PROPAGATION);

      // add a constraint for all events,
      // considering regression/cbmc-concurrency/pthread_create_tso1
      for(event_listt::const_iterator
          n_it=next_thread->second.begin();
          n_it!=next_thread->second.end();
          n_it++)
      {
        if(!(*n_it)->is_memory_barrier())
        {
          if(use_deagle)
            add_oc_edge(equation, e_it, *n_it, is_apo(e_it, *n_it) ? "apo" : "po", true_exprt());
          else if(use_zord)
          {
            add_constraint(equation, zord_pred(ID_pred_po, e_it, *n_it), "thread_spawn", e_it->source);
            if(is_apo(e_it, *n_it))
              add_constraint(equation, zord_pred(ID_pred_atom, e_it, *n_it), "thread_spawn", e_it->source);
          }
          else
            add_constraint(equation, before(e_it, *n_it), "thread-spawn", e_it->source);
        }
      }
    }
  }
}

#if 0
void memory_model_sct::thread_spawn(
  symex_target_equationt &equation,
  const per_thread_mapt &per_thread_map)
{
  // thread spawn: the spawn precedes the first
  // instruction of the new thread in program order

  unsigned next_thread_id=0;
  for(eventst::const_iterator
      e_it=equation.SSA_steps.begin();
      e_it!=equation.SSA_steps.end();
      e_it++)
  {
    if(is_spawn(e_it))
    {
      per_thread_mapt::const_iterator next_thread=
        per_thread_map.find(++next_thread_id);
      if(next_thread==per_thread_map.end())
        continue;

      // For SC and several weaker memory models a memory barrier
      // at the beginning of a thread can simply be ignored, because
      // we enforce program order in the thread-spawn constraint
      // anyway. Memory models with cumulative memory barriers
      // require explicit handling of these.
      event_listt::const_iterator n_it=next_thread->second.begin();
      for( ;
          n_it!=next_thread->second.end() &&
          (*n_it)->is_memory_barrier();
          ++n_it)
      {
      }

      if(n_it!=next_thread->second.end())
        add_constraint(
          equation,
          before(e_it, *n_it),
          "thread-spawn",
          e_it->source);
    }
  }
}
#endif

void memory_model_sct::program_order(
  symex_target_equationt &equation)
{
  per_thread_mapt per_thread_map;
  build_per_thread_map(equation, per_thread_map);

  thread_spawn(equation, per_thread_map);

  if(use_deagle)
    build_guard_map_write(equation);

  std::set<std::pair<std::string, std::string>> apo_set;
  equation.build_array_update_set(apo_set);
  equation.build_same_pointer_set(apo_set);
  equation.build_same_line_atomic_set(ns, apo_set);

  // iterate over threads

  for(per_thread_mapt::const_iterator
      t_it=per_thread_map.begin();
      t_it!=per_thread_map.end();
      t_it++)
  {
    const event_listt &events=t_it->second;

    // iterate over relevant events in the thread

    event_it previous=equation.SSA_steps.end();

    for(event_listt::const_iterator
        e_it=events.begin();
        e_it!=events.end();
        e_it++)
    {
      if((*e_it)->is_memory_barrier())
         continue;

      if(previous==equation.SSA_steps.end())
      {
        // first one?
        previous=*e_it;
        continue;
      }

      if(use_deagle)
        add_oc_edge(equation, previous, *e_it, is_apo(apo_set, previous, *e_it) ? "apo" : "po", true_exprt());
      else if(use_zord)
      {
        add_constraint(equation, zord_pred(ID_pred_po, previous, *e_it), "thread_spawn", (*e_it)->source);
        if(is_apo(previous, *e_it))
          add_constraint(equation, zord_pred(ID_pred_atom, previous, *e_it), "thread_spawn", (*e_it)->source);
      }
      else
        add_constraint(equation, before(previous, *e_it), "po", (*e_it)->source);

      previous=*e_it;
    }
  }
}

void memory_model_sct::write_serialization_external(
  symex_target_equationt &equation)
{
  for(auto a_it=address_map.begin(); a_it!=address_map.end(); a_it++)
  {
    const a_rect &a_rec=a_it->second;

    // This is quadratic in the number of writes
    // per address. Perhaps some better encoding
    // based on 'places'?
    for(auto w_it1=a_rec.writes.begin(); w_it1!=a_rec.writes.end(); ++w_it1)
    {
      auto next=w_it1;
      ++next;

      for(auto w_it2=next; w_it2!=a_rec.writes.end(); ++w_it2)
      {
        // ws is a total order, no two elements have the same rank
        // s -> w_evt1 before w_evt2; !s -> w_evt2 before w_evt1

        bool can12 = !(assume_local_consistency && po(*w_it2, *w_it1));
        bool can21 = !(assume_local_consistency && po(*w_it1, *w_it2));
        if((*w_it1)->is_init())
          can21 = false;
        if((*w_it2)->is_init())
          can12 = false;

        if(can12 && can21)
        {
          symbol_exprt s12=nondet_bool_symbol("ws");
          symbol_exprt s21=nondet_bool_symbol("ws");

          add_constraint(equation, equal_exprt(or_exprt(s12, s21), and_exprt((*w_it1)->guard, (*w_it2)->guard)), "ws-some", (*w_it1)->source);
          // add_constraint(equation, not_exprt(and_exprt(s12, s21)), "ws-only-one", (*w_it1)->source);
          if(use_deagle)
          {
            add_oc_edge(equation, *w_it1, *w_it2, "ws", s12);
            add_oc_edge(equation, *w_it2, *w_it1, "ws", s21);
          }
          else if(use_zord)
          {
            add_constraint(equation, zord_pred(ID_pred_ws, *w_it1, *w_it2, s12), "ws", (*w_it1)->source);
            add_constraint(equation, zord_pred(ID_pred_ws, *w_it2, *w_it1, s21), "ws", (*w_it1)->source);
          }
          else
          {
            add_constraint(equation, implies_exprt(s12, before(*w_it1, *w_it2)), "ws", (*w_it1)->source);
            add_constraint(equation, implies_exprt(s21, before(*w_it2, *w_it1)), "ws", (*w_it1)->source);
          }
        }
        else if(can12)
        {
          symbol_exprt s12=nondet_bool_symbol("ws");
          add_constraint(equation, equal_exprt(s12, and_exprt((*w_it1)->guard, (*w_it2)->guard)), "ws-some", (*w_it1)->source);
          if(use_deagle)
            add_oc_edge(equation, *w_it1, *w_it2, "ws", s12);
          else if(use_zord)
            add_constraint(equation, zord_pred(ID_pred_ws, *w_it1, *w_it2, s12), "ws", (*w_it1)->source);
          else
            add_constraint(equation, implies_exprt(s12, before(*w_it1, *w_it2)), "ws", (*w_it1)->source);
        }
        else if(can21)
        {
          symbol_exprt s21=nondet_bool_symbol("ws");
          add_constraint(equation, equal_exprt(s21, and_exprt((*w_it2)->guard, (*w_it1)->guard)), "ws-some", (*w_it1)->source);
          if(use_deagle)
            add_oc_edge(equation, *w_it2, *w_it1, "ws", s21);
          else if(use_zord)
            add_constraint(equation, zord_pred(ID_pred_ws, *w_it2, *w_it1, s21), "ws", (*w_it1)->source);
          else
            add_constraint(equation, implies_exprt(s21, before(*w_it2, *w_it1)), "ws", (*w_it1)->source);
        }
        else
          std::cout << "WARNING: either " << id(*w_it1) << " or " << id(*w_it2) << " cannot coherence before each other.\n";
      }
    }
  }

  // for(address_mapt::const_iterator
  //     a_it=address_map.begin();
  //     a_it!=address_map.end();
  //     a_it++)
  // {
  //   const a_rect &a_rec=a_it->second;

  //   // This is quadratic in the number of writes
  //   // per address. Perhaps some better encoding
  //   // based on 'places'?
  //   for(event_listt::const_iterator
  //       w_it1=a_rec.writes.begin();
  //       w_it1!=a_rec.writes.end();
  //       ++w_it1)
  //   {
  //     event_listt::const_iterator next=w_it1;
  //     ++next;

  //     for(event_listt::const_iterator w_it2=next;
  //         w_it2!=a_rec.writes.end();
  //         ++w_it2)
  //     {
  //       // external?
  //       if((*w_it1)->source.thread_nr==
  //          (*w_it2)->source.thread_nr)
  //         continue;

  //       // ws is a total order, no two elements have the same rank
  //       // s -> w_evt1 before w_evt2; !s -> w_evt2 before w_evt1

  //       symbol_exprt s=nondet_bool_symbol("ws-ext");

  //       // write-to-write edge
  //       add_constraint(
  //         equation,
  //         implies_exprt(s, before(*w_it1, *w_it2)),
  //         "ws-ext",
  //         (*w_it1)->source);

  //       add_constraint(
  //         equation,
  //         implies_exprt(not_exprt(s), before(*w_it2, *w_it1)),
  //         "ws-ext",
  //         (*w_it1)->source);
  //     }
  //   }
  // }
}

void memory_model_sct::from_read(symex_target_equationt &equation)
{
  // from-read: (w', w) in ws and (w', r) in rf -> (r, w) in fr

  for(address_mapt::const_iterator
      a_it=address_map.begin();
      a_it!=address_map.end();
      a_it++)
  {
    const a_rect &a_rec=a_it->second;

    // This is quadratic in the number of writes per address.
    for(event_listt::const_iterator
        w_prime=a_rec.writes.begin();
        w_prime!=a_rec.writes.end();
        ++w_prime)
    {
      event_listt::const_iterator next=w_prime;
      ++next;

      for(event_listt::const_iterator w=next;
          w!=a_rec.writes.end();
          ++w)
      {
        exprt ws1, ws2;

        if(po(*w_prime, *w) &&
           !program_order_is_relaxed(*w_prime, *w))
        {
          ws1=true_exprt();
          ws2=false_exprt();
        }
        else if(po(*w, *w_prime) &&
                !program_order_is_relaxed(*w, *w_prime))
        {
          ws1=false_exprt();
          ws2=true_exprt();
        }
        else
        {
          ws1=before(*w_prime, *w);
          ws2=before(*w, *w_prime);
        }

        // smells like cubic
        for(choice_symbolst::const_iterator
            c_it=choice_symbols.begin();
            c_it!=choice_symbols.end();
            c_it++)
        {
          event_it r=c_it->first.first;
          exprt rf=c_it->second;
          exprt cond;
          cond.make_nil();

          if(c_it->first.second==*w_prime && !ws1.is_false())
          {
            exprt fr=before(r, *w);

            // the guard of w_prime follows from rf; with rfi
            // optimisation such as the previous write_symbol_primed
            // it would even be wrong to add this guard
            cond=
              implies_exprt(
                and_exprt(r->guard, (*w)->guard, ws1, rf),
                fr);
          }
          else if(c_it->first.second==*w && !ws2.is_false())
          {
            exprt fr=before(r, *w_prime);

            // the guard of w follows from rf; with rfi
            // optimisation such as the previous write_symbol_primed
            // it would even be wrong to add this guard
            cond=
              implies_exprt(
                and_exprt(r->guard, (*w_prime)->guard, ws2, rf),
                fr);
          }

          if(cond.is_not_nil())
            add_constraint(equation,
              cond, "fr", r->source);
        }
      }
    }
  }
}

void memory_model_sct::build_guard_of(symex_target_equationt &equation)
{
  for(auto e_it = equation.SSA_steps.begin(); e_it != equation.SSA_steps.end(); e_it++)
  {
    if(!e_it->is_shared_read() && !e_it->is_shared_write() && !e_it->is_spawn() && !e_it->is_memory_barrier())
      continue;

    symbol_exprt e_clk(get_name_with_clk(e_it), typet(ID_event));
    symbol_exprt e_guard(get_name_with_clk(e_it) + "$$guard", bool_typet());

    add_constraint(equation, binary_exprt(e_clk, ID_guard_of, e_guard, bool_typet()), "zord_guard", e_it->source);
    add_constraint(equation, equal_exprt(e_guard, e_it->guard), "zord_guard", e_it->source);
  }
}

#include <util/arith_tools.h>
#include <util/mathematical_types.h>

void memory_model_sct::build_addr_of(symex_target_equationt &equation)
{

  std::map<irep_idt, int> addr_to_id;

  for(auto e_it = equation.SSA_steps.begin(); e_it != equation.SSA_steps.end(); e_it++)
  {
    if(!e_it->is_shared_read() && !e_it->is_shared_write() && !e_it->is_spawn() && !e_it->is_memory_barrier())
      continue;

    symbol_exprt e_clk(get_name_with_clk(e_it), typet(ID_event));

    auto addr = (e_it->is_shared_read() || e_it->is_shared_write()) ? e_it->ssa_lhs.get_l1_object_identifier() : get_name(e_it);
    if(addr_to_id.find(addr) == addr_to_id.end())
      addr_to_id[addr] = addr_to_id.size();
    int addr_id = addr_to_id[addr];
    constant_exprt addr_const = from_integer(addr_id, integer_typet());

    add_constraint(equation, binary_exprt(e_clk, ID_addr_of, addr_const, bool_typet()), "zord_addr", e_it->source);
  }
}
