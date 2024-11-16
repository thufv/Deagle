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

    remove_atomic_typecast(equation);

    build_labels(equation);
    build_lkmm_nesting(equation);
    build_loc(equation);
    build_thd(equation);
    build_id(equation);

    build_po(equation);
    build_co(equation);
    build_rf(equation);

    std::set<std::string> reads;
    build_shared_reads(equation, reads);
    build_data(equation, reads);
    build_addr(equation, reads);
    build_ctrl(equation, reads);

    build_lkmm_crit(equation);

    // calculate must may sets
    must_may_calculator = must_may_calculatort(cat, equation.oc_edges, equation.oc_labels);
    must_may_calculator.calculate();
    cat.unary_musts = must_may_calculator.unary_musts;
    cat.unary_mays = must_may_calculator.unary_mays;
    cat.binary_musts = must_may_calculator.binary_musts;
    cat.binary_mays = must_may_calculator.binary_mays;

    cat.recursion_simulation();
    cat.build_propagate_map_all();
    build_free(equation);

    equation.cat = cat;
    equation.use_cat = true;
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
    if(guard_expr == true_exprt() && strict_guard)
        guard_expr = simplify_expr(and_exprt(e1->guard, e2->guard), ns);

    add_necessary_oc_edge(equation, get_name(e1), get_name(e2), kind, guard_expr);
}

void memory_model_generalt::add_necessary_oc_edge(symex_target_equationt &equation, std::string e1_str, std::string e2_str, std::string kind, exprt guard_expr)
{
    if(cat.binary_relations.find(kind) == cat.binary_relations.end())
        return;

    if(guard_expr == true_exprt() && strict_guard)
    {
        exprt guard1 = true_exprt();
        if(id_to_guard.find(e1_str) != id_to_guard.end())
            guard1 = id_to_guard[e1_str];
        exprt guard2 = true_exprt();
        if(id_to_guard.find(e2_str) != id_to_guard.end())
            guard2 = id_to_guard[e2_str];
        guard_expr = simplify_expr(and_exprt(guard1, guard2), ns);
    }
    add_oc_edge(equation, e1_str, e2_str, kind, guard_expr);
}

void memory_model_generalt::add_necessary_oc_label(symex_target_equationt &equation, event_it e, std::string label)
{
    if(cat.unary_relations.find(label) == cat.unary_relations.end())
        return;

    if(strict_guard)
        add_oc_label(equation, e, label, simplify_expr(e->guard, ns));
    else
        add_oc_label(equation, e, label, true_exprt());
}

void memory_model_generalt::add_apo(symex_target_equationt &equation, event_it e1, event_it e2, std::set<event_it>& added_rmw_labels)
{
    add_necessary_oc_edge(equation, e1, e2, "rmw", true_exprt());
    if(added_rmw_labels.find(e1) == added_rmw_labels.end())
    {
        added_rmw_labels.insert(e1);
        add_necessary_oc_label(equation, e1, "RMW");
        if(e1->is_shared_write())
            add_necessary_oc_label(equation, e1, "STRONG");
    }
    if(added_rmw_labels.find(e2) == added_rmw_labels.end())
    {
        added_rmw_labels.insert(e2);
        add_necessary_oc_label(equation, e2, "RMW");
        if(e2->is_shared_write())
            add_necessary_oc_label(equation, e2, "STRONG");
    }
}

