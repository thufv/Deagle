/*******************************************************************\

Module: Memory model for partial order concurrency

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#include <util/std_expr.h>
#include <util/i2string.h>

#include "memory_model_sc.h"
#include <iostream>

/*******************************************************************\

Function: memory_model_sct::operator()

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void memory_model_sct::operator()(symex_target_equationt &equation)
{
    // __SZH_ADD_BEGIN__: an ugly patch to make events in an atomic block in the same thread
    bool in_atomic = false;
    int thread_nr = -1;
    for(eventst::iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(e_it->is_atomic_begin())
        {
            in_atomic = true;
        }
        if(e_it->is_atomic_end())
        {
            in_atomic = false;
            thread_nr = -1;
        }
        if(in_atomic && (e_it->is_shared_read() || e_it->is_shared_write()))
        {
            if(thread_nr == -1)
            {
                thread_nr = e_it->source.thread_nr;
            }
            else
            {
                if(thread_nr != e_it->source.thread_nr)
                {
                    std::cout << "ERROR: wrong thread_nr\n";
                    e_it->source.thread_nr = thread_nr;
                }
            }
        }
    }
    // __SZH_ADD_END__

    print(8, "Adding SC constraints");

    build_event_lists(equation);

//    if (equation.exceed_events_limit())
//        return;

    build_clock_type(equation);
    set_events_ssa_id(equation);

    //// __FHY_ADD_BEGIN__
    choice_symbols.clear();

    read_from(equation);
    std::cout << equation.SSA_steps.size() << " steps after addressing read_from relations:" << "\n";
    //  read_from_backup(equation); std::cout << equation.SSA_steps.size() << " steps" << "\n";

//  if(equation.order_flag)
//	program_order(equation);

    if(solve_method == solve_method_smt2)
        write_serialization_external(equation);
    //std::cout << equation.SSA_steps.size() << " steps after addressing write sequences relations: " << "\n";

#if front_deduce_all_fr
        //from_read(equation);
#endif

    program_order(equation);
    std::cout << equation.SSA_steps.size() << " steps after addressing program orders: :" << "\n";

//	std::cout << "===================OCLT Records: "<< equation.oclt_type_table.size()<<" ===================\n";
//	for(const auto& it : equation.oclt_type_table){
//		std::cout<< "oclt record: " << it.first.first << "  " <<
//				 it.first.second << "  " << it.second.first << "  " <<
//				 it.second.second << "\n";
//	}
    //// __FHY_ADD_END__
    // __SZH_ADD_BEGIN__

    std::set<irep_idt> left_set;

    for(auto& step: equation.SSA_steps)
    {
        if(step.type == goto_trace_stept::ASSIGNMENT)
            left_set.insert(step.ssa_lhs.get_identifier());
    }

    for(auto& step: equation.SSA_steps)
    {
        if(step.type == goto_trace_stept::ASSIGNMENT)
        {
            if(step.ssa_rhs.id() == ID_symbol)
            {
                auto right = to_symbol_expr(step.ssa_rhs);
                auto identifier = right.get_identifier();
                auto str = id2string(identifier);
                if (left_set.find(identifier) == left_set.end() && str.find("main") != std::string::npos &&
                    str.find("arg") != std::string::npos)
                {
                    left_set.insert(identifier);
                    std::cout << "set " << str << " to zero\n";

                    auto constant_zero = constant_exprt(step.ssa_lhs.type());
                    constant_zero.set_value(integer2binary(0, 32));
                    auto equal_zero = equal_exprt(step.ssa_rhs, constant_zero);
                    std::cout << equation.SSA_steps.size() << " steps between fhy's idea\n";
                    add_constraint(equation, equal_zero, "fhy's idea", step.source);
                    std::cout << equation.SSA_steps.size() << " steps after fhy's idea\n";
                }
            }
        }
    }

    // __SZH_ADD_END__

}

/*******************************************************************\

Function: memory_model_sct::before

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

exprt memory_model_sct::before(event_it e1, event_it e2)
{
    return partial_order_concurrencyt::before(
            e1, e2, AX_PROPAGATION);
}

symbol_exprt memory_model_sct::clock(event_it e1)
{
    return partial_order_concurrencyt::clock(e1, AX_PROPAGATION);
}

/*******************************************************************\

Function: memory_model_sct::program_order_is_relaxed

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool memory_model_sct::program_order_is_relaxed(
        partial_order_concurrencyt::event_it e1,
        partial_order_concurrencyt::event_it e2) const
{
    assert(is_shared_read(e1) || is_shared_write(e1));
    assert(is_shared_read(e2) || is_shared_write(e2));

    return false;
}

/*******************************************************************\

Function: memory_model_sct::build_per_thread_map

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

#if TRADITIONAL
void memory_model_sct::build_per_thread_map( const symex_target_equationt &equation, per_thread_mapt &dest) const
{
    // this orders the events within a thread

    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        // concurreny-related?
        if(!e_it->is_shared_read() &&
           !e_it->is_shared_write() &&
           !e_it->is_spawn() &&
           !e_it->is_memory_barrier())
            continue;

        dest[e_it->source.thread_nr].push_back(e_it);
    }
}
#else
void memory_model_sct::build_per_thread_map( const symex_target_equationt &equation, per_thread_mapt &dest) const
{
    // this orders the events within a thread

    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        // concurreny-related?
        if(!e_it->is_shared_read() &&
           !e_it->is_shared_write() &&
           !e_it->is_spawn() &&
           !e_it->is_memory_barrier() &&
           !e_it->is_thread_join())
            continue;

        //// __FHY_ADD_BEGIN_210409__
        /*
         * Here auxiliary variables cannot be simplified just as SC.
         * Because some auxiliary variables act as pthread_join()
         */
