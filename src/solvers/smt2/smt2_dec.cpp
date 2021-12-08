/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <cstdlib>

#if defined(__linux__) || \
    defined(__FreeBSD_kernel__) || \
    defined(__GNU__) || \
    defined(__unix__) || \
    defined(__CYGWIN__) || \
    defined(__MACH__)
#include <unistd.h>
#endif

#include <util/std_expr.h>
#include <util/std_types.h>
#include <util/tempfile.h>
#include <util/arith_tools.h>
#include <util/i2string.h>
#include <util/ieee_float.h>

#include "smt2_dec.h"
#include "smt2irep.h"

/*******************************************************************\

Function: smt2_dect::decision_procedure_text

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

std::string smt2_dect::decision_procedure_text() const
{
    return "SMT2 "+logic+
           (use_FPA_theory?" (with FPA)":"")+
           " using "+
           (solver==GENERIC?"Generic":
            solver==BOOLECTOR?"Boolector":
            solver==CVC3?"CVC3":
            solver==CVC4?"CVC3":
            solver==MATHSAT?"MathSAT":
            solver==OPENSMT?"OpenSMT":
            solver==YICES?"Yices":
            solver==Z3?"Z3":
            "(unknown)");
}

/*******************************************************************\

Function: smt2_temp_filet::smt2_temp_filet

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

smt2_temp_filet::smt2_temp_filet()
{
    temp_out_filename=get_temporary_file("smt2_dec_out_", "");

    temp_out.open(
            temp_out_filename.c_str(),
            std::ios_base::out | std::ios_base::trunc
    );
}

/*******************************************************************\

Function: smt2_temp_filet::~smt2_temp_filet

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

smt2_temp_filet::~smt2_temp_filet()
{
    temp_out.close();

    if(temp_out_filename!="")
        unlink(temp_out_filename.c_str());

    if(temp_result_filename!="")
        unlink(temp_result_filename.c_str());
}

/*******************************************************************\

Function: smt2_dect::dec_solve

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

decision_proceduret::resultt smt2_dect::dec_solve()
{
    // this closes the SMT2 file

  //// __FHY_ADD_BEGIN__
#if use_node_guard
    set_oclt_node_guard();
#endif
  //// __FHY_ADD_END__

    write_footer();
    temp_out.close();

    temp_result_filename=
            get_temporary_file("smt2_dec_result_", "");

    std::string command;
    //// __FHY_ADD_BEGIN__
    char buffer[64];
    getcwd(buffer, 64);
    std::string z3_path = buffer;

    z3_path += "/z3-210427";

    //// __FHY_ADD_END__

    switch(solver)
    {
        case BOOLECTOR:
            command = "boolector --smt2 "
                      + temp_out_filename
                      + " -m --output "
                      + temp_result_filename;
            break;

        case CVC3:
            command = "cvc3 +model -lang smtlib -output-lang smtlib "
                      + temp_out_filename
                      + " > "
                      + temp_result_filename;
            break;

        case CVC4:
            command = "cvc4 -L smt2 "
                      + temp_out_filename
                      + " > "
                      + temp_result_filename;
            break;

        case MATHSAT:
            // The options below were recommended by Alberto Griggio
            // on 10 July 2013
            command = "mathsat -input=smt2 \
 -preprocessor.toplevel_propagation=true \
 -preprocessor.simplification=7 \
 -dpll.branching_random_frequency=0.01 \
 -dpll.branching_random_invalidate_phase_cache=true \
 -dpll.restart_strategy=3 \
 -dpll.glucose_var_activity=true \
 -dpll.glucose_learnt_minimization=true \
 -theory.bv.eager=true \
 -theory.bv.bit_blast_mode=1 \
 -theory.bv.delay_propagated_eqs=true \
 -theory.fp.mode=1 \
 -theory.fp.bit_blast_mode=2 \
 -theory.arr.mode=1"
                      " < "+temp_out_filename
                      + " > "+temp_result_filename;
            break;

        case OPENSMT:
            command = "opensmt "
                      + temp_out_filename
                      + " > "
                      + temp_result_filename;
            break;


        case YICES:
            //    command = "yices -smt -e "   // Calling convention for older versions
            command = "yices-smt2 "  //  Calling for 2.2.1
                      + temp_out_filename
                      + " > "
                      + temp_result_filename;
            break;

        case Z3:
            //// __FHY_ADD_BEGIN__
            command = z3_path
                      + " -smt2 -st "
                      + temp_out_filename
                      + " | tee "
                      + temp_result_filename;
            //// __FHY_ADD_END__
            break;

        default:
            assert(false);
    }

#if defined(__linux__) || defined(__APPLE__)
    command+=" 2>&1";
#endif

    int res=system(command.c_str());
    if(res<0)
    {
        error() << "error running SMT2 soler" << eom;
        return decision_proceduret::D_ERROR;
    }

    std::ifstream in(temp_result_filename.c_str());

    create_graph(in);

    in.close();
    in.open(temp_result_filename.c_str());

    return read_result(in);
}

// __SZH_ADD_BEGIN__
#include <regex>

void smt2_dect::create_graph(std::istream& in)
{
    std::string line;
    while(std::getline(in, line)) //read graph
    {
        std::cout << "create_graph get " << line << "\n";

        std::smatch sm;
        if(std::regex_match(line, sm, std::regex("\\(oclt ([^ ]*) ([^ ]*)\\)")))
        {
            std::cout << "add edge " << sm[1] << " " << sm[2] << "\n";

            std::string from_name = sm[1];
            std::string to_name = sm[2];

            if(name_to_id.find(from_name) == name_to_id.end())
            {
                name_to_id[from_name] = smt2_graph_name.size();
                smt2_graph_name.push_back(from_name);
                smt2_graph_ins.push_back(std::set<int>());
                smt2_graph_outs.push_back(std::set<int>());
            }
            int from_id = name_to_id[from_name];

            if(name_to_id.find(to_name) == name_to_id.end())
            {
                name_to_id[to_name] = smt2_graph_name.size();
                smt2_graph_name.push_back(to_name);
                smt2_graph_ins.push_back(std::set<int>());
                smt2_graph_outs.push_back(std::set<int>());
            }
            int to_id = name_to_id[to_name];

            smt2_graph_outs[from_id].insert(to_id);
            smt2_graph_ins[to_id].insert(from_id);
        }
    }

    //topological sort
    std::vector<int> degree_zeros;
    for(int i = 0; i < smt2_graph_name.size(); i++)
        if(smt2_graph_ins[i].empty())
            degree_zeros.push_back(i);

    while(!degree_zeros.empty())
    {
        int degree_zero = degree_zeros.back();
        degree_zeros.pop_back();

        std::string name = smt2_graph_name[degree_zero];

        int new_id = smt2_graph_name_sorted.size();
        smt2_graph_name_sorted.push_back(name);
        name_to_id_sorted[name] = new_id;

        for(auto out : smt2_graph_outs[degree_zero])
        {
            smt2_graph_ins[out].erase(degree_zero);
            if(smt2_graph_ins[out].empty())
                degree_zeros.push_back(out);
        }
    }

    for(auto name: smt2_graph_name_sorted)
    {
        if(name.find("__CPROVER") != std::string::npos)
            continue;
        
        if(name.find("argv'") != std::string::npos)
            continue;

        if(name.find("$wclk") == std::string::npos)
            continue;

        std::string subname = name.substr(0, name.find("$"));
        write_order.push_back(subname);
    }
}

// __SZH_ADD_END__

/*******************************************************************\

Function: smt2_dect::read_result

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

decision_proceduret::resultt smt2_dect::read_result(std::istream &in)
{
    std::string line;
    decision_proceduret::resultt res=D_ERROR;

    boolean_assignment.clear();
    boolean_assignment.resize(no_boolean_variables, false);

    typedef hash_map_cont<irep_idt, irept, irep_id_hash> valuest;
    valuest values;

    while(in)
    {
        irept parsed=smt2irep(in);

        if(parsed.id()=="sat")
            res=D_SATISFIABLE;
        else if(parsed.id()=="unsat")
            res=D_UNSATISFIABLE;
        else if(parsed.id()=="" &&
                parsed.get_sub().size()==1 &&
                parsed.get_sub().front().get_sub().size()==2)
        {
            const irept &s0=parsed.get_sub().front().get_sub()[0];
            const irept &s1=parsed.get_sub().front().get_sub()[1];

            // ( (B0 true) )
            // ( (|c::__CPROVER_pipe_count#1| (_ bv0 32)) )

            values[s0.id()]=s1;
        }
    }

    for(identifier_mapt::iterator
                it=identifier_map.begin();
        it!=identifier_map.end();
        it++)
    {
        std::string conv_id=convert_identifier(it->first);
        const irept &value=values[conv_id];
        it->second.value=parse_rec(value, it->second.type);
    }

    // Booleans
    for(unsigned v=0; v<no_boolean_variables; v++)
    {
        const irept &value=values["B"+i2string(v)];
        boolean_assignment[v]=(value.id()==ID_true);
    }

    return res;
}

