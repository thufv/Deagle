/*******************************************************************\

Module: Traces of GOTO Programs

Author: Daniel Kroening

  Date: July 2005

\*******************************************************************/

#include <cassert>
#include <ostream>

#include <util/arith_tools.h>
#include <util/symbol.h>

#include <ansi-c/printf_formatter.h>
#include <langapi/language_util.h>

#include "goto_trace.h"

/*******************************************************************\

Function: goto_tracet::output

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_tracet::output(
  const class namespacet &ns,
  std::ostream &out) const
{
  for(stepst::const_iterator it=steps.begin();
      it!=steps.end();
      it++)
    it->output(ns, out);
}

/*******************************************************************\

Function: goto_tracet::output

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void goto_trace_stept::output(
  const namespacet &ns,
  std::ostream &out) const
{
  out << "*** ";

  if(!pc->source_location.is_nil())
    out << pc->source_location << "\n";

  switch(type)
  {
  case goto_trace_stept::ASSERT: out << "ASSERT"; break;
  case goto_trace_stept::ASSUME: out << "ASSUME"; break;
  case goto_trace_stept::LOCATION: out << "LOCATION"; break;
  case goto_trace_stept::ASSIGNMENT: out << "ASSIGNMENT"; break;
  case goto_trace_stept::DECL: out << "DECL"; break;
  case goto_trace_stept::OUTPUT: out << "OUTPUT"; break;
  case goto_trace_stept::INPUT: out << "INPUT"; break;
  case goto_trace_stept::ATOMIC_BEGIN: out << "ATOMC_BEGIN"; break;
  case goto_trace_stept::ATOMIC_END: out << "ATOMIC_END"; break;
  case goto_trace_stept::SHARED_READ: out << "SHARED_READ"; break;
  case goto_trace_stept::SHARED_WRITE: out << "ASSIGNMENT"; break;
  case goto_trace_stept::FUNCTION_CALL: out << "FUNCTION CALL"; break;
  case goto_trace_stept::FUNCTION_RETURN: out << "FUNCTION RETURN"; break;
  default: assert(false);
  }

  if(type==ASSERT || type==ASSUME)
    out << " (" << cond_value << ")";

  out << " ";

  if(pc->is_other() || pc->is_assign())
  {
    irep_idt identifier=lhs_object.get_identifier();
    out << "  " << identifier
        << " = " << from_expr(ns, identifier, lhs_object_value)
        << "\n";
  }
  else if(pc->is_assert())
  {
    if(!cond_value)
    {
      out << "Violated property:" << "\n";
      if(pc->source_location.is_nil())
        out << "  " << pc->source_location << "\n";
      
      if(comment!="")
        out << "  " << comment << "\n";
      out << "  " << from_expr(ns, "", pc->guard) << "\n";
      out << "\n";
    }
  }
  else if(is_function_call())
	  out << identifier << "\n";
  
  out << "\n";
}

/*******************************************************************\

Function: counterexample_value_binary

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

std::string counterexample_value_binary(
  const exprt &expr,
  const namespacet &ns)
{
  const typet &type=ns.follow(expr.type());
  
  if(expr.id()==ID_constant)
  {
    if(type.id()==ID_unsignedbv ||
       type.id()==ID_signedbv ||
       type.id()==ID_bv ||
       type.id()==ID_fixedbv ||
       type.id()==ID_floatbv ||
       type.id()==ID_pointer ||
       type.id()==ID_c_enum ||
       type.id()==ID_c_enum_tag)
    {
      return expr.get_string(ID_value);
    }
    else if(type.id()==ID_bool)
    {
      return expr.is_true()?"1":"0";
    }
  }
  else if(expr.id()==ID_array)
  {
    std::string result;
  
    forall_operands(it, expr)
    {
      if(result=="") result="{ "; else result+=", ";
      result+=counterexample_value_binary(*it, ns);
    }
      
    return result+" }";
  }
  else if(expr.id()==ID_struct)
  {
    std::string result="{ ";
  
    forall_operands(it, expr)
    {
      if(it!=expr.operands().begin()) result+=", ";
      result+=counterexample_value_binary(*it, ns);
    }
      
    return result+" }";
  }
  else if(expr.id()==ID_union)
  { 
    assert(expr.operands().size()==1);
    return counterexample_value_binary(expr.op0(), ns);
  }

  return "?";
}

/*******************************************************************\

Function: counterexample_value

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void counterexample_value(
  std::ostream &out,
  const namespacet &ns,
  const symbol_exprt &lhs_object,
  const exprt &full_lhs,
  const exprt &value)
{
  const irep_idt &identifier=lhs_object.get_identifier();
  std::string value_string;
  
  if(value.is_nil())
    value_string="(assignment removed)";
  else
  {
    value_string=from_expr(ns, identifier, value);

    // the binary representation
    value_string+=" ("+counterexample_value_binary(value, ns)+")";
  }

  out << "  "
      << from_expr(ns, identifier, full_lhs)
      << "=" << value_string
      << "\n";
}

/*******************************************************************\

Function: show_state_header

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void show_state_header(
  std::ostream &out,
  const goto_trace_stept &state,
  const source_locationt &source_location,
  unsigned step_nr)
{
  out << "\n";
  
  if(step_nr==0)
    out << "Initial State";
  else
    out << "State " << step_nr;
  
  out << " " << source_location
      << " thread " << state.thread_nr << "\n";
  out << "----------------------------------------------------" << "\n";
}

/*******************************************************************\

Function: is_index_member_symbol

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool is_index_member_symbol(const exprt &src)
{
  if(src.id()==ID_index)
    return is_index_member_symbol(src.op0());
  else if(src.id()==ID_member)
    return is_index_member_symbol(src.op0());
  else if(src.id()==ID_symbol)
    return true;
  else
    return false;
}

/*******************************************************************\

Function: show_goto_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void show_goto_trace_backup(
  std::ostream &out,
  const namespacet &ns,
  const goto_tracet &goto_trace)
{
  unsigned prev_step_nr=0;
  bool first_step=true;

  for(goto_tracet::stepst::const_iterator
      it=goto_trace.steps.begin();
      it!=goto_trace.steps.end();
      it++)
  {
    switch(it->type)
    {
    case goto_trace_stept::ASSERT:
      if(!it->cond_value)
      {
        out << "\n";
        out << "Violated property:" << "\n";
        if(!it->pc->source_location.is_nil())
          out << "  " << it->pc->source_location << "\n";
        out << "  " << it->comment << "\n";

        if(it->pc->is_assert())
          out << "  " << from_expr(ns, "", it->pc->guard) << "\n";
        
        out << "\n";
      }
      break;
      
    case goto_trace_stept::ASSUME:
      break;
      
    case goto_trace_stept::LOCATION:
      break;

    case goto_trace_stept::ASSIGNMENT:
      if(it->pc->is_assign() ||
         it->pc->is_return() || // returns have a lhs!
         it->pc->is_function_call() ||
         (it->pc->is_other() && it->lhs_object.is_not_nil()))
      {
        if(prev_step_nr!=it->step_nr || first_step)
        {
          first_step=false;
          prev_step_nr=it->step_nr;
          show_state_header(out, *it, it->pc->source_location, it->step_nr);
        }

        // see if the full lhs is something clean
        if(is_index_member_symbol(it->full_lhs))
          counterexample_value(out, ns, it->lhs_object, it->full_lhs, it->full_lhs_value);
        else
          counterexample_value(out, ns, it->lhs_object, it->lhs_object, it->lhs_object_value);
      }
      break;

    case goto_trace_stept::DECL:
      if(prev_step_nr!=it->step_nr || first_step)
      {
        first_step=false;
        prev_step_nr=it->step_nr;
        show_state_header(out, *it, it->pc->source_location, it->step_nr);
      }

      counterexample_value(out, ns, it->lhs_object, it->full_lhs, it->full_lhs_value);
      break;

    case goto_trace_stept::OUTPUT:
      if(it->formatted)
      {
        printf_formattert printf_formatter(ns);
        printf_formatter(id2string(it->format_string), it->io_args);
        printf_formatter.print(out);
        out << "\n";
      }
      else
      {
        show_state_header(out, *it, it->pc->source_location, it->step_nr);
        out << "  OUTPUT " << it->io_id << ":";

        for(std::list<exprt>::const_iterator
            l_it=it->io_args.begin();
            l_it!=it->io_args.end();
            l_it++)
        {
          if(l_it!=it->io_args.begin()) out << ";";
          out << " " << from_expr(ns, "", *l_it);

          // the binary representation
          out << " (" << counterexample_value_binary(*l_it, ns) << ")";
        }
      
        out << "\n";
      }
      break;

    case goto_trace_stept::INPUT:
      show_state_header(out, *it, it->pc->source_location, it->step_nr);
      out << "  INPUT " << it->io_id << ":";

      for(std::list<exprt>::const_iterator
          l_it=it->io_args.begin();
          l_it!=it->io_args.end();
          l_it++)
      {
        if(l_it!=it->io_args.begin()) out << ";";
        out << " " << from_expr(ns, "", *l_it);

        // the binary representation
        out << " (" << counterexample_value_binary(*l_it, ns) << ")";
      }
      
      out << "\n";
      break;
      
    case goto_trace_stept::FUNCTION_CALL:
    case goto_trace_stept::FUNCTION_RETURN:
    case goto_trace_stept::SPAWN:
    case goto_trace_stept::MEMORY_BARRIER:
    case goto_trace_stept::ATOMIC_BEGIN:
    case goto_trace_stept::ATOMIC_END:
    case goto_trace_stept::DEAD:
      break;
    
    case goto_trace_stept::CONSTRAINT:
      assert(false);
      break;
      
    case goto_trace_stept::SHARED_READ:
    case goto_trace_stept::SHARED_WRITE:
      assert(false);
      break;
      
    default:
      assert(false);
    }
  }
}

/*******************************************************************\

Function: show_goto_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void show_goto_trace(
  std::ostream &out,
  const namespacet &ns,
  const goto_tracet &goto_trace)
{
  unsigned prev_step_nr=0;
  bool first_step=true;

  for(goto_tracet::stepst::const_iterator
      it=goto_trace.steps.begin();
      it!=goto_trace.steps.end();
      it++)
  {
    switch(it->type)
    {
    case goto_trace_stept::ASSERT:
      if(!it->cond_value)
      {
        out << "\n";
        out << "Violated property:" << "\n";
        if(!it->pc->source_location.is_nil())
          out << "  " << it->pc->source_location << "\n";
        out << "  " << it->comment << "\n";

        if(it->pc->is_assert())
          out << "  " << from_expr(ns, "", it->pc->guard) << "\n";

        out << "\n";
      }
      break;

    case goto_trace_stept::ASSUME:
      break;

    case goto_trace_stept::LOCATION:
      break;

    case goto_trace_stept::ASSIGNMENT:
      if(it->pc->is_assign() ||
         it->pc->is_return() || // returns have a lhs!
         it->pc->is_function_call() ||
         (it->pc->is_other() && it->lhs_object.is_not_nil()))
      {
        if(prev_step_nr!=it->step_nr || first_step)
        {
          first_step=false;
          prev_step_nr=it->step_nr;
          show_state_header(out, *it, it->pc->source_location, it->step_nr);
        }

        // see if the full lhs is something clean
        if(is_index_member_symbol(it->full_lhs))
          counterexample_value(out, ns, it->lhs_object, it->full_lhs, it->full_lhs_value);
        else
          counterexample_value(out, ns, it->lhs_object, it->lhs_object, it->lhs_object_value);
      }
      break;

    case goto_trace_stept::DECL:
      if(prev_step_nr!=it->step_nr || first_step)
      {
        first_step=false;
        prev_step_nr=it->step_nr;
        show_state_header(out, *it, it->pc->source_location, it->step_nr);
      }

      counterexample_value(out, ns, it->lhs_object, it->full_lhs, it->full_lhs_value);
      break;

    case goto_trace_stept::OUTPUT:
      if(it->formatted)
      {
        printf_formattert printf_formatter(ns);
        printf_formatter(id2string(it->format_string), it->io_args);
        printf_formatter.print(out);
        out << "\n";
      }
      else
      {
        show_state_header(out, *it, it->pc->source_location, it->step_nr);
        out << "  OUTPUT " << it->io_id << ":";

        for(std::list<exprt>::const_iterator
            l_it=it->io_args.begin();
            l_it!=it->io_args.end();
            l_it++)
        {
          if(l_it!=it->io_args.begin()) out << ";";
          out << " " << from_expr(ns, "", *l_it);

          // the binary representation
          out << " (" << counterexample_value_binary(*l_it, ns) << ")";
        }

        out << "\n";
      }
      break;

    case goto_trace_stept::INPUT:
      show_state_header(out, *it, it->pc->source_location, it->step_nr);
      out << "  INPUT " << it->io_id << ":";

      for(std::list<exprt>::const_iterator
          l_it=it->io_args.begin();
          l_it!=it->io_args.end();
          l_it++)
      {
        if(l_it!=it->io_args.begin()) out << ";";
        out << " " << from_expr(ns, "", *l_it);

        // the binary representation
        out << " (" << counterexample_value_binary(*l_it, ns) << ")";
      }

      out << "\n";
      break;

    default:
      break;
    }
  }
}
