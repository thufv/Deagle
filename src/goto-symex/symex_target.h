/*******************************************************************\

Module: Generate Equation using Symbolic Execution

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_GOTO_SYMEX_SYMEX_TARGET_H
#define CPROVER_GOTO_SYMEX_SYMEX_TARGET_H

#include <list>
#include <map>
#include <string>

#include "../goto-programs/goto_program.h"
#include "../goto-programs/goto_program_template.h"
#include "../util/expr.h"
#include "../util/irep.h"

class symbol_exprt;

class symex_targett
{
public:
  symex_targett():
	  array_thread_id(false),
	  thread_malloc(false),
	  aux_enable(true),
	  thread_thread(false)
  {
  }

  virtual ~symex_targett() { }
  
  struct sourcet
  {
    unsigned thread_nr;
    goto_programt::const_targett pc;
    bool is_set;
  
    sourcet():
      thread_nr(0),
      is_set(false)
    {
    }

    explicit sourcet(
      goto_programt::const_targett _pc):
      thread_nr(0),
      pc(_pc),
      is_set(true)
    {
    }

    explicit sourcet(const goto_programt &_goto_program):
      thread_nr(0),
      pc(_goto_program.instructions.begin()),
      is_set(true)
    {
    }
  };
  
  typedef enum { STATE, HIDDEN, PHI, GUARD } assignment_typet;
  std::map<irep_idt, int> thread_id_map;
  std::map<int, irep_idt> thread_function_map;
  bool array_thread_id;
  bool thread_malloc;
  bool thread_thread;
  
  // added by ylz, denote whether aux_variables is enable in this example,
  // it is true by default, and it is false if there exist loop_bound_exceeded
  // in some state.source.thread_nr > 0
  bool aux_enable;

  // read event
  virtual void shared_read(
    const exprt &guard,
    const symbol_exprt &ssa_rhs,
    const symbol_exprt &original_rhs,
    unsigned atomic_section_id,
    const sourcet &source,
	const bool array_assign = false,
	const bool struct_assign = false)=0;

  // write event
  virtual void shared_write(
    const exprt &guard,
    const symbol_exprt &ssa_rhs,
    const symbol_exprt &original_rhs,
    unsigned atomic_section_id,
    const sourcet &source)=0;

  // write event - lhs must be symbol
  virtual void assignment(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs_object,
    const exprt &ssa_full_lhs,
    const exprt &original_full_lhs,
    const exprt &ssa_rhs,
    const sourcet &source,
    assignment_typet assignment_type)=0;

  // declare fresh variable - lhs must be symbol
  virtual void decl(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs_object,
    const sourcet &source)=0;

  // note the death of a variable - lhs must be symbol
  virtual void dead(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs_object,
    const sourcet &source)=0;

  // record a function call
  virtual void function_call(
    const exprt &guard,
    const irep_idt &identifier,
    const sourcet &source,
	const irep_idt pthread_join_id = "")=0;

  // record return from a function
  virtual void function_return(
    const exprt &guard,
    const irep_idt &identifier,
    const sourcet &source)=0;

  // just record a location
  virtual void location(
    const exprt &guard,
    const sourcet &source)=0;

  // record output
  virtual void output(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &output_id,
    const std::list<exprt> &args)=0;
  
  // record formatted output
  virtual void output_fmt(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &output_id,
    const irep_idt &fmt,
    const std::list<exprt> &args)=0;
  
  // record input
  virtual void input(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &input_id,
    const std::list<exprt> &args)=0;
  
  // record an assumption
  virtual void assumption(
    const exprt &guard,
    const exprt &cond,
    const sourcet &source)=0;

  // record an assertion
  virtual void assertion(
    const exprt &guard,
    const exprt &cond,
    const std::string &msg,
    const sourcet &source)=0;

  // record a constraint
  virtual void constraint(
    const exprt &cond,
    const std::string &msg,
    const sourcet &source)=0;

  // record thread spawn
  virtual void spawn(
    const exprt &guard,
    const sourcet &source)=0;

  // record memory barrier
  virtual void memory_barrier(
    const exprt &guard,
    const sourcet &source)=0;

  // record atomic section
  virtual void atomic_begin(
    const exprt &guard,
    unsigned atomic_section_id,
    const sourcet &source)=0;
  virtual void atomic_end(
    const exprt &guard,
    unsigned atomic_section_id,
    const sourcet &source)=0;
};

bool operator < (
  const symex_targett::sourcet &a,
  const symex_targett::sourcet &b);

#endif
