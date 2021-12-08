// __SZH_ADD_BEGIN__

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

#include <solvers/sat/satcheck.h>
#include <solvers/sat/satcheck_minisat2.h>

#include <solvers/refinement/bv_refinement.h>
#include <solvers/smt1/smt1_dec.h>
#include <solvers/smt2/smt2_dec.h>
#include <solvers/cvc/cvc_dec.h>
#include <util/time_stopping.h>

#include <solvers/prop/aig_prop.h>

#include "counterexample_beautification.h"
#include "version.h"
#include "eog.h"
#include <iostream>
#include <iomanip>
#include <vector>

#include "bmc.h"
#include "bv_cbmc.h"

#include "../../minisat-2.2.1/minisat/core/ClosureSolver.h"

bool bmct::decide_closure()
{
    auto solver = std::auto_ptr<satcheck_minisat_closuret>(new satcheck_minisat_closuret);
    bv_cbmct bv_cbmc(ns, *solver);

    if(options.get_option("arrays-uf")=="never")
    {
        bv_cbmc.unbounded_array = bv_cbmct::U_NONE;
        std::cout << "arrays-uf is never\n";
    }
    else if(options.get_option("arrays-uf")=="always")
    {
        bv_cbmc.unbounded_array = bv_cbmct::U_ALL;
        std::cout << "arrays-uf is always\n";
    }

    // Use EOG to compute the first constraints
    exprt init_cons;

    compute_init_refinement(bv_cbmc, init_cons);
    bv_cbmc.set_to_true(init_cons);

    // std::cout << "see the equation!\n";
    // equation.output(std::cout);
    // std::cout << "finish seeing!\n";

    //set constraint
//    do_unwind_module(bv_cbmc);
//    equation.convert(bv_cbmc);
//
//    forall_expr_list(it, bmc_constraints)
//        bv_cbmc.set_to_true(*it);
    do_conversion(bv_cbmc);

    //set graph
    oclt_convert_tablet oclt_convert_table;

    for(auto& entry: equation.oclt_type_table_graph)
    {
        if(entry.kind_name == "coi" || entry.kind_name == "coe") //ignore co
            continue;

        literalt l = bv_cbmc.convert(entry.decide_var_expr);

        Minisat::Lit l_final = Minisat::mkLit(l.var_no(), l.sign());

        oclt_convert_table.push_back(std::make_pair(std::make_pair(entry.u_name, entry.v_name), std::make_pair(l_final, entry.kind_name)));
    }

    oclt_guard_mapt oclt_write_guard;
    oclt_location_mapt oclt_write_location;
    for(auto& e_it: equation.oclt_write_map)
    {
        std::string name = id2string(e_it->ssa_lhs.get_identifier());
        int location = 0;
        if(e_it->source.is_set)
            location = std::atoi(e_it->source.pc->source_location.get_line().c_str());
        //literalt guard_literal = e_it->guard_literal;
        literalt guard_literal = bv_cbmc.convert(e_it->guard);
        Minisat::Lit l_final = Minisat::mkLit(guard_literal.var_no(), guard_literal.sign());
        oclt_write_guard.insert(std::make_pair(name, l_final));
        oclt_write_location.insert(std::make_pair(name, location));
    }

    solver->get_solver()->save_raw_graph(oclt_convert_table, oclt_write_guard, oclt_write_location);

    //solve
    std::cout << "decide_closure call prop_solve of solver\n";
    //auto result = solver->prop_solve();
    auto result = run_decision_procedure(bv_cbmc);

    switch(result)
    {
    case propt::P_SATISFIABLE:
    {
        std::cout << "closure solver says sat\n";

        auto write_order_string = solver->get_solver()->get_write_order();
        std::vector<irep_idt> write_order;
        for(auto& str : write_order_string)
            write_order.push_back(str);

        // for(auto& name: write_order)
        //     std::cout << "\t" << name << "\n";

        report_failure_witness(bv_cbmc, write_order);
        return true;
    }
    case propt::P_UNSATISFIABLE:
        std::cout << "closure solver says unsat\n";
        report_success_witness();
        return false;
    case propt::P_ERROR:
        std::cout << "ERROR: closure solver fails\n";
        return false;
    }

}

// __SZH_ADD_END__