//        if (!equation.thread_malloc && equation.aux_enable && e_it->is_aux_var())
//            continue;

        if(e_it->is_redundant_var())
            continue;
        //// __FHY_ADD_END_210409__

        dest[e_it->source.thread_nr].push_back(e_it);
    }
}
#endif

/*******************************************************************\

Function: memory_model_sct::thread_spawn

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

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
            if(next_thread==per_thread_map.end()) continue;

            // For SC and several weaker memory models a memory barrier
            // at the beginning of a thread can simply be ignored, because
            // we enforce program order in the thread-spawn constraint
            // anyway. Memory models with cumulative memory barriers
            // require explicit handling of these.
            event_listt::const_iterator n_it=next_thread->second.begin();

            for( ;n_it!=next_thread->second.end() &&
                  !(*n_it)->is_shared_read() &&
                  !(*n_it)->is_shared_write();
                  ++n_it);

            if(n_it!=next_thread->second.end())
            {
                auto n = *n_it;

                if(solve_method == solve_method_smt2)
                    add_constraint(equation, before(e_it, *n_it), "thread-spawn", e_it->source);

                std::string e1_str = id2string(e_it->ssa_lhs.get_identifier());
                std::string e2_str = id2string((*n_it)->ssa_lhs.get_identifier());

                if(e1_str == "")
                    e1_str = fill_spawn(e_it);

                if(e2_str == "")
                    e2_str = fill_spawn(n);

                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));

//				std::cout << "SPAWN PO: (" << e1_str
//						  << ", " << e2_str  << ")" << "\n";
            }
        }
    }
}

std::string memory_model_sct::fill_spawn(event_it& event)
{
    return "t"+i2string(event->source.thread_nr+1)+"$"+ i2string(numbering[event])+"$spwnclk";
}

/*******************************************************************\

Function: memory_model_sct::program_order

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void memory_model_sct::program_order(symex_target_equationt &equation)
{
    std::vector<event_listt> po_orders;
    per_thread_mapt per_thread_map;

    // build_per_thread_map
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        // __SZH_ADD_BEGIN__: inspired by FHY
//        std::string temp_str = id2string(e_it->ssa_lhs.get_identifier());
//        if(temp_str.find("main") != std::string::npos && temp_str.find("arg") != std::string::npos)
//        {
//            std::cout << "set " << temp_str << " to zero\n";
//
//            auto constant_zero = constant_exprt(e_it->ssa_lhs.type());
//            constant_zero.set_value(integer2binary(0, 32));
//            auto equal_zero = equal_exprt(e_it->ssa_lhs, constant_zero);
//
//            std::cout << equation.SSA_steps.size() << " steps between fhy's idea\n";
//            add_constraint(equation, equal_zero, "fhy's idea", e_it->source);
//            std::cout << equation.SSA_steps.size() << " steps after fhy's idea\n";
//        }
        // __SZH_ADD_END__

        // concurreny-related?
        if(!e_it->is_shared_read() &&
           !e_it->is_shared_write() &&
           !e_it->is_spawn() &&
           !e_it->is_memory_barrier() &&
           !e_it->is_verify_atomic_begin(equation.mutex_flag) &&
           !e_it->is_verify_atomic_end(equation.mutex_flag) &&
           !e_it->is_thread_join())
            continue;

        // __SZH_ADD_BEGIN__: for sat closure
        if(e_it->is_shared_write())
        {
            std::string temp_str = rw_clock_id(e_it).c_str();

            //abstract
            if(e_it->guard.is_true())
            {
                equation.oclt_node_guard_map.insert(std::make_pair(temp_str, true_exprt()));
            }
            else
            {
                equation.oclt_node_guard_map.insert(std::pair<std::string, exprt>(temp_str, e_it->guard));
            }

            equation.oclt_write_map.insert(e_it);
        }
        // __SZH_ADD_END__

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
    for(per_thread_mapt::const_iterator t_it=per_thread_map.begin();
        t_it!=per_thread_map.end(); t_it++)
    {
        const event_listt &events=t_it->second;
        event_listt temp_list;
        bool atomic_flag = false;
        //// __FHY_ADD_BEGIN_210412__
        int start_flag = 0;
//        bool start_flag = false;
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

            if(event->is_memory_barrier())
                continue;

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
//                start_flag = false;
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

                    // __SZH_ADD_BEGIN__
                    event_value_map[address] = event;
                    // __SZH_ADD_END__

//                    if (event_value_map.find(address) == event_value_map.end()) {
//                        event_value_map[address] = event;
//                    }
//                    else {
//                        continue;
//                    }
                }
                else if (event->is_shared_write()) {
                    event_value_map[address] = event;
                }
            }

            if(previous==equation.SSA_steps.end())
            {
                // first one?
                previous=event;
                //// __FHY_ADD_BEGIN_210412__
//                if(atomic_flag && !start_flag)
//                {
//                    start_flag = true;
//                }
                if (atomic_flag)
                {
                    atomic_flag = false;
                    start_flag++;
                }
                //// __FHY_ADD_END_210412__

                continue;
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

            //// __FHY_ADD_BEGIN__
            temp_list.emplace_back(event);
            if(solve_method == solve_method_smt2)
                add_constraint(equation, before(previous, event), "po", event->source);
            std::string e1_str = id2string(id(previous));
            std::string e2_str = id2string(id(event));

            if(e1_str == "")
                e1_str = fill_spawn(previous);

            if(e2_str == "")
                e2_str = fill_spawn(event);

            //std::cout << "e1: " << e1_str << "  e2: " << e2_str << "---" << previous->atomic_section_id
            //        << " : " << event->atomic_section_id << "\n";
            /*
             *  Origin if expression has " || array_assign " but we remove that because nondet-array-false.i
             */
            if((start_flag) || (previous->atomic_section_id!=0 && previous->atomic_section_id
                == event->atomic_section_id) || array_assign)
            {
                //std::cout<<"EPO1: ("<<e1_str<<", "<<e2_str<<") \n";
                //std::cout <<"atomic_id: "<<previous->atomic_section_id<<" : "<<event->atomic_section_id<<"\n";
                equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "epo")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "epo", "", true_exprt()));
            }
            else
            {
                //std::cout<<"PO1: ("<<e1_str<<", "<<e2_str<<") \n";
                //std::cout <<"atomic_id: "<<previous->atomic_section_id<<" : "<<event->atomic_section_id<<"\n";
                equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
            }
            //// __FHY_ADD_END__

            if (array_assign && array_event->original_lhs_object.get_identifier()
                        == event->original_lhs_object.get_identifier())
            {
                array_assign = false;
            }

            if (struct_assign && struct_event->original_lhs_object.get_identifier()
                        == event->original_lhs_object.get_identifier())
            {
                struct_assign = false;
            }

            //// __FHY_ADD_BEGIN_210412__
