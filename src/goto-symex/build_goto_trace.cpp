/*******************************************************************\

Module: Traces of GOTO Programs

Author: Daniel Kroening

  Date: July 2005

\*******************************************************************/

#include <cassert>
#include <algorithm>

#include <util/threeval.h>
#include <util/simplify_expr.h>
#include <util/arith_tools.h>

#include <solvers/prop/prop_conv.h>
#include <solvers/prop/prop.h>

#include "partial_order_concurrency.h"

#include "build_goto_trace.h"

/*******************************************************************\

Function: build_full_lhs_rec

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

exprt build_full_lhs_rec(
  const prop_convt &prop_conv,
  const namespacet &ns,
  const exprt &src_original, // original identifiers
  const exprt &src_ssa)      // renamed identifiers
{
  if(src_ssa.id()!=src_original.id())
    return src_original;
    
  const irep_idt id=src_original.id();

  if(id==ID_index)
  {
    // get index value from src_ssa
    exprt index_value=prop_conv.get(to_index_expr(src_ssa).index());
    
    if(index_value.is_not_nil())
    {
      simplify(index_value, ns);
      index_exprt tmp=to_index_expr(src_original);
      tmp.index()=index_value;
      tmp.array()=
        build_full_lhs_rec(prop_conv, ns, 
          to_index_expr(src_original).array(),
          to_index_expr(src_ssa).array());
      return tmp;
    }

    return src_original;
  }
  else if(id==ID_member)
  {
    member_exprt tmp=to_member_expr(src_original);
    tmp.struct_op()=build_full_lhs_rec(
      prop_conv, ns,
      to_member_expr(src_original).struct_op(),
      to_member_expr(src_ssa).struct_op());
  }
  else if(id==ID_if)
  {
    if_exprt tmp2=to_if_expr(src_original);
    
    tmp2.false_case()=build_full_lhs_rec(prop_conv, ns, 
      tmp2.false_case(), to_if_expr(src_ssa).false_case());

    tmp2.true_case()=build_full_lhs_rec(prop_conv, ns,
      tmp2.true_case(), to_if_expr(src_ssa).true_case());

    exprt tmp=prop_conv.get(to_if_expr(src_ssa).cond());

    if(tmp.is_true())
      return tmp2.true_case();
    else if(tmp.is_false())
      return tmp2.false_case();
    else
      return tmp2;
  }
  else if(id==ID_typecast)
  {
    typecast_exprt tmp=to_typecast_expr(src_original);
    tmp.op()=build_full_lhs_rec(prop_conv, ns,
      to_typecast_expr(src_original).op(), to_typecast_expr(src_ssa).op());
    return tmp;
  }
  else if(id==ID_byte_extract_little_endian ||
          id==ID_byte_extract_big_endian)
  {
    exprt tmp=src_original;
    assert(tmp.operands().size()==2);
    tmp.op0()=build_full_lhs_rec(prop_conv, ns, tmp.op0(), src_ssa.op0());

    // re-write into big case-split
    
  }
  
  return src_original;
}

/*******************************************************************\

Function: adjust_lhs_object

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

exprt adjust_lhs_object(
  const prop_convt &prop_conv,
  const namespacet &ns,
  const exprt &src)
{
  return nil_exprt();
}

/*******************************************************************\

Function: build_goto_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void build_goto_trace_backup(
  const symex_target_equationt &target,
  const prop_convt &prop_conv,
  const namespacet &ns,
  goto_tracet &goto_trace)
{
  // We need to re-sort the steps according to their clock.
  // Furthermore, read-events need to occur before write
  // events with the same clock.
  
  typedef std::map<mp_integer, goto_tracet::stepst> time_mapt;
  time_mapt time_map;
  
  mp_integer current_time=0;
  
  for(symex_target_equationt::SSA_stepst::const_iterator
      it=target.SSA_steps.begin();
      it!=target.SSA_steps.end();
      it++)
  {
    const symex_target_equationt::SSA_stept &SSA_step=*it;
    
    if(prop_conv.l_get(SSA_step.guard_literal)!=tvt(true))
      continue;

    if(it->is_constraint() ||
       it->is_spawn())
      continue;
    else if(it->is_atomic_begin())
    {
      // for atomic sections the timing can only be determined once we see
      // a shared read or write (if there is none, the time will be
      // reverted to the time before entering the atomic section); we thus
      // use a temporary negative time slot to gather all events
      current_time*=-1;
      continue;
    }
    else if(it->is_shared_read() || it->is_shared_write() ||
            it->is_atomic_end())
    {
      mp_integer time_before=current_time;

      if(it->is_shared_read() || it->is_shared_write())
      {
        // these are just used to get the time stamp
        exprt clock_value=prop_conv.get(
          symbol_exprt(partial_order_concurrencyt::rw_clock_id(it)));

        to_integer(clock_value, current_time);
      }
      else if(it->is_atomic_end() && current_time<0)
        current_time*=-1;

      assert(current_time>=0);
      // move any steps gathered in an atomic section

      if(time_before<0)
      {
        time_mapt::iterator entry=
          time_map.insert(std::make_pair(
              current_time,
              goto_tracet::stepst())).first;
        entry->second.splice(entry->second.end(), time_map[time_before]);
        time_map.erase(time_before);
      }

      continue;
    }

    // drop hidden assignments
    if(it->is_assignment() &&
       SSA_step.assignment_type!=symex_target_equationt::STATE)
      continue;

    goto_tracet::stepst &steps=time_map[current_time];
    steps.push_back(goto_trace_stept());    
    goto_trace_stept &goto_trace_step=steps.back();
    
    goto_trace_step.thread_nr=SSA_step.source.thread_nr;
    goto_trace_step.pc=SSA_step.source.pc;
    goto_trace_step.comment=SSA_step.comment;
    goto_trace_step.lhs_object=SSA_step.original_lhs_object;
    goto_trace_step.type=SSA_step.type;
    goto_trace_step.format_string=SSA_step.format_string;
    goto_trace_step.io_id=SSA_step.io_id;
    goto_trace_step.formatted=SSA_step.formatted;
    goto_trace_step.identifier=SSA_step.identifier;
    
    if(SSA_step.original_full_lhs.is_not_nil())
      goto_trace_step.full_lhs=
        build_full_lhs_rec(
          prop_conv, ns, SSA_step.original_full_lhs, SSA_step.ssa_full_lhs);
    
    if(SSA_step.ssa_lhs.is_not_nil())
      goto_trace_step.lhs_object_value=prop_conv.get(SSA_step.ssa_lhs);
    
    if(SSA_step.ssa_full_lhs.is_not_nil())
    {
      goto_trace_step.full_lhs_value=prop_conv.get(SSA_step.ssa_full_lhs);
      simplify(goto_trace_step.full_lhs_value, ns);
    }
    
    for(std::list<exprt>::const_iterator
        j=SSA_step.converted_io_args.begin();
        j!=SSA_step.converted_io_args.end();
        j++)
    {
      const exprt &arg=*j;
      if(arg.is_constant() ||
         arg.id()==ID_string_constant)
        goto_trace_step.io_args.push_back(arg);
      else
      {
        exprt tmp=prop_conv.get(arg);
        goto_trace_step.io_args.push_back(tmp);
      }
    }

    if(SSA_step.is_assert() ||
       SSA_step.is_assume())
    {
      goto_trace_step.cond_expr=SSA_step.cond_expr;

      goto_trace_step.cond_value=
        prop_conv.l_get(SSA_step.cond_literal).is_true();
    }
    else if(SSA_step.is_location() &&
            SSA_step.source.pc->is_goto())
    {
      goto_trace_step.cond_expr=SSA_step.source.pc->guard;

      const bool backwards=SSA_step.source.pc->is_backwards_goto();

      symex_target_equationt::SSA_stepst::const_iterator next=it;
      ++next;
      assert(next!=target.SSA_steps.end());

      // goto was taken if backwards and next is enabled or forward
      // and next is not active;
      // there is an ambiguity here if a forward goto is to the next
      // instruction, which we simply ignore for now
      goto_trace_step.goto_taken=
        backwards==
        (prop_conv.l_get(next->guard_literal)==tvt(true));
    }
  }
  
  // Now assemble into a single goto_trace.
  // This expoits sorted-ness of the map.
  for(time_mapt::iterator t_it=time_map.begin();
      t_it!=time_map.end(); t_it++)
  {
    goto_trace.steps.splice(goto_trace.steps.end(), t_it->second);
  }

  // produce the step numbers
  unsigned step_nr=0;
  
  for(goto_tracet::stepst::iterator
      s_it=goto_trace.steps.begin();
      s_it!=goto_trace.steps.end();
      s_it++)
    s_it->step_nr=++step_nr;
  
  // Now delete anything after failed assertion
  for(goto_tracet::stepst::iterator
      s_it1=goto_trace.steps.begin();
      s_it1!=goto_trace.steps.end();
      s_it1++)
    if(s_it1->is_assert() && !s_it1->cond_value)
    {
      s_it1++;

      for(goto_tracet::stepst::iterator
          s_it2=s_it1;
          s_it2!=goto_trace.steps.end();
          s_it2=goto_trace.steps.erase(s_it2));
        
      break;
    }
}


/*******************************************************************\

Function: build_goto_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void build_goto_trace(
  const symex_target_equationt &target,
  const prop_convt &prop_conv,
  const namespacet &ns,
  goto_tracet &goto_trace)
{
  // We need to re-sort the steps according to their clock.
  // Furthermore, read-events need to occur before write
  // events with the same clock.

  for(symex_target_equationt::SSA_stepst::const_iterator
      it=target.SSA_steps.begin();
      it!=target.SSA_steps.end();
      it++)
  {
    const symex_target_equationt::SSA_stept &SSA_step=*it;

    if(prop_conv.l_get(SSA_step.guard_literal)!=tvt(true))
      continue;

    if (!(it->is_shared_write() && it->is_program_var()) && !it->is_function_call() && !it->is_assert())
    	continue;

    const source_locationt &source_location=it->source.pc->source_location;
    if (source_location.is_nil() || source_location.get_line().empty())
    	continue;

    std::string file = id2string(source_location.get_file());
    if (file.empty() || file =="<built-in-additions>" || file.find("<builtin-library>") != file.npos)
    	continue;

    if (it->is_function_call()) {
    	std::string fun_name = id2string(it->identifier);
    	if (fun_name.find("pthread") != fun_name.npos && fun_name != "c::pthread_create")
    		continue;
    }

    goto_tracet::stepst &steps=goto_trace.steps;
    steps.push_back(goto_trace_stept());
    goto_trace_stept &goto_trace_step=steps.back();

    goto_trace_step.thread_nr=SSA_step.source.thread_nr;
    goto_trace_step.pc=SSA_step.source.pc;
    goto_trace_step.comment=SSA_step.comment;
    goto_trace_step.lhs_object=SSA_step.original_lhs_object;
    goto_trace_step.type=SSA_step.type;
    goto_trace_step.format_string=SSA_step.format_string;
    goto_trace_step.io_id=SSA_step.io_id;
    goto_trace_step.formatted=SSA_step.formatted;
    goto_trace_step.identifier=SSA_step.identifier;

    if(SSA_step.original_full_lhs.is_not_nil())
      goto_trace_step.full_lhs=
        build_full_lhs_rec(
          prop_conv, ns, SSA_step.original_full_lhs, SSA_step.ssa_full_lhs);

    if(SSA_step.ssa_lhs.is_not_nil())
      goto_trace_step.lhs_object_value=prop_conv.get(SSA_step.ssa_lhs);

    if(SSA_step.ssa_full_lhs.is_not_nil())
    {
      goto_trace_step.full_lhs_value=prop_conv.get(SSA_step.ssa_full_lhs);
      simplify(goto_trace_step.full_lhs_value, ns);
    }

    for(std::list<exprt>::const_iterator
        j=SSA_step.converted_io_args.begin();
        j!=SSA_step.converted_io_args.end();
        j++)
    {
      const exprt &arg=*j;
      if(arg.is_constant() ||
         arg.id()==ID_string_constant)
        goto_trace_step.io_args.push_back(arg);
      else
      {
        exprt tmp=prop_conv.get(arg);
        goto_trace_step.io_args.push_back(tmp);
      }
    }

    if(SSA_step.is_assert() ||
       SSA_step.is_assume())
    {
      goto_trace_step.cond_expr=SSA_step.cond_expr;

      goto_trace_step.cond_value=
        prop_conv.l_get(SSA_step.cond_literal).is_true();
    }
    else if(SSA_step.is_location() &&
            SSA_step.source.pc->is_goto())
    {
      goto_trace_step.cond_expr=SSA_step.source.pc->guard;

      const bool backwards=SSA_step.source.pc->is_backwards_goto();

      symex_target_equationt::SSA_stepst::const_iterator next=it;
      ++next;
      assert(next!=target.SSA_steps.end());

      // goto was taken if backwards and next is enabled or forward
      // and next is not active;
      // there is an ambiguity here if a forward goto is to the next
      // instruction, which we simply ignore for now
      goto_trace_step.goto_taken=
        backwards==
        (prop_conv.l_get(next->guard_literal)==tvt(true));
    }
  }

  // produce the step numbers
  unsigned step_nr=0;

  for(goto_tracet::stepst::iterator
      s_it=goto_trace.steps.begin();
      s_it!=goto_trace.steps.end();
      s_it++)
    s_it->step_nr=++step_nr;
}

/*******************************************************************\

Function: build_goto_trace

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

int cmp(const goto_trace_stept& a, const goto_trace_stept& b)
{
	int locationA = a.assert_location > 0 ? a.assert_location : a.pc->location_number;
	int locationB = b.assert_location > 0 ? b.assert_location : b.pc->location_number;
	return (a.thread_nr < b.thread_nr) ||
		   ((a.thread_nr == b.thread_nr) && (locationA < locationB));
}

bool has_extract(exprt expr)
{
    std::vector<symbol_exprt> ret;

    std::vector<exprt> tovisit;
    tovisit.push_back(expr);

    while(!tovisit.empty())
    {
        auto visiting = tovisit.back();
        tovisit.pop_back();

        if (visiting.id() == ID_byte_extract_little_endian)
            return true;

        for(auto operand : visiting.operands())
            tovisit.push_back(operand);
    }

    return false;
}

std::string remove_c_prefix(std::string str)
{
    if(str.substr(0, 3) == "c::")
        return str.substr(3);
    return str;
}

void build_goto_trace_eog(
  symex_target_equationt &target,
  const prop_convt &prop_conv,
  const namespacet &ns,
  goto_tracet &goto_trace,
  std::vector<irep_idt>& write_order)
{
  // We need to re-sort the steps according to their clock.
  // Furthermore, read-events need to occur before write
  // events with the same clock.

	int ttt = 0;

  int assert_location = -1;
  std::vector<goto_trace_stept> steps;
  for(symex_target_equationt::SSA_stepst::iterator
      it=target.SSA_steps.begin();
      it!=target.SSA_steps.end();
      it++)
  {
    const symex_target_equationt::SSA_stept &SSA_step=*it;

//	  if (SSA_step.is_shared_write())
//		  std::cout << "CCCCCCC " << SSA_step.ssa_lhs.get_identifier() << " " << SSA_step.source.pc->source_location.get_line() << " " << SSA_step.source.thread_nr << "\n";
//	  else if (SSA_step.is_function_call())
//		  std::cout << "FFFFFFF " << SSA_step.identifier << " " << SSA_step.source.pc->source_location.get_line() << " " << SSA_step.source.thread_nr << "\n";
//	  else if (SSA_step.is_assert())
//		  std::cout << "AAAAAAA " << "ASSERT "  << " " << SSA_step.source.pc->source_location.get_line() << " " << SSA_step.source.thread_nr << "\n";
//	  else
//		  std::cout << "BBBBBBB\n";


    if(prop_conv.l_get(SSA_step.guard_literal)!=tvt(true))
      continue;

    if (!(it->is_shared_write() && it->is_program_var()) && !it->is_function_call() && !it->is_assert())
    	continue;

    //szh add
    if(!it->source.is_set)
      continue;

    const source_locationt &source_location=it->source.pc->source_location;
    if (source_location.is_nil() || source_location.get_line().empty())
    	continue;

    std::string file = id2string(source_location.get_file());
    if (file.empty() || file =="<built-in-additions>" || file.find("<builtin-library>") != file.npos)
    	continue;

    if(it->is_assert() && has_extract(it->cond_expr))
        continue;

    if (it->is_function_call()) {
    	std::string fun_name = id2string(it->identifier);
    	if (fun_name.empty() ||
    		(fun_name.find("pthread") != fun_name.npos && fun_name != "c::pthread_create") ||
    		fun_name.find("c::__VERIFIER_atomic") != fun_name.npos)
    		continue;

    	// szh modified
//    	if (fun_name == "c::__VERIFIER_assert" || fun_name == "c::ldv_assert")
//    	{
//    		assert_location = SSA_step.source.pc->location_number;
//    		continue;
//    	}
    }

    steps.push_back(goto_trace_stept());
    goto_trace_stept &goto_trace_step=steps.back();

    goto_trace_step.thread_nr=SSA_step.source.thread_nr;
    goto_trace_step.pc=SSA_step.source.pc;
    goto_trace_step.comment=SSA_step.comment;
    goto_trace_step.lhs_object=SSA_step.original_lhs_object;
    goto_trace_step.type=SSA_step.type;
    goto_trace_step.format_string=SSA_step.format_string;
    goto_trace_step.io_id=SSA_step.io_id;
    goto_trace_step.formatted=SSA_step.formatted;
    goto_trace_step.identifier=SSA_step.identifier;
    goto_trace_step.ssa_lhs = SSA_step.ssa_lhs;

    if (SSA_step.is_assert() && assert_location > 0)
    {
    	goto_trace_step.assert_location = assert_location;
    	assert_location = -1;
    }

    if(SSA_step.original_full_lhs.is_not_nil())
      goto_trace_step.full_lhs=
        build_full_lhs_rec(
          prop_conv, ns, SSA_step.original_full_lhs, SSA_step.ssa_full_lhs);

    if(SSA_step.ssa_lhs.is_not_nil())
      goto_trace_step.lhs_object_value=prop_conv.get(SSA_step.ssa_lhs);

    if(SSA_step.ssa_full_lhs.is_not_nil())
    {
      goto_trace_step.full_lhs_value=prop_conv.get(SSA_step.ssa_full_lhs);
      simplify(goto_trace_step.full_lhs_value, ns);
    }

    for(std::list<exprt>::const_iterator
        j=SSA_step.converted_io_args.begin();
        j!=SSA_step.converted_io_args.end();
        j++)
    {
      const exprt &arg=*j;
      if(arg.is_constant() ||
         arg.id()==ID_string_constant)
        goto_trace_step.io_args.push_back(arg);
      else
      {
        exprt tmp=prop_conv.get(arg);
        goto_trace_step.io_args.push_back(tmp);
      }
    }

    if(SSA_step.is_assert() ||
       SSA_step.is_assume())
    {
      goto_trace_step.cond_expr=SSA_step.cond_expr;

      goto_trace_step.cond_value=
        prop_conv.l_get(SSA_step.cond_literal).is_true();
    }
    else if(SSA_step.is_location() &&
            SSA_step.source.pc->is_goto())
    {
      goto_trace_step.cond_expr=SSA_step.source.pc->guard;

      const bool backwards=SSA_step.source.pc->is_backwards_goto();

      symex_target_equationt::SSA_stepst::const_iterator next=it;
      ++next;
      assert(next!=target.SSA_steps.end());

      // goto was taken if backwards and next is enabled or forward
      // and next is not active;
      // there is an ambiguity here if a forward goto is to the next
      // instruction, which we simply ignore for now
      goto_trace_step.goto_taken=
        backwards==
        (prop_conv.l_get(next->guard_literal)==tvt(true));
    }
    ttt++;
  }

  // sort according to program order
//  sort(steps.begin(), steps.end(), cmp);

  for(std::vector<goto_trace_stept>::iterator
	s_it=steps.begin();
	s_it!=steps.end();
	s_it++)
  {
//	  if (s_it->is_shared_write()){
//		  std::cout << "CCCCCCC " << s_it->ssa_lhs.get_identifier() << " " << s_it->pc->source_location.get_line() << " " << s_it->thread_nr << "\n";
//	  }
//	  else if (s_it->is_function_call())
//		  std::cout << "FFFFFFF " << s_it->identifier << " " << s_it->pc->source_location.get_line() << " " << s_it->thread_nr << "\n";
//	  else
//		  std::cout << "AAAAAAA " << "ASSERT " << s_it->thread_nr << " " << s_it->pc->source_location.get_line() << " " << s_it->thread_nr << "\n";

//	  if (s_it->is_function_call())
//		  std::cout << s_it->identifier << " " << s_it->thread_nr << " " << s_it->pc->location_number << " " << s_it->pc->source_location.get_line() << "\n";
//	  else if (s_it->is_shared_write())
//		  std::cout << s_it->ssa_lhs.get_identifier() << " "  << s_it->thread_nr << " " << s_it->pc->location_number << " " << s_it->pc->source_location.get_line() << "\n";
//	  else
//		  std::cout << "ASSERT" << " "  << s_it->thread_nr << " " << s_it->pc->location_number << " " << s_it->pc->source_location.get_line() << "\n";
  }

  //=======================================================
  // first order those share_writes according to write_order
  goto_tracet::stepst& nst = goto_trace.steps;
  for (unsigned i = 0; i < write_order.size(); i++)
  {
	  for(std::vector<goto_trace_stept>::iterator
		s_it=steps.begin();
		s_it!=steps.end();
		s_it++)
	  {
		  if (s_it->ssa_lhs.get_identifier() == write_order[i])
		  {
			  nst.push_back(*s_it);
			  break;
		  }
	  }
  }
  goto_tracet::stepst::iterator t_it=nst.begin();
  for(unsigned i = 0; i < steps.size(); i++)
  {
	  if (steps[i].is_function_call() && steps[i].identifier == "c::main")
		  break;
	  steps[i] = *t_it;
	  t_it++;
  }

  if (target.thread_thread){
	  for(std::vector<goto_trace_stept>::iterator
		s_it=steps.begin();
		s_it!=steps.end();)
	  {
		  // judge if s_it has been used
		  if (s_it->is_function_call() && s_it->identifier != "c::pthread_create") {
			  unsigned i;
			  std::string name2 = id2string(s_it->identifier);
			  for (i = 0; i < steps.size(); i++) {
				  std::string name1 = id2string(steps[i].pc->source_location.get_function());
				  if (name1 == name2.substr(3, name2.size() - 3))
					  break;
			  }
			  if (i == steps.size()) {
				  steps.erase(s_it);
				  continue;
			  }
		  }
		  s_it++;
	  }
  }

  // then insert function calls and pthread_creates into nst properly
  std::vector<goto_tracet::stepst::iterator> thread_begin;
  thread_begin.push_back(nst.begin());

  std::map<int, int> threadId_map;

  std::vector<goto_trace_stept> asserts;
  int thread_num = 0;
  for(std::vector<goto_trace_stept>::iterator
	s_it=steps.begin();
	s_it!=steps.end();
	s_it++)
  {
//	  if (s_it->is_assert())
//		  asserts.push_back(*s_it);
        if(s_it->is_function_call())
        {
            if(s_it->identifier == "c::__VERIFIER_assert" || s_it->identifier == "c::ldv_assert")
                asserts.push_back(*s_it);
            if(s_it->identifier == "c::reach_error")
            {
//                std::string caller = remove_c_prefix(s_it->pc->source_location.get_function().c_str());
//                if(caller != "__VERIFIER_assert" && caller != "ldv_assert")
                    asserts.push_back(*s_it);
            }
        }

	  if (threadId_map.find(s_it->thread_nr) == threadId_map.end())
	  {
		  int size = threadId_map.size();
		  threadId_map[s_it->thread_nr] = size;
	  }
	  if (s_it->is_function_call())
	  {
		  // if (s_it->is_function_call())
			//   std::cout << s_it->identifier << " " << s_it->pc->location_number << " " << s_it->thread_nr << "\n";
		  // else if (s_it->is_shared_write())
			//   std::cout << s_it->ssa_lhs.get_identifier() << " " << s_it->pc->location_number << " " << s_it->thread_nr << "\n";
		  // else
			//   std::cout << "ASSERT" << " " << s_it->pc->location_number << " " << s_it->thread_nr << "\n";



      if(s_it->identifier != "c::reach_error" && s_it->identifier != "c::__VERIFIER_assert" && s_it->identifier != "c::ldv_assert")
      {
        if (s_it == steps.begin())
            nst.insert(nst.begin(), *s_it);
        else
        {
          std::vector<goto_trace_stept>::iterator p_it = s_it;
          p_it--;
          if (p_it->is_assert()) {
              p_it--;
          }

          if (s_it->thread_nr == p_it->thread_nr)
          {
            goto_tracet::stepst::iterator t_it;
            for (t_it = nst.begin(); t_it != nst.end(); t_it++)
            {
                if (t_it->ssa_lhs.get_identifier() == p_it->ssa_lhs.get_identifier() &&
                    t_it->pc->source_location.get_line() == p_it->pc->source_location.get_line()) {
                    break;
                }
            }
            assert(t_it != nst.end());
            t_it++;
            nst.insert(t_it, *s_it);

            if (s_it->is_function_call() && id2string(s_it->identifier) == "c::pthread_create")
            {
              thread_begin.push_back(t_it);
            }
          }
          else
          {
            nst.insert(thread_begin[threadId_map[s_it->thread_nr]], *s_it);

            if (s_it->is_function_call() && id2string(s_it->identifier) == "c::pthread_create")
            {
              thread_begin.push_back(thread_begin[threadId_map[s_it->thread_nr]]);
            }
          }
        }
      }
	  }
  }



  // insert main to the proper position
  if (nst.begin()->is_function_call() && nst.begin()->identifier == "c::main") {
	  goto_trace_stept f_main = nst.front();
	  nst.pop_front();
	  for(goto_tracet::stepst::iterator s_it=nst.begin(); s_it!=nst.end(); s_it++)
	  {
		  if (!(s_it->is_shared_write() && s_it->pc->source_location.get_function() == ""))
		  {
			  nst.insert(s_it, f_main);
			  break;
		  }
	  }
  }

  // insert asserts to the proper position
  for (unsigned i = 0; i < asserts.size(); i++)
  {
	  goto_trace_stept& assert_step = asserts[i];

//	  std::cout << "inserting assertion ";
//	  assert_step.output(ns, std::cout);

	  int assert_loc = assert_step.assert_location > 0 ? assert_step.assert_location : assert_step.pc->location_number;
	  std::string caller = remove_c_prefix(assert_step.pc->source_location.get_function().c_str());

      goto_tracet::stepst::iterator begin_it;
      for(begin_it = nst.begin(); begin_it != nst.end(); begin_it++)
      {
          if(begin_it->thread_nr == assert_step.thread_nr && begin_it->is_function_call())
          {
              std::string id = remove_c_prefix(begin_it->identifier.c_str());
              if(id == caller)
              {
                  begin_it++;
                  break;
              }
          }
          if(begin_it->thread_nr == assert_step.thread_nr)
          {
              std::string another_caller = remove_c_prefix(begin_it->pc->source_location.get_function().c_str());
              if(another_caller == caller)
                  break;
          }
      }

	  goto_tracet::stepst::iterator s_it;
      for(s_it = begin_it; s_it!=nst.end(); s_it++)
	  {
		  if (s_it->thread_nr == assert_step.thread_nr && s_it->pc->location_number > assert_loc)
		  {
			  break;
		  }

      //szh add
      if (s_it->thread_nr == assert_step.thread_nr && s_it->pc->location_number == assert_loc - 1)
      {
        s_it++;
        break;
      }
	  }
	  nst.insert(s_it, assert_step);
  }

  //=======================================

//  for(goto_tracet::stepst::iterator
//        s_it=nst.begin();
//        s_it!=nst.end();
//        s_it++)
//  {
//	  if (s_it->is_shared_write()){
//		  std::cout << "CCCCCCCCCCC " << s_it->ssa_lhs.get_identifier() << " " << s_it->pc->source_location.get_function() << "\n";
//		  if (s_it->pc->source_location.get_function() == "")
//			  std::cout << "HHHHHHHH\n";
//	  }
//	  else if (s_it->is_function_call())
//		  std::cout << "CCCCCCCCCCC " << s_it->identifier << " " << s_it->pc->source_location.get_line() << "\n";
//	  else
//		  std::cout << "CCCCCCCCCCC " << "ASSERT\n";
//  }
  //=======================================================

  // produce the step numbers
  unsigned step_nr=0;

  for(goto_tracet::stepst::iterator
      s_it=goto_trace.steps.begin();
      s_it!=goto_trace.steps.end();
      s_it++)
    s_it->step_nr=++step_nr;
}
