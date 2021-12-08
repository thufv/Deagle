/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#include <util/std_expr.h>
#include <util/simplify_expr.h>

#include "memory_model_tso.h"

/*******************************************************************\

Function: memory_model_tsot::operator()

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void memory_model_tsot::operator()(symex_target_equationt &equation)
{
  print(8, "Adding TSO constraints");

  build_event_lists(equation);
  build_clock_type(equation);

  read_from(equation);
  if(solve_method == solve_method_smt2)
    write_serialization_external(equation);
  program_order(equation);
//#ifndef CPROVER_MEMORY_MODEL_SUP_CLOCK
//  from_read(equation);
//#endif

// __SZH_ADD_BEGIN__
#if front_deduce_all_fr
  from_read(equation);
#endif
// __SZH_ADD_END__

}

/*******************************************************************\

Function: memory_model_tsot::before

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

exprt memory_model_tsot::before(event_it e1, event_it e2)
{
  return partial_order_concurrencyt::before(
    e1, e2, AX_SC_PER_LOCATION | AX_PROPAGATION);
}

/*******************************************************************\

Function: memory_model_tsot::program_order_is_relaxed

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool memory_model_tsot::program_order_is_relaxed(
  partial_order_concurrencyt::event_it e1,
  partial_order_concurrencyt::event_it e2) const
{
  assert(is_shared_read(e1) || is_shared_write(e1));
  assert(is_shared_read(e2) || is_shared_write(e2));

  // no po relaxation within atomic sections
  if(e1->atomic_section_id!=0 && e1->atomic_section_id==e2->atomic_section_id)
    return false;

  // write to read program order is relaxed
  return is_shared_write(e1) && is_shared_read(e2);
}

/*******************************************************************\

Function: memory_model_tsot::program_order

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

typedef std::map<partial_order_concurrencyt::event_it, std::set<partial_order_concurrencyt::event_it> > event_i_mapt;

bool is_redundant(event_i_mapt& pred_map, event_i_mapt& succ_map, partial_order_concurrencyt::event_it e_it, partial_order_concurrencyt::event_it e_it2)
{
  auto& succ_set1 = succ_map[e_it];

  if(succ_set1.find(e_it2) != succ_set1.end()) //already reachable
    return true;
  auto& pred_set1 = pred_map[e_it];
  auto& succ_set2 = succ_map[e_it2];

  for(auto pred: pred_set1)
  {
    auto& pred_succ = succ_map[pred];
    for(auto add: succ_set2)
      pred_succ.insert(add);
  }

  for(auto succ: succ_set2)
  {
    auto succ_pred = pred_map[succ];
    for(auto add: pred_set1)
      succ_pred.insert(add);
  }

  return false;
}

// __FHY_ADD_BEGIN__
bool is_redundant(event_i_mapt& succ_map, partial_order_concurrencyt::event_it e_it, partial_order_concurrencyt::event_it e_it2)
{
    auto & succ_set1 = succ_map[e_it];
    auto & succ_set2 = succ_map[e_it2];
    if(succ_set1.find(e_it2) != succ_set1.end())
        return true;

    for(auto succ : succ_set2)
    {
        succ_set1.insert(succ);
    }
    return false;
}
// __FHY_ADD_END__

#if TRADITIONAL
void memory_model_tsot::program_order(
  symex_target_equationt &equation)
{
  per_thread_mapt per_thread_map;
  build_per_thread_map(equation, per_thread_map);

  thread_spawn(equation, per_thread_map);
  
  // iterate over threads

  for(per_thread_mapt::const_iterator
      t_it=per_thread_map.begin();
      t_it!=per_thread_map.end();
      t_it++)
  {
    const event_listt &events=t_it->second;
    
    // iterate over relevant events in the thread
    
    for(event_listt::const_iterator
        e_it=events.begin();
        e_it!=events.end();
        e_it++)
    {
      if(is_memory_barrier(*e_it))
        continue;

      event_listt::const_iterator next=e_it;
      ++next;

      exprt mb_guard_r, mb_guard_w;
      mb_guard_r.make_false();
      mb_guard_w.make_false();

      for(event_listt::const_iterator
          e_it2=next;
          e_it2!=events.end();
          e_it2++)
      {
        if((is_spawn(*e_it) && !is_memory_barrier(*e_it2)) ||
           is_spawn(*e_it2))
        {
          add_constraint(
            equation,
            before(*e_it, *e_it2),
            "po",
            (*e_it)->source);
          
          std::string e1_str = id2string(id(*e_it));
          std::string e2_str = id2string(id(*e_it2));
          equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));

          if(is_spawn(*e_it2))
            break;
          else
            continue;
        }

        if(is_memory_barrier(*e_it2))
        {
          const codet &code=to_code((*e_it2)->source.pc->code);

          if(is_shared_read(*e_it) &&
             !code.get_bool(ID_RRfence) &&
             !code.get_bool(ID_RWfence))
            continue;
          else if(is_shared_write(*e_it) &&
             !code.get_bool(ID_WRfence) &&
             !code.get_bool(ID_WWfence))
            continue;

          if(code.get_bool(ID_RRfence) ||
             code.get_bool(ID_WRfence))
            mb_guard_r=or_exprt(mb_guard_r, (*e_it2)->guard);

          if(code.get_bool(ID_RWfence) ||
             code.get_bool(ID_WWfence))
            mb_guard_w=or_exprt(mb_guard_w, (*e_it2)->guard);

          continue;
        }

        exprt cond=true_exprt();
        exprt ordering=nil_exprt();

        if(address(*e_it)==address(*e_it2))
        {
          ordering=partial_order_concurrencyt::before(
            *e_it, *e_it2, AX_SC_PER_LOCATION);
        }
        else if(program_order_is_relaxed(*e_it, *e_it2))
        {
          if(is_shared_read(*e_it2))
            cond=mb_guard_r;
          else
            cond=mb_guard_w;

          simplify(cond, ns);
        }

        if(!cond.is_false())
        {
          if(ordering.is_nil())
            ordering=partial_order_concurrencyt::before(
              *e_it, *e_it2, AX_PROPAGATION);

          add_constraint(
            equation,
            implies_exprt(cond, ordering),
            "po",
            (*e_it)->source);

          std::string e1_str = id2string(id(*e_it));
          std::string e2_str = id2string(id(*e_it2));
          if((*e_it)->atomic_section_id!=0 && (*e_it)->atomic_section_id == (*e_it2)->atomic_section_id)
            equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "epo")));
          else
            equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
        }
      }
    }
  }
}
#elif ALL_PPO
void memory_model_tsot::program_order(symex_target_equationt &equation)
{
  per_thread_mapt per_thread_map;

  // build_per_thread_map
  build_per_thread_map(equation, per_thread_map);

  thread_spawn(equation, per_thread_map);

  // auxiliary variables to analysis program orders
  bool join_flag = false;
  std::vector<int> unhandled;
  std::map<int, event_it> join_nodes;
  int thr_n = 0;
  int num = 0;

  // iterate over threads
  for(per_thread_mapt::const_iterator t_it=per_thread_map.begin(); t_it!=per_thread_map.end(); t_it++)
  {
      //std::cout << "======== begin thread " << num << "===========\n";
      const event_listt &events=t_it->second;

      bool array_assign = false;
      bool struct_assign = false;

      event_it array_event;
      event_it struct_event;
      event_listt::const_iterator join_node;

      std::map<irep_idt, event_it> event_value_map;
      unhandled.clear();
      join_flag = false;

      //// __FHY_ADD_BEGIN_210414__
      bool start_flag = false;
      bool atomic_flag = false;
      //// __FHY_ADD_END_210414__

      // __SZH_ADD_BEGIN__
//      event_i_mapt pred_map;
      event_i_mapt succ_map;

      for(auto event : events)
      {
//          pred_map[event] = std::set<partial_order_concurrencyt::event_it>({event});
          succ_map[event] = std::set<partial_order_concurrencyt::event_it>({event});
      }
      // __SZH_ADD_END__

      for(auto e_it = events.end()-1; e_it != events.begin()-1; e_it--)
      {
          if((*e_it)->is_thread_join())
          {
              //join_flag = true;

              if(equation.array_thread_id)
                  unhandled.push_back(++thr_n);
              else{
                  int thr= equation.thread_id_map[(*e_it)->pthread_join_id];
                  unhandled.push_back(thr);
              }

              //// __FHY_ADD_BEGIN_210409__
              join_node = e_it;
              join_node++;
              for(; join_node != events.end(); join_node++)
              {
                  if(((*join_node)->is_shared_read() || (*join_node)->is_shared_write())
                        && (!equation.aux_enable || !(*join_node)->is_aux_var()))
                  {
                      break;
                  }
              }
              //std::cout << (join_node == events.end()) << "\n";
              if(join_node != events.end())
              {
                  join_flag = true;
              }
              //// __FHY_ADD_END_210409__
              continue;
          }

          if((*e_it)->is_memory_barrier())
              continue;

          //// __FHY_ADD_BEGIN_210414__
          if((*e_it)->is_verify_atomic_end(equation.mutex_flag))
          {
              if(valid_mutex(equation))
              {
                  atomic_flag = true;
              }
              continue;
            
            // todo: this appears when merging sat_closure into z3_final, where it should be?
            // add_constraint(equation, before(*e_it, *e_it2), "po", (*e_it)->source);
            // std::string e1_str = id2string(id(*e_it));
            // std::string e2_str = id2string(id(*e_it2));
            // equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
            // equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
          }

          if((*e_it)->is_verify_atomic_begin(equation.mutex_flag))
          {
              atomic_flag = false;
              start_flag = false;
              continue;
          }
          //// __FHY_ADD_END_210414__

          if(join_flag &&(equation.thread_malloc || !equation.aux_enable || !(*e_it)->is_aux_var()))
          {
              join_flag = false;
              for(int i : unhandled)
              {
                  //// __FHY_ADD_BEGIN_210409__
                  join_nodes[i] = (*join_node);
                  //// __FHY_ADD_END_210409__
              }
          }

          if((*e_it)->array_assign)
          {
              array_assign = true;
              array_event = *e_it;
          }

          if((*e_it)->is_shared_read() && (*e_it)->struct_assign)
          {
              struct_assign = true;
              struct_event = *e_it;
          }

          auto next = e_it;
          ++next;

          exprt mb_guard_r, mb_guard_w;
          mb_guard_r.make_false();
          mb_guard_w.make_false();

          for(auto e_it2 = next; e_it2 != events.end(); e_it2++)
          {
              if((*e_it2)->is_thread_join())
                  continue;

              if((*e_it2)->is_verify_atomic_end(equation.mutex_flag) ||
                 (*e_it2)->is_verify_atomic_begin(equation.mutex_flag)  )
              {
                  start_flag = false;
                  continue;
              }

              if((is_spawn(*e_it) && !is_memory_barrier(*e_it2))|| is_spawn(*e_it2))
              {
                  std::string e1_str = id2string(id(*e_it));
                  std::string e2_str = id2string(id(*e_it2));

//                  if(!is_redundant(pred_map, succ_map, *e_it, *e_it2))
                  if(!is_redundant(succ_map, *e_it, *e_it2))
                  {
//                      std::cout << "TSO PO: " << "e1: " << e1_str << "  e2: " << e2_str <<"\n";
                      add_constraint(equation, before(*e_it, *e_it2), "po", (*e_it)->source);
                      equation.oclt_type_table.insert(
                              std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                      equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                  }

                  if(is_spawn(*e_it2))
                      break;
                  else
                      continue;
              }

              if(is_memory_barrier(*e_it2))
              {
                  const codet &code=to_code((*e_it2)->source.pc->code);

                  if(is_shared_read(*e_it) && !code.get_bool(ID_RRfence) && !code.get_bool(ID_RWfence))
                      continue;
                  else if(is_shared_write(*e_it) && !code.get_bool(ID_WRfence) && !code.get_bool(ID_WWfence))
                      continue;

                  if(code.get_bool(ID_RRfence) || code.get_bool(ID_WRfence))
                      mb_guard_r=or_exprt(mb_guard_r, (*e_it2)->guard);

                  if(code.get_bool(ID_RWfence) || code.get_bool(ID_WWfence))
                      mb_guard_w=or_exprt(mb_guard_w, (*e_it2)->guard);

                  continue;
              }

              exprt cond=true_exprt();
              exprt ordering=nil_exprt();

              if(address(*e_it)==address(*e_it2))
              {
                  ordering=partial_order_concurrencyt::before(*e_it, *e_it2, AX_SC_PER_LOCATION);
              }
              else if(program_order_is_relaxed(*e_it, *e_it2))
              {
//                  std::string e1_str = id2string(id(*e_it));
//                  std::string e2_str = id2string(id(*e_it2));
//                  std::cout << "Relaxed-PO: " << e1_str << " : " << e2_str << " --- "
//                            << (*e_it)->atomic_section_id << " : " << (*e_it2)->atomic_section_id << "\n";

                  if(is_shared_read(*e_it2))
                      cond=mb_guard_r;
                  else
                      cond=mb_guard_w;

                  simplify(cond, ns);
              }

              if(!cond.is_false())
              {
                  if(ordering.is_nil())
                  {
                      ordering=partial_order_concurrencyt::before(*e_it, *e_it2, AX_PROPAGATION);
                  }

                  std::string e1_str = id2string(id(*e_it));
                  std::string e2_str = id2string(id(*e_it2));

//                  if(!is_redundant(pred_map, succ_map, *e_it, *e_it2))
                  if(!is_redundant(succ_map, *e_it, *e_it2))
                  {
                      if(((*e_it)->atomic_section_id!=0 &&
                         (*e_it)->atomic_section_id == (*e_it2)->atomic_section_id) || array_assign || start_flag)
                      {
//                          std::cout<<"EPO1: ("<<e1_str<<", "<<e2_str<<") \n";
//                          std::cout <<"atomic_id: "<<(*e_it)->atomic_section_id<<" : "<<(*e_it2)->atomic_section_id<<"\n";

                          equation.oclt_type_table.insert(
                                  std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "epo")));

                          equation.oclt_type_table_graph.push_back(
                                  oclt_table_entryt(e1_str, e2_str, "epo", "", true_exprt()));
                      }
                      else
                      {
//                          std::cout<<"PO1: ("<<e1_str<<", "<<e2_str<<") \n";
//                          std::cout <<"atomic_id: "<<(*e_it)->atomic_section_id<<" : "<<(*e_it2)->atomic_section_id<<"\n";
                          equation.oclt_type_table.insert(
                                  std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));

                          equation.oclt_type_table_graph.push_back(
                                  oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                      }
                      add_constraint(equation, implies_exprt(cond, ordering),"po", (*e_it)->source);
                  }
              }

              if(array_assign &&
                 array_event->original_lhs_object.get_identifier() == (*e_it2)->original_lhs_object.get_identifier())
              {
                  array_assign = false;
              }
              if (struct_assign &&
                  struct_event->original_lhs_object.get_identifier() == (*e_it2)->original_lhs_object.get_identifier())
              {
                  struct_assign = false;
              }
          }
          if(atomic_flag && !start_flag)
          {
              start_flag = true;
          }
      }
  }

  if(!join_nodes.empty())
  {
      for(per_thread_mapt::const_iterator t_it =per_thread_map.begin(); t_it !=per_thread_map.end(); t_it++)
      {
          const event_listt  &events = t_it->second;
          if(!events.empty())
          {
              event_listt ::const_reverse_iterator  e_it = events.rbegin();
              for(; e_it != events.rend(); e_it++)
              {
                  if (((*e_it)->is_shared_read() || (*e_it)->is_shared_write())
                      && (equation.thread_malloc || !equation.aux_enable || !(*e_it)->is_aux_var()))
                  {
                      break;
                  }
              }

              if (e_it != events.rend() && join_nodes.find((*e_it)->source.thread_nr) != join_nodes.end())
              {
                  auto e2 = join_nodes[(*e_it)->source.thread_nr];
//                  std::cout<<"PO2: ("<<(*e_it)->ssa_lhs.get_identifier()<<", "<<e2->ssa_lhs.get_identifier()<< ") \n";
                  add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                  std::string e1_str = id2string(id(*e_it));
                  std::string e2_str = id2string(id((e2)));

                  equation.oclt_type_table.insert(
                          std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                  equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
              }
              else if (e_it != events.rend() && (*e_it)->is_thread_exited_write() &&
                            join_nodes.find((*e_it)->source.thread_nr+1) != join_nodes.end())
              {
                  auto e2 = join_nodes[(*e_it)->source.thread_nr+1];
//					std::cout<<"PO2: ("<<(*e_it)->ssa_lhs.get_identifier()<<", "<<e2->ssa_lhs.get_identifier()<< ") \n";
                  add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                  std::string e1_str = id2string(id(*e_it));
                  std::string e2_str = id2string(id((e2)));
                  equation.oclt_type_table.insert(
                          std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                  equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
              }
          }
      }
  }
}
#elif PARTIAL_PPO
void memory_model_tsot::program_order(symex_target_equationt &equation)
{
    std::vector<event_listt> po_orders;
    per_thread_mapt per_thread_map;

    // build_per_thread_map
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        // concurreny-related?
        if(!e_it->is_shared_read() &&
           !e_it->is_shared_write() &&
           !e_it->is_spawn() &&
           !e_it->is_memory_barrier() &&
           !e_it->is_verify_atomic_begin(equation.mutex_flag) &&
           !e_it->is_verify_atomic_end(equation.mutex_flag) &&
           !e_it->is_thread_join())
            continue;

        if (!e_it->is_verify_lock(equation.mutex_flag) && !e_it->is_verify_unlock(equation.mutex_flag) &&
            !e_it->is_thread_join() && e_it->guard_literal.is_true())
            continue;

        if (!equation.thread_malloc && equation.aux_enable && e_it->is_aux_var())
            continue;

        //// __FHY_ADD_BEGIN__
        if(e_it->is_redundant_var())
            continue;
        //// __FHY_ADD_END__

        per_thread_map[e_it->source.thread_nr].push_back(e_it);
    }

    thread_spawn(equation, per_thread_map);

    bool join_flag = false;
    std::vector<int> unhandled;
    std::map<int, event_it> join_nodes;
    int thr_n = 0;
    int num = 0;

    // iterate over threads
    for(per_thread_mapt::const_iterator t_it=per_thread_map.begin(); t_it!=per_thread_map.end(); t_it++)
    {
//		std::cout << "======== begin thread " << num << "===========\n";
        const event_listt &events=t_it->second;

        bool atomic_flag = false;
        //// __FHY_ADD_BEGIN_210412__
        int start_flag = 0;
        //// __FHY_ADD_END_210412__
        bool lock_flag = false;
        bool array_assign = false;
        bool struct_assign = false;

        event_it array_event;
        event_it struct_event;

        std::map<irep_idt, event_it> event_value_map;
        unhandled.clear();
        join_flag = false;

        // iterate over relevant events in the thread
        event_it previous=equation.SSA_steps.end();
        exprt mb_guard_r, mb_guard_w;
        mb_guard_r.make_false();
        mb_guard_w.make_false();

        for(auto event : events)
        {
            if(event->is_thread_join())
            {
                join_flag = true;
                if (equation.array_thread_id)
                    unhandled.push_back(++thr_n);
                else {
                    int thr = equation.thread_id_map[event->pthread_join_id];
                    unhandled.push_back(thr);
                }
                continue;
            }

            if (event->is_verify_atomic_begin(equation.mutex_flag))
            {
                if (valid_mutex(equation))
                {
                    atomic_flag = true;
                    if (event->is_verify_lock(equation.mutex_flag))
                    {
                        event_value_map.clear();
                        lock_flag = true;
                    }
                }
                continue;
            }

            if (event->is_verify_atomic_end(equation.mutex_flag))
            {
                atomic_flag = false;
                start_flag--;
                if (event->is_verify_unlock(equation.mutex_flag))
                {
                    event_value_map.clear();
                    lock_flag = false;
                }
                continue;
            }

            if (join_flag && (equation.thread_malloc || !equation.aux_enable || !event->is_aux_var()))
            {
                join_flag = false;
                for (int i : unhandled)
                {
                    join_nodes[i] = event;
                }
                unhandled.clear();
            }

            if (lock_flag) {
                irep_idt address = event->original_lhs_object.get_identifier();
                if (event->is_shared_read()) {
                    if (event_value_map.find(address) == event_value_map.end()) {
                        event_value_map[address] = event;
                    }
                    else {
                        continue;
                    }
                }
                else if (event->is_shared_write()) {
                    event_value_map[address] = event;
                }
            }

            if(event->is_memory_barrier())
            {
                assert(previous != equation.SSA_steps.end());
                const codet & code = to_code(event->source.pc->code);
                if(is_shared_read(previous) && !code.get_bool(ID_RRfence) && !code.get_bool(ID_RWfence))
                    continue;
                if(is_shared_write(previous) && !code.get_bool(ID_WRfence) && !code.get_bool(ID_WWfence))
                    continue;
                if(code.get_bool(ID_RRfence) || code.get_bool(ID_WRfence))
                    mb_guard_r = event->guard;
                if(code.get_bool(ID_RWfence) || code.get_bool(ID_WWfence))
                    mb_guard_w = event->guard;
                continue;
            }

            if((is_spawn(previous) && !is_memory_barrier(event)) || is_spawn(event))
            {
                assert(previous != equation.SSA_steps.end());
                std::string e1_str = id2string(id(previous));
                std::string e2_str = id2string(id(event));

//                std::cout << "Spawn: e1: " << e1_str << "  e2: " << e2_str << "\n";
                add_constraint(equation,before(previous, event),"po", previous->source);

                equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "epo")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "epo", "", true_exprt()));

                previous = event;
                continue;

            }


            if(previous==equation.SSA_steps.end())
            {
                // first one?
                previous=event;
                //// __FHY_ADD_BEGIN_210412__
                if (atomic_flag)
                {
                    atomic_flag = false;
                    start_flag++;
                }
                //// __FHY_ADD_END_210412__
                continue;

                // todo: this appears when merging sat_closure into z3_final, where it should be?
                // equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                // equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
            }

            if (previous->array_assign)
            {
                array_assign = true;
                array_event = previous;
            }

            if(previous->is_shared_read() && previous->struct_assign)
            {
                struct_assign = true;
                struct_event = previous;
            }

            exprt cond = true_exprt();
            exprt ordering = nil_exprt();

            if(address(previous) == address(event))
            {
                ordering = partial_order_concurrencyt::before(previous, event, AX_SC_PER_LOCATION);
            }
            else if(program_order_is_relaxed(previous, event))
            {
                if(is_shared_read(event))
                    cond = mb_guard_r;
                else
                    cond = mb_guard_w;
                simplify(cond, ns);
            }

            if(ordering.is_nil())
                ordering = before(previous, event);

            std::string e1_str = id2string(id(previous));
            std::string e2_str = id2string(id(event));

            ////  Origin if expression has " || array_assign " but we remove that because nondet-array-false.i
            if((start_flag>0) || (previous->atomic_section_id!=0 && previous->atomic_section_id
                == event->atomic_section_id) || array_assign)
            {
//                std::cout<<"EPO1: ("<<e1_str<<", "<<e2_str<<") \n";
//                std::cout <<"atomic_id: "<<previous->atomic_section_id<<" : "<<event->atomic_section_id<<"\n";

                add_constraint(equation, implies_exprt(cond, ordering), "po", event->source);
                equation.oclt_type_table.insert(
                        std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "epo")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "epo", "", true_exprt()));
            }
            // cond -> ppo
            else if(!cond.is_false())
            {
//                std::cout<<"PO1: ("<<e1_str<<", "<<e2_str<<") \n";
//                std::cout <<"atomic_id: "<<previous->atomic_section_id<<" : "<<event->atomic_section_id<<"\n";
                add_constraint(equation, implies_exprt(cond, ordering), "po", event->source);
                equation.oclt_type_table.insert(
                        std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
            }
            // relaxed po
            else{
//                std::cout<<"Relaxed PO1: ("<<e1_str<<", "<<e2_str<<") \n";
//                std::cout <<"atomic_id: "<<previous->atomic_section_id<<" : "<<event->atomic_section_id<<"\n";
                add_constraint(equation, ordering, "relaxed-po", event->source);
                equation.oclt_type_table.insert(
                        std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "relaxed-po")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "relaxed-po", "", true_exprt()));
            }

            if (array_assign &&
                array_event->original_lhs_object.get_identifier() == event->original_lhs_object.get_identifier())
            {
                array_assign = false;
            }

            if (struct_assign
                && struct_event->original_lhs_object.get_identifier() == event->original_lhs_object.get_identifier())
            {
                struct_assign = false;
            }
            //// __FHY_ADD_BEGIN_210412__
            if (atomic_flag)
            {
                atomic_flag = false;
                start_flag++;
            }
            //// __FHY_ADD_END_210412__

            previous=event;
            mb_guard_w.make_false();
            mb_guard_r.make_false();
        }
//		std::cout << "======== end thread " << num++ << "===========\n";
    }

    if (!join_nodes.empty())
    {
        for(per_thread_mapt::const_iterator t_it=per_thread_map.begin(); t_it!=per_thread_map.end(); t_it++)
        {
            const event_listt &events=t_it->second;
            if (!events.empty()) {
                event_listt::const_reverse_iterator e_it = events.rbegin();
                for (; e_it != events.rend(); e_it++)
                {
                    if (((*e_it)->is_shared_read() || (*e_it)->is_shared_write())
                        && (equation.thread_malloc || !equation.aux_enable || !(*e_it)->is_aux_var()))
                    {
                        break;
                    }
                }

                if (e_it != events.rend() && join_nodes.find((*e_it)->source.thread_nr) != join_nodes.end()) {
                    auto e2 = join_nodes[(*e_it)->source.thread_nr];
//					std::cout<<"PO2: ("<<(*e_it)->ssa_lhs.get_identifier()<<", "<<e2->ssa_lhs.get_identifier()<< ") \n";
                    add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                    std::string e1_str = id2string(id(*e_it));
                    std::string e2_str = id2string(id((e2)));
                    equation.oclt_type_table.insert(
                            std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                    equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                }
                else if (e_it != events.rend() && (*e_it)->is_thread_exited_write() &&
                        join_nodes.find((*e_it)->source.thread_nr+1) != join_nodes.end()) {

                    auto e2 = join_nodes[(*e_it)->source.thread_nr+1];
//					std::cout<<"PO2: ("<<(*e_it)->ssa_lhs.get_identifier()<<", "<<e2->ssa_lhs.get_identifier()<< ") \n";
                    add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                    std::string e1_str = id2string(id(*e_it));
                    std::string e2_str = id2string(id((e2)));
                    equation.oclt_type_table.insert(
                            std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                    equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                }
            }
        }
    }
}
// __FHY_ADD_END__
#endif