//            if(atomic_flag && !start_flag)
//            {
//                start_flag = true;
//            }
            if (atomic_flag)
            {
                atomic_flag = false;
                start_flag++;
            }
            //// __FHY_ADD_END_210412__

            previous=event;
        }
        po_orders.emplace_back(temp_list);
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
                    if(solve_method == solve_method_smt2)
                        add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                    //// __FHY_ADD_BEGIN__
                    std::string e1_str = id2string(id(*e_it));
                    std::string e2_str = id2string(id((e2)));
                    equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                    equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                    //// __FHY_ADD_END__
                }
                else if (e_it != events.rend() && (*e_it)->is_thread_exited_write()
                        && join_nodes.find((*e_it)->source.thread_nr+1) != join_nodes.end()) {

                    auto e2 = join_nodes[(*e_it)->source.thread_nr+1];
//					std::cout<<"PO2: ("<<(*e_it)->ssa_lhs.get_identifier()<<", "<<e2->ssa_lhs.get_identifier()<< ") \n";
                    if(solve_method == solve_method_smt2)
                        add_constraint(equation, before(*e_it, e2), "po", (*e_it)->source);

                    //// __FHY_ADD_BEGIN__
                    std::string e1_str = id2string(id(*e_it));
                    std::string e2_str = id2string(id((e2)));
                    equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "po")));
                    equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "po", "", true_exprt()));
                    //// __FHY_ADD_END__
                }
            }
        }
    }
