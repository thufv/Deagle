/*******************************************************************\

Module: Memory models for C11 memory model

(would be later extended to all memory models)

Author: Zhihang Sun, sunzh20@mails.tsinghua.edu.cn

\*******************************************************************/

#include "memory_model_general.h"
#include <util/find_symbols.h>

#include <iostream>

void memory_model_generalt::operator()(symex_target_equationt &equation, message_handlert &message_handler)
{
    messaget log{message_handler};
    log.statistics() << "Adding C11 constraints" << messaget::eom;

    build_event_lists(equation, message_handler);
    build_clock_type();

    equation.cat = cat;
    equation.use_cat = true;

    remove_atomic_typecast(equation);

    build_labels(equation);
    build_loc(equation);
    build_thd(equation);

    build_po(equation);
    build_co(equation);
    build_rf(equation);

    std::set<std::string> reads;
    build_shared_reads(equation, reads);
    build_data(equation, reads);
    build_addr(equation, reads);
    build_ctrl(equation, reads);

    cat.show();

    // std::cout << "memory model is built!\n";
    // equation.output(std::cout);
}

//useless now
exprt memory_model_generalt::before(event_it e1, event_it e2)
{
  return partial_order_concurrencyt::before(
    e1, e2, AX_PROPAGATION);
}

void memory_model_generalt::add_necessary_oc_edge(symex_target_equationt &equation, event_it e1, event_it e2, std::string kind, exprt guard_expr)
{
    add_necessary_oc_edge(equation, get_name(e1), get_name(e2), kind, guard_expr);
}

void memory_model_generalt::add_necessary_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr)
{
    if(cat.binary_relations.find(kind) == cat.binary_relations.end())
        return;
    add_oc_edge(equation, e1_str, e2_str, kind, guard_expr);
}

void memory_model_generalt::add_necessary_oc_label(symex_target_equationt &equation, event_it e, std::string label)
{
    if(cat.unary_relations.find(label) == cat.unary_relations.end())
        return;
    add_oc_label(equation, e, label);
}

/*typedef enum
{
    memory_order_relaxed = 0,
    memory_order_consume = 1,
    memory_order_acquire = 2,
    memory_order_release = 3,
    memory_order_acq_rel = 4,
    memory_order_seq_cst = 5
} memory_order;*/
std::string mos[6] = {"RLX", "CON", "ACQ", "REL", "ACQ_REL", "SC"};

