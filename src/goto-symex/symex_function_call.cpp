/*******************************************************************\

Module: Symbolic Execution of ANSI-C

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <cassert>

#include <util/expr_util.h>
#include <util/i2string.h>
#include <util/cprover_prefix.h>
#include <util/prefix.h>
#include <util/arith_tools.h>
#include <util/base_type.h>
#include <util/std_expr.h>
#include <util/symbol_table.h>
#include <iostream>

#include <ansi-c/c_types.h>

#include "goto_symex.h"

/*******************************************************************\

Function: goto_symext::get_unwind_recursion

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool goto_symext::get_unwind_recursion(
        const irep_idt &identifier,
        const unsigned thread_nr,
        unsigned unwind)
{
    return false;
}

/*******************************************************************\

Function: goto_symext::argument_assignments

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::argument_assignments(
        const irep_idt function_identifier,
        const code_typet &function_type,
        statet &state,
        const exprt::operandst &arguments)
{
    // iterates over the operands
    exprt::operandst::const_iterator it1=arguments.begin();

    // these are the types of the arguments
    const code_typet::parameterst &parameter_types=function_type.parameters();

    // iterates over the types of the parameters
    for(code_typet::parameterst::const_iterator
                it2=parameter_types.begin();
        it2!=parameter_types.end();
        it2++)
    {
        // if you run out of actual arguments there was a mismatch
        if(it1==arguments.end())
        {
            std::string error=
                    "call to `"+id2string(function_identifier)+"': "
                                                               "not enough arguments";
            throw state.source.pc->source_location.as_string()+": "+error;
        }

        const code_typet::parametert &parameter=*it2;

        // this is the type the n-th argument should be
        const typet &arg_type=parameter.type();

        const irep_idt &identifier=parameter.get_identifier();

        if(identifier==irep_idt())
            throw "no identifier for function parameter";

        const symbolt &symbol=ns.lookup(identifier);
        symbol_exprt lhs=symbol.symbol_expr();

        if(it1->is_nil())
        {
            // 'nil' argument doesn't get assigned
        }
        else
        {
            exprt rhs=*it1;

            // it should be the same exact type
            if(!base_type_eq(arg_type, rhs.type(), ns))
            {
                const typet &f_arg_type=ns.follow(arg_type);
                const typet &f_rhs_type=ns.follow(rhs.type());

                // we are willing to do some limited conversion
                if((f_arg_type.id()==ID_signedbv ||
                    f_arg_type.id()==ID_unsignedbv ||
                    f_arg_type.id()==ID_bool ||
                    f_arg_type.id()==ID_pointer) &&
                   (f_rhs_type.id()==ID_signedbv ||
                    f_rhs_type.id()==ID_unsignedbv ||
                    f_rhs_type.id()==ID_bool ||
                    f_rhs_type.id()==ID_pointer))
                {
                    rhs.make_typecast(arg_type);
                }
                else
                {
                    std::string error="function call: argument \""+
                                      id2string(identifier)+"\" type mismatch: got "+
                                      it1->type().to_string()+", expected "+
                                      arg_type.to_string();
                    throw error;
                }
            }

            guardt guard;
            state.rename(lhs, ns, goto_symex_statet::L1);
            symex_assign_symbol(state, lhs, nil_exprt(), rhs, guard, VISIBLE);
        }

        it1++;
    }

    if(function_type.has_ellipsis())
    {
        // These are va_arg arguments; their types may differ from call to call
        unsigned va_count=0;
        const symbolt *va_sym=0;
        while(!ns.lookup(id2string(function_identifier)+"::va_arg"+i2string(va_count),
                         va_sym))
            ++va_count;

        for( ; it1!=arguments.end(); it1++, va_count++)
        {
            irep_idt id=id2string(function_identifier)+"::va_arg"+i2string(va_count);

            // add to symbol table
            symbolt symbol;
            symbol.name=id;
            symbol.base_name="va_arg"+i2string(va_count);
            symbol.type=it1->type();

            new_symbol_table.move(symbol);

            symbol_exprt lhs=symbol_exprt(id, it1->type());

            guardt guard;
            state.rename(lhs, ns, goto_symex_statet::L1);
            symex_assign_symbol(state, lhs, nil_exprt(), *it1, guard, VISIBLE);
        }
    }
    else if(it1!=arguments.end())
    {
        // we got too many arguments, but we will just ignore them
    }
}

/*******************************************************************\

Function: goto_symext::symex_function_call

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::symex_function_call(
        const goto_functionst &goto_functions,
        statet &state,
        const code_function_callt &code)
{
    const exprt &function=code.function();

    if(function.id()==ID_symbol)
        symex_function_call_symbol(goto_functions, state, code);
    else if(function.id()==ID_if)
        throw "symex_function_call can't do if";
    else if(function.id()==ID_dereference)
        throw "symex_function_call can't do dereference";
    else
        throw "unexpected function for symex_function_call: "+function.id_string();
}

/*******************************************************************\

Function: goto_symext::symex_function_call_symbol

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::symex_function_call_symbol(
        const goto_functionst &goto_functions,
        statet &state,
        const code_function_callt &code)
{
    target.location(state.guard.as_expr(), state.source);

    assert(code.function().id()==ID_symbol);

    const irep_idt &identifier=
            to_symbol_expr(code.function()).get_identifier();

    if (identifier == "c::pthread_create" && state.source.thread_nr > 0){
        thread_thread = true;
        target.thread_thread = true;
    }
    if (identifier == "c::pthread_cond_wait")
        has_pthread_cond_wait = true;

    if(identifier=="c::CBMC_trace")
    {
        symex_trace(state, code);
    }
    else if(has_prefix(id2string(identifier), CPROVER_FKT_PREFIX))
    {
        symex_fkt(state, code);
    }
    else if(has_prefix(id2string(identifier), CPROVER_MACRO_PREFIX))
    {
        symex_macro(state, code);
    }
    else
        symex_function_call_code(goto_functions, state, code);
}

/*******************************************************************\

Function: goto_symext::symex_function_call_code

  Inputs:

 Outputs:

 Purpose: do function call by inlining

\*******************************************************************/