#if origin_frontend_set_partial_fr
     //ADD POF Relation
    for(const auto & event_list : po_orders)
    {
        for(auto e_it = event_list.begin(); e_it != event_list.end(); ++e_it)
        {
            if(!is_shared_write(*e_it))
                continue;
            auto next = e_it;
            next++;
            bool meet_flag = false;
            const a_rect &rec = address_map[address(*e_it)];

            for(auto e_it2 = next; e_it2!= event_list.end(); ++e_it2)
            {
                if(!is_shared_read(*e_it2))
                    continue;
                for(auto event: rec.reads){
                    if(event->ssa_lhs.get_identifier() == (*e_it2)->ssa_lhs.get_identifier())
                    {
                        meet_flag = true;
                        break;
                    }
                }
                if(meet_flag)
                {
//					std::cout << "POF: (" << (*e_it)->ssa_lhs.get_identifier()
//							  << ", " << (*e_it2)->ssa_lhs.get_identifier() << ") \n";
//					add_constraint(equation, before(*e_it, *e_it2), "pof", (*e_it)->source);
//
//					//// __FHY_ADD_BEGIN__
//					std::string e1_str = id2string(id(*e_it));
//					std::string e2_str = id2string(id(*e_it2));
//					equation.oclt_type_table.insert(
//							std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "pof")));
//					//// __FHY_ADD_END__

                    // Add rf -> ws relation
                    for(const auto& it : choice_symbols)
                    {
                        assert(is_shared_read(it.first.first));
                        if(it.first.first->source.thread_nr == it.first.second->source.thread_nr)
                            continue;
                        if(it.first.first != *e_it2)
                        {
                            continue;
                        }
                        else
                        {
                            const auto &w1 = it.first.second;
                            auto temp_pair = std::make_pair(*e_it, w1);
                            if(wse_symbols.find(temp_pair) != wse_symbols.end())
                            {
                                const auto &ws_lit = wse_symbols[temp_pair];
//								std::cout << "RF -> COE: ";
//								std::cout << "( " << id2string(id(w1)) << " : " << id2string(id(*e_it2)) << ") " << " && ";
//								std::cout << "( " << id2string(id(*e_it)) << " : " << id2string(id(*e_it2)) << ") " << " -> ";
//								std::cout << "( " << id2string(id(*e_it)) << " : " << id2string(id(w1)) << ") \n";
                                add_constraint(equation, or_exprt(not_exprt(it.second), ws_lit), "rf -> ws", (*e_it)->source);
//                                add_constraint(equation, or_exprt(not_exprt(it.second), not_exprt((*e_it)->guard), ws_lit), "rf -> ws", (*e_it)->source);
                            }
                        }
                    }
                    meet_flag = false;
//					break;
                }
            }
        }
    }
#endif
}