void memory_model_generalt::build_labels(symex_target_equationt& equation)
{
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        std::string function_id = e_it->source.function_id.c_str();
        bool is_init = (function_id == "__CPROVER_initialize");

        if(e_it->is_shared_read())
        {
            add_necessary_oc_label(equation, e_it, "R");
            add_necessary_oc_label(equation, e_it, "M");
            add_necessary_oc_label(equation, e_it, "_");
            if(is_init)
            {
                add_necessary_oc_label(equation, e_it, "IR");
                add_necessary_oc_label(equation, e_it, "IM");
                add_necessary_oc_label(equation, e_it, "I");
            }
        }
        if(e_it->is_shared_write())
        {
            add_necessary_oc_label(equation, e_it, "W");
            add_necessary_oc_label(equation, e_it, "M");
            add_necessary_oc_label(equation, e_it, "_");
            if(is_init)
            {
                add_necessary_oc_label(equation, e_it, "IW");
                add_necessary_oc_label(equation, e_it, "IM");
                add_necessary_oc_label(equation, e_it, "I");
            }
        }
        if(e_it->is_memory_barrier())
        {
            add_necessary_oc_label(equation, e_it, "F");
            add_necessary_oc_label(equation, e_it, "_");

            // Here we model sync/lwsync/isync as fences: Fsync, Flwsync, Fisync.
            // Binary sync relations are modeled as:
            // let sync = [M]; po; [Fsync]; po; [M]
            // let lwsync = ([R]; po; [Flwsync]; po; [M]) | ([M]; po; [Flwsync]; po; [W])
            // let ctrlisync = [R]; ctrl; [Fisync]; po
            // Please manually add those definitions into cat modules.
            std::string function_id = e_it->source.function_id.c_str();
            if(function_id == "fence" || function_id == "sync")
                add_necessary_oc_label(equation, e_it, "Fsync");
            if(function_id == "lwfence" || function_id == "lwsync")
                add_necessary_oc_label(equation, e_it, "Flwsync");
            if(function_id == "isync")
                add_necessary_oc_label(equation, e_it, "Fisync");
        }

        bool is_atomic = e_it->is_memory_barrier();

        if(e_it->is_shared_write())
        {
            std::string function_id = e_it->source.function_id.c_str();
            if( function_id == "__atomic_store" || function_id == "__atomic_store_n")
            {
                add_necessary_oc_label(equation, e_it, "A");
                is_atomic = true;
            }
        }

        if(e_it->is_shared_read())
        {
            std::string function_id = e_it->source.function_id.c_str();
            if(function_id == "__atomic_load" || function_id == "__atomic_load_n")
            {
                add_necessary_oc_label(equation, e_it, "A");
                is_atomic = true;
            }
        }
        
        if(is_atomic)
        {
            for(auto e_it2 = e_it; e_it2 != equation.SSA_steps.begin(); e_it2--)
            {
                if(!e_it2->is_assignment())
                    continue;
                auto lhs = e_it2->ssa_lhs;
                auto rhs = e_it2->ssa_rhs;
                std::string lhs_id = lhs.get_identifier().c_str();
                if(lhs_id.find("::memorder") != std::string::npos && rhs.id() == ID_constant)
                {
                    int mo = string2integer(rhs.get_string(ID_value), 16).to_long();
                    if(mo < 0 || mo >= 6)
                    {
                        std::cout << "ERROR: Unsupported memorder " << mo << "\n";
                        std::exit(255);
                    }
                    add_necessary_oc_label(equation, e_it, mos[mo]);
                    break;
                }
            }
            
        }
    }
}

void memory_model_generalt::build_loc(symex_target_equationt &equation)
{
    per_loc_mapt per_loc_map;
    build_per_loc_map(equation, per_loc_map);

    for(auto& pair: per_loc_map)
    {
        auto& list = pair.second;
        for(auto e_it1 = list.begin(); e_it1 != list.end(); e_it1++)
            for(auto e_it2 = list.begin(); e_it2 != list.end(); e_it2++)
                add_necessary_oc_edge(equation, *e_it1, *e_it2, "loc", true_exprt());
    }
}

void memory_model_generalt::build_thd(symex_target_equationt &equation)
{
    per_thread_mapt per_thread_map;
    build_per_thread_map(equation, per_thread_map);

    for(auto& pair: per_thread_map)
    {
        auto& list = pair.second;
        for(auto e_it = list.begin(); e_it != list.end(); e_it++)
        {
            std::string e_str = id2string(id(*e_it));
            if(e_str == "")
                e_str = fill_name(*e_it);
            equation.oc_thread_map[e_str] = (*e_it)->source.thread_nr;
        }
    }
}

void memory_model_generalt::build_id(symex_target_equationt &equation)
{
    for(auto e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(e_it->is_shared_write() || e_it->is_shared_read() || e_it->is_memory_barrier() || e_it->is_spawn())
            add_necessary_oc_edge(equation, e_it, e_it, "id", true_exprt());
    }
}

void add_po_closure(std::vector<std::string>& event_names, std::map<std::string, int>& event_name_to_id, 
    std::vector<std::set<int>>& outs, std::vector<std::set<int>>& ins, std::string e1, std::string e2)
{
    int e1_id, e2_id;
    if(event_name_to_id.find(e1) != event_name_to_id.end())
        e1_id = event_name_to_id[e1];
    else
    {
        e1_id = event_names.size();
        event_names.push_back(e1);
        event_name_to_id[e1] = e1_id;
        outs.push_back(std::set<int>());
        ins.push_back(std::set<int>());
    }
    if(event_name_to_id.find(e2) != event_name_to_id.end())
        e2_id = event_name_to_id[e2];
    else
    {
        e2_id = event_names.size();
        event_names.push_back(e2);
        event_name_to_id[e2] = e2_id;
        outs.push_back(std::set<int>());
        ins.push_back(std::set<int>());
    }
    std::set<int> e1_ins = ins[e1_id];
    e1_ins.insert(e1_id);
    std::set<int> e2_outs = outs[e2_id];
    e2_outs.insert(e2_id);
    for(auto e1_in : e1_ins)
        for(auto e2_out : e2_outs)
        {
            outs[e1_in].insert(e2_out);
            ins[e2_out].insert(e1_in);
        }
}