void goto_symext::symex_function_call_code(
        const goto_functionst &goto_functions,
        statet &state,
        const code_function_callt &call)
{
    const irep_idt &identifier=
            to_symbol_expr(call.function()).get_identifier();

    // find code in function map

    goto_functionst::function_mapt::const_iterator it=
            goto_functions.function_map.find(identifier);

    if(it==goto_functions.function_map.end())
        throw "failed to find `"+id2string(identifier)+"' in function_map";

    const goto_functionst::goto_functiont &goto_function=it->second;

    const bool stop_recursing=get_unwind_recursion(
            identifier,
            state.source.thread_nr,
            state.top().loop_iterations[identifier].count);

    // see if it's too much
    if(stop_recursing)
    {
        if(options.get_bool_option("partial-loops"))
        {
            // it's ok, ignore
        }
        else
        {
            if(options.get_bool_option("unwinding-assertions"))
                claim(false_exprt(), "recursion unwinding assertion", state);

            // add to state guard to prevent further assignments
            state.guard.add(false_exprt());
        }

        state.source.pc++;
        return;
    }

    // record the call
    if (identifier == "c::pthread_create")
    {
        std::string id = from_expr(ns, "", (*(call.arguments().begin())));
        if (id[0] == '&')
            id = id.substr(1, id.size() - 1);
        target.thread_id_map[irep_idt(id)] = state.threads.size();
        target.thread_function_map[state.threads.size()] = "c::" + from_expr(ns, "", call.arguments()[2]);
        target.function_call(state.guard.as_expr(), identifier, state.source, irep_idt(id));
    }

    else if (identifier == "c::pthread_join")
    {
        std::string id = from_expr(ns, "", (*(call.arguments().begin())));

        if (id.find('[', 0) != std::string::npos)
            target.array_thread_id = true;

        target.function_call(state.guard.as_expr(), identifier, state.source, irep_idt(id));
    }
    else
    {
        target.function_call(state.guard.as_expr(), identifier, state.source);
    }

    // added by ylz08
    if (identifier == "c::__VERIFIER_atomic_begin" || identifier == "c::__VERIFIER_atomic_end"
        || (goto_functions.mutex_flag == 0 && (identifier == "c::pthread_mutex_lock" || identifier == "c::pthread_mutex_unlock"))
        || identifier == "c::pthread_cond_wait")
    {
        state.source.pc++;
        return;
    }

    if(!goto_function.body_available)
    {
        no_body(identifier);

        // record the return
        target.function_return(state.guard.as_expr(), identifier, state.source);

        if(call.lhs().is_not_nil())
        {
            side_effect_expr_nondett rhs(call.lhs().type());
            rhs.add_source_location()=call.source_location();
            state.rename(rhs, ns, goto_symex_statet::L1);
            code_assignt code(call.lhs(), rhs);
            symex_assign(state, to_code_assign(code)); /* TODO: clean_expr? */
        }

        state.source.pc++;
        return;
    }

    // read the arguments -- before the locality renaming
    exprt::operandst arguments=call.arguments();
//  for(unsigned i=0; i<arguments.size(); i++)
//    state.rename(arguments[i], ns);

    // __SZH_ADD_BEGIN__
    // added by ylz08: if the fourth argument of pthread_create is a pointer, set it to be global
    if (identifier == "c::pthread_create")
    {
        assert(arguments.size() == 4);
        exprt arg = arguments[3];
//        if (arg.id() == ID_symbol || (arg.id() == ID_typecast && arg.op0().id() == ID_symbol)) {
//            if (arg.id() != ID_symbol)
//                arg = arg.op0();
//            const symbolt& symbol = ns.lookup(state.get_original_name(to_symbol_expr(arg).get_identifier()));
//            if (symbol.type.id() == ID_pointer) {
//                ns.get_symbol(state.get_original_name(to_symbol_expr(arg).get_identifier())).is_thread_local = false;
//                std::cout << "ylz makes a symbol false\n";
//            }
//        }
        std::vector<exprt*> expressions;
        expressions.push_back(&arg);
        while(!expressions.empty())
        {
            auto expr = expressions.back();
            expressions.pop_back();
            if(expr->id() == ID_symbol)
            {
                ns.get_symbol(state.get_original_name(to_symbol_expr(*expr).get_identifier())).is_thread_local = false;
                std::cout << "set symbol " << to_symbol_expr(*expr).get_identifier() << " global\n";
            }
            else
            {
                for(auto& operand: expr->operands())
                    expressions.push_back(&operand);
            }
        }
    }
    // __SZH_ADD_END__

    // produce a new frame
    assert(!state.call_stack().empty());
    goto_symex_statet::framet &frame=state.new_frame();

    // preserve locality of local variables
    locality(identifier, state, goto_function);

    // assign arguments
    argument_assignments(identifier, goto_function.type, state, arguments);

    frame.end_of_function=--goto_function.body.instructions.end();
    frame.return_value=call.lhs();
    frame.calling_location=state.source;
    frame.function_identifier=identifier;

    const goto_symex_statet::framet &p_frame=state.previous_frame();
    for(goto_symex_statet::framet::loop_iterationst::const_iterator
                it=p_frame.loop_iterations.begin();
        it!=p_frame.loop_iterations.end();
        ++it)
        if(it->second.is_recursion)
            frame.loop_iterations.insert(*it);
    // increase unwinding counter
    frame.loop_iterations[identifier].is_recursion=true;
    frame.loop_iterations[identifier].count++;

    state.source.is_set=true;
    state.source.pc=goto_function.body.instructions.begin();
}

