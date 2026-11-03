/*******************************************************************\

Module: Symbolic Execution

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

/// \file
/// Symbolic Execution
/// Implementation of functions to build SSA equation.

#include "symex_target_equation.h"

#include <chrono>

#include <util/std_expr.h>
#include <util/find_symbols.h>
#include <util/byte_operators.h>

#include "solver_hardness.h"
#include "ssa_step.h"

static std::function<void(solver_hardnesst &)>
hardness_register_ssa(std::size_t step_index, const SSA_stept &step)
{
  return [step_index, &step](solver_hardnesst &hardness) {
    hardness.register_ssa(step_index, step.cond_expr, step.source.pc);
  };
}

void symex_target_equationt::shared_read(
  const exprt &guard,
  const ssa_exprt &ssa_object,
  unsigned atomic_section_id,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::SHARED_READ);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.ssa_lhs=ssa_object;
  SSA_step.atomic_section_id=atomic_section_id;

  merge_ireps(SSA_step);
}

void symex_target_equationt::shared_write(
  const exprt &guard,
  const ssa_exprt &ssa_object,
  unsigned atomic_section_id,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::SHARED_WRITE);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.ssa_lhs=ssa_object;
  SSA_step.atomic_section_id=atomic_section_id;

  merge_ireps(SSA_step);
}

/// spawn a new thread
void symex_target_equationt::spawn(
  const exprt &guard,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::SPAWN);
  SSA_stept &SSA_step=SSA_steps.back();
  SSA_step.guard=guard;

  merge_ireps(SSA_step);
}

void symex_target_equationt::memory_barrier(
  const exprt &guard,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::MEMORY_BARRIER);
  SSA_stept &SSA_step=SSA_steps.back();
  SSA_step.guard=guard;

  merge_ireps(SSA_step);
}

/// start an atomic section
void symex_target_equationt::atomic_begin(
  const exprt &guard,
  unsigned atomic_section_id,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::ATOMIC_BEGIN);
  SSA_stept &SSA_step=SSA_steps.back();
  SSA_step.guard=guard;
  SSA_step.atomic_section_id=atomic_section_id;

  merge_ireps(SSA_step);
}

/// end an atomic section
void symex_target_equationt::atomic_end(
  const exprt &guard,
  unsigned atomic_section_id,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::ATOMIC_END);
  SSA_stept &SSA_step=SSA_steps.back();
  SSA_step.guard=guard;
  SSA_step.atomic_section_id=atomic_section_id;

  merge_ireps(SSA_step);
}

void symex_target_equationt::assignment(
  const exprt &guard,
  const ssa_exprt &ssa_lhs,
  const exprt &ssa_full_lhs,
  const exprt &original_full_lhs,
  const exprt &ssa_rhs,
  const sourcet &source,
  assignment_typet assignment_type)
{
  PRECONDITION(ssa_lhs.is_not_nil());

  SSA_steps.emplace_back(SSA_assignment_stept{source,
                                              guard,
                                              ssa_lhs,
                                              ssa_full_lhs,
                                              original_full_lhs,
                                              ssa_rhs,
                                              assignment_type});

  merge_ireps(SSA_steps.back());
}

void symex_target_equationt::decl(
  const exprt &guard,
  const ssa_exprt &ssa_lhs,
  const exprt &initializer,
  const sourcet &source,
  assignment_typet assignment_type)
{
  PRECONDITION(ssa_lhs.is_not_nil());

  SSA_steps.emplace_back(source, goto_trace_stept::typet::DECL);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.ssa_lhs=ssa_lhs;
  SSA_step.ssa_full_lhs = initializer;
  SSA_step.original_full_lhs=ssa_lhs.get_original_expr();
  SSA_step.hidden=(assignment_type!=assignment_typet::STATE);

  // the condition is trivially true, and only
  // there so we see the symbols
  SSA_step.cond_expr=equal_exprt(SSA_step.ssa_lhs, SSA_step.ssa_lhs);

  merge_ireps(SSA_step);
}

/// declare a fresh variable
void symex_target_equationt::dead(
  const exprt &,
  const ssa_exprt &,
  const sourcet &)
{
  // we currently don't record these
}

void symex_target_equationt::location(
  const exprt &guard,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::LOCATION);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;

  merge_ireps(SSA_step);
}

void symex_target_equationt::function_call(
  const exprt &guard,
  const irep_idt &function_id,
  const std::vector<renamedt<exprt, L2>> &function_arguments,
  const sourcet &source,
  const bool hidden)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::FUNCTION_CALL);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard = guard;
  SSA_step.called_function = function_id;
  for(const auto &arg : function_arguments)
    SSA_step.ssa_function_arguments.emplace_back(arg.get());
  SSA_step.hidden = hidden;

  merge_ireps(SSA_step);
}

void symex_target_equationt::function_return(
  const exprt &guard,
  const irep_idt &function_id,
  const sourcet &source,
  const bool hidden)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::FUNCTION_RETURN);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard = guard;
  SSA_step.called_function = function_id;
  SSA_step.hidden = hidden;

  merge_ireps(SSA_step);
}

void symex_target_equationt::output(
  const exprt &guard,
  const sourcet &source,
  const irep_idt &output_id,
  const std::list<renamedt<exprt, L2>> &args)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::OUTPUT);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  for(const auto &arg : args)
    SSA_step.io_args.emplace_back(arg.get());
  SSA_step.io_id=output_id;

  merge_ireps(SSA_step);
}

void symex_target_equationt::output_fmt(
  const exprt &guard,
  const sourcet &source,
  const irep_idt &output_id,
  const irep_idt &fmt,
  const std::list<exprt> &args)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::OUTPUT);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.io_args=args;
  SSA_step.io_id=output_id;
  SSA_step.formatted=true;
  SSA_step.format_string=fmt;

  merge_ireps(SSA_step);
}

void symex_target_equationt::input(
  const exprt &guard,
  const sourcet &source,
  const irep_idt &input_id,
  const std::list<exprt> &args)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::INPUT);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.io_args=args;
  SSA_step.io_id=input_id;

  merge_ireps(SSA_step);
}

void symex_target_equationt::assumption(
  const exprt &guard,
  const exprt &cond,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::ASSUME);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.cond_expr=cond;

  merge_ireps(SSA_step);
}

void symex_target_equationt::assertion(
  const exprt &guard,
  const exprt &cond,
  const std::string &msg,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::ASSERT);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.cond_expr=cond;
  SSA_step.comment=msg;

  merge_ireps(SSA_step);
}

void symex_target_equationt::goto_instruction(
  const exprt &guard,
  const renamedt<exprt, L2> &cond,
  const sourcet &source)
{
  SSA_steps.emplace_back(source, goto_trace_stept::typet::GOTO);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=guard;
  SSA_step.cond_expr = cond.get();

  merge_ireps(SSA_step);
}

void symex_target_equationt::constraint(
  const exprt &cond,
  const std::string &msg,
  const sourcet &source)
{
  // like assumption, but with global effect
  SSA_steps.emplace_back(source, goto_trace_stept::typet::CONSTRAINT);
  SSA_stept &SSA_step=SSA_steps.back();

  SSA_step.guard=true_exprt();
  SSA_step.cond_expr=cond;
  SSA_step.comment=msg;

  merge_ireps(SSA_step);
}

void symex_target_equationt::convert_without_assertions(
  decision_proceduret &decision_procedure)
{
  with_solver_hardness(decision_procedure, [&](solver_hardnesst &hardness) {
    hardness.register_ssa_size(SSA_steps.size());
  });

  convert_guards(decision_procedure);
  convert_assignments(decision_procedure);
  convert_decls(decision_procedure);
  convert_assumptions(decision_procedure);
  convert_goto_instructions(decision_procedure);
  convert_function_calls(decision_procedure);
  convert_io(decision_procedure);
  convert_constraints(decision_procedure);
}

void symex_target_equationt::convert(decision_proceduret &decision_procedure)
{
  const auto convert_SSA_start = std::chrono::steady_clock::now();

  convert_without_assertions(decision_procedure);
  convert_assertions(decision_procedure);

  const auto convert_SSA_stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> convert_SSA_runtime =
    std::chrono::duration<double>(convert_SSA_stop - convert_SSA_start);
  log.status() << "Runtime Convert SSA: " << convert_SSA_runtime.count() << "s"
               << messaget::eom;
}

void symex_target_equationt::convert_assignments(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.is_assignment() && !step.ignore && !step.converted)
    {
      log.conditional_output(log.debug(), [&step](messaget::mstreamt &mstream) {
        step.output(mstream);
        mstream << messaget::eom;
      });

      decision_procedure.set_to_true(step.cond_expr);
      step.converted = true;
      with_solver_hardness(
        decision_procedure, hardness_register_ssa(step_index, step));
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_decls(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.is_decl() && !step.ignore && !step.converted)
    {
      // The result is not used, these have no impact on
      // the satisfiability of the formula.
      decision_procedure.handle(step.cond_expr);
      decision_procedure.handle(
        equal_exprt{step.ssa_full_lhs, step.ssa_full_lhs});
      step.converted = true;
      with_solver_hardness(
        decision_procedure, hardness_register_ssa(step_index, step));
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_guards(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.ignore)
      step.guard_handle = false_exprt();
    else
    {
      log.conditional_output(log.debug(), [&step](messaget::mstreamt &mstream) {
        step.output(mstream);
        mstream << messaget::eom;
      });

      step.guard_handle = decision_procedure.handle(step.guard);
      with_solver_hardness(
        decision_procedure, hardness_register_ssa(step_index, step));
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_assumptions(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.is_assume())
    {
      if(step.ignore)
        step.cond_handle = true_exprt();
      else
      {
        log.conditional_output(
          log.debug(), [&step](messaget::mstreamt &mstream) {
            step.output(mstream);
            mstream << messaget::eom;
          });

        step.cond_handle = decision_procedure.handle(step.cond_expr);

        with_solver_hardness(
          decision_procedure, hardness_register_ssa(step_index, step));
      }
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_goto_instructions(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.is_goto())
    {
      if(step.ignore)
        step.cond_handle = true_exprt();
      else
      {
        log.conditional_output(
          log.debug(), [&step](messaget::mstreamt &mstream) {
            step.output(mstream);
            mstream << messaget::eom;
          });

        step.cond_handle = decision_procedure.handle(step.cond_expr);
        with_solver_hardness(
          decision_procedure, hardness_register_ssa(step_index, step));
      }
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_constraints(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(step.is_constraint() && !step.ignore && !step.converted)
    {
      log.conditional_output(log.debug(), [&step](messaget::mstreamt &mstream) {
        step.output(mstream);
        mstream << messaget::eom;
      });

      decision_procedure.set_to_true(step.cond_expr);
      step.converted = true;

      with_solver_hardness(
        decision_procedure, hardness_register_ssa(step_index, step));
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_assertions(
  decision_proceduret &decision_procedure,
  bool optimized_for_single_assertions)
{
  // we find out if there is only _one_ assertion,
  // which allows for a simpler formula

  std::size_t number_of_assertions=count_assertions();

  if(number_of_assertions==0)
    return;

  if(number_of_assertions == 1 && optimized_for_single_assertions)
  {
    std::size_t step_index = 0;
    for(auto &step : SSA_steps)
    {
      // hide already converted assertions in the error trace
      if(step.is_assert() && step.converted)
        step.hidden = true;

      if(step.is_assert() && !step.ignore && !step.converted)
      {
        step.converted = true;
        decision_procedure.set_to_false(step.cond_expr);
        step.cond_handle = false_exprt();

        with_solver_hardness(
          decision_procedure, hardness_register_ssa(step_index, step));
        return; // prevent further assumptions!
      }
      else if(step.is_assume())
      {
        decision_procedure.set_to_true(step.cond_expr);

        with_solver_hardness(
          decision_procedure, hardness_register_ssa(step_index, step));
      }
      ++step_index;
    }

    UNREACHABLE; // unreachable
  }

  // We do (NOT a1) OR (NOT a2) ...
  // where the a's are the assertions
  or_exprt::operandst disjuncts;
  disjuncts.reserve(number_of_assertions);

  exprt assumption=true_exprt();

  std::vector<goto_programt::const_targett> involved_steps;

  for(auto &step : SSA_steps)
  {
    // hide already converted assertions in the error trace
    if(step.is_assert() && step.converted)
      step.hidden = true;

    if(step.is_assert() && !step.ignore && !step.converted)
    {
      step.converted = true;

      log.conditional_output(log.debug(), [&step](messaget::mstreamt &mstream) {
        step.output(mstream);
        mstream << messaget::eom;
      });

      implies_exprt implication(
        assumption,
        step.cond_expr);

      // do the conversion
      step.cond_handle = decision_procedure.handle(implication);

      with_solver_hardness(
        decision_procedure,
        [&involved_steps, &step](solver_hardnesst &hardness) {
          involved_steps.push_back(step.source.pc);
        });

      // store disjunct
      disjuncts.push_back(not_exprt(step.cond_handle));
    }
    else if(step.is_assume())
    {
      // the assumptions have been converted before
      // avoid deep nesting of ID_and expressions
      if(assumption.id()==ID_and)
        assumption.copy_to_operands(step.cond_handle);
      else
        assumption = and_exprt(assumption, step.cond_handle);

      with_solver_hardness(
        decision_procedure,
        [&involved_steps, &step](solver_hardnesst &hardness) {
          involved_steps.push_back(step.source.pc);
        });
    }
  }

  const auto assertion_disjunction = disjunction(disjuncts);
  // the below is 'true' if there are no assertions
  decision_procedure.set_to_true(assertion_disjunction);

  with_solver_hardness(
    decision_procedure,
    [&assertion_disjunction, &involved_steps](solver_hardnesst &hardness) {
      hardness.register_assertion_ssas(assertion_disjunction, involved_steps);
    });
}

void symex_target_equationt::convert_function_calls(
  decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(!step.ignore)
    {
      and_exprt::operandst conjuncts;
      step.converted_function_arguments.reserve(step.ssa_function_arguments.size());

      for(const auto &arg : step.ssa_function_arguments)
      {
        if(arg.is_constant() ||
           arg.id()==ID_string_constant)
          step.converted_function_arguments.push_back(arg);
        else
        {
          const irep_idt identifier="symex::args::"+std::to_string(argument_count++);
          symbol_exprt symbol(identifier, arg.type());

          equal_exprt eq(arg, symbol);
          merge_irep(eq);

          decision_procedure.set_to(eq, true);
          conjuncts.push_back(eq);
          step.converted_function_arguments.push_back(symbol);
        }
      }
      with_solver_hardness(
        decision_procedure,
        [step_index, &conjuncts, &step](solver_hardnesst &hardness) {
          hardness.register_ssa(
            step_index, conjunction(conjuncts), step.source.pc);
        });
    }
    ++step_index;
  }
}

void symex_target_equationt::convert_io(decision_proceduret &decision_procedure)
{
  std::size_t step_index = 0;
  for(auto &step : SSA_steps)
  {
    if(!step.ignore)
    {
      and_exprt::operandst conjuncts;
      for(const auto &arg : step.io_args)
      {
        if(arg.is_constant() ||
           arg.id()==ID_string_constant)
          step.converted_io_args.push_back(arg);
        else
        {
          const irep_idt identifier =
            "symex::io::" + std::to_string(io_count++);
          symbol_exprt symbol(identifier, arg.type());

          equal_exprt eq(arg, symbol);
          merge_irep(eq);

          decision_procedure.set_to(eq, true);
          conjuncts.push_back(eq);
          step.converted_io_args.push_back(symbol);
        }
      }
      with_solver_hardness(
        decision_procedure,
        [step_index, &conjuncts, &step](solver_hardnesst &hardness) {
          hardness.register_ssa(
            step_index, conjunction(conjuncts), step.source.pc);
        });
    }
    ++step_index;
  }
}

/// Merging causes identical ireps to be shared.
/// This is only enabled if the definition SHARING is defined.
/// \param SSA_step The step you want to have shared values.
void symex_target_equationt::merge_ireps(SSA_stept &SSA_step)
{
  merge_irep(SSA_step.guard);

  merge_irep(SSA_step.ssa_lhs);
  merge_irep(SSA_step.ssa_full_lhs);
  merge_irep(SSA_step.original_full_lhs);
  merge_irep(SSA_step.ssa_rhs);

  merge_irep(SSA_step.cond_expr);

  for(auto &step : SSA_step.io_args)
    merge_irep(step);

  for(auto &arg : SSA_step.ssa_function_arguments)
    merge_irep(arg);

  // converted_io_args is merged in convert_io
}

void symex_target_equationt::output(std::ostream &out) const
{
  for(const auto &step : SSA_steps)
  {
    step.output(out);
    out << "--------------\n";
  }
}

// __SZH_ADD_BEGIN__

#include <iostream>
#include <regex>
#include <util/arith_tools.h>

#include <util/format_expr.h>

std::string get_addr(std::string name)
{
  std::regex pattern("([^\\#]*)#\\d+([^\\d].*)");
  std::smatch results;
  if(regex_match(name, results, pattern))
    return results[1].str() + results[2].str();
  return name.substr(0, name.find_first_of('#'));
}

std::string get_addr_prefix(std::string name)
{
  return name.substr(0, name.find_first_of('#'));
}

struct expr_condt
{
  exprt expr;
  exprt::operandst condition; //conjunction
  bool is_trivial_assign;
  bool related_to_assign;

  expr_condt() {}
  expr_condt(exprt _expr, exprt::operandst _cond, bool _flag, bool _flag2) :
    expr(_expr), condition(_cond), is_trivial_assign(_flag), related_to_assign(_flag2) {}
};

struct index_or_membert
{
  bool is_index;
  exprt index;
  irep_idt member;
  index_or_membert() {}
  index_or_membert(const exprt& _index) : is_index(true), index(_index) {}
  index_or_membert(const irep_idt& _member) : is_index(false), member(_member) {}
  friend std::ostream& operator<<(std::ostream& out, const index_or_membert& elem)
  {
    if(elem.is_index)
      out << "index " << format(elem.index);
    else
      out << "member " << elem.member;
    return out;
  }
};

std::string get_component_name(std::string str)
{
  static std::regex pattern(".*\\.\\.([A-Za-z0-9_]+)");
  std::smatch result;
  bool ret = std::regex_match(str, result, pattern);
  if(!ret)
    return "";
  return result[1];
}

void build_expr_conds_indices_members(std::vector<expr_condt>& expr_conds, const exprt& array_struct, std::vector<index_or_membert>& indices_members, exprt::operandst& conds, bool is_trivial_assign, bool related_to_assign)
{
  if(indices_members.empty())
  {
    expr_conds.push_back(expr_condt(array_struct, conds, is_trivial_assign, related_to_assign));
    return;
  }
  
  auto index_member = indices_members.back();
  indices_members.pop_back();

  if(index_member.is_index)
  {
    assert(array_struct.id() == ID_array);
    auto array = to_array_expr(array_struct);
    auto index = index_member.index;
    for(int i = 0; i < int(array.operands().size()); i++)
    {
      auto& operand = array.operands()[i];
      constant_exprt const_expr = from_integer(i, index.type());
      conds.push_back(equal_exprt(index, const_expr));
      build_expr_conds_indices_members(expr_conds, operand, indices_members, conds, is_trivial_assign, related_to_assign);
      conds.pop_back();
    }
  }
  else
  {
    assert(array_struct.id() == ID_struct);
    auto structure = to_struct_expr(array_struct);
    std::string expected_component_name = index_member.member.c_str();
    for(auto& operand : structure.operands())
    {
      if(operand.id() != ID_symbol)
        continue; // temporary, any better solution?
      std::string operand_name = to_symbol_expr(operand).get_identifier().c_str();
      std::string component_name = get_component_name(operand_name);

      if(expected_component_name == component_name)
        expr_conds.push_back(expr_condt(operand, conds, is_trivial_assign, related_to_assign));
    }
  }

  indices_members.push_back(index_member);
}

void build_phi_rec(std::map<symbol_exprt, std::set<std::pair<symbol_exprt, exprt>>>& result_map, symbol_exprt left, exprt expr, exprt cond)
{
  if(expr.id() == ID_symbol)
    result_map[left].insert(std::make_pair(to_symbol_expr(expr), cond));
  if(expr.id() == ID_if)
  {
    auto& if_cond = to_if_expr(expr).cond();
    auto& true_case = to_if_expr(expr).true_case();
    auto& false_case = to_if_expr(expr).false_case();
    build_phi_rec(result_map, left, true_case, and_exprt(cond, if_cond));
    build_phi_rec(result_map, left, false_case, and_exprt(cond, not_exprt(if_cond)));
  }
}

std::map<symbol_exprt, std::set<std::pair<symbol_exprt, exprt>>> build_assignment_phi(symex_target_equationt& equation)
{
  std::map<symbol_exprt, std::set<std::pair<symbol_exprt, exprt>>> ret;
  for(auto& step : equation.SSA_steps)
  {
    if(step.is_assignment() && step.assignment_type == symex_targett::assignment_typet::PHI)
      build_phi_rec(ret, step.ssa_lhs, step.ssa_rhs, true_exprt());
  }
  return ret;
}

void update_available_cond_map(std::map<std::string, exprt>& available_cond_map, const symbol_exprt& symbol, exprt& cond, const namespacet& ns)
{
  std::string str = symbol.get_identifier().c_str();

  simplify(cond, ns);
  if(cond.is_false())
    return;

  if(available_cond_map.find(str) == available_cond_map.end())
    available_cond_map[str] = cond;
  else if(available_cond_map[str] != cond)
    available_cond_map[str] = or_exprt(available_cond_map[str], cond);
}

void update_available_cond_map(std::map<std::string, exprt>& available_cond_map, const symbol_exprt& symbol, exprt::operandst& conds, const namespacet& ns)
{
  auto cond_conj = conjunction(conds);
  update_available_cond_map(available_cond_map, symbol, cond_conj, ns);
}

void symex_target_equationt::build_available_cond_map(std::map<std::string, exprt>& available_cond_map, const namespacet& ns)
{
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;
    
    auto& left = e_it->ssa_lhs;
    auto& right = e_it->ssa_rhs;
    std::vector<expr_condt> expr_conds{expr_condt(right, exprt::operandst(), true, true)};
    while(!expr_conds.empty())
    {
      auto expr_cond = expr_conds.back();
      auto& expr = expr_cond.expr;
      auto& cond = expr_cond.condition;
      auto is_trivial_assign = expr_cond.is_trivial_assign;
      auto related_to_assign = expr_cond.related_to_assign;

      expr_conds.pop_back();

      // handle {arr[[0]], arr[[1]], arr[[2]], ...}[i]:
      // not all of them take effect, depending on i
      if(expr.id() == ID_index || expr.id() == ID_member)
      {
        auto curr_expr = expr;
        std::vector<index_or_membert> indices_members;
        while(curr_expr.id() == ID_index || curr_expr.id() == ID_member || curr_expr.id() == ID_byte_update_little_endian)
        {
          if(curr_expr.id() == ID_index)
          {
            indices_members.push_back(curr_expr.op1());
            curr_expr = curr_expr.op0();
          }
          else if (curr_expr.id() == ID_member)
          {
            indices_members.push_back(to_member_expr(curr_expr).get_component_name());
            curr_expr = curr_expr.op0();
          }
          else // ID_byte_update_little_endian
          {
            curr_expr = curr_expr.op0();
          }
        }
        if(curr_expr.id() == ID_array || curr_expr.id() == ID_struct)
        {
          auto curr_cond = cond;
          build_expr_conds_indices_members(expr_conds, curr_expr, indices_members, curr_cond, is_trivial_assign, related_to_assign);
        }
        else if(curr_expr.id() == ID_symbol && !is_trivial_assign)
        {
          std::string write_str = left.get_identifier().c_str();
          std::string read_str = to_symbol_expr(curr_expr).get_identifier().c_str();

          if(get_addr(write_str) != get_addr(read_str) &&
            get_addr_prefix(write_str) != get_addr(read_str) &&
            get_addr(write_str) != get_addr_prefix(read_str)) // not self assignment
          {
            is_trivial_assign = false;
          }

          if(!is_trivial_assign)
          {
            if(related_to_assign)
              update_available_cond_map(available_cond_map, left, cond, ns);
            update_available_cond_map(available_cond_map, to_symbol_expr(curr_expr), cond, ns);
          }
        }
      }
      // handle arr#i[[index]] = cond ? new_expr : arr#j[[index]]:
      //if cond does not hold, arr#i[[index]] is dummy; arr#j[[index]] is dummy
      else if(expr.id() == ID_if)
      {
        auto& if_cond = to_if_expr(expr).cond();
        auto& true_case = to_if_expr(expr).true_case();
        auto& false_case = to_if_expr(expr).false_case();

        expr_conds.push_back(expr_condt(if_cond, cond, false, false));

        cond.push_back(if_cond);
        expr_conds.push_back(expr_condt(true_case, cond, is_trivial_assign, related_to_assign));
        cond.pop_back();

        cond.push_back(not_exprt(if_cond));
        expr_conds.push_back(expr_condt(false_case, cond, is_trivial_assign, related_to_assign));
        cond.pop_back();
      }
      else if(expr.id() == ID_byte_update_little_endian)
      {
        expr_conds.push_back(expr_condt(expr.op2(), cond, is_trivial_assign, related_to_assign));
      }
      else if(expr.id() == ID_array || expr.id() == ID_struct)
      {
        for(auto& operand: expr.operands())
          expr_conds.push_back(expr_condt(operand, cond, is_trivial_assign, related_to_assign));
      }
      else if(expr.id() == ID_with)
      {
        expr_conds.push_back(expr_condt(expr.op2(), cond, is_trivial_assign, related_to_assign));
      }
      else // normally add children, not trivial
      {
        for(auto& operand : expr.operands())
          expr_conds.push_back(expr_condt(operand, cond, false, related_to_assign));
      }

      if(expr.id() == ID_symbol) // assign from symbol to symbol, trivial?
      {
        std::string write_str = left.get_identifier().c_str();
        std::string read_str = to_symbol_expr(expr).get_identifier().c_str();

        if(get_addr(write_str) != get_addr(read_str) &&
          get_addr_prefix(write_str) != get_addr(read_str) &&
          get_addr(write_str) != get_addr_prefix(read_str)) // not self assignment
        {
          is_trivial_assign = false;
        }

        if(!is_trivial_assign)
        {
          if(related_to_assign)
            update_available_cond_map(available_cond_map, left, cond, ns);
          update_available_cond_map(available_cond_map, to_symbol_expr(expr), cond, ns);
        }
      }
      else if(!expr.has_operands()) // assign from non-symbol (constant?) to symbol, always not trivial
      {
        if(related_to_assign)
          update_available_cond_map(available_cond_map, left, cond, ns);
      }

    }
  }

  auto assignment_phi_map = build_assignment_phi(*this);
  for(auto pair : assignment_phi_map)
  {
    auto left = pair.first;
    std::string left_str = left.get_identifier().c_str();
    exprt::operandst conds;

    if(available_cond_map.find(left_str) == available_cond_map.end())
    {
      // transfers cond from right to left
      for(auto right_pair : pair.second)
      {
        auto right = right_pair.first;
        std::string right_str = right.get_identifier().c_str();
        auto cond = right_pair.second;
        if(available_cond_map.find(right_str) != available_cond_map.end())
          conds.push_back(and_exprt(available_cond_map[right_str], cond));
      }
      auto cond_disj = disjunction(conds);
      update_available_cond_map(available_cond_map, left, cond_disj, ns);
    }
    else
    {
      // transfers cond from left to right
      auto left_cond = available_cond_map[left_str];
      for(auto right_pair : pair.second)
      {
        auto right = right_pair.first;
        std::string right_str = right.get_identifier().c_str();
        auto cond = right_pair.second;
        if(available_cond_map.find(right_str) != available_cond_map.end())
          continue;
        exprt::operandst two_conds{left_cond, cond};
        update_available_cond_map(available_cond_map, right, two_conds, ns);
      }
    }
  }

  // previously preserved
  for(auto preserved_access : preserved_accesses)
    if(available_cond_map.find(preserved_access) == available_cond_map.end())
      available_cond_map[preserved_access] = true_exprt();

  // std::cout << "viewing available cond map!\n";
  // for(auto pair : available_cond_map)
  //   std::cout << pair.first << "'s condition is " << format(pair.second) << "\n";
}

void symex_target_equationt::build_array_update_set(std::set<std::pair<std::string, std::string>>& apo_set)
{
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    auto& left = e_it->ssa_lhs;
    auto& right = e_it->ssa_rhs;

    if(right.id() == ID_with)
    {
      auto& old = to_with_expr(right).old();
      if(old.id() != ID_symbol) // todo: more complex cases?
        continue;

      std::string left_str = left.get_identifier().c_str();
      std::string right_str = to_symbol_expr(old).get_identifier().c_str();
      apo_set.insert(std::make_pair(right_str, left_str));
    }

    if(right.id() == ID_byte_update_little_endian)
    {
      auto& old = to_byte_update_expr(right).op();
      if(old.id() != ID_symbol) // todo: more complex cases?
        continue;

      std::string left_str = left.get_identifier().c_str();
      std::string right_str = to_symbol_expr(old).get_identifier().c_str();
      apo_set.insert(std::make_pair(right_str, left_str));
    }
  }
}

void symex_target_equationt::build_same_pointer_set(std::set<std::pair<std::string, std::string>>& apo_set)
{
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    auto& right = e_it->ssa_rhs;
    if(right.id() != ID_if)
      continue;

    for(if_exprt expr = to_if_expr(right); expr.false_case().id() == ID_if; expr = to_if_expr(expr.false_case()))
    {
      auto next_expr = to_if_expr(expr.false_case());
      auto expr_symbols = find_symbols(expr.cond());
      auto next_expr_symbols = find_symbols(next_expr.cond());
      for(auto& expr_symbol : expr_symbols)
        for(auto& next_expr_symbol : next_expr_symbols)
        {
          std::string str = id2string(expr_symbol.get_identifier());
          std::string next_str = id2string(next_expr_symbol.get_identifier());
          apo_set.insert(std::make_pair(str, next_str));
        }
    }
  }
}

bool same_source(const symex_targett::sourcet& source1, const symex_targett::sourcet& source2)
{
  return source1.function_id == source2.function_id &&
    source1.pc->source_location() == source2.pc->source_location() &&
    source1.thread_nr == source2.thread_nr;
}

void symex_target_equationt::apply_same_line_atomic_set(const namespacet& ns, std::vector<symex_target_equationt::event_it>& same_line_events, std::set<std::pair<std::string, std::string>>& apo_set)
{
  for(int i = 0; i < int(same_line_events.size()); i++)
  {
    if(!symbol_is_atomic(ns, same_line_events[i]->ssa_lhs.get_l1_object_identifier()))
      continue;
    for(int j = i + 1; j < int(same_line_events.size()); j++)
    {
      std::string it1_basename = same_line_events[i]->ssa_lhs.get_l1_object_identifier().c_str();
      std::string it2_basename = same_line_events[j]->ssa_lhs.get_l1_object_identifier().c_str();
      if(it1_basename == it2_basename)
      {
        for(int k = i; k < j; k++)
        {
          std::string from_name = same_line_events[k]->ssa_lhs.get_identifier().c_str();
          std::string to_name = same_line_events[k + 1]->ssa_lhs.get_identifier().c_str();
          apo_set.insert(std::make_pair(from_name, to_name));
          std::cout << from_name << " and " << to_name << " should be atomic\n";
        }
      }
    }
  }
}

void symex_target_equationt::build_same_line_atomic_set(const namespacet& ns, std::set<std::pair<std::string, std::string>>& apo_set)
{
  std::vector<event_it> same_line_events;

  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!same_line_events.empty() && !same_source(e_it->source, same_line_events.back()->source))
    {
      apply_same_line_atomic_set(ns, same_line_events, apo_set);
      same_line_events.clear();
    }

    if(!e_it->is_shared_read() && !e_it->is_shared_write())
      continue;

    same_line_events.push_back(e_it);
  }
  apply_same_line_atomic_set(ns, same_line_events, apo_set);
}

void symex_target_equationt::dynamic_object_atomicity(const namespacet& ns)
{
  // dynamic objects can also be atomic, if
  // ptr = address_of(dynamic_object[0])
  // where ptr is atomic
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    const auto& lhs = e_it->ssa_lhs;
    const auto& rhs = e_it->ssa_rhs;
    if(rhs.id() != ID_address_of)
      continue;
    if(!symbol_is_atomic(ns, lhs.get_l1_object_identifier()))
      continue;

    auto rhs_symbols = find_symbols(rhs);
    for(auto& rhs_symbol : rhs_symbols)
    {
      std::string rhs_dynamic = rhs_symbol.type().get("#dynamic").c_str();
      if(rhs_dynamic == "1")
      {
        std::cout << rhs_symbol.get_identifier() << " is an atomic_dynamic_object\n";
        atomic_dynamic_objects.insert(id2string(rhs_symbol.get_identifier()));
      }
    }
  }
}

#include <util/bitvector_types.h>

int symex_target_equationt::get_byte_length(const exprt& expr)
{
  int length = 4; // default
  if(can_cast_type<signedbv_typet>(expr.type()))
    length = to_signedbv_type(expr.type()).get_width() / 8;
  if(can_cast_type<unsignedbv_typet>(expr.type()))
    length = to_unsignedbv_type(expr.type()).get_width() / 8;
  return length;
}

void symex_target_equationt::build_byte_update_map(std::map<std::string, std::pair<exprt, exprt>>& byte_update_map, const namespacet& ns)
{
  // arr#2 = byte_update_little_endian(arr#1, i, value)
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    auto& left = e_it->ssa_lhs;
    auto& right = e_it->ssa_rhs;
    if(right.id() != ID_byte_update_little_endian)
      continue;
    auto& old = to_byte_update_expr(right).op();

    int update_length = get_byte_length(to_byte_update_expr(right).value());
    auto index_from = to_byte_update_expr(right).offset();
    simplify(index_from, ns);
    auto index_to = plus_exprt(index_from, from_integer(update_length, index_from.type()));
    simplify(index_to, ns);

    std::string left_str = left.get_identifier().c_str();
    byte_update_map[left_str] = std::make_pair(index_from, index_to);

    if(old.id() != ID_symbol) // todo: more complex cases?
      continue;
    std::string right_str = to_symbol_expr(old).get_identifier().c_str();
    byte_update_map[right_str] = std::make_pair(index_from, index_to);
  }
}

void symex_target_equationt::build_with_map(std::map<std::string, exprt>& with_map, const namespacet& ns)
{
  // arr#2 = with(arr#1, i, value)
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    auto& left = e_it->ssa_lhs;
    auto& right = e_it->ssa_rhs;
    if(right.id() != ID_with)
      continue;
    auto& old = to_with_expr(right).old();
    auto index = to_with_expr(right).where();

    std::string left_str = left.get_identifier().c_str();
    with_map[left_str] = index;

    if(old.id() != ID_symbol) // todo: more complex cases?
      continue;
    std::string right_str = to_symbol_expr(old).get_identifier().c_str();
    with_map[right_str] = index;
  }

  // a single arr#1[i]
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(!e_it->is_assignment())
      continue;

    auto& right = e_it->ssa_rhs;
    std::vector<exprt> exprs{right};
    while(!exprs.empty())
    {
      auto expr = exprs.back();
      exprs.pop_back();
      if(expr.id() == ID_index && expr.op0().id() == ID_symbol) // todo: more complex cases?
      {
        std::string str = to_symbol_expr(expr.op0()).get_identifier().c_str();
        auto index = expr.op1();
        with_map[str] = index;
      }
      
      for(auto& operand : expr.operands())
        exprs.push_back(operand);
    }
  }
}

#include <util/symbol.h>
#include <util/format_type.h>
#include <util/pointer_expr.h>

bool type_is_atomic(const typet& type)
{
  if(type.id() == ID_array)
    return type_is_atomic(to_array_type(type).subtype());
  // todo: if(type.id() == ID_struct) {}

  std::string symbol_typedef = type.get("#typedef").c_str();
  std::string symbol_atomic = type.get("#atomic").c_str();
  if(symbol_typedef.find("atomic_") != std::string::npos || symbol_atomic == "1")
    return true;
  if(type.id() == ID_pointer) // todo: to be added later
    if(type_is_atomic(to_pointer_type(type).base_type()))
      return true;
  return false;
}

irep_idt remove_suffix(irep_idt id)
{
  std::string str = id.c_str();

  std::regex pattern(R"(([^.[]+)(\.\.[^\[]+)?(\[\[[^\]]+\]\]))");
  std::smatch matches;

  if (std::regex_match(str, matches, pattern))
    return matches[1].str();
  return id;
}

bool symex_target_equationt::symbol_is_atomic(const namespacet& ns, irep_idt symbol_name) {
  symbol_name = remove_suffix(symbol_name);

  if(atomic_dynamic_objects.find(id2string(symbol_name)) != atomic_dynamic_objects.end())
    return true;

  const symbolt *symbol_ptr;
  if(ns.lookup(symbol_name, symbol_ptr)) // true if not found
    return false;

  return type_is_atomic(symbol_ptr->type);
}

void symex_target_equationt::build_datarace(const namespacet& ns, bool filter)
{
  typedef std::vector<std::pair<event_it, bool>> event_listt; // first: event itself, second: whether atomic
  typedef std::map<std::string, std::map<unsigned, event_listt>> per_loc_per_thread_mapt;
  per_loc_per_thread_mapt per_loc_per_thread_map;

  // build per_loc_per_thread_map
  bool atomic = false;
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(e_it->is_atomic_begin())
    {
      atomic = true;
      continue;
    }
    if(e_it->is_atomic_end())
    {
      atomic = false;
      continue;
    }

    if(e_it->is_shared_read() || e_it->is_shared_write())
    {
      auto e_str = id2string(e_it->ssa_lhs.get_identifier());
      auto e_str_prefix = get_addr(e_str);

      if(e_str_prefix.find("__CPROVER") != std::string::npos) //ignore __CPROVER trash
        continue;

      per_loc_per_thread_map[e_str_prefix][e_it->source.thread_nr].push_back(std::make_pair(e_it, atomic));
    }
  }

  int races_before_filter = 0;
  int races_after_filter = 0;

  std::vector<std::pair<event_it, event_it>> races;

  std::set<race_identifier_t> categories_before_filter;
  std::set<race_identifier_t> categories_after_filter;

  for(auto& per_thread_map_pair : per_loc_per_thread_map)
  {
    auto& per_thread_map = per_thread_map_pair.second;
    for(auto s_it = per_thread_map.begin(); s_it != per_thread_map.end(); s_it++)
      for(auto t_it = s_it; ; )
      {
        t_it++;
        if(t_it == per_thread_map.end())
          break;
        
        const event_listt& first_events = s_it->second;
        const event_listt& second_events = t_it->second;

        for(auto& first_event_pair : first_events)
          for(auto& second_event_pair : second_events)
          {
            auto first_event = first_event_pair.first;
            auto first_event_atomic = first_event_pair.second;
            auto second_event = second_event_pair.first;
            auto second_event_atomic = second_event_pair.second;

            if(first_event_atomic && second_event_atomic) // it seems that a guarded and an unguarded event may race
              continue;

            if(first_event->is_shared_read() && second_event->is_shared_read())
              continue;

            // check whether the variable is like atomic_int
            if(symbol_is_atomic(ns, first_event->ssa_lhs.get_l1_object_identifier()))
              continue;

            std::string var_name = id2string(first_event->ssa_lhs.get_identifier());
            var_name = var_name.substr(0, var_name.find_first_of('#'));
            std::string first_event_line = first_event->source.pc->source_location().get_line().c_str();
            std::string second_event_line = second_event->source.pc->source_location().get_line().c_str();

            race_identifier_t race_id(var_name, first_event_line, second_event_line, true, true);
            categories_before_filter.insert(race_id);

            races_before_filter++;
            // std::cout << "racing before goblint filter: " << first_event->ssa_lhs.get_identifier() << " " << second_event->ssa_lhs.get_identifier() << "\n";
            // std::cout << "linenumber: " << first_event_line << " " << second_event_line << "\n";

            if(filter)
              if( datarace_lines.find(first_event_line) == datarace_lines.end() || 
                  datarace_lines.find(second_event_line) == datarace_lines.end())
                continue;

            races.push_back(std::make_pair(first_event, second_event));
            categories_after_filter.insert(race_id);

            races_after_filter++;
            // std::cout << "racing after goblint filter: " << first_event->ssa_lhs.get_identifier() << " " << second_event->ssa_lhs.get_identifier() << "\n";
          }
      }
  }
  
  if(filter)
  {
    std::cout << "Lines where datarace may occur: ";
    for(auto line : datarace_lines)
        std::cout << line << " ";
    std::cout << "\n";
    std::cout << "The number of races before filter: " << races_before_filter << "\n";
    std::cout << "The number of races after filter: " << races.size() << "\n";
    std::cout << "The number of race categories before filter: " << categories_before_filter.size() << "\n";
    std::cout << "The number of race categories after filter: " << categories_after_filter.size() << "\n";
  }
  else
  {
    std::cout << "The number of races before filter: " << races_before_filter << "\n";
    std::cout << "The number of races after filter: " << races.size() << "\n";
    std::cout << "The number of race categories before filter: " << categories_before_filter.size() << "\n";
    std::cout << "The number of race categories after filter: " << categories_after_filter.size() << "\n";
  }


  for(auto& race: races)
  {
    auto& first_event = race.first;
    auto& second_event = race.second;

    std::string var_name = id2string(first_event->ssa_lhs.get_identifier());
    var_name = var_name.substr(0, var_name.find_first_of('#'));
    //var_name = ""; // try not to distinguish var

    std::string first_event_line = first_event->source.pc->source_location().get_line().c_str();
    bool first_event_is_write = first_event->is_shared_write();
    first_event_is_write = true; // try not to distinguish w/r
    
    std::string second_event_line = second_event->source.pc->source_location().get_line().c_str();
    bool second_event_is_write = second_event->is_shared_write();
    second_event_is_write = true; // try not to distinguish w/r

    race_identifier_t race_id(var_name, first_event_line, second_event_line, first_event_is_write, second_event_is_write);

    //if(linenumbers_to_races[race_id].size() < 5) //temporary
      linenumbers_to_races[race_id].push_back(race);
  }

  for(auto& pair : linenumbers_to_races)
  {
    std::cout << "\t" << pair.first.first_linenumber << "(" << pair.first.first_is_write << ") - " << pair.first.second_linenumber << "(" << pair.first.second_is_write << ") " << pair.first.var_name << "\n";
    // for(auto race : pair.second)
    // {
    //   std::string e1_str = id2string(race.first->ssa_lhs.get_identifier());
    //   std::string e2_str = id2string(race.second->ssa_lhs.get_identifier());
    //   std::cout << "\t\t" << e1_str << " " << e2_str << "\n";
    // }
  }

  //next_races = linenumbers_to_races.begin();
}

bool symex_target_equationt::choose_datarace()
{
  // datarace_pairs.clear();

  // auto& chosen_races_pair = *next_races;
  // auto& chosen_races_identifier = chosen_races_pair.first;
  // auto& chosen_races = chosen_races_pair.second;

  // std::cout << "Solving races of variable " << chosen_races_identifier.var_name << " between " << chosen_races_identifier.first_linenumber << " and " << chosen_races_identifier.second_linenumber << "\n";

  // for(auto& race : chosen_races)
  // {
  //     datarace_pairs.push_back(race);
  //     std::cout << "\tIncluding " << id2string(race.first->ssa_lhs.get_identifier()) << " " << id2string(race.second->ssa_lhs.get_identifier()) << "\n";
  // }

  // next_races++;
  // return next_races != linenumbers_to_races.end();

  for(auto& races_it : linenumbers_to_races) // temporarily add all races in one pot
  {
    auto& races = races_it.second;
    for(auto& race : races)
      datarace_pairs.push_back(race);
  }
  return true;
}

void symex_target_equationt::remove_repeating_updates()
{
  auto shared_symbols = build_shared_symbols();

  for(auto& step : SSA_steps)
  {
    if(!step.is_assignment())
      continue;

    // for x#(i+1) = if(cond) then new_value else x#i
    // when cond is false, this is a useless repeated update, so we can remove it.
    // The whole assignment is converted into x#(i+i) = new_value (with cond as its guard)
    if(step.ssa_rhs.id() != ID_if)
      continue;
    auto if_expr = to_if_expr(step.ssa_rhs);
    auto true_case = if_expr.true_case();
    auto false_case = if_expr.false_case();
    if(false_case.id() != ID_symbol)
      continue;

    std::string false_case_str = to_symbol_expr(false_case).get_identifier().c_str();
    if(shared_symbols.find(false_case_str) == shared_symbols.end())
      continue;

    auto lhs_name = step.ssa_lhs.get_l1_object_identifier();
    auto rhs_name = to_ssa_expr(false_case).get_l1_object_identifier();
    if(lhs_name != rhs_name)
      continue;

    step.ssa_rhs = true_case;
    step.guard = and_exprt(step.guard, if_expr.cond());
    step.cond_expr.op1() = step.ssa_rhs;
  }
}

std::set<std::string> symex_target_equationt::build_shared_symbols()
{
  // collect names of all shared read/write events

  std::set<std::string> shared_events;
  for(auto e_it = SSA_steps.begin(); e_it != SSA_steps.end(); e_it++)
    if(e_it->is_shared_read() || e_it->is_shared_write())
      shared_events.insert(e_it->ssa_lhs.get_identifier().c_str());
  return shared_events;
}

std::set<std::string> symex_target_equationt::build_symbols_in_use()
{
  std::set<std::string> symbols_in_use;
  for(auto e_it = SSA_steps.begin(); e_it != SSA_steps.end(); e_it++)
  {
    if(e_it->is_assignment())
    {
      auto symbol_exprs = find_symbols(e_it->ssa_rhs);
      for(const auto& symbol_expr : symbol_exprs)
        symbols_in_use.insert(symbol_expr.get_identifier().c_str());
    }

    if(e_it->is_assert() || e_it->is_assume())
    {
      auto symbol_exprs = find_symbols(e_it->cond_expr);
      for(const auto& symbol_expr : symbol_exprs)
        symbols_in_use.insert(symbol_expr.get_identifier().c_str());
    }

    auto symbol_exprs_guard = find_symbols(e_it->guard);
    for(const auto& symbol_expr : symbol_exprs_guard)
      symbols_in_use.insert(symbol_expr.get_identifier().c_str());
  }
  return symbols_in_use;
}

void symex_target_equationt::concurrency_slice()
{
  remove_repeating_updates();

  bool changed = true;
  while(changed)
  {
    std::cout << "Concurrency slicing iteration, current SSA size: " << SSA_steps.size() << "\n";
    int original_size = SSA_steps.size();
    auto shared_symbols = build_shared_symbols();
    auto symbols_in_rhs = build_symbols_in_use();

    for(auto e_it = SSA_steps.begin(); e_it != SSA_steps.end();)
    {
      if(e_it->is_assignment())
      {
        std::string lhs_name = e_it->ssa_lhs.get_identifier().c_str();
        if(shared_symbols.find(lhs_name) == shared_symbols.end() && symbols_in_rhs.find(lhs_name) == symbols_in_rhs.end() && lhs_name.find("memorder") == std::string::npos)
        {
          // std::cout << "Removing step: " << e_it->ssa_lhs.get_identifier() << " := " << format(e_it->ssa_rhs) << "\n";
          e_it = SSA_steps.erase(e_it);
          continue;
        }
      }
      e_it++;
    }
    remove_dummy_accesses();
    std::cout << "After a slicing iteration, SSA size becomes: " << SSA_steps.size() << "\n";
    changed = (SSA_steps.size() != original_size);
  }
}

void symex_target_equationt::remove_dummy_accesses()
{
  std::set<std::string> solid_accesses;
  for(const auto &acquisition : lock_acquisitions)
    for(const auto &symbol : find_symbols(
          and_exprt(acquisition.guard, acquisition.condition)))
      solid_accesses.insert(id2string(symbol.get_identifier()));

  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    std::set<symbol_exprt> appeared_symbols;
    find_symbols(e_it->guard, appeared_symbols);
    find_symbols(e_it->cond_expr, appeared_symbols);

    for(auto& appeared_symbol : appeared_symbols)
    {
      std::string appeared_str = appeared_symbol.get_identifier().c_str();
      solid_accesses.insert(appeared_str);
    }

    if(e_it->is_assignment())
    {
      std::string left_str = e_it->ssa_lhs.get_identifier().c_str();
      solid_accesses.insert(left_str);

      auto right_symbols = find_symbols(e_it->ssa_rhs);
      for(auto& right_symbol : right_symbols)
      {
        std::string right_str = right_symbol.get_identifier().c_str();
        solid_accesses.insert(right_str);
      }
    }
  }

  std::set<std::pair<std::string, std::string>> goto_lines;

  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(e_it->is_goto())
    {
      std::string file = e_it->source.pc->source_location().get_file().c_str();
      std::string line = e_it->source.pc->source_location().get_line().c_str();
      goto_lines.insert(std::make_pair(file, line));
    }
  }

  std::cout << "Start removing dummy accesses: " << SSA_steps.size() << " steps before removal\n";
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end();)
  {
    // concurreny-related?
    if(e_it->is_shared_read() || e_it->is_shared_write())
    {
      std::string shared_str = e_it->ssa_lhs.get_identifier().c_str();
      if(solid_accesses.find(shared_str) == solid_accesses.end())
      {
        std::string file = e_it->source.pc->source_location().get_file().c_str();
        std::string line = e_it->source.pc->source_location().get_line().c_str();
        if(goto_lines.find(std::make_pair(file, line)) == goto_lines.end())
        {
          std::cout << shared_str << " is dummy and removed\n";
          e_it = SSA_steps.erase(e_it);
          continue;
        }
        else
        {
          std::cout << shared_str << " is dummy but preserved\n";
          preserved_accesses.insert(shared_str);
        }
      }
    }
    e_it++;
  }
  std::cout << SSA_steps.size() << " steps after removal\n";
}
// __SZH_ADD_END__


// __WP_ADD_BEGIN__
void symex_target_equationt::build_deadlock()
{
  // build per_thread_lock_guard
  bool isatomic = false;
  bool islocked = false;
  for(event_it e_it=SSA_steps.begin(); e_it!=SSA_steps.end(); e_it++)
  {
    if(e_it->is_function_return() && id2string(e_it->source.function_id) == "main") {
      program_returns.push_back(e_it);
      continue;
    }

    if(e_it->is_assert()) {
      if (id2string(e_it->source.function_id) == "abort")
        program_returns.push_back(e_it);
      else if (islocked) {
        std::string target = "unwinding assertion loop";
        if (e_it->comment.find(target) != std::string::npos) {
          loop_asserts_in_critical.push_back(e_it);
          
          std::cout << "comment:" << e_it->comment << "\n------\n";
        }
      }
      continue;
    }

    if(e_it->is_atomic_begin())
    {
      isatomic = true;
      continue;
    }
    
    if(e_it->is_atomic_end())
    {
      isatomic = false;
      continue;
    }

    if(!isatomic)
      continue;

    if(e_it->is_shared_write() && id2string(e_it->source.function_id) == "pthread_mutex_lock")
    {
      // auto e_str = id2string(e_it->ssa_lhs.get_identifier());
      // auto e_str_prefix = get_addr(e_str);

      // std::cout << "thread_id:" << e_it->source.thread_nr 
      // << "\nvar_name:" << e_str
      // << "\nline:" << e_it->source.pc->source_location().get_line().c_str()
      // << "\nvar_guard:" << e_it->guard.pretty() << "\n------\n";

      islocked = true;
      continue;
    }

    if(e_it->is_shared_write() && id2string(e_it->source.function_id) == "pthread_mutex_unlock")
    {
      islocked = false;
      continue;
    }
  }
}
// __WP_ADD_END__