void memory_model_generalt::build_po(symex_target_equationt &equation)
{
    std::vector<std::string> event_names;
    std::map<std::string, int> event_name_to_id;
    std::vector<std::set<int>> outs;
    std::vector<std::set<int>> ins;

    per_thread_mapt per_thread_map;
    build_per_thread_map(equation, per_thread_map);

    //thread spawn
    std::vector<event_it> join_events;
    for(auto e_it = equation.SSA_steps.begin(); e_it != equation.SSA_steps.end(); e_it++)
    {
        if(!e_it->is_shared_read())
            continue;
        std::string read_name = id2string(id(e_it));
        if(read_name.find("__CPROVER_threads_exited") == std::string::npos)
            continue;
        
        std::string function_name = e_it->source.function_id.c_str();
        if(function_name.find("pthread_join") == std::string::npos)
            continue;
        
        join_events.push_back(e_it);
    }

    unsigned next_thread_id=0;
    for(auto e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(e_it->is_spawn())
        {
            auto next_thread = per_thread_map.find(++next_thread_id);
            if(next_thread == per_thread_map.end())
                continue;

            // add a constraint for all events,
            // considering regression/cbmc-concurrency/pthread_create_tso1
            for(auto n_it=next_thread->second.begin(); n_it!=next_thread->second.end(); n_it++)
            {
                if(!(*n_it)->is_memory_barrier())
                {
                    add_po_closure(event_names, event_name_to_id, outs, ins, get_name(e_it), get_name(*n_it));
                    // add_necessary_oc_edge(equation, e_it, *n_it, "po", true_exprt());
                    if(is_apo(e_it, *n_it))
                        add_necessary_oc_edge(equation, e_it, *n_it, "rmw", true_exprt());
                }
            }

            int join_id = int(next_thread_id) - 1;
            if(join_id < int(join_events.size()))
            {
                auto join_event = join_events[join_id];
                for(auto n_it=next_thread->second.begin(); n_it!=next_thread->second.end(); n_it++)
                {
                    if(!(*n_it)->is_memory_barrier())
                    {
                        add_po_closure(event_names, event_name_to_id, outs, ins, get_name(*n_it), get_name(join_event));
                        // add_necessary_oc_edge(equation, *n_it, join_event, "po", true_exprt());
                        if(is_apo(e_it, *n_it))
                            add_necessary_oc_edge(equation, *n_it, join_event, "rmw", true_exprt());
                    }
                }
            }

        }
    }

    build_guard_map_all(equation);

    // iterate over threads
    for(auto t_it=per_thread_map.begin(); t_it!=per_thread_map.end(); t_it++)
    {
        const event_listt &events=t_it->second;

        // this lists all program order

        for(auto e1_it = events.begin(); e1_it != events.end(); e1_it++)
            for(auto e2_it = e1_it; e2_it != events.end(); e2_it++)
            {
                if(e1_it == e2_it)
                    continue;
                add_po_closure(event_names, event_name_to_id, outs, ins, get_name(*e1_it), get_name(*e2_it));
                // add_necessary_oc_edge(equation, *e1_it, *e2_it, "po", true_exprt());
                if(is_apo(*e1_it, *e2_it))
                    add_necessary_oc_edge(equation, *e1_it, *e2_it, "rmw", true_exprt());
            }
        // this ignores those can be derived through transitivity
        // event_it previous=equation.SSA_steps.end();
        // for(auto e_it=events.begin(); e_it!=events.end(); e_it++)
        // {
        //     if((*e_it)->is_memory_barrier())
        //         continue;
        //     if(previous==equation.SSA_steps.end())
        //     {
        //         // first one?
        //         previous=*e_it;
        //         continue;
        //     }
        //     add_necessary_oc_edge(equation, previous, *e_it, "po", true_exprt());
        //     if(is_apo(previous, *e_it))
        //         add_necessary_oc_edge(equation, previous, *e_it, "rmw", true_exprt());
        //     previous=*e_it;
        // }
    }

    for(int e1_id = 0; e1_id < int(event_names.size()); e1_id++)
    {
        for(int e2_id : outs[e1_id])
        {
            std::string e1 = event_names[e1_id];
            std::string e2 = event_names[e2_id];
            add_necessary_oc_edge(equation, e1, e2, "po", true_exprt());
        }
    }
}