/*******************************************************************\

Function: goto_symext::pop_frame

  Inputs:

 Outputs:

 Purpose: pop one call frame

\*******************************************************************/

void goto_symext::pop_frame(statet &state)
{
    assert(!state.call_stack().empty());

    {
        statet::framet &frame=state.top();

        // restore program counter
        state.source.pc=frame.calling_location.pc;

        // restore L1 renaming
        state.level1.restore_from(frame.old_level1);

        // clear function-locals from L2 renaming
        for(statet::framet::local_variablest::const_iterator
                    it=frame.local_variables.begin();
            it!=frame.local_variables.end();
            it++)
        {
            if (!ns.lookup(state.get_original_name(*it)).is_shared())
                state.level2.remove(*it);
        }
    }

    state.pop_frame();
}

/*******************************************************************\

Function: goto_symext::symex_end_of_function

  Inputs:

 Outputs:

 Purpose: do function call by inlining

\*******************************************************************/

void goto_symext::symex_end_of_function(statet &state)
{
    // first record the return
    target.function_return(
            state.guard.as_expr(), state.source.pc->function, state.source);

    // then get rid of the frame
    pop_frame(state);
}

/*******************************************************************\

Function: goto_symext::locality

  Inputs:

 Outputs:

 Purpose: preserves locality of local variables of a given
          function by applying L1 renaming to the local
          identifiers

\*******************************************************************/

