/*******************************************************************\

Module: Generate Equation using Symbolic Execution

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_BASIC_SYMEX_EQUATION_H
#define CPROVER_BASIC_SYMEX_EQUATION_H

#include <list>
#include <iosfwd>

#include <util/merge_irep.h>

#include <goto-programs/goto_program.h>
#include <goto-programs/goto_trace.h>

#include <solvers/prop/literal.h>

#include "symex_target.h"
#include "../solvers/prop/literal.h"

class decision_proceduret;
class namespacet;
class prop_convt;
class edge;

// __SZH_ADD_BEGIN__
struct oclt_table_entryt
{
    std::string u_name;
    std::string v_name;
    std::string kind_name;
    std::string decide_var_name;
    exprt decide_var_expr;

    oclt_table_entryt(std::string _u_name, std::string _v_name, std::string _kind_name, std::string _decide_var_name, exprt _decide_var_expr):
            u_name(_u_name), v_name(_v_name), kind_name(_kind_name), decide_var_name(_decide_var_name), decide_var_expr(_decide_var_expr) {}

    bool operator<(const oclt_table_entryt& right) const
    {
        if(u_name != right.u_name)
            return u_name < right.u_name;
        if(v_name != right.v_name)
            return v_name < right.v_name;
        if(kind_name != right.kind_name)
            return kind_name < right.kind_name;
        return decide_var_name < right.decide_var_name;
    }
};
// __SZH_ADD_END__

class symex_target_equationt:public symex_targett
{
public:
  explicit symex_target_equationt(const namespacet &_ns);
  virtual ~symex_target_equationt();

  // read event
  virtual void shared_read(
    const exprt &guard,
    const symbol_exprt &ssa_object,
    const symbol_exprt &original_object,
    unsigned atomic_section_id,
    const sourcet &source,
	const bool array_assign = false,
	const bool struct_assign = false);

  // write event
  virtual void shared_write(
    const exprt &guard,
    const symbol_exprt &ssa_object,
    const symbol_exprt &original_object,
    unsigned atomic_section_id,
    const sourcet &source);

  // assignment to a variable - lhs must be symbol
  virtual void assignment(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs,
    const exprt &ssa_full_lhs,
    const exprt &original_full_lhs,
    const exprt &ssa_rhs,
    const sourcet &source,
    assignment_typet assignment_type);
    
  // declare fresh variable - lhs must be symbol
  virtual void decl(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs_object,
    const sourcet &source);

  // note the death of a variable - lhs must be symbol
  virtual void dead(
    const exprt &guard,
    const symbol_exprt &ssa_lhs,
    const symbol_exprt &original_lhs_object,
    const sourcet &source);

  // record a function call
  virtual void function_call(
    const exprt &guard,
    const irep_idt &identifier,
    const sourcet &source,
	const irep_idt pthread_join_id = "");

  // record return from a function
  virtual void function_return(
    const exprt &guard,
    const irep_idt &identifier,
    const sourcet &source);

  // just record a location
  virtual void location(
    const exprt &guard,
    const sourcet &source);
  
  // output
  virtual void output(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &fmt,
    const std::list<exprt> &args);
  
  // output, formatted
  virtual void output_fmt(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &output_id,
    const irep_idt &fmt,
    const std::list<exprt> &args);
  
  // input
  virtual void input(
    const exprt &guard,
    const sourcet &source,
    const irep_idt &input_id,
    const std::list<exprt> &args);
  
  // record an assumption
  virtual void assumption(
    const exprt &guard,
    const exprt &cond,
    const sourcet &source);

  // record an assertion
  virtual void assertion(
    const exprt &guard,
    const exprt &cond,
    const std::string &msg,
    const sourcet &source);

  // record a (global) constraint
  virtual void constraint(
    const exprt &cond,
    const std::string &msg,
    const sourcet &source);

  // record thread spawn
  virtual void spawn(
    const exprt &guard,
    const sourcet &source);

  // record memory barrier
  virtual void memory_barrier(
    const exprt &guard,
    const sourcet &source);

  // record atomic section
  virtual void atomic_begin(
    const exprt &guard,
    unsigned atomic_section_id,
    const sourcet &source);
  virtual void atomic_end(
    const exprt &guard,
    unsigned atomic_section_id,
    const sourcet &source);

  void convert(prop_convt &prop_conv);
  void convert_assignments(decision_proceduret &decision_procedure) const;
  void convert_decls(prop_convt &prop_conv) const;
  void convert_assumptions(prop_convt &prop_conv);
  void convert_assertions(prop_convt &prop_conv);
  void convert_constraints(decision_proceduret &decision_procedure) const;
  void convert_guards(prop_convt &prop_conv);
  void convert_io(decision_proceduret &decision_procedure);

  exprt make_expression() const;

  class SSA_stept
  {
  public:
    sourcet source;
    goto_trace_stept::typet type;
    
    bool is_assert() const          { return type==goto_trace_stept::ASSERT; }
    bool is_assume() const          { return type==goto_trace_stept::ASSUME; }
    bool is_assignment() const      { return type==goto_trace_stept::ASSIGNMENT; }
    bool is_constraint() const      { return type==goto_trace_stept::CONSTRAINT; }
    bool is_location() const        { return type==goto_trace_stept::LOCATION; }
    bool is_output() const          { return type==goto_trace_stept::OUTPUT; }
    bool is_decl() const            { return type==goto_trace_stept::DECL; }
    bool is_function_call() const   { return type==goto_trace_stept::FUNCTION_CALL; }
    bool is_function_return() const { return type==goto_trace_stept::FUNCTION_RETURN; }
    bool is_shared_read() const     { return type==goto_trace_stept::SHARED_READ; }
    bool is_shared_write() const    { return type==goto_trace_stept::SHARED_WRITE; }
    bool is_spawn() const           { return type==goto_trace_stept::SPAWN; }
    bool is_memory_barrier() const  { return type==goto_trace_stept::MEMORY_BARRIER; }
    bool is_atomic_begin() const    { return type==goto_trace_stept::ATOMIC_BEGIN; }
    bool is_atomic_end() const      { return type==goto_trace_stept::ATOMIC_END; }
    bool is_verify_lock(bool mutex) const {
    	return type == goto_trace_stept::FUNCTION_CALL &&
    			(identifier == "c::__VERIFIER_atomic_begin" || (identifier == "c::pthread_mutex_lock" && !mutex));
    }
    bool is_verify_unlock(bool mutex) const {
    	return type == goto_trace_stept::FUNCTION_CALL &&
    			(identifier == "c::__VERIFIER_atomic_end" || (identifier == "c::pthread_mutex_unlock" && !mutex));
    }
    bool is_verify_atomic_begin(bool mutex) const {
    	return type == goto_trace_stept::FUNCTION_CALL &&
    			(identifier == "c::__VERIFIER_atomic_begin" ||
    			 (identifier == "c::pthread_mutex_lock" && !mutex) ||
    			 identifier == "c::__VERIFIER_atomic_acquire" ||
    			 identifier == "c::__VERIFIER_atomic_malloc_ThreadInfo");
    }
    bool is_verify_atomic_end(bool mutex) const {
    	return type == goto_trace_stept::FUNCTION_CALL &&
    			(identifier == "c::__VERIFIER_atomic_end" ||
    			 (identifier == "c::pthread_mutex_unlock" && !mutex) ||
    			 identifier == "c::__VERIFIER_atomic_release" ||
    			 identifier == "c::__VERIFIER_atomic_free_ThreadInfo");
    }
    bool is_thread_create() const {
    	return type == goto_trace_stept::FUNCTION_CALL && (identifier == "c::pthread_create");
    }
    bool is_thread_create_return() const {
    	return type == goto_trace_stept::FUNCTION_RETURN && (identifier == "c::pthread_create");
    }
    bool is_thread_join() const {
    	return type == goto_trace_stept::FUNCTION_CALL && (identifier == "c::pthread_join");
    }
    bool is_aux_var() const {
    	return (((type == goto_trace_stept::SHARED_READ) || (type == goto_trace_stept::SHARED_WRITE)) &&
    			(original_lhs_object.get_identifier() == "c::__CPROVER_next_thread_id"  ||
    			 original_lhs_object.get_identifier() == "c::__CPROVER_threads_exited"));
    }

    //// __FHY_ADD_BEGIN__
    bool is_redundant_var() const {
        std::string var = id2string(original_lhs_object.get_identifier());
        return var == "c::__CPROVER_deallocated" || var == "c::__CPROVER_dead_object" ||
               var == "c::__CPROVER_malloc_object" || var == "c::__CPROVER_malloc_size" ||
               var == "c::__CPROVER_malloc_is_new_array" || var == "c::__CPROVER_memory_leak" ||
               var == "c::__CPROVER_pipe_count";
    }
    //// __FHY_ADD_END__

      bool is_thread_exited_write() const {
          return ((type == goto_trace_stept::SHARED_WRITE) &&
                  (original_lhs_object.get_identifier() == "c::__CPROVER_threads_exited"));
      }
    
    bool is_program_var() const {
    	if ((type != goto_trace_stept::SHARED_READ) && (type != goto_trace_stept::SHARED_WRITE))
    		return false;
    	std::string name = id2string(original_lhs_object.get_identifier());
    	if (name.find("__CPROVER") != name.npos || name.find("argv") != name.npos)
    		return false;
    	if (name.find("object") != name.npos)
    		return false;
    	return true;
    }

    exprt guard;
    literalt guard_literal;
    unsigned id; //ylz
    bool array_assign; // ylz
    bool struct_assign; //szh

    // for ASSIGNMENT and DECL
    symbol_exprt ssa_lhs, original_lhs_object;
    exprt ssa_full_lhs, original_full_lhs;
    exprt ssa_rhs;
    assignment_typet assignment_type;
    bool event_flag;   // ylz: true if it contains an event
    
    // for ASSUME/ASSERT/CONSTRAINT
    exprt cond_expr; 
    literalt cond_literal;
    std::string comment;
    
    // for INPUT/OUTPUT
    irep_idt format_string, io_id;
    bool formatted;
    std::list<exprt> io_args;
    std::list<exprt> converted_io_args;
    
    // for function call/return
    irep_idt identifier;

    // ylz, for pthread_create, pthread_join
    irep_idt pthread_join_id;

    // for SHARED_READ/SHARED_WRITE and ATOMIC_BEGIN/ATOMIC_END
    unsigned atomic_section_id;
    unsigned appear_ssa_id;  // ylz
    
    // for slicing
    bool ignore;
    bool rely;

    SSA_stept():
      guard(static_cast<const exprt &>(get_nil_irep())),
      ssa_lhs(static_cast<const symbol_exprt &>(get_nil_irep())),
      original_lhs_object(static_cast<const symbol_exprt &>(get_nil_irep())),
      ssa_full_lhs(static_cast<const exprt &>(get_nil_irep())),
      original_full_lhs(static_cast<const exprt &>(get_nil_irep())),
      ssa_rhs(static_cast<const exprt &>(get_nil_irep())),
      cond_expr(static_cast<const exprt &>(get_nil_irep())),
      formatted(false),
      atomic_section_id(0),
	  appear_ssa_id(0),
	  event_flag(false),
	  id(0),
	  array_assign(false),
      ignore(false),
	  rely(false)
    {
    }
    
    void output(
      const namespacet &ns,
      std::ostream &out) const;

    void output_backup(
          const namespacet &ns,
          std::ostream &out) const;
  };
  
  // added by ylz:for slice
  // =============================================================
  typedef std::map<const irep_idt, SSA_stept*> irept_stept_map;
  irept_stept_map shared_read_map;
  irept_stept_map shared_write_map;
  irept_stept_map assignment_map;
  std::vector<irep_idt> rely_symbols;
  void compute_maps();
  void initial_rely_symbols();
  void compute_rely_symbols();
  bool is_shared_read(irep_idt var);
  void compute_address_map();
  void slice();

  // lists of reads and writes per address
  typedef std::vector<SSA_stept*> stept_listt;
  struct a_rect
  {
	  stept_listt reads, writes;
  };

  typedef std::map<irep_idt, a_rect> address_mapt;
  address_mapt address_map;

  bool mutex_flag;	// mark the manner we dispose pthread_mutex_lock
  bool order_flag;			// mark if we should define an unique order for each event
  bool symmetry_flag;		// mark if we should use the symmetry strategy, if events_num >
  int events_num;			// number of events
  int maximum_events_limit; // maximum number of events limit
  std::map<int, int> same_threads_id;

  void set_mutex_flag(bool flag)
  {mutex_flag = flag;}

  void set_order_flag(bool flag)
  {order_flag = flag;}

  void set_symmetry_flag(bool flag)
  {symmetry_flag = flag;}

  void set_events_num(int num)
  {events_num = num;}

  bool exceed_events_limit()
  {return events_num > maximum_events_limit;}
  //==============================================================


  unsigned count_assertions() const
  {
    unsigned i=0;
    for(SSA_stepst::const_iterator
        it=SSA_steps.begin();
        it!=SSA_steps.end(); it++)
      if(it->is_assert()) i++;
    return i;
  }
  
  unsigned count_ignored_SSA_steps() const
  {
    unsigned i=0;
    for(SSA_stepst::const_iterator
        it=SSA_steps.begin();
        it!=SSA_steps.end(); it++)
      if(it->ignore) i++;
    return i;
  }

  typedef std::list<SSA_stept> SSA_stepst;
  SSA_stepst SSA_steps;
	
	//// __FHY_ADD_BEGIN__
	/*
	 * Every element in oclt_symbolt has form:
	 * <<write_event, read_event>, <implies_literal.to_string(), oclt_type>> for example:
	 * < <wx1clk,rx2clk>, "rf1", "rf-order" >
	 * < <wx1clk, ry1clk>, "", "po" >
	 */
	typedef symex_target_equationt::SSA_stepst eventst;
	typedef eventst::const_iterator event_it;
	typedef std::multimap<std::pair<std::string, std::string>, std::pair<std::string, std::string> > oclt_symbolt;
    oclt_symbolt oclt_type_table;

	typedef std::vector<oclt_table_entryt> oclt_symbol_closuret;
    oclt_symbol_closuret oclt_type_table_graph;

    std::map<std::string, exprt> oclt_node_guard_map;
    std::set<event_it> oclt_write_map;
	//// __FHY_ADD_END__

	typedef std::map<event_it, std::string> spawn_namet;
	spawn_namet  spawn_name;
  
  // ylz: for build eog
  class eq_edge{
  public:
	  const SSA_stept* m_src;
	  const SSA_stept* m_dst;
	  eq_edge(const SSA_stept* src, const SSA_stept* dst): m_src(src), m_dst(dst) {}
  };

  typedef std::map<symbol_exprt, eq_edge*> choice_symbol_mapt;
  choice_symbol_mapt choice_symbol_map;

  typedef std::map<edge*, exprt> edge_symbol_mapt;
  edge_symbol_mapt edge_symbol_map;

  SSA_stepst::iterator get_SSA_step(unsigned s)
  {
    SSA_stepst::iterator it=SSA_steps.begin();
    for(; s!=0; s--)
    {
      assert(it!=SSA_steps.end());
      it++;
    }
    return it;
  }

  void output(std::ostream &out) const;

  int hash() const;

  void clear()
  {
    SSA_steps.clear();
    thread_id_map.clear();
    shared_read_map.clear();
    shared_write_map.clear();
    assignment_map.clear();
    rely_symbols.clear();
    address_map.clear();
    choice_symbol_map.clear();
    edge_symbol_map.clear();
	array_thread_id = false;
	thread_malloc = false;
	aux_enable = true;
  }
  
  bool has_threads() const
  {
    for(SSA_stepst::const_iterator it=SSA_steps.begin();
        it!=SSA_steps.end();
        it++)
      if(it->source.thread_nr!=0)
        return true;

    return false;
  }

  const namespacet &ns;
protected:

  // for enforcing sharing in the expressions stored
  merge_irept merge_irep;
  void merge_ireps(SSA_stept &SSA_step);
};

extern inline bool operator<(
  const symex_target_equationt::SSA_stepst::const_iterator a,
  const symex_target_equationt::SSA_stepst::const_iterator b)
{
  return &(*a)<&(*b);
}

std::ostream &operator<<(std::ostream &out, const symex_target_equationt::SSA_stept &step);
std::ostream &operator<<(std::ostream &out, const symex_target_equationt &equation);

#endif