void memory_model_generalt::build_rf(symex_target_equationt &equation)
{
    memory_model_baset::read_from(equation);
}

void memory_model_generalt::build_co(symex_target_equationt &equation)
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

                symbol_exprt s=nondet_bool_symbol("co");
                add_necessary_oc_edge(equation, *w_it1, *w_it2, "co", s);
                add_necessary_oc_edge(equation, *w_it2, *w_it1, "co", not_exprt(s));
            }
        }
    }
}

void memory_model_generalt::build_shared_reads(symex_target_equationt &equation, std::set<std::string>& reads)
{
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_shared_read())
            reads.insert(id2string(id(e_it)));
}

void memory_model_generalt::build_guard_map(symex_target_equationt& equation, std::map<symbol_exprt, exprt>& guard_map)
{
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_assignment() && e_it->assignment_type == symex_targett::assignment_typet::GUARD)
            guard_map[e_it->ssa_lhs] = e_it->ssa_rhs;
}

void memory_model_generalt::build_data(symex_target_equationt &equation, std::set<std::string>& reads)
{
    std::map<symbol_exprt, std::set<symbol_exprt>> assignment_symbol_map;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_assignment())
            assignment_symbol_map[to_symbol_expr(e_it->ssa_lhs)] = find_symbols(e_it->ssa_rhs);
    
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_shared_write())
        {
            symbol_exprt lhs = e_it->ssa_lhs;
            if(assignment_symbol_map.find(lhs) == assignment_symbol_map.end())
                continue;

            std::string e_str = id2string(id(e_it));
            if(e_str == "")
                e_str = fill_name(e_it);
            
            std::set<symbol_exprt> dependent_symbols = assignment_symbol_map[lhs];
            std::vector<symbol_exprt> wl;
            wl.assign(dependent_symbols.begin(), dependent_symbols.end());
            while(!wl.empty())
            {
                auto symbol = wl.back();
                wl.pop_back();
                if(assignment_symbol_map.find(symbol) != assignment_symbol_map.end())
                {
                    auto new_symbols = assignment_symbol_map[symbol];
                    for(auto new_symbol : new_symbols)
                    {
                        if(dependent_symbols.find(new_symbol) != dependent_symbols.end())
                            continue;
                        dependent_symbols.insert(new_symbol);
                        wl.push_back(new_symbol);
                    }
                }
            }

            for(auto dependent_symbol : dependent_symbols)
            {
                std::string dependent_symbol_name = dependent_symbol.get_identifier().c_str();
                if(reads.find(dependent_symbol_name) != reads.end())
                    add_necessary_oc_edge(equation, dependent_symbol_name, e_str, "data", true_exprt());
            }
        }
}

void find_offset_symbols(const exprt& expr, std::set<symbol_exprt>& offset_symbols)
{
    if(expr.id() == ID_pointer_offset)
        find_symbols(expr, offset_symbols);
    for(auto& operand : expr.operands())
        find_offset_symbols(operand, offset_symbols);
}