void memory_model_generalt::build_free(symex_target_equationt &equation)
{
    std::vector<event_it> shared_events;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
        if(e_it->is_shared_read() || e_it->is_shared_write() || e_it->is_memory_barrier() || e_it->is_spawn())
            shared_events.push_back(e_it);

    for(auto& free_relation : cat.free_relations)
    {
        if(free_relation.substr(free_relation.size() - 6) == "_prime")
        {
            std::string original_relation = free_relation.substr(0, free_relation.size() - 6);
            auto& may_set = cat.binary_mays[original_relation];
            cat.binary_mays[free_relation] = may_set;
            for(auto may_edge : may_set)
            {
                std::string free_var_name = "free_" + free_relation + "_" + may_edge.first + "_" + may_edge.second;
                symbol_exprt free_var(free_var_name, bool_typet());
                add_necessary_oc_edge(equation, may_edge.first, may_edge.second, free_relation, free_var);
            }
            std::cout << free_relation << "'s original version is " << original_relation << "\n";
        }
        else
        {
            for(int n1 = 0; n1 < int(shared_events.size()); n1++)
                for(int n2 = 0; n2 < int(shared_events.size()); n2++)
                {
                    std::string free_var_name = "free_" + free_relation + "_" + std::to_string(n1) + "_" + std::to_string(n2);
                    symbol_exprt free_var(free_var_name, bool_typet());
                    add_necessary_oc_edge(equation, shared_events[n1], shared_events[n2], free_relation, free_var);
                }
        }
    }
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

std::vector<std::string> c11_mos{"RLX", "CON", "ACQ", "REL", "ACQ_REL", "SC"};

/* typedef enum
{
    memory_order_relaxed,
    memory_order_once,
    memory_order_acquire,
    memory_order_release,
    mb,
    wmb,
    rmb,
    rcu_lock,
    rcu_unlock,
    rcu_sync,
    before_atomic,
    after_atomic,
    after_spinlock,
    barrier,
} memory_order; */

std::vector<std::string> lkmm_mos{"Relaxed", "Once", "Acquire", "Release", "Mb", "Wmb", "Rmb",
    "Rcu-lock", "Rcu-unlock", "Sync-rcu",
    "Before-atomic", "After-atomic", "After-spinlock", "Barrier", "After-unlock-lock"};

void memory_model_generalt::build_labels(symex_target_equationt& equation)
{
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(e_it->is_shared_read())
        {
            add_necessary_oc_label(equation, e_it, "R");
            add_necessary_oc_label(equation, e_it, "M");
            add_necessary_oc_label(equation, e_it, "_");
        }
        else if(e_it->is_shared_write())
        {
            add_necessary_oc_label(equation, e_it, "W");
            add_necessary_oc_label(equation, e_it, "M");
            add_necessary_oc_label(equation, e_it, "_");
            if(e_it->is_init())
                add_necessary_oc_label(equation, e_it, "IW");
        }
        else if(e_it->is_memory_barrier())
        {
            add_necessary_oc_label(equation, e_it, "F");
            add_necessary_oc_label(equation, e_it, "~M");
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

        bool is_c11 = false;
        bool is_lkmm = false;
        std::string function_id = id2string(e_it->source.function_id);

        if(e_it->is_shared_write())
        {
            if(function_id == "__atomic_store" || function_id == "__atomic_store_n")
            {
                add_necessary_oc_label(equation, e_it, "A");
                is_c11 = true;
            }
            if(function_id == "__LKMM_STORE")
                is_lkmm = true;
        }

        if(e_it->is_shared_read())
        {
            if(function_id == "__atomic_load" || function_id == "__atomic_load_n")
            {
                add_necessary_oc_label(equation, e_it, "A");
                is_c11 = true;
            }
            if(function_id == "__LKMM_LOAD")
                is_lkmm = true;
        }

        if(e_it->is_memory_barrier())
        {
            if(function_id == "__atomic_thread_fence")
            {
                add_necessary_oc_label(equation, e_it, "A");
                is_c11 = true;
            }
            if(function_id == "__LKMM_FENCE")
                is_lkmm = true;
        }
        
        if(is_c11 || is_lkmm)
        {
            for(auto e_it2 = e_it; e_it2 != equation.SSA_steps.begin(); e_it2--)
            {
                if(!e_it2->is_assignment())
                    continue;
                auto lhs = e_it2->ssa_lhs;
                auto rhs = e_it2->ssa_rhs;
                std::string lhs_id = lhs.get_identifier().c_str();
                auto& mos = is_c11 ? c11_mos : lkmm_mos; // assume not both!
                if(lhs_id.find(function_id + "::memorder") != std::string::npos && rhs.id() == ID_constant)
                {
                    int mo = string2integer(rhs.get_string(ID_value), 16).to_long();
                    if(mo < 0 || mo >= int(mos.size()))
                    {
                        std::cout << "ERROR: Unsupported memorder " << mo << "\n";
                        std::exit(255);
                    }
                    add_necessary_oc_label(equation, e_it, mos[mo]);

                    if(mos[mo] == "Rcu-lock")
                        lkmm_locks.push_back(e_it);
                    if(mos[mo] == "Rcu-unlock")
                        lkmm_unlocks.push_back(e_it);

                    break;
                }
            }
            
        }
    }
}

void memory_model_generalt::build_lkmm_nesting(symex_target_equationt &equation)
{
    bool in_spin_lock = false;
    bool in_spin_unlock = false;
    bool in_atomic_op = false;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin(); e_it!=equation.SSA_steps.end(); e_it++)
    {
        if(e_it->is_function_call() && e_it->called_function == "__LKMM_SPIN_LOCK")
        {
            std::cout << "in spin lock\n";
            in_spin_lock = true;
        }
        if(e_it->is_function_return() && e_it->source.function_id == "__LKMM_SPIN_LOCK")
        {
            std::cout << "out of spin lock\n";
            in_spin_lock = false;
        }
        if(e_it->is_function_call() && e_it->called_function == "__LKMM_SPIN_UNLOCK")
        {
            std::cout << "in spin unlock\n";
            in_spin_unlock = true;
        }
        if(e_it->is_function_return() && e_it->source.function_id == "__LKMM_SPIN_UNLOCK")
        {
            std::cout << "out of spin unlock\n";
            in_spin_unlock = false;
        }
        if(e_it->is_function_call() && e_it->called_function == "__LKMM_ATOMIC_OP")
        {
            std::cout << "in atomic op\n";
            in_atomic_op = true;
        }
        if(e_it->is_function_return() && e_it->source.function_id == "__LKMM_ATOMIC_OP")
        {
            std::cout << "out of atomic op\n";
            in_atomic_op = false;
        }

        if(in_spin_lock && e_it->is_shared_read())
            add_necessary_oc_label(equation, e_it, "LKR");
        if(in_spin_lock && e_it->is_shared_write())
            add_necessary_oc_label(equation, e_it, "LKW");
        if(in_spin_unlock && (e_it->is_shared_write() || e_it->is_shared_read()))
            add_necessary_oc_label(equation, e_it, "UL");

        if(in_atomic_op && e_it->is_shared_read())
            add_necessary_oc_label(equation, e_it, "Noreturn");
        else if(e_it->is_shared_read() || e_it->is_shared_write() || e_it->is_memory_barrier())
            add_necessary_oc_label(equation, e_it, "~Noreturn");
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

    // internal
    for(auto& pair: per_thread_map)
    {
        auto& list = pair.second;
        for(auto e_it1 = list.begin(); e_it1 != list.end(); e_it1++)
            for(auto e_it2 = list.begin(); e_it2 != list.end(); e_it2++)
                add_necessary_oc_edge(equation, *e_it1, *e_it2, "int", true_exprt());
    }

    // external
    for(auto& pair1: per_thread_map)
        for(auto& pair2: per_thread_map)
        {
            if(pair1.first == pair2.first)
                continue;
            auto& list1 = pair1.second;
            auto& list2 = pair2.second;
        for(auto e_it1 = list1.begin(); e_it1 != list1.end(); e_it1++)
            for(auto e_it2 = list2.begin(); e_it2 != list2.end(); e_it2++)
                add_necessary_oc_edge(equation, *e_it1, *e_it2, "ext", true_exprt());
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

bool memory_model_generalt::is_mm_apo(event_it e1, event_it e2)
{
    if(!e1->is_shared_read() && !e1->is_shared_write())
        return false;
    if(!e2->is_shared_read() && !e2->is_shared_write())
        return false;
    if(id2string(e1->ssa_lhs.get_original_name()) != id2string(e2->ssa_lhs.get_original_name()))
        return false;
    return is_apo(e1, e2);
}

void memory_model_generalt::build_po(symex_target_equationt &equation)
{
    std::vector<std::string> event_names;
    std::map<std::string, int> event_name_to_id;
    std::vector<std::set<int>> outs;
    std::vector<std::set<int>> ins;

    per_thread_mapt per_thread_map;
    build_per_thread_map(equation, per_thread_map, true);

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

    std::set<event_it> added_rmw_labels;

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
                    if(is_mm_apo(e_it, *n_it))
                        add_apo(equation, e_it, *n_it, added_rmw_labels);
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
                        if(is_mm_apo(e_it, *n_it))
                            add_apo(equation, *n_it, join_event, added_rmw_labels);
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
                if(is_mm_apo(*e1_it, *e2_it))
                    add_apo(equation, *e1_it, *e2_it, added_rmw_labels);
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
        //     if(is_mm_apo(previous, *e_it))
        //         add_apo(equation, previous, *e_it, added_rmw_labels);
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

    // for __CPROVER variable accesses, we always consider them as RMW and Atomic
    for(auto t_it=per_thread_map.begin(); t_it!=per_thread_map.end(); t_it++)
    {
        const event_listt &events=t_it->second;
        for(auto e_it = events.begin(); e_it != events.end(); e_it++)
        {
            if(!(*e_it)->is_shared_write() && !(*e_it)->is_shared_read())
                continue;
            auto str = get_name(*e_it);
            if(str.find("__CPROVER") != std::string::npos && added_rmw_labels.find(*e_it) == added_rmw_labels.end())
                add_necessary_oc_label(equation, *e_it, "RMW");
            if(str.find("__CPROVER") != std::string::npos)
                add_necessary_oc_label(equation, *e_it, "A");
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

                bool can12 = !(assume_local_consistency && po(*w_it2, *w_it1));
                bool can21 = !(assume_local_consistency && po(*w_it1, *w_it2));
                if((*w_it1)->is_init())
                    can21 = false;
                if((*w_it2)->is_init())
                    can12 = false;

                if(can12 && can21)
                {
                    symbol_exprt s12=nondet_bool_symbol("co");
                    symbol_exprt s21=nondet_bool_symbol("co");

                    add_constraint(equation, equal_exprt(or_exprt(s12, s21), and_exprt((*w_it1)->guard, (*w_it2)->guard)), "co-some", (*w_it1)->source);
                    add_constraint(equation, not_exprt(and_exprt(s12, s21)), "co-only-one", (*w_it1)->source);
                    add_necessary_oc_edge(equation, *w_it1, *w_it2, "co", s12);
                    add_necessary_oc_edge(equation, *w_it2, *w_it1, "co", s21);
                }
                else if(can12)
                {
                    symbol_exprt s12=nondet_bool_symbol("co");
                    add_constraint(equation, equal_exprt(s12, and_exprt((*w_it1)->guard, (*w_it2)->guard)), "co-some", (*w_it1)->source);
                    add_necessary_oc_edge(equation, *w_it1, *w_it2, "co", s12);
                }
                else if(can21)
                {
                    symbol_exprt s21=nondet_bool_symbol("co");
                    add_constraint(equation, equal_exprt(s21, and_exprt((*w_it2)->guard, (*w_it1)->guard)), "co-some", (*w_it1)->source);
                    add_necessary_oc_edge(equation, *w_it2, *w_it1, "co", s21);
                }
                else
                    std::cout << "WARNING: either " << id(*w_it1) << " or " << id(*w_it2) << " cannot coherence before each other.\n";
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

void memory_model_generalt::build_lkmm_crit(symex_target_equationt &equation)
{
    for(auto lkmm_lock : lkmm_locks)
        for(auto lkmm_unlock : lkmm_unlocks)
            if(po(lkmm_lock, lkmm_unlock))
                add_necessary_oc_edge(equation, lkmm_lock, lkmm_unlock, "rcu-rscs", true_exprt());
}

#include <util/format_expr.h>
#include <util/bitvector_types.h>
#include <util/byte_operators.h>

void remove_double_typecast_single(exprt& expr, const namespacet& ns) // in place
{
    bool outside_is_cast = (expr.id() == ID_typecast);
    if(!outside_is_cast)
        return;

    int offset = 0;
    bool inside_is_cast_to_empty = (expr.op0().id() == ID_typecast) && (expr.op0().type().id() == ID_empty);
    bool inside_is_extract_to_empty = (expr.op0().id() == ID_byte_extract_little_endian) && (expr.op0().type().id() == ID_empty);
    if(!inside_is_cast_to_empty && !inside_is_extract_to_empty)
        return;

    auto source_expr = expr.op0().op0();
    auto source_type = source_expr.type();
    auto target_type = expr.type();

    std::cout << "Converting from " << format(expr);

    if(source_type == target_type && offset == 0) // source_type and target_type are identical, easy!
        expr = source_expr;
    else if(inside_is_cast_to_empty)
        expr = typecast_exprt(source_expr, target_type);
    else if(inside_is_extract_to_empty)
    {
        auto offset_expr = expr.op0().op1();
        expr = byte_extract_exprt(ID_byte_extract_little_endian, source_expr, offset_expr, target_type);
    }
    std::cout << " to " << format(expr) << "\n";
}

void remove_double_typecast_rec(exprt& expr, const namespacet& ns)
{
    remove_double_typecast_single(expr, ns);
    for(auto& operand : expr.operands())
        remove_double_typecast_rec(operand, ns);
}

void remove_empty_typecast_single(exprt& expr)
{
    if(expr.id() != ID_typecast)
        return;

    auto source_expr = expr.op0();
    auto target_type = expr.type();
    if(target_type.id() == ID_empty)
    {
        std::cout << "Converting from " << format(expr) << " to " << format(source_expr) << "\n";
        expr = source_expr;
    }
}

void remove_empty_typecast_rec(exprt& expr)
{
    remove_empty_typecast_single(expr);
    for(auto& operand : expr.operands())
        remove_empty_typecast_rec(operand);
}

void memory_model_generalt::remove_atomic_typecast(symex_target_equationt &equation)
{
    for(auto& step : equation.SSA_steps)
    {
        if(!step.is_assignment())
            continue;

        remove_double_typecast_rec(step.ssa_rhs, ns);
        remove_empty_typecast_rec(step.ssa_rhs);
        step.cond_expr.op1() = step.ssa_rhs;
    }
}

void must_may_calculatort::back_to_str()
{
    for(auto& unary_pair : int_unary_musts)
    {
        auto& kind = unary_pair.first;
        auto& int_unary_set = unary_pair.second;
        for(auto id : int_unary_set)
            unary_musts[kind].insert(id2str[id]);
    }
    for(auto& unary_pair : int_unary_mays)
    {
        auto& kind = unary_pair.first;
        auto& int_unary_set = unary_pair.second;
        for(auto id : int_unary_set)
            unary_mays[kind].insert(id2str[id]);
    }
    for(auto& binary_pair : int_binary_musts)
    {
        auto& kind = binary_pair.first;
        auto& int_binary_set = binary_pair.second;
        for(auto pair : int_binary_set)
            binary_musts[kind].insert(std::make_pair(id2str[pair.first], id2str[pair.second]));
    }
    for(auto& binary_pair : int_binary_mays)
    {
        auto& kind = binary_pair.first;
        auto& int_binary_set = binary_pair.second;
        for(auto pair : int_binary_set)
            binary_mays[kind].insert(std::make_pair(id2str[pair.first], id2str[pair.second]));
    }
}

void must_may_calculatort::calculate()
{
    build_scc();
    build_topological();
    calculate_sets();
    back_to_str();

    // for(auto unary_relation_name : cat.unary_relations)
    // {
    //     std::cout << unary_relation_name << " is unary, with must set:\n";
    //     for(auto label : unary_musts[unary_relation_name])
    //         std::cout << label << " ";
    //     std::cout << "\n";
    //     std::cout << unary_relation_name << " is unary, with may set:\n";
    //     for(auto label : unary_mays[unary_relation_name])
    //         std::cout << label << " ";
    //     std::cout << "\n";
    // }
    // for(auto binary_relation_name : cat.binary_relations)
    // {
    //     std::cout << binary_relation_name << " is binary, with must set:\n";
    //     for(auto edge : binary_musts[binary_relation_name])
    //         std::cout << "(" << edge.first << ", " << edge.second << ") ";
    //     std::cout << "\n";
    //     std::cout << binary_relation_name << " is binary, with may set:\n";
    //     for(auto edge : binary_mays[binary_relation_name])
    //         std::cout << "(" << edge.first << ", " << edge.second << ") ";
    //     std::cout << "\n";
    // }
}

void must_may_calculatort::tarjan(std::string kind, std::stack<std::string>& stack, std::map<std::string, bool>& in_stack, std::map<std::string, int>& dfn, std::map<std::string, int>& low, int& count)
{
    count++;
    dfn[kind] = low[kind] = count;
    stack.push(kind);
    in_stack[kind] = true;

    auto& edges = cat.get_relation_from_name(kind).operands;
    for(auto next_relation : edges)
    {
        if(dfn.find(next_relation) == dfn.end() || !dfn[next_relation])
        {
            tarjan(next_relation, stack, in_stack, dfn, low, count);
            low[kind] = std::min(low[kind], low[next_relation]);
        }
        else if(in_stack.find(next_relation) != in_stack.end() && in_stack[next_relation])
            low[kind] = std::min(low[kind], dfn[next_relation]);
    }
    if(dfn[kind] == low[kind])
    {
        std::vector<std::string> scc;
        std::string k;
        do {
            k = stack.top();
            scc.push_back(k);
            stack.pop();
            in_stack[k] = false;
        } while(k != kind);
        int scc_id = sccs.size();
        sccs.push_back(scc);
        for(auto kind : scc)
            kind_to_scc_id[kind] = scc_id;
    }
}

void must_may_calculatort::build_scc()
{
    int count = 0;
    std::stack<std::string> stack;
    std::map<std::string, bool> in_stack;
    std::map<std::string, int> dfn, low;
    for(auto unary_relation : cat.unary_relations)
    {
        if(dfn.find(unary_relation) == dfn.end())
            tarjan(unary_relation, stack, in_stack, dfn, low, count);
    }
    for(auto binary_relation : cat.binary_relations)
    {
        if(dfn.find(binary_relation) == dfn.end())
            tarjan(binary_relation, stack, in_stack, dfn, low, count);
    }
}

void must_may_calculatort::build_topological()
{
    std::vector<std::set<int>> forward_edges;
    std::vector<int> in_degrees;
    forward_edges.resize(sccs.size());
    in_degrees.resize(sccs.size());

    for(auto relation_name : cat.necessary_relations)
    {
        auto& relation = cat.get_relation_from_name(relation_name);
        for(auto operand : relation.operands)
        {
            int from_scc_id = kind_to_scc_id[operand];
            int to_scc_id = kind_to_scc_id[relation_name];
            if(from_scc_id == to_scc_id)
                continue;
            auto insert_result = forward_edges[from_scc_id].insert(to_scc_id);
            if(insert_result.second)
                in_degrees[to_scc_id]++;
        }
    }

    std::vector<int> zero_in_degree_items;
    for(int scc_id = 0; scc_id < int(sccs.size()); scc_id++)
        if(in_degrees[scc_id] == 0)
            zero_in_degree_items.push_back(scc_id);

    while(!zero_in_degree_items.empty())
    {
        int from_scc_id = zero_in_degree_items.back();
        sorted_scc_ids.push_back(from_scc_id);
        zero_in_degree_items.pop_back();

        for(int to_scc_id : forward_edges[from_scc_id])
        {
            in_degrees[to_scc_id]--;
            if(in_degrees[to_scc_id] <= 0)
                zero_in_degree_items.push_back(to_scc_id);
        }
    }
}

#include <algorithm>

template<typename T> void must_may_calculatort::calc_union(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result)
{
    std::set<T> left_set(left.begin(), left.end());
    std::set<T> right_set(right.begin(), right.end());
    std::set<T> result_set(result.begin(), result.end());
    std::set_union(left_set.begin(), left_set.end(), right_set.begin(), right_set.end(), std::inserter(result_set, result_set.begin()));
    result.assign(result_set.begin(), result_set.end());
}

template<typename T> void must_may_calculatort::calc_intersection(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result)
{
    std::set<T> left_set(left.begin(), left.end());
    std::set<T> right_set(right.begin(), right.end());
    std::set<T> result_set(result.begin(), result.end());
    std::set_intersection(left_set.begin(), left_set.end(), right_set.begin(), right_set.end(), std::inserter(result_set, result_set.begin()));
    result.assign(result_set.begin(), result_set.end());
}

template<typename T> void must_may_calculatort::calc_difference(const std::vector<T>& left, const std::vector<T>& right, std::vector<T>& result)
{
    std::set<T> left_set(left.begin(), left.end());
    std::set<T> right_set(right.begin(), right.end());
    std::set<T> result_set(result.begin(), result.end());
    std::set_difference(left.begin(), left.end(), right.begin(), right.end(), std::inserter(result_set, result_set.begin()));
    result.assign(result_set.begin(), result_set.end());
}

void must_may_calculatort::calc_composition(const int_binary_sett& left, const int_binary_sett& right, int_binary_sett& result)
{
    std::set<std::pair<int, int>> result_set(result.begin(), result.end());
    for(auto left_edge : left)
        for(auto right_edge : right)
            if(left_edge.second == right_edge.first)
                result_set.insert(std::make_pair(left_edge.first, right_edge.second));
    result.assign(result_set.begin(), result_set.end());
}

void must_may_calculatort::calc_product(const int_unary_sett& left, const int_unary_sett& right, int_binary_sett& result)
{
    result.clear();
    for(auto left_label : left)
        for(auto right_label : right)
            result.push_back(std::make_pair(left_label, right_label));
}

void must_may_calculatort::calc_bracket(const int_unary_sett& source, int_binary_sett& result)
{
    result.clear();
    for(auto label : source)
        result.push_back(std::make_pair(label, label));
}

void must_may_calculatort::calc_flip(const int_binary_sett& source, int_binary_sett& result)
{
    result.clear();
    for(auto edge : source)
        result.push_back(std::make_pair(edge.second, edge.first));
}

void must_may_calculatort::calc_transitive_closure(const int_binary_sett& source, int_binary_sett& result)
{
    int result_size_before = 0;
    do
    {
        result_size_before = result.size();
        calc_composition(source, source, result);
    } while (result_size_before < int(result.size()));
}

void must_may_calculatort::calculate_single_kind(std::string kind, int arity)
{
    std::cout << "calculate the must may set of " << kind << "\n";

    auto relation = cat.get_relation_from_name(kind);
    switch(relation.op_type)
    {
    case ALT:
        if(arity == 1)
        {
            calc_union(int_unary_musts[relation.operands[0]], int_unary_musts[relation.operands[1]], int_unary_musts[kind]);
            calc_union(int_unary_mays[relation.operands[0]], int_unary_mays[relation.operands[1]], int_unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_union(int_binary_musts[relation.operands[0]], int_binary_musts[relation.operands[1]], int_binary_musts[kind]);
            calc_union(int_binary_mays[relation.operands[0]], int_binary_mays[relation.operands[1]], int_binary_mays[kind]);
        }
        break;
    case SEQ:
        calc_composition(int_binary_musts[relation.operands[0]], int_binary_musts[relation.operands[1]], int_binary_musts[kind]);
        calc_composition(int_binary_mays[relation.operands[0]], int_binary_mays[relation.operands[1]], int_binary_mays[kind]);
        break;
    case AND:
        if(arity == 1)
        {
            calc_intersection(int_unary_musts[relation.operands[0]], int_unary_musts[relation.operands[1]], int_unary_musts[kind]);
            calc_intersection(int_unary_mays[relation.operands[0]], int_unary_mays[relation.operands[1]], int_unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_intersection(int_binary_musts[relation.operands[0]], int_binary_musts[relation.operands[1]], int_binary_musts[kind]);
            calc_intersection(int_binary_mays[relation.operands[0]], int_binary_mays[relation.operands[1]], int_binary_mays[kind]);
        }
        break;
    case SUB:
        if(arity == 1)
        {
            calc_difference(int_unary_musts[relation.operands[0]], int_unary_mays[relation.operands[1]], int_unary_musts[kind]);
            calc_difference(int_unary_mays[relation.operands[0]], int_unary_musts[relation.operands[1]], int_unary_mays[kind]);
        }
        if(arity == 2)
        {
            calc_difference(int_binary_musts[relation.operands[0]], int_binary_mays[relation.operands[1]], int_binary_musts[kind]);
            calc_difference(int_binary_mays[relation.operands[0]], int_binary_musts[relation.operands[1]], int_binary_mays[kind]);
        }
        break;
    case PROD:
        calc_product(int_unary_musts[relation.operands[0]], int_unary_musts[relation.operands[1]], int_binary_musts[kind]);
        calc_product(int_unary_mays[relation.operands[0]], int_unary_mays[relation.operands[1]], int_binary_mays[kind]);
        break;
    case BRACKET:
        calc_bracket(int_unary_musts[relation.operands[0]], int_binary_musts[kind]);
        calc_bracket(int_unary_mays[relation.operands[0]], int_binary_mays[kind]);
        break;
    case FLIP:
        calc_flip(int_binary_musts[relation.operands[0]], int_binary_musts[kind]);
        calc_flip(int_binary_mays[relation.operands[0]], int_binary_mays[kind]);
        break;
    case PLUS:
        calc_transitive_closure(int_binary_musts[relation.operands[0]], int_binary_musts[kind]);
        calc_transitive_closure(int_binary_mays[relation.operands[0]], int_binary_mays[kind]);
        break;
    case TERMINAL:
        if(arity == 1)
        {
            if(int_unary_musts.find(kind) == int_unary_musts.end())
                int_unary_musts[kind] = int_unary_sett();
            if(int_unary_mays.find(kind) == int_unary_mays.end())
                int_unary_mays[kind] = int_unary_sett();
        }
        if(arity == 2)
        {
            if(int_binary_musts.find(kind) == int_binary_musts.end())
                int_binary_musts[kind] = int_binary_sett();
            if(int_binary_mays.find(kind) == int_binary_mays.end())
                int_binary_mays[kind] = int_binary_sett();
        }
    default:
        break;
    }
}

void must_may_calculatort::calculate_sets()
{
    for(auto scc_id : sorted_scc_ids)
    {
        auto& scc = sccs[scc_id];

        bool need_must_may_set = false;
        for(auto kind : scc)
            if(cat.need_must_may_set_relations.find(kind) != cat.need_must_may_set_relations.end())
            {
                need_must_may_set = true;
                break;
            }
        if(!need_must_may_set)
            continue;

        if(scc.size() == 1) // not recursive
        {
            auto kind = scc[0];
            calculate_single_kind(kind, cat.get_arity(kind));
        }
        else
        {
            bool changed = false;
            do
            {
                changed = false;
                for(auto& kind : scc)
                {
                    int original_total_size = int_unary_musts[kind].size() + int_unary_mays[kind].size() + int_binary_musts[kind].size() + int_binary_mays[kind].size();
                    calculate_single_kind(kind, cat.get_arity(kind));
                    int new_total_size = int_unary_musts[kind].size() + int_unary_mays[kind].size() + int_binary_musts[kind].size() + int_binary_mays[kind].size();
                    if(new_total_size > original_total_size)
                        changed = true;
                }
            } while (changed);
            
        }
    }
}