void goto_symext::locality(
        const irep_idt function_identifier,
        statet &state,
        const goto_functionst::goto_functiont &goto_function)
{
    unsigned &frame_nr=
            state.threads[state.source.thread_nr].function_frame[function_identifier];
    frame_nr++;

    std::set<irep_idt> local_identifiers;

    get_local_identifiers(goto_function, local_identifiers);

    statet::framet &frame=state.top();

    for(std::set<irep_idt>::const_iterator
                it=local_identifiers.begin();
        it!=local_identifiers.end();
        it++)
    {
        // get L0 name
        irep_idt l0_name=state.rename_identifier(*it, ns, goto_symex_statet::L0);

        // save old L1 name for popping the frame
        statet::level1t::current_namest::const_iterator c_it=
                state.level1.current_names.find(l0_name);

        if(c_it!=state.level1.current_names.end())
            frame.old_level1[l0_name]=c_it->second;

        // do L1 renaming -- these need not be unique, as
        // identifiers may be shared among functions
        // (e.g., due to inlining or other code restructuring)

        irep_idt l1_name;
        unsigned offset=0;

        do
        {
            state.level1.rename_identifier(l0_name, frame_nr+offset);
            l1_name=state.level1(l0_name);
            offset++;
        }
        while(state.l1_history.find(l1_name)!=state.l1_history.end());

        // now unique -- store
        frame.local_variables.insert(l1_name);
        state.l1_history.insert(l1_name);
    }
}

/*******************************************************************\

Function: goto_symext::return_assignment

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::return_assignment(statet &state)
{
    statet::framet &frame=state.top();

    const goto_programt::instructiont &instruction=*state.source.pc;
    assert(instruction.is_return());
    const code_returnt &code=to_code_return(instruction.code);

    target.location(state.guard.as_expr(), state.source);

    if(code.operands().size()==1)
    {
        exprt value=code.op0();

        clean_expr(value, state, false);

        if(frame.return_value.is_not_nil())
        {
            code_assignt assignment(frame.return_value, value);

            if(!base_type_eq(assignment.lhs().type(),
                             assignment.rhs().type(), ns))
                throw "goto_symext::return_assignment type mismatch at "+
                      instruction.source_location.as_string()+":\n"+
                      "assignment.lhs().type():\n"+assignment.lhs().type().pretty()+"\n"+
                      "assignment.rhs().type():\n"+assignment.rhs().type().pretty();

            symex_assign(state, assignment);
        }
    }
    else
    {
        if(frame.return_value.is_not_nil())
            throw "return with unexpected value";
    }
}

/*******************************************************************\

Function: goto_symext::symex_return

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::symex_return(statet &state)
{
    return_assignment(state);

    // we treat this like an unconditional
    // goto to the end of the function

    // put into state-queue
    statet::goto_state_listt &goto_state_list=
            state.top().goto_state_map[state.top().end_of_function];

    goto_state_list.push_back(statet::goto_statet(state));

    // kill this one
    state.guard.make_false();
}

/*******************************************************************\

Function: goto_symext::symex_step_return

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_symext::symex_step_return(statet &state)
{
    return_assignment(state);
}

