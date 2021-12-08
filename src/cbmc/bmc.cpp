/*******************************************************************\

Module: Symbolic Execution of ANSI-C

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

#include <util/string2int.h>
#include <util/i2string.h>
#include <util/source_location.h>
#include <util/time_stopping.h>
#include <util/message_stream.h>

#include <langapi/mode.h>
#include <langapi/languages.h>
#include <langapi/language_util.h>

#include <ansi-c/ansi_c_language.h>

#include <goto-programs/xml_goto_trace.h>

#include <goto-symex/build_goto_trace.h>
#include <goto-symex/slice.h>
#include <goto-symex/slice_by_trace.h>
#include <goto-symex/memory_model_sc.h>
#include <goto-symex/memory_model_tso.h>
#include <goto-symex/memory_model_pso.h>

#include "bmc.h"
#include "bv_cbmc.h"


/*******************************************************************\

Function: bmct::do_unwind_module

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::do_unwind_module(
        decision_proceduret &decision_procedure)
{
}

/*******************************************************************\

Function: bmct::error_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::error_trace(const prop_convt &prop_conv)
{
    status() << "Building error trace" << eom;

    goto_tracet goto_trace;
    build_goto_trace(equation, prop_conv, ns, goto_trace);

#if 0
    if(options.get_option("vcd")!="")
  {
    if(options.get_option("vcd")=="-")
      output_vcd(ns, goto_trace, std::cout);
    else
    {
      std::ofstream out(options.get_option("vcd").c_str());
      output_vcd(ns, goto_trace, out);
    }
  }
#endif

    std::ofstream output;
    switch(ui)
    {
        case ui_message_handlert::PLAIN:
            std::cout << "\n" << "Counterexample:" << "\n";
            show_goto_trace(std::cout, ns, goto_trace);

            // write into counterexample.witness
            output.open("witness.graphml", std::ios::app);
            output << "\n" << "Counterexample:" << "\n";
            show_goto_trace(output, ns, goto_trace);
            output.close();

            break;

        case ui_message_handlert::XML_UI:
        {
            xmlt xml;
            convert(ns, goto_trace, xml);
            std::cout << xml << "\n";
        }
            break;

        default:
            assert(false);
    }

    const std::string graphml=options.get_option("graphml-cex");
    if(!graphml.empty())
    {
#if 0
        cfg_baset<empty_cfg_nodet> cfg;
    assert(gf_ptr);
    cfg(*gf_ptr);
#endif

        graphmlt cex_graph;
#if 0
        convert(ns, goto_trace, cfg, cex_graph);
#else
        convert(ns, goto_trace, cex_graph);
#endif

        if(graphml=="-")
            write_graphml(cex_graph, std::cout);
        else
        {
            std::ofstream out(graphml.c_str());
            write_graphml(cex_graph, out);
        }
    }
}

/*******************************************************************\

Function: bmct::do_conversion

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::do_conversion(prop_convt &prop_conv)
{
    // convert HDL
    do_unwind_module(prop_conv);

    // convert SSA
    equation.convert(prop_conv);

    // the 'extra constraints'
    forall_expr_list(it, bmc_constraints)
        prop_conv.set_to_true(*it);
}

/*******************************************************************\

Function: bmct::run_decision_procedure

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

decision_proceduret::resultt
bmct::run_decision_procedure(prop_convt &prop_conv)
{
    status() << "Passing problem to "
             << prop_conv.decision_procedure_text() << eom;

    prop_conv.set_message_handler(get_message_handler());

    // stop the time
    absolute_timet sat_start=current_time();
#if output_reason
    //// __FHY_ADD_BEGIN__
    for(auto ssa_step : equation.SSA_steps)
    {
//      if(ssa_step.is_assert())
        ssa_step.output(ns, std::cout);
    }
    //// __FHY_ADD_END__
#endif
    //// __FHY_ADD_BEGIN__
//    for(auto ssa_step : equation.SSA_steps)
//    {
//        ssa_step.output(ns, std::cout);
//    }
    //std::cout << "see the equation!\n";
    //equation.output(std::cout);
    //std::cout << "finish seeing!\n";
    //// __FHY_ADD_END__

    do_conversion(prop_conv);

    status() << "Running " << prop_conv.decision_procedure_text() << eom;

    decision_proceduret::resultt dec_result=prop_conv.dec_solve();
    // output runtime

    absolute_timet sat_stop=current_time();
    status() << "Runtime decision procedure: "
             << (sat_stop-sat_start) << "s" << eom;

    return dec_result;
}


/*******************************************************************\

Function: bmct::run_decision_procedure

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

decision_proceduret::resultt
bmct::incremental_solve(prop_convt &prop_conv, exprt& constraint)
{
    // stop the time
    absolute_timet sat_start=current_time();

    prop_conv.set_to_true(constraint);

    status() << "Running " << prop_conv.decision_procedure_text() << eom;

    decision_proceduret::resultt dec_result=prop_conv.dec_solve();
    // output runtime

    absolute_timet sat_stop=current_time();
    status() << "Runtime decision procedure: "
             << (sat_stop-sat_start) << "s" << eom;

    return dec_result;
}

/*******************************************************************\

Function: bmct::report_success

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::report_success()
{
    result() << "VERIFICATION SUCCESSFUL" << eom;

    write_true_witness();

    switch(ui)
    {
        case ui_message_handlert::PLAIN:
            break;

        case ui_message_handlert::XML_UI:
        {
            xmlt xml("cprover-status");
            xml.data="SUCCESS";
            std::cout << xml;
            std::cout << "\n";
        }
            break;

        default:
            assert(false);
    }
}

/*******************************************************************\

Function: bmct::report_failure

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::report_failure()
{
    result() << "VERIFICATION FAILED" << eom;

    switch(ui)
    {
        case ui_message_handlert::PLAIN:
            break;

        case ui_message_handlert::XML_UI:
        {
            xmlt xml("cprover-status");
            xml.data="FAILURE";
            std::cout << xml;
            std::cout << "\n";
        }
            break;

        default:
            assert(false);
    }
}

/*******************************************************************\

Function: bmct::show_program

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::show_program()
{
    unsigned count=1;

    languagest languages(ns, new_ansi_c_language());

    std::cout << "\n" << "Program constraints:" << "\n";

    for(symex_target_equationt::SSA_stepst::const_iterator
                it=equation.SSA_steps.begin();
        it!=equation.SSA_steps.end(); it++)
    {
        if(it->is_assignment())
        {
            std::string string_value;
            languages.from_expr(it->cond_expr, string_value);
            std::cout << "(" << count << ") " << string_value << "\n";

            if(!it->guard.is_true())
            {
                languages.from_expr(it->guard, string_value);
                std::cout << std::string(i2string(count).size()+3, ' ');
                std::cout << "guard: " << string_value << "\n";
            }

            count++;
        }
        else if(it->is_assert())
        {
            std::string string_value;
            languages.from_expr(it->cond_expr, string_value);
            std::cout << "(" << count << ") ASSERT("
                      << string_value <<") " << "\n";

            if(!it->guard.is_true())
            {
                languages.from_expr(it->guard, string_value);
                std::cout << std::string(i2string(count).size()+3, ' ');
                std::cout << "guard: " << string_value << "\n";
            }

            count++;
        }
    }
}

/*******************************************************************\

Function: bmct::decide

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool bmct::decide(prop_convt &prop_conv)
{
    if(options.get_bool_option("beautify-pbs") ||
       options.get_bool_option("beautify-greedy"))
        throw "sorry, this solver does not support beautification";

    prop_conv.set_message_handler(get_message_handler());

    bool result=true;

    switch(run_decision_procedure(prop_conv))
    {
        case decision_proceduret::D_UNSATISFIABLE:
            result=false;
            report_success();
            break;

        case decision_proceduret::D_SATISFIABLE:
            error_trace(prop_conv);

            // __SZH_ADD_BEGIN__ : error trace for smt2
            if(memory_model->solve_method == memory_model_baset::solve_method_smt2)
            {
                report_failure_witness(prop_conv, ((smt2_dect*)(&prop_conv))->write_order);
            }
            else
            {
                report_failure();
            }
            // __SZH_ADD_END__

            
            break;

        default:
            error() << "decision procedure failed" << eom;
    }

    return result;
}

/*******************************************************************\

Function: bmct::setup_unwind

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void bmct::setup_unwind()
{
    const std::string &set=options.get_option("unwindset");
    std::string::size_type length=set.length();

    for(std::string::size_type idx=0; idx<length; idx++)
    {
        std::string::size_type next=set.find(",", idx);
        std::string val=set.substr(idx, next-idx);

        unsigned thread_nr;
        bool thread_nr_set=false;

        if(!val.empty() &&
           isdigit(val[0]) &&
           val.find(":")!=std::string::npos)
        {
            std::string nr=val.substr(0, val.find(":"));
            thread_nr=unsafe_string2unsigned(nr);
            thread_nr_set=true;
            val.erase(0, nr.size()+1);
        }

        if(val.rfind(":")!=std::string::npos)
        {
            std::string id=val.substr(0, val.rfind(":"));
            long uw=unsafe_string2int(val.substr(val.rfind(":")+1));

            if(thread_nr_set)
                symex.set_unwind_thread_loop_limit(thread_nr, id, uw);
            else
                symex.set_unwind_loop_limit(id, uw);
        }

        if(next==std::string::npos) break;
        idx=next;
    }

    if(options.get_option("unwind")!="")
        symex.set_unwind_limit(options.get_unsigned_int_option("unwind"));
}

// ==============================================================
// THE FOLLOWING FUNCTIONS ARE FOR YOGAR-CBMC
// ==============================================================

bool bmct::run(goto_functionst &goto_functions)
{
    gf_ptr=&goto_functions;

    const std::string mm=options.get_option("mm");

    if(mm.empty() || mm=="sc")
        memory_model.reset(new memory_model_sct(ns));
    else if(mm=="tso")
        memory_model.reset(new memory_model_tsot(ns));
    else if(mm=="pso")
        memory_model.reset(new memory_model_psot(ns));
    else
        throw "Invalid memory model "+mm+" -- use one of sc, tso, pso";

    // __SZH_ADD_BEGIN__: choose a method
    if(options.get_bool_option("closure"))
    {
        std::cout << "memory model: sat closure mode\n";
        memory_model->solve_method = memory_model_baset::solve_method_sat_closure;
    }
    if(options.get_bool_option("icd"))
    {
        std::cout << "memory model: sat icd mode\n";
        memory_model->solve_method = memory_model_baset::solve_method_sat_icd;
    }
    else if(options.get_bool_option("z3"))
    {
        std::cout << "memory model: smt2 mode\n";
        memory_model->solve_method = memory_model_baset::solve_method_smt2;
    }
    else
    {
        std::cout << "memory model: traditional sat mode\n";
        memory_model->solve_method = memory_model_baset::solve_method_sat;
    }
    // __SZH_ADD_END__

    //symex.total_claims=0;
    symex.set_message_handler(get_message_handler());
    symex.options=options;

    status() << "Starting Bounded Model Checking" << eom;

    symex.last_source_location.make_nil();

    bool reduce_threads = false;
    std::list<symex_target_equationt::SSA_stept> SSA_steps;
    try
    {
//    goto_functions.output(ns, std::cout);

        // preprocess, if pointer pt is global, and pt = &data, then set data to be global
        equation.set_mutex_flag(goto_functions.mutex_flag);

        value_sett value_set;
        set_unwind_limit(goto_functions, value_set);
//    setup_unwind();

        symex.original_for_unwind_limit = goto_functions.for_unwind_limit;
        symex(goto_functions, value_set);

        equation.slice();

        std::vector<int> valid_threads;
        bool can_be_reduced = identify_same_threads(goto_functions, valid_threads);
        can_be_reduced = false;
        if (can_be_reduced && !has_pthread_join())
        {
            std::cout << "do thread reduction\n";

            reduce_threads = true;
            SSA_steps = equation.SSA_steps;
            reduce_threads_num(memory_model, valid_threads);
            std::cout << "checkpoint1\n";
        }
        else
        {
            if(equation.has_threads())
            {
                memory_model->set_message_handler(get_message_handler());
                (*memory_model)(equation);
                std::cout << "checkpoint2\n";
            }
        }

        std::cout << "valid threads ok\n";

//    if (equation.exceed_events_limit())
//    	reduce_loop_bound(goto_functions, memory_model);

        statistics() << "size of program expression: "
                     << equation.SSA_steps.size()
                     << " steps" << eom;

        //equation.output(std::cout);
    }

    catch(std::string &error_str)
    {
        message_streamt message_stream(get_message_handler());
        message_stream.err_location(symex.last_source_location);
        message_stream.error(error_str);
        return true;
    }

    catch(const char *error_str)
    {
        message_streamt message_stream(get_message_handler());
        message_stream.err_location(symex.last_source_location);
        message_stream.error(error_str);
        return true;
    }

    catch(std::bad_alloc)
    {
        error() << "Out of memory" << eom;
        return true;
    }

    try
    {
        if(options.get_option("slice-by-trace")!="")
        {
            symex_slice_by_tracet symex_slice_by_trace(ns);

            symex_slice_by_trace.slice_by_trace
                    (options.get_option("slice-by-trace"), equation);
        }

        if(equation.has_threads())
        {
            // we should build a thread-aware SSA slicer
            statistics() << "no slicing due to threads" << eom;
        }
        else
        {
            if(options.get_bool_option("slice-formula"))
            {
                slice(equation);
                statistics() << "slicing removed "
                             << equation.count_ignored_SSA_steps()
                             << " assignments" << eom;
            }
            else
            {
                simple_slice(equation);
                statistics() << "simple slicing removed "
                             << equation.count_ignored_SSA_steps()
                             << " assignments" << eom;
            }
        }

        {
            statistics() << "Generated " << symex.total_claims
                         << " VCC(s), " << symex.remaining_claims
                         << " remaining after simplification" << eom;
        }

        if(options.get_bool_option("show-vcc"))
        {
            show_vcc();
            return false;
        }

        if(options.get_bool_option("cover-assertions"))
        {
            satcheckt satcheck;
            satcheck.set_message_handler(get_message_handler());
            bv_cbmct bv_cbmc(ns, satcheck);
            bv_cbmc.set_message_handler(get_message_handler());

            if(options.get_option("arrays-uf")=="never")
                bv_cbmc.unbounded_array=bv_cbmct::U_NONE;
            else if(options.get_option("arrays-uf")=="always")
                bv_cbmc.unbounded_array=bv_cbmct::U_ALL;

            cover_assertions(goto_functions, bv_cbmc);
            return false;
        }

        // any properties to check at all?
        if(!options.get_bool_option("program-only") &&
           symex.remaining_claims==0)
        {
            report_success();
            return false;
        }

        if(options.get_bool_option("all-properties"))
        {
            satcheckt satcheck;
            satcheck.set_message_handler(get_message_handler());
            bv_cbmct bv_cbmc(ns, satcheck);
            bv_cbmc.set_message_handler(get_message_handler());

            if(options.get_option("arrays-uf")=="never")
                bv_cbmc.unbounded_array=bv_cbmct::U_NONE;
            else if(options.get_option("arrays-uf")=="always")
                bv_cbmc.unbounded_array=bv_cbmct::U_ALL;

            return all_properties(goto_functions, bv_cbmc);
        }



        if(options.get_bool_option("smt1"))
            return decide_smt1();
        else if(options.get_bool_option("smt2"))
            return decide_smt2();
        else if(options.get_bool_option("dimacs"))
            return write_dimacs();
        else if(options.get_bool_option("refine"))
            return decide_bv_refinement();
        else if(options.get_bool_option("aig"))
            return decide_aig();
        // __SZH_ADD_BEGIN__
        else if(options.get_bool_option("closure"))
            return decide_closure();
        else if(options.get_bool_option("icd"))
            return decide_ICD();
        // __SZH_ADD_END__
        else
        {
            if(options.get_bool_option("program-only"))
            {
                show_program();
                return false;
            }
            //================================
            int result = reduce_threads ? decide_default(true) : decide_default(false);

            // exceed the time limit, reduce the threads number
            if (reduce_threads && result != 1) {
                std::cout << "\n============================================\n";
                std::cout << "Consider all threads\n";
                std::cout << "============================================\n";
                equation.SSA_steps.clear();
                equation.choice_symbol_map.clear();
                equation.SSA_steps = SSA_steps;
                equation.order_flag = false;

                if(equation.has_threads())
                {
                    memory_model->set_message_handler(get_message_handler());
                    (*memory_model)(equation);
                }

                result = decide_default();
            }
            return result;
            //================================
        }
    }

    catch(std::string &error_str)
    {
        error(error_str);
        return true;
    }

    catch(const char *error_str)
    {
        error(error_str);
        return true;
    }

    catch(std::bad_alloc)
    {
        error() << "Out of memory" << eom;
        return true;
    }
}

bool has_nondet_loop_cond(std::vector<std::string> loop_cond_ids, std::vector<std::string> nondet_ids, std::vector<std::pair<std::string, std::string>> assignments)
{
//    std::cout << "loop cond ids: ";
//    for(auto id : loop_cond_ids)
//        std::cout << id << " ";
//    std::cout << "\n";
//
//    std::cout << "nondet ids: ";
//    for(auto id : nondet_ids)
//        std::cout << id << " ";
//    std::cout << "\n";
//
//    std::cout << "assignments:\n";
//    for(auto pair: assignments)
//        std::cout << "\t" << pair.first << " " << pair.second << "\n";

    std::set<std::string> loop_cond_id_set(loop_cond_ids.begin(), loop_cond_ids.end());
    std::vector<std::string> unchecked_nondet_ids(nondet_ids);
    std::set<std::string> pushed_nondet_ids(nondet_ids.begin(), nondet_ids.end());

    while(!unchecked_nondet_ids.empty())
    {
        auto nondet_id = unchecked_nondet_ids.back();
        unchecked_nondet_ids.pop_back();

        if(loop_cond_id_set.find(nondet_id) != loop_cond_id_set.end())
            return true;

        for(auto& assignment : assignments)
            if(assignment.second == nondet_id && pushed_nondet_ids.find(assignment.first) == pushed_nondet_ids.end())
            {
                unchecked_nondet_ids.push_back(assignment.first);
                pushed_nondet_ids.insert(assignment.first);
            }
    }
    return false;
}

//---------------------------------------------------------------
// Functions for dynamically set the loop bound
//---------------------------------------------------------------
void bmct::set_unwind_limit(const goto_functionst& goto_functions, value_sett& value_set) {

    // preprocess, if pointer pt is global, and pt = &data, then set data to be global
    value_sett tmp;
    symex_target_equationt equation_pre(ns);
    symex_bmct symex_pre(ns, new_symbol_table, equation_pre);
    symex_pre.set_unwind_limit(1);
    symex_pre.original_for_unwind_limit = goto_functions.for_unwind_limit;
    value_set = symex_pre(goto_functions, tmp);

    // get unwinding info
    setup_unwind();

    //// __FHY_ADD_BEGIN__
    if(options.get_option("unwind")!="") // user set an unwind param
    {
        int max_limit = options.get_unsigned_int_option("unwind");
        symex.set_unwind_limit(max_limit);
    }
    else // no unwind param, use SV-COMP strategy
    {
        // the program has arrays, set the unwind depth to be 2
        int max_limit = 2;

        if(symex_pre.has_array && goto_functions.for_unwind_limit >0 && goto_functions.for_unwind_limit < 20)
            max_limit = goto_functions.for_unwind_limit + 1;
        else if(has_nondet_loop_cond(goto_functions.loop_cond_ids, goto_functions.nondet_ids, symex_pre.assignments))
        {
            std::cout << "no nondet_loop_cond permitted\n";
            std::exit(1);
        }
            //max_limit = 6;
        else if (symex_pre.has_array)
            max_limit = 2;
        else if (symex_pre.has_pthread_cond_wait)
            max_limit = 2;
        else if (goto_functions.for_unwind_limit > 0)
            max_limit = goto_functions.for_unwind_limit + 1;

        symex.set_unwind_limit(max_limit);

        if(max_limit >= 9999 || (goto_functions.for_unwind_limit >= 9999 && symex_pre.has_mutex_array)) {
            std::cout << "too many unwind!\n";
            std::exit(1);
        }
    }

    //// __FHY_ADD_END__


}

// ------------------------------------------------------------
// Functions for reduce threads number
// ------------------------------------------------------------
void bmct::reduce_threads_num(std::auto_ptr<memory_model_baset>& memory_model, std::vector<int>& valid_threads)
{
    std::cout << "reduce the number of those same threads to a half\n";

    std::list<symex_target_equationt::SSA_stept> SSA_steps;
    bool create_invalid_thread = false;
    for(eventst::const_iterator e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end(); e_it++)
    {
        if (!e_it->is_constraint() && valid_threads[e_it->source.thread_nr] == 1)
            SSA_steps.push_back(*e_it);
    }

    equation.SSA_steps.clear();
    equation.choice_symbol_map.clear();
    equation.SSA_steps = SSA_steps;
    equation.order_flag = true;

    // add a partial ordering, if required
    if(equation.has_threads())
    {
        memory_model->set_message_handler(get_message_handler());
        (*memory_model)(equation);
    }
}

bool bmct::identify_same_threads(goto_functionst &goto_functions, std::vector<int>& valid_threads)
{
    bool can_be_reduced = false;
    std::vector<std::vector<int>> same_threads;
    int assignments_num[1000] = {0};
    unsigned max = 0;

    for(eventst::const_iterator e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end(); e_it++)
    {
        if (e_it->is_assignment()) {
            assignments_num[e_it->source.thread_nr]++;
            if (e_it->source.thread_nr >= max)
                max = e_it->source.thread_nr;
        }
    }

    size_t i, j;
    for (i = 1; i <= max; i++) {
        for (j = 0; j < same_threads.size(); j++) {
            if (is_same_thread(goto_functions, i, same_threads[j][0])) {
                same_threads[j].push_back(i);
                break;
            }
        }
        if (j == same_threads.size()) {
            std::vector<int> tmp;
            tmp.push_back(i);
            same_threads.push_back(tmp);
        }
    }

    equation.same_threads_id[0] = -1;
    for (i = 0; i < same_threads.size(); i++){
        for (j = 0; j < same_threads[i].size(); j++)
            equation.same_threads_id[same_threads[i][j]] = i;
    }

    valid_threads.push_back(1);
    for (i = 1; i <= max; i++)
        valid_threads.push_back(0);
    for (i = 0; i < same_threads.size(); i++) {
        if (same_threads[i].size() > 1) {
            for (j = 0; j < same_threads[i].size() / 2; j++)
                valid_threads[same_threads[i][j]] = 1;
            can_be_reduced = true;
        }
        else {
            valid_threads[same_threads[i][0]] = 1;
        }
    }

//    std::cout << "==========================\n";
//	for (i = 0; i < valid_threads.size(); i++)
//		std::cout << valid_threads[i] << " ";
//	std::cout << "\n";
    return can_be_reduced;
}

bool bmct::is_same_thread(goto_functionst &goto_functions, int id1, int id2)
{
    irep_idt func_name1 = equation.thread_function_map[id1];
    irep_idt func_name2 = equation.thread_function_map[id2];
    if (func_name1 == func_name2)
        return true;

    return goto_functions.is_same_func(ns, func_name1, func_name2);
}

bool bmct::has_pthread_join()
{
    for(eventst::const_iterator e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end(); e_it++)
    {
        if (e_it->is_thread_join())
            return true;
    }

    return false;
}

// ------------------------------------------------------------
// Functions for reduce loop bound
// ------------------------------------------------------------
void bmct::reduce_loop_bound(const goto_functionst &goto_functions,
                             std::auto_ptr<memory_model_baset>& memory_model)
{
    std::cout << "reduce unwind of unbounded loops\n";

//	equation.symmetry_flag = true;

//	equation.set_order_flag(true);

    compute_reduced_unwind();

    setup_unwind();

    equation.clear();

    symex(goto_functions);

    equation.slice();

    if(equation.has_threads())
    {
        memory_model->set_message_handler(get_message_handler());
        (*memory_model)(equation);
    }
}

void bmct::compute_reduced_unwind() {
    std::string unwindset_value;
    size_t i;
    for (i = 0; i < symex.unbounded_loops.size() - 1; i++) {
        unwindset_value += as_string(symex.unbounded_loops[i]) + std::string(":1,");
    }
    unwindset_value += as_string(symex.unbounded_loops[i]) + std::string(":1");
    options.set_option("unwindset", unwindset_value.c_str());

    std::cout << "set --unwindset " << unwindset_value << "\n";
}

// ------------------------------------------------------------------
// Functions for writting witness.graphml, for all modes
// ------------------------------------------------------------------

#include <unistd.h>
#include <sys/stat.h>

void bmct::write_true_witness()
{
    std::string witness_filename = "witness.graphml";
    if(options.get_option("graphml-cex") != "")
        witness_filename = options.get_option("graphml-cex");
    std::ofstream out(witness_filename, std::ios::out);

    std::cout << "write true witness to " << witness_filename << "\n";

    write_graphml_true(out, options.get_option("original_file"));
    out.close();
}

void bmct::write_false_witness(const prop_convt &prop_conv, std::vector<irep_idt>& write_order)
{
    goto_tracet goto_trace;
    build_goto_trace_eog(equation, prop_conv, ns, goto_trace, write_order);
//	build_goto_trace(equation, prop_conv, ns, goto_trace);

    goto_trace.output(ns, std::cout);

    graphmlt cex_graph;
    convert(ns, goto_trace, cex_graph);

    std::string witness_filename = "witness.graphml";
    if(options.get_option("graphml-cex") != "")
        witness_filename = options.get_option("graphml-cex");
    std::ofstream out(witness_filename, std::ios::out);

    std::cout << "write false witness to " << witness_filename << "\n";

    write_graphml_false(cex_graph, out, options.get_option("original_file"));
    out.close();
}

void bmct::report_success_witness()
{
    result() << "VERIFICATION SUCCESSFUL" << eom;

    write_true_witness();

    switch(ui)
    {
        case ui_message_handlert::PLAIN:
            break;

        case ui_message_handlert::XML_UI:
        {
            xmlt xml("cprover-status");
            xml.data="SUCCESS";
            std::cout << xml;
            std::cout << "\n";
        }
            break;

        default:
            assert(false);
    }
}

void bmct::report_failure_witness(const prop_convt &prop_conv, std::vector<irep_idt>& write_order)
{
    result() << "VERIFICATION FAILED\n" << eom;

    status() << "Building error trace: report failure witness\n" << eom;

    write_false_witness(prop_conv, write_order);

    switch(ui)
    {
        case ui_message_handlert::PLAIN:
            break;

        case ui_message_handlert::XML_UI:
        {
            xmlt xml("cprover-status");
            xml.data="FAILURE";
            std::cout << xml;
            std::cout << "\n";
        }
            break;

        default:
            assert(false);
    }
}