void memory_model_generalt::build_addr(symex_target_equationt &equation, std::set<std::string>& reads)
{
    std::map<symbol_exprt, exprt> assignment_map;
    std::map<symbol_exprt, std::set<symbol_exprt>> assignment_symbol_map;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_assignment())
        {
            assignment_map[to_symbol_expr(e_it->ssa_lhs)] = e_it->ssa_rhs;
            assignment_symbol_map[to_symbol_expr(e_it->ssa_lhs)] = find_symbols(e_it->ssa_rhs);
        }
    
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_shared_write())
        {
            symbol_exprt lhs = e_it->ssa_lhs;
            if(assignment_map.find(lhs) == assignment_map.end())
                continue;

            std::string e_str = id2string(id(e_it));
            if(e_str == "")
                e_str = fill_name(e_it);
            
            exprt rhs = assignment_map[lhs];
            std::set<symbol_exprt> offset_symbols;
            find_offset_symbols(rhs, offset_symbols);

            std::vector<symbol_exprt> wl;
            wl.assign(offset_symbols.begin(), offset_symbols.end());
            while(!wl.empty())
            {
                auto symbol = wl.back();
                wl.pop_back();
                if(assignment_symbol_map.find(symbol) != assignment_symbol_map.end())
                {
                    auto new_symbols = assignment_symbol_map[symbol];
                    for(auto new_symbol : new_symbols)
                    {
                        if(offset_symbols.find(new_symbol) != offset_symbols.end())
                            continue;
                        offset_symbols.insert(new_symbol);
                        wl.push_back(new_symbol);
                    }
                }
            }

            for(auto offset_symbol : offset_symbols)
            {
                std::string offset_symbol_name = offset_symbol.get_identifier().c_str();
                if(reads.find(offset_symbol_name) != reads.end())
                    add_necessary_oc_edge(equation, offset_symbol_name, e_str, "addr", true_exprt());
            }
        }
}

void memory_model_generalt::build_ctrl(symex_target_equationt &equation, std::set<std::string>& reads)
{
    std::map<symbol_exprt, exprt> guard_map;
    build_guard_map(equation, guard_map);

    std::map<symbol_exprt, std::set<symbol_exprt>> assignment_symbol_map;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_assignment())
            assignment_symbol_map[to_symbol_expr(e_it->ssa_lhs)] = find_symbols(e_it->ssa_rhs);

    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(!e_it->is_shared_read() && !e_it->is_shared_write() && !e_it->is_spawn() && !e_it->is_memory_barrier())
            continue;
        
        std::string e_str = id2string(id(e_it));
        if(e_str == "")
            e_str = fill_name(e_it);

        std::set<symbol_exprt> guard_symbols;
        find_symbols(e_it->guard, guard_symbols);

        std::set<symbol_exprt> dependent_symbols;
        for(auto& guard_symbol : guard_symbols)
        {
            if(guard_map.find(guard_symbol) == guard_map.end())
                continue;
            find_symbols(guard_map[guard_symbol], dependent_symbols);
        }

        std::vector<symbol_exprt> wl;
        wl.assign(dependent_symbols.begin(), dependent_symbols.end());
        while(!wl.empty())
        {
            auto symbol = wl.back();
            wl.pop_back();
            if(assignment_symbol_map.find(symbol) != assignment_symbol_map.end())
            {
                auto new_symbols = assignment_symbol_map[symbol];
                for(auto new_symbol : new_symbols)
                {
                    if(dependent_symbols.find(new_symbol) != dependent_symbols.end())
                        continue;
                    dependent_symbols.insert(new_symbol);
                    wl.push_back(new_symbol);
                }
            }
        }

        for(auto dependent_symbol : dependent_symbols)
        {
            std::string dependent_symbol_name = dependent_symbol.get_identifier().c_str();
            if(reads.find(dependent_symbol_name) != reads.end())
                add_necessary_oc_edge(equation, dependent_symbol_name, e_str, "ctrl", true_exprt());
        }
    }
}

void memory_model_generalt::remove_atomic_typecast(symex_target_equationt &equation)
{
    for(auto& step : equation.SSA_steps)
    {
        if(!step.is_assignment())
            continue;

        bool is_double_cast = (step.ssa_rhs.id() == ID_typecast) && (step.ssa_rhs.op0().id() == ID_typecast);
        if(is_double_cast)
        {
            auto new_rhs = step.ssa_rhs.op0().op0();
            step.ssa_rhs = new_rhs;
            step.cond_expr.op1() = step.ssa_rhs;
        }
    }
}