/*******************************************************************\

Function: memory_model_sct::write_serialization_external

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void memory_model_sct::write_serialization_external(
        symex_target_equationt &equation)
{
    per_thread_mapt per_thread_map;

    for(address_mapt::const_iterator
                a_it=address_map.begin();
        a_it!=address_map.end();
        a_it++)
    {
        const a_rect &a_rec=a_it->second;

        // This is quadratic in the number of writes
        // per address. Perhaps some better encoding
        // based on 'places'?
        for(event_listt::const_iterator w_it1=a_rec.writes.begin();
            w_it1!=a_rec.writes.end();
            ++w_it1)
        {
            event_listt::const_iterator next=w_it1;
            ++next;

            for(event_listt::const_iterator w_it2=next;
                w_it2!=a_rec.writes.end();
                ++w_it2)
            {
                // external?
                //// __FHY_ADD_BEGIN__
                if((*w_it1)->source.thread_nr == (*w_it2)->source.thread_nr)
                {
#if front_deduce_all_fr
                    continue;
#endif
                    exprt ws1, ws2;

                    std::string e1_str = id2string(id(*w_it1));
                    std::string e2_str = id2string(id((*w_it2)));

                    if(po(*w_it1, *w_it2) && !program_order_is_relaxed(*w_it1, *w_it2))
                    {
#if origin_frontend_set_partial_fr
                        ws1=true_exprt();
                        ws2=false_exprt();

//						//// __FHY_ADD_BEGIN__
//						add_constraint(equation, before(*w_it1, *w_it2), "coi", (*w_it1)->source);
//
//						std::string e1_str = id2string(id(*w_it1));
//						std::string e2_str = id2string(id((*w_it2)));
//
//						equation.oclt_type_table.insert(
//								std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "coi")));
#elif front_deduce_fr_by_coi
                        ws1=true_exprt();
                        ws2=false_exprt();
#else
                        symbol_exprt s = nondet_bool_symbol("coi");
                        wse_symbols[std::make_pair(*w_it1,*w_it2)] = s;
                        std::string ws_lit = s.get_identifier().c_str();

                        add_constraint(equation, implies_exprt(s, before(*w_it1, *w_it2)), "coi", (*w_it1)->source);

                        add_constraint(
                                equation,
                                implies_exprt(s, and_exprt((*w_it1)->guard, (*w_it2)->guard)),
                                "coi",
                                (*w_it1)->source);
                        add_constraint(
                                equation,
                                implies_exprt(and_exprt((*w_it1)->guard, (*w_it2)->guard), s),
                                "",
                                (*w_it1)->source);

						equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(ws_lit, "coi")));
                        equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "coi", ws_lit, s));
#endif
                    }
                    else if(po(*w_it2, *w_it1) && !program_order_is_relaxed(*w_it2, *w_it1))
                    {
#if origin_frontend_set_partial_fr
                        ws1=false_exprt();
                        ws2=true_exprt();

//						//// __FHY_ADD_BEGIN__
//						add_constraint(equation, before(*w_it2, *w_it1), "coi", (*w_it2)->source);
//
//						std::string e1_str = id2string(id(*w_it2));
//						std::string e2_str = id2string(id((*w_it1)));
//
//						equation.oclt_type_table.insert(
//								std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair("", "coi")));
//						//// __FHY_ADD_END__
#elif front_deduce_fr_by_coi
                        ws1=false_exprt();
                        ws2=true_exprt();
#else
                        symbol_exprt s = nondet_bool_symbol("coi");
                        wse_symbols[std::make_pair(*w_it2,*w_it1)] = s;
                        std::string ws_lit = s.get_identifier().c_str();

                        add_constraint(
                                equation,
                                implies_exprt(s, before(*w_it2, *w_it1)),
                                "coi",
                                (*w_it2)->source);

                        add_constraint(
                                equation,
                                implies_exprt(s, and_exprt((*w_it1)->guard, (*w_it2)->guard)),
                                "coi",
                                (*w_it2)->source);
                        add_constraint(
                                equation,
                                implies_exprt(and_exprt((*w_it1)->guard, (*w_it2)->guard), s),
                                "",
                                (*w_it2)->source);

						equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(ws_lit, "coi")));
                        equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "coi", ws_lit, s));
#endif
                    }
                    else
                    {
                        // two write events in a thread must have po relation.
                        // two write events in a thread may have po relation.
                        assert(false);
                    }

#if origin_frontend_set_partial_fr
                    //// smells like cubic
                    for(choice_symbolst::const_iterator c_it=choice_symbols.begin();
                        c_it!=choice_symbols.end(); c_it++)
                    {
                        event_it r=c_it->first.first;
                        exprt rf=c_it->second;
                        exprt cond;
                        cond.make_nil();

                        std::string e1_str;
                        std::string e2_str;
                        if(c_it->first.second==*w_it1 && !ws1.is_false()
                           && (r->atomic_section_id == 0 || r->atomic_section_id != (*w_it2)->atomic_section_id))
                        {
                            exprt fr = before(r, *w_it2);

                            e1_str = id2string(id(r));
                            e2_str = id2string(id(*w_it2));

//                            std::cout << "R: " << e1_str << "  atomic id: " << r->atomic_section_id << "\n";
//                            std::cout << "W: " << e2_str << "  atomic id: " << (*w_it2)->atomic_section_id << "\n\n";

                            // the guard of w_prime follows from rf; with rfi
                            // optimisation such as the previous write_symbol_primed
                            // it would even be wrong to add this guard
                            cond = implies_exprt( and_exprt(r->guard, (*w_it2)->guard, rf), fr);
//                            cond = implies_exprt( and_exprt(rf, (*w_it2)->guard), fr);


//							std::cout << "RF && COI -> FR: ";
//							std::cout << "( " << id2string(id(*w_it1)) << " : " << e1_str << ") " << " && ";
//							std::cout << "( " << id2string(id(*w_it1)) << " : " << e2_str << ") " << " -> ";
//							std::cout << "( " << e1_str << " : " << e2_str << ") \n";
                        }
                        else if(c_it->first.second==*w_it2 && !ws2.is_false()
                                && (r->atomic_section_id == 0 || r->atomic_section_id != (*w_it1)->atomic_section_id))
                        {
                            exprt fr=before(r, *w_it1);

                            e1_str = id2string(id(r));
                            e2_str = id2string(id(*w_it1));

//                            std::cout << "R: " << e1_str << "  atomic id: " << r->atomic_section_id << "\n";
//                            std::cout << "W: " << e2_str << "  atomic id: " << (*w_it1)->atomic_section_id << "\n\n";

                            // the guard of w follows from rf; with rfi
                            // optimisation such as the previous write_symbol_primed
                            // it would even be wrong to add this guard
                            cond = implies_exprt( and_exprt(r->guard, (*w_it1)->guard, rf), fr);
//                            cond = implies_exprt(and_exprt(rf, (*w_it1)->guard), fr);

//							std::cout << "RF && COI -> FR: ";
//							std::cout << "( " << id2string(id(*w_it2)) << " : " << e1_str << ") " << " && ";
//							std::cout << "( " << id2string(id(*w_it2)) << " : " << e2_str << ") " << " -> ";
//							std::cout << "( " << e1_str << " : " << e2_str << ") \n";
                        }

                        if(cond.is_not_nil())
                        {
                            std::string rf_lit = from_expr(rf);
                            equation.oclt_type_table.insert(
                                    std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(rf_lit, "fr")));
                            add_constraint(equation, cond, "fr", r->source);
                        }

                    }
#elif front_deduce_fr_by_coi
                    //// smells like cubic
                    for(choice_symbolst::const_iterator c_it=choice_symbols.begin();
                        c_it!=choice_symbols.end(); c_it++)
                    {
                        event_it r=c_it->first.first;
                        exprt rf=c_it->second;
                        exprt cond;
                        cond.make_nil();

                        std::string e1_str;
                        std::string e2_str;
                        if(c_it->first.second==*w_it1 && !ws1.is_false()
                           && (r->atomic_section_id == 0 || r->atomic_section_id != (*w_it2)->atomic_section_id))
                        {
                            e1_str = id2string(id(r));
                            e2_str = id2string(id(*w_it2));

                            symbol_exprt fr = symbol_exprt(nondet_bool_symbol("fr"));
                            std::string fr_lit = fr.get_identifier().c_str();

                            add_constraint(equation,
                                            implies_exprt(fr, before(r, *w_it2)), "fr-order", r->source);
                            equation.oclt_type_table.insert(
                                    std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(fr_lit, "fr")));

                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, (*w_it2)->guard)),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                            implies_exprt(and_exprt(rf, (*w_it2)->guard), fr),
                                            "fr",
                                            r->source);
                        }
                        else if(c_it->first.second==*w_it2 && !ws2.is_false()
                                && (r->atomic_section_id == 0 || r->atomic_section_id != (*w_it1)->atomic_section_id))
                        {
                            e1_str = id2string(id(r));
                            e2_str = id2string(id(*w_it1));

                            symbol_exprt fr = symbol_exprt(nondet_bool_symbol("fr"));
                            std::string fr_lit = fr.get_identifier().c_str();

                            add_constraint(equation,
                                           implies_exprt(fr, before(r, *w_it1)), "fr-order", r->source);
                            equation.oclt_type_table.insert(
                                    std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(fr_lit, "fr")));

                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, (*w_it1)->guard)),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                           implies_exprt(and_exprt(rf, (*w_it1)->guard), fr),
                                           "fr",
                                           r->source);
                        }
                    }
#endif
                    continue;
                }
                //// __FHY_ADD_END__

                // ws is a total order, no two elements have the same rank
                // s -> w_evt1 before w_evt2; !s -> w_evt2 before w_evt1

                //// __FHY_ADD_BEGIN__
#if origin_frontend_set_partial_fr
                std::string e1_str = id2string(id(*w_it1));
                std::string e2_str = id2string(id(*w_it2));

                symbol_exprt s = nondet_bool_symbol("coe");
                std::string ws_lit = s.get_identifier().c_str();
//                wse_symbols[std::make_pair(*w_it1,*w_it2)] = s;
//                wse_symbols[std::make_pair(*w_it2,*w_it1)] = not_exprt(s);
//				std::cout << "COE: (" << (*w_it1)->ssa_lhs.get_identifier()
//						  << ", "<< (*w_it2)->ssa_lhs.get_identifier() << ") \n";


//                std::cout << e1_str << " : " << e2_str;
//                std::cout << " number: " << numbering[*w_it1] << " : " << numbering[*w_it2] << "\n";

                equation.oclt_type_table.insert(
                        std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(ws_lit, "coe")));

                std::string not_ws_lit = "(not " + ws_lit + ")";
                equation.oclt_type_table.insert(
                        std::make_pair(std::make_pair(e2_str, e1_str), std::make_pair(not_ws_lit, "coe")));


                // write-to-write edge
                add_constraint(
                        equation,
                        implies_exprt(s, before(*w_it1, *w_it2)),
                        "ws-ext",
                        (*w_it1)->source);

                add_constraint(
                        equation,
                        implies_exprt(not_exprt(s), before(*w_it2, *w_it1)),
                        "ws-ext",
                        (*w_it1)->source);
#else
                std::string e1_str = id2string(id(*w_it1));
                std::string e2_str = id2string(id(*w_it2));

                symbol_exprt s1 = nondet_bool_symbol("coe");
                std::string ws_lit = s1.get_identifier().c_str();
                wse_symbols[std::make_pair(*w_it1,*w_it2)] = s1;

                equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(ws_lit, "coe")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "coe", ws_lit, s1));

                add_constraint(equation,implies_exprt(s1, before(*w_it1, *w_it2)), "coe", (*w_it1)->source);
                add_constraint(
                        equation,
                        implies_exprt(s1, and_exprt((*w_it1)->guard, (*w_it2)->guard)),
                        "coe",
                        (*w_it1)->source);

                symbol_exprt s2 = nondet_bool_symbol("coe");
                ws_lit = s2.get_identifier().c_str();
                wse_symbols[std::make_pair(*w_it2,*w_it1)] = s2;

                equation.oclt_type_table.insert(std::make_pair(std::make_pair(e2_str, e1_str), std::make_pair(ws_lit, "coe")));
                equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "coe", ws_lit, s2));

                add_constraint(
                        equation,
                        implies_exprt(s2, before(*w_it2, *w_it1)),
                        "coe",
                        (*w_it1)->source);
                add_constraint(
                        equation,
                        implies_exprt(s2, and_exprt((*w_it1)->guard, (*w_it2)->guard)),
                        "coe",
                        (*w_it1)->source);

                add_constraint( equation,
                                implies_exprt(and_exprt((*w_it1)->guard, (*w_it2)->guard), or_exprt(s1, s2)),
                                "",
                                (*w_it1)->source);
                //// __FHY_ADD_END__
#endif
            }
        }
    }
}

/*******************************************************************\

Function: memory_model_sct::from_read

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

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
                    std::string e1_str;
                    std::string e2_str;

                    //// __FHY_ADD_BEGIN__
                    if(c_it->first.second == *w_prime && !ws1.is_false() &&
                       (r->atomic_section_id == 0 || r->atomic_section_id != (*w)->atomic_section_id))
                    {
                        e1_str = id2string(id(r));
                        e2_str = id2string(id(*w));

                        symbol_exprt fr = symbol_exprt(nondet_bool_symbol("fr"));
                        std::string fr_lit = fr.get_identifier().c_str();


                        add_constraint(equation,
                                       implies_exprt(fr, before(r, *w)), "fr-order", r->source);

                        equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(fr_lit, "fr")));
                        equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "fr", fr_lit, fr));

                        if(!ws1.is_true())
                        {
                            exprt ws = wse_symbols.find(std::make_pair(*w_prime, *w))->second;
                            add_constraint(equation,
                                           implies_exprt(and_exprt(rf, ws), fr),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, ws)),
                                           "fr",
                                           r->source);
                        }
                        else
                        {
                            add_constraint(equation,
                                           implies_exprt(and_exprt(rf, (*w)->guard), fr),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, (*w)->guard)),
                                           "fr",
                                           r->source);
                        }
                    }
                    else if(c_it->first.second==*w && !ws2.is_false() &&
                            (r->atomic_section_id == 0 || r->atomic_section_id != (*w_prime)->atomic_section_id))
                    {
                        e1_str = id2string(id(r));
                        e2_str = id2string(id(*w_prime));

                        symbol_exprt fr = symbol_exprt(nondet_bool_symbol("fr"));
                        std::string fr_lit = fr.get_identifier().c_str();


                        add_constraint(equation,
                                       implies_exprt(fr, before(r, *w_prime)), "fr-order", r->source);
                        
                        equation.oclt_type_table.insert(std::make_pair(std::make_pair(e1_str, e2_str), std::make_pair(fr_lit, "fr")));
                        equation.oclt_type_table_graph.push_back(oclt_table_entryt(e1_str, e2_str, "fr", fr_lit, fr));

                        if(!ws2.is_true())
                        {
                            exprt ws = wse_symbols.find(std::make_pair(*w, *w_prime))->second;
                            add_constraint(equation,
                                           implies_exprt(and_exprt(rf, ws), fr),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, ws)),
                                           "fr",
                                           r->source);
                        }
                        else
                        {
                            add_constraint(equation,
                                           implies_exprt(and_exprt(rf, (*w_prime)->guard), fr),
                                           "fr",
                                           r->source);
                            add_constraint(equation,
                                           implies_exprt(fr, and_exprt(rf, (*w_prime)->guard)),
                                           "fr",
                                           r->source);
                        }
                    }
                    //// __FHY_ADD_END__
//                        add_constraint(equation,
//                                       cond, "fr", r->source);
                }
            }
        }
    }
}

void memory_model_sct::get_symbols(const exprt &expr, std::vector<symbol_exprt>& symbols)
{
    forall_operands(it, expr)
            get_symbols(*it, symbols);

    if(expr.id()==ID_symbol)
        symbols.push_back(to_symbol_expr(expr));
}

void memory_model_sct::set_events_ssa_id(symex_target_equationt &equation)
{
    int id = 0;
    for(eventst::iterator
                e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end();
        e_it++)
    {
        if(e_it->is_assignment())
        {
            unsigned event_num = 0;
            std::vector<symbol_exprt> symbols;
            get_symbols(e_it->cond_expr, symbols);
            for (unsigned i = 0; i < symbols.size(); i++)
            {
                unsigned result = set_single_event_ssa_id(equation, symbols[i], id);
                event_num += result;
            }
            if (event_num > 0)
                e_it->event_flag = true;
        }
        id++;
    }
}

unsigned memory_model_sct::set_single_event_ssa_id(symex_target_equationt &equation, symbol_exprt event, int id)
{
    for(eventst::iterator
                e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end();
        e_it++)
    {
        if ((e_it->is_shared_read() || e_it->is_shared_write()))
        {
            if (e_it->ssa_lhs.get_identifier() == event.get_identifier())
            {
                e_it->appear_ssa_id = id;
                return 1;
            }
        }
    }
    return 0;
}


