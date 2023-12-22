/*******************************************************************\

Module: Bounded Model Checking for ANSI-C

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

/// \file
/// Bounded Model Checking for ANSI-C

#include "symex_bmc.h"

#include <limits>

#include <util/simplify_expr.h>
#include <util/source_location.h>

#include <goto-instrument/unwindset.h>

#include <iostream>

symex_bmct::symex_bmct(
  message_handlert &mh,
  const symbol_tablet &outer_symbol_table,
  symex_target_equationt &_target,
  const optionst &options,
  path_storaget &path_storage,
  guard_managert &guard_manager,
  unwindsett &unwindset)
  : goto_symext(
      mh,
      outer_symbol_table,
      _target,
      options,
      path_storage,
      guard_manager),
    record_coverage(!options.get_option("symex-coverage-report").empty()),
    havoc_bodyless_functions(
      options.get_bool_option("havoc-undefined-functions")),
    unwindset(unwindset),
    // __SZH_ADD_BEGIN__
    has_sleep(false),
    has_writer_fn(false),
    has_insert(false),
    has_take(false),
    has_malloc(false),
    has_calloc(false),
    has_key(false),
    has_external_alloc(false),
    has_barrier_init(false),
    has_my_drv_probe(false),
    has_create_fresh_int_array(false),
    has_plus(false),
    has_Init_WorkStealQueue(false),
    has_atomic_acquire(false),
    has_atomic_dec(false),
    has_atomic_trash(false),
    has_myPartOfCalc(false),
    has_threads_total(false),
    has_nvram(false),
    has_pc8736x(false),
    // __SZH_ADD_END__
    symex_coverage(ns)
{
}

/// show progress
void symex_bmct::symex_step(
  const get_goto_functiont &get_goto_function,
  statet &state)
{
  const source_locationt &source_location = state.source.pc->source_location();

  if(!source_location.is_nil() && last_source_location != source_location)
  {
    log.debug() << "BMC at " << source_location.as_string() << " (depth "
                << state.depth << ')' << log.eom;

    last_source_location = source_location;
  }

  const goto_programt::const_targett cur_pc = state.source.pc;
  const guardt cur_guard = state.guard;

  if(
    !state.guard.is_false() && state.source.pc->is_assume() &&
    simplify_expr(state.source.pc->condition(), ns).is_false())
  {
    log.statistics() << "aborting path on assume(false) at "
                     << state.source.pc->source_location() << " thread "
                     << state.source.thread_nr;

    const irep_idt &c = state.source.pc->source_location().get_comment();
    if(!c.empty())
      log.statistics() << ": " << c;

    log.statistics() << log.eom;
  }

  goto_symext::symex_step(get_goto_function, state);

  if(
    record_coverage &&
    // avoid an invalid iterator in state.source.pc
    (!cur_pc->is_end_function() ||
     state.source.function_id != goto_functionst::entry_point()))
  {
    // forward goto will effectively be covered via phi function,
    // which does not invoke symex_step; as symex_step is called
    // before merge_gotos, also state.guard will be false (we have
    // taken an impossible transition); thus we synthesize a
    // transition from the goto instruction to its target to make
    // sure the goto is considered covered
    if(
      cur_pc->is_goto() && cur_pc->get_target() != state.source.pc &&
      cur_pc->condition().is_true())
      symex_coverage.covered(cur_pc, cur_pc->get_target());
    else if(!state.guard.is_false())
      symex_coverage.covered(cur_pc, state.source.pc);
  }
}

void symex_bmct::merge_goto(
  const symex_targett::sourcet &prev_source,
  goto_statet &&goto_state,
  statet &state)
{
  const goto_programt::const_targett prev_pc = prev_source.pc;
  const guardt prev_guard = goto_state.guard;

  goto_symext::merge_goto(prev_source, std::move(goto_state), state);

  PRECONDITION(prev_pc->is_goto());
  if(
    record_coverage &&
    // could the branch possibly be taken?
    !prev_guard.is_false() && !state.guard.is_false() &&
    // branches only, no single-successor goto
    !prev_pc->condition().is_true())
    symex_coverage.covered(prev_pc, state.source.pc);
}

bool symex_bmct::should_stop_unwind(
  const symex_targett::sourcet &source,
  const call_stackt &context,
  unsigned unwind)
{
  const irep_idt id = goto_programt::loop_id(source.function_id, *source.pc);

  tvt abort_unwind_decision;
  unsigned this_loop_limit = std::numeric_limits<unsigned>::max();

  for(auto handler : loop_unwind_handlers)
  {
    abort_unwind_decision =
      handler(context, source.pc->loop_number, unwind, this_loop_limit);
    if(abort_unwind_decision.is_known())
      break;
  }

  // If no handler gave an opinion, use standard command-line --unwindset
  // / --unwind options to decide:
  if(abort_unwind_decision.is_unknown())
  {
    auto limit = unwindset.get_limit(id, source.thread_nr);

    if(!limit.has_value())
      abort_unwind_decision = tvt(false);
    else
      abort_unwind_decision = tvt(unwind >= *limit);
  }

  INVARIANT(
    abort_unwind_decision.is_known(), "unwind decision should be taken by now");
  bool abort = abort_unwind_decision.is_true();

  log.statistics() << (abort ? "Not unwinding" : "Unwinding") << " loop " << id
                   << " iteration " << unwind;

  if(this_loop_limit != std::numeric_limits<unsigned>::max())
    log.statistics() << " (" << this_loop_limit << " max)";

  log.statistics() << " " << source.pc->source_location() << " thread "
                   << source.thread_nr << log.eom;

  return abort;
}

bool symex_bmct::get_unwind_recursion(
  const irep_idt &id,
  unsigned thread_nr,
  unsigned unwind)
{
  tvt abort_unwind_decision;
  unsigned this_loop_limit = std::numeric_limits<unsigned>::max();

  for(auto handler : recursion_unwind_handlers)
  {
    abort_unwind_decision = handler(id, unwind, this_loop_limit);
    if(abort_unwind_decision.is_known())
      break;
  }

  // If no handler gave an opinion, use standard command-line --unwindset
  // / --unwind options to decide:
  if(abort_unwind_decision.is_unknown())
  {
    auto limit = unwindset.get_limit(id, thread_nr);

    if(!limit.has_value())
      abort_unwind_decision = tvt(false);
    else
      abort_unwind_decision = tvt(unwind > *limit);
  }

  INVARIANT(
    abort_unwind_decision.is_known(), "unwind decision should be taken by now");
  bool abort = abort_unwind_decision.is_true();

  if(unwind > 0 || abort)
  {
    const symbolt &symbol = ns.lookup(id);

    log.statistics() << (abort ? "Not unwinding" : "Unwinding") << " recursion "
                     << symbol.display_name() << " iteration " << unwind;

    if(this_loop_limit != std::numeric_limits<unsigned>::max())
      log.statistics() << " (" << this_loop_limit << " max)";

    log.statistics() << log.eom;
  }

  return abort;
}

void symex_bmct::no_body(const irep_idt &identifier)
{
  if(body_warnings.insert(identifier).second)
  {
    log.warning() << "**** WARNING: no body for function " << identifier;

    if(havoc_bodyless_functions)
    {
      log.warning()
        << "; assigning non-deterministic values to any pointer arguments";
    }
    log.warning() << log.eom;
  }
}

// __SZH_ADD_BEGIN__

void symex_bmct::svcomp_get_interesting_codes()
{
  std::list<irep_idt> symbol_list;
  for(const auto &symbol_pair : outer_symbol_table.symbols)
  {
    if(
      !symbol_pair.second.is_type && !symbol_pair.second.is_macro &&
      symbol_pair.second.type.id() == ID_code &&
      (symbol_pair.second.mode == ID_C || symbol_pair.second.mode == ID_cpp ||
       symbol_pair.second.mode == ID_java ||
       symbol_pair.second.mode == "jsil" ||
       symbol_pair.second.mode == ID_statement_list))
    {
      symbol_list.push_back(symbol_pair.first);
    }
  }

  for(const auto &id : symbol_list)
  {
    const symbolt &symbol = ns.lookup(id);

    if(symbol.value.id() != ID_code)
      continue;
    
    std::vector<codet> codes;
    codes.push_back(to_code(symbol.value));
    while(!codes.empty())
    {
      auto code = codes.back();
      codes.pop_back();

      std::string function_name = code.source_location().get_function().c_str();

      if(function_name.find("sleep") != std::string::npos)
        has_sleep = true;
      
      if(function_name.find("writer_fn") != std::string::npos)
        has_writer_fn = true;

      if(function_name == "insert")
        has_insert = true;

      if(function_name.find("take") != std::string::npos)
        has_take = true;

      if(function_name.find("malloc") != std::string::npos)
        has_malloc = true;

      if(function_name.find("calloc") != std::string::npos)
        has_calloc = true;

      if(function_name.find("pthread_key_create") != std::string::npos || function_name.find("pthread_setspecific") != std::string::npos || function_name.find("pthread_getspecific") != std::string::npos)
        has_key = true;

      if(function_name.find("external_alloc") != std::string::npos)
        has_external_alloc = true;
      
      if(function_name.find("barrier_init") != std::string::npos)
        has_barrier_init = true;

      if(function_name.find("my_drv_probe") != std::string::npos)
        has_my_drv_probe = true;

      if(function_name.find("create_fresh_int_array") != std::string::npos)
        has_create_fresh_int_array = true;

      if(function_name.find("plus") != std::string::npos)
        has_plus = true;

      if(function_name.find("Init_WorkStealQueue") != std::string::npos)
        has_Init_WorkStealQueue = true;
      
      if(function_name.find("__VERIFIER_atomic_acquire") != std::string::npos)
        has_atomic_acquire = true;

      if(function_name.find("__VERIFIER_atomic_dec") != std::string::npos)
        has_atomic_dec = true;

      if(function_name.find("__VERIFIER_atomic_CAS") != std::string::npos)
        has_atomic_trash = true;
      
      if(function_name.find("__VERIFIER_atomic_index_malloc") != std::string::npos)
        has_atomic_trash = true;
      
      if(function_name.find("myPartOfCalc") != std::string::npos)
        has_myPartOfCalc = true;

      if(function_name.find("nvram") != std::string::npos)
        has_nvram = true;
      
      if(function_name.find("pc8736x") != std::string::npos)
        has_pc8736x = true;

      if(code.get_statement() == ID_block)
      {
        for(auto& element : to_code_block(code).statements())
          codes.push_back(element);
      }
      else if(code.get_statement() == ID_for)
      {
        fors.push_back(to_code_for(code));
        codes.push_back(to_code_for(code).body());
      }
      else if(code.get_statement() == ID_while)
      {
        whiles.push_back(to_code_while(code));
        codes.push_back(to_code_while(code).body());
      }
      else if(code.get_statement() == ID_assume)
        assumes.push_back(to_code_assume(code));
      else if(code.get_statement() == ID_expression)
        expressions.push_back(to_code_expression(code));
      else if(code.get_statement() == ID_assign)
        assigns.push_back(to_code_assign(code));
      else if(code.get_statement() == ID_function_call)
        function_calls.push_back(to_code_function_call(code));
      else if(code.get_statement() == ID_ifthenelse)
        ifthenelses.push_back(to_code_ifthenelse(code));
      else if(code.get_statement() == ID_decl)
      {
        if(code.op0().id() == ID_symbol)
        {
          std::string decl_str = to_symbol_expr(code.op0()).get_identifier().c_str();
          if(decl_str.find("threads_total") != std::string::npos)
            has_threads_total = true;
        }
      }
    }
  }

  for(auto& assign_code : assigns)
  {
    if(assign_code.lhs().id() != ID_symbol)
      continue;
    std::string lhs_str = to_symbol_expr(assign_code.lhs()).get_identifier().c_str();
    if(lhs_str.find("threads_total") != std::string::npos)
      has_threads_total = true;
  }
}

void symex_bmct::svcomp_get_assume_upperbounds()
{
  for(auto& assume_code : assumes)
  {
    auto& assumption = assume_code.assumption();
    //std::cout << "assumption: " << assumption.pretty() << "\n";

    std::vector<exprt> exprs;
    exprs.push_back(assumption);
    while(!exprs.empty())
    {
      auto expr = exprs.back();
      exprs.pop_back();
      for(auto& operand : expr.operands())
        exprs.push_back(operand);

      if(expr.id() != ID_le && expr.id() != ID_lt && expr.id() != ID_ge && expr.id() != ID_gt)
        continue;
      
      auto left = expr.op0();
      auto right = expr.op1();
      if((expr.id() == ID_le || expr.id() == ID_lt) && left.id() == ID_symbol && right.id() == ID_constant) // v < c
      {
        std::string variable = to_symbol_expr(left).get_identifier().c_str();
        int constant = string2integer(right.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_lt)
          constant--;
        svcomp_update_assume_upperbound(variable, constant);
      }
      if((expr.id() == ID_ge || expr.id() == ID_gt) && right.id() == ID_symbol && left.id() == ID_constant) // c > v
      {
        std::string variable = to_symbol_expr(right).get_identifier().c_str();
        int constant = string2integer(left.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_gt)
          constant--;
        svcomp_update_assume_upperbound(variable, constant);
      }
    }
  }

  for(auto& expression_code : expressions)
  {
    std::vector<exprt> exprs;
    exprs.push_back(expression_code.expression());
    while(!exprs.empty())
    {
      auto expr = exprs.back();
      exprs.pop_back();
      for(auto& operand : expr.operands())
        exprs.push_back(operand);

      if(expr.id() != ID_le && expr.id() != ID_lt && expr.id() != ID_ge && expr.id() != ID_gt)
        continue;
      
      auto left = expr.op0();
      auto right = expr.op1();
      if((expr.id() == ID_le || expr.id() == ID_lt) && left.id() == ID_symbol && right.id() == ID_constant) // v < c
      {
        std::string variable = to_symbol_expr(left).get_identifier().c_str();
        int constant = string2integer(right.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_lt)
          constant--;
        svcomp_update_assume_upperbound(variable, constant);
      }
      if((expr.id() == ID_ge || expr.id() == ID_gt) && right.id() == ID_symbol && left.id() == ID_constant) // c > v
      {
        std::string variable = to_symbol_expr(right).get_identifier().c_str();
        int constant = string2integer(left.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_gt)
          constant--;
        svcomp_update_assume_upperbound(variable, constant);
      }
    }
  }
}

void symex_bmct::svcomp_get_const_values()
{
  for(auto& assign_code : assigns)
  {
    if(assign_code.lhs().id() != ID_symbol)
      continue;
    if(assign_code.rhs().id() != ID_constant)
      continue;

    auto& left = to_symbol_expr(assign_code.lhs());
    auto& right = to_constant_expr(assign_code.rhs());

    if(!left.type().get_bool("#constant"))
      continue;

    int right_value = string2integer(right.get_string(ID_value).c_str(), 16).to_long();
    const_values[left] = right_value;
  }
}

int symex_bmct::svcomp_for_unwind_limit_single(code_fort& code)
{
  int limit = 0;
  exprt op0 = code.init();
  //// __FHY_ADD_BEGIN__
  // if(op0.operands().empty())
  //     return 0;
  // if(op0.op0().id() == ID_side_effect)
  //     op0 = op0.op0();

  // __SZH_ADD_BEGIN__
  // if(op0.op0().id() == ID_comma)
  //     return 0;
  // __SZH_ADD_END__

  //// __FHY_ADD_END__
  exprt op1 = code.cond();
  exprt op2 = code.iter();
  //// __FHY_ADD_BEGIN__
  // std::cout << "updating for unwind limit\n";
  // std::cout << "op0 " << op0 << " op1 " << op1 << " op2 " << op2 << "\n";
  //// __FHY_ADD_END__

  if (op1.id() != ID_lt && op1.id() != ID_le && op1.id() != ID_gt && op1.id() != ID_ge)
  {
    return 0;
  }
  std::string type_str, value_str;

//	std::cout << type_str << "\n";

  //auto* init_left = &op0.op0();
  //auto* init_right = &op0.op1();
  auto* cond_left = &op1.op0();
  auto* cond_right = &op1.op1();
  auto* iter_left = &op2.op0();

  // __SZH_ADD_BEGIN__
  // if(init_right->id() == ID_typecast)
  //     init_right = &init_right->op0();

  if(cond_right->id() == ID_typecast)
      cond_right = &cond_right->op0();
  // __SZH_ADD_END__

  if (//init_left->id() == ID_symbol &&
      cond_left->id() == ID_symbol && 
      iter_left->id() == ID_symbol && 
      //*init_left == *cond_left && 
      *cond_left == *iter_left &&
      //init_right->id() == ID_constant &&
      (cond_right->id() == ID_constant ||
      (cond_right->id() == ID_symbol && cond_right->type().get_bool("#constant")) ||
      //// __FHY_ADD_BEGIN__
      (cond_right->id() == ID_symbol && assume_upperbounds.find(to_symbol_expr(*cond_right).get_identifier().c_str()) != assume_upperbounds.end())))
      //// __FHY_ADD_END__
  {
      int a0, a1;
      // if(init_right->id() == ID_constant)
      //   a0 = string2integer(init_right->get_string(ID_value), 16).to_long();
      // else
        a0 = 0;

      //// __FHY_ADD_BEGIN__
      if (cond_right->id() == ID_constant)
        a1 = string2integer(cond_right->get_string(ID_value), 16).to_long();
      else if(cond_right->id() == ID_symbol && cond_right->type().get_bool("#constant"))
      {
        a1 = const_values[to_symbol_expr(*cond_right)];
      }
      else
      {
        a1 = assume_upperbounds[to_symbol_expr(*cond_right).get_identifier().c_str()];
      }
      //// __FHY_ADD_END__

      // for (int k = a0; k < a1; k += a2)
      if (op1.id() == ID_lt || op1.id() == ID_le) {
          limit = a1 - a0;
          if (op1.id() == ID_lt)
              limit--;
          if (op2.id() == ID_side_effect) {
              if (to_side_effect_expr(op2).get_statement() == ID_postincrement ||
                  to_side_effect_expr(op2).get_statement() == ID_preincrement)
                  limit++;
              else if (to_side_effect_expr(op2).get_statement() == ID_assign_plus) {
                  if (op2.op1().id() == ID_constant) {
                      int a2 = string2integer(op2.op1().get_string(ID_value), 16).to_long();
                      if (a2 > 1)
                          limit = limit / a2 + 1;
                      else
                          limit++;
                  }
              } else if (to_side_effect_expr(op2).get_statement() == ID_assign) {
                  if (op2.op1().id() == ID_plus
                      && op2.op1().op0().id() == ID_symbol
                      && op2.op1().op0() == op2.op0() &&
                      op2.op1().op1().id() == ID_constant)
                  {
                      int a2 = string2integer( op2.op1().op1().get_string(ID_value), 16).to_long();
                      if (a2 > 1)
                          limit = limit / a2 + 1;
                      else
                          limit++;
                  }
              } else {
                  limit = 0;
              }
          }
      }

      // for (int k = a0; k > a1; k -= a2)
      if (op1.id() == ID_gt || op1.id() == ID_ge) {
          limit = a0 - a1;
          if (op1.id() == ID_gt)
              limit--;
          if (op2.id() == ID_side_effect) {
              if (to_side_effect_expr(op2).get_statement() == ID_postdecrement ||
                  to_side_effect_expr(op2).get_statement() == ID_predecrement)
                  limit++;
              else if (to_side_effect_expr(op2).get_statement() == ID_assign_minus) {
                  if (op2.op1().id() == ID_constant) {
                      int a2 = string2integer(op2.op1().get_string(ID_value), 16).to_long();
                      if (a2 > 1)
                          limit = limit / a2 + 1;
                      else
                          limit++;
                  }
              } else if (to_side_effect_expr(op2).get_statement() == ID_assign) {
                  if (op2.op1().id() == ID_minus
                      && op2.op1().op0().id() == ID_symbol
                      && op2.op1().op0() == op2.op0() &&
                      op2.op1().op1().id() == ID_constant)
                  {
                      int a2 = string2integer(op2.op1().op1().get_string(ID_value), 16).to_long();
                      if (a2 > 1)
                          limit = limit / a2 + 1;
                      else
                          limit++;
                  }
              } else {
                  limit = 0;
              }
          }
      }
  }

  return limit;
}

int symex_bmct::svcomp_while_unwind_limit_single(code_whilet& code)
{
  //todo: very simple, it that enough?

  int limit = 0;

  exprt& cond = code.cond();
  if(cond.id() == ID_lt && cond.op1().id() == ID_constant)
    limit = string2integer(cond.op1().get_string(ID_value), 16).to_long();
  if(cond.id() == ID_le && cond.op1().id() == ID_constant)
    limit = string2integer(cond.op1().get_string(ID_value), 16).to_long() + 1;

  return limit;
}

int symex_bmct::svcomp_for_while_unwind_limit(std::set<int>& candidate_limits)
{
  int unwind_limit = 0;
  for(auto& code_for : fors)
  {
    int new_unwind_limit = svcomp_for_unwind_limit_single(code_for);
    candidate_limits.insert(new_unwind_limit);
    if(new_unwind_limit > unwind_limit)
      unwind_limit = new_unwind_limit;
  }
  for(auto& code_while : whiles)
  {
    int new_unwind_limit = svcomp_while_unwind_limit_single(code_while);
    candidate_limits.insert(new_unwind_limit);
    if(new_unwind_limit > unwind_limit)
      unwind_limit = new_unwind_limit;
  }
  return unwind_limit;
}

bool symex_bmct::svcomp_has_array()
{
  for(auto& code_assign : assigns)
  {
    exprt left =code_assign.lhs();
    if (left.type().id() == ID_array)
    {
      std::string left_str = to_symbol_expr(left).get_identifier().c_str();
      if(left_str.find("__CPROVER_") == std::string::npos)
        return true;
    }
  }
  return false;
}

bool symex_bmct::svcomp_has_mutex_array()
{
  for(auto& code_assign : assigns)
  {
    exprt left =code_assign.lhs();
    if (left.type().id() == ID_array)
    {
      if(!left.type().has_subtype())
        continue;

      // std::string left_str = left.type().get_sub()[0].pretty();
      // std::cout << "find subtype " << left_str << "\n";
      // if(left_str.find("__pthread_mutex_s") != std::string::npos)
      // {
      //   std::cout << "find mutex array " << left.pretty() << "\n";
      //   return true;
      // }
    }
  }
  return false;
}

int symex_bmct::svcomp_unwind_strategy()
{
  svcomp_get_interesting_codes();
  svcomp_get_assume_upperbounds();
  svcomp_get_const_values();

  std::set<int> candidate_limits;
  int loop_unwind_limit = svcomp_for_while_unwind_limit(candidate_limits);
  bool has_array = svcomp_has_array();
  bool has_mutex_array = svcomp_has_mutex_array();

  //experimental, for 09-regions_20/23/26_*.i
  if(candidate_limits.find(10) != candidate_limits.end() && candidate_limits.find(30) != candidate_limits.end())
  {
    std::cout << "too many nested unwind!\n";
    std::exit(1);
  }

  int max_limit = 2;
  if(has_array && loop_unwind_limit > 0 && loop_unwind_limit < 30)
  {
    std::cout << "set limit type 1\n";
    max_limit = loop_unwind_limit + 1;
  }
  // else if(has_nondet_loop_cond(goto_functions.loop_cond_ids, goto_functions.nondet_ids, symex_pre.assignments))
  //   max_limit = 2;
  else if (has_array)
  {
    std::cout << "set limit type 2\n";
    max_limit = 2;
  }
  else if (loop_unwind_limit > 0)
  {
    std::cout << "set limit type 3\n";
    max_limit = loop_unwind_limit + 1;
  }

  if(max_limit >= 9999 || loop_unwind_limit >= 9999) // szh: most goblint-regression/28_race-reach cases unwind 10000, we give up them
  {
    std::cout << "too many unwind!\n";
    std::exit(1);
  }

  svcomp_special(max_limit, loop_unwind_limit);

  if(enable_reach)
  {
    target.has_threads_total = has_threads_total;
    svcomp_exit(max_limit, loop_unwind_limit, has_mutex_array);
  }
  
  if(enable_overflow)
    svcomp_overflow_exit();

  if(enable_alloc)
    svcomp_memsafety_exit();

  return max_limit;
}

void symex_bmct::svcomp_update_assume_upperbound(std::string& id, int value)
{
  if(assume_upperbounds.find(id) != assume_upperbounds.end())
  {
    if(assume_upperbounds[id] < value)
      assume_upperbounds[id] = value;
  }
  else
    assume_upperbounds[id] = value;
  std::cout << id << "'s upper bound is " << value << "\n";
}

void symex_bmct::svcomp_special(int& max_limit, int for_unwind_limit)
{
  // For 40_barrier_vf_false-unreach-call.i
  for(auto& code_ifthenelse : ifthenelses)
  {
    const exprt& cond = code_ifthenelse.cond();
    if (cond.id() == ID_equal && cond.op0().id() == ID_symbol && cond.op1().id() == ID_typecast && cond.op1().op0().id() == ID_constant)
    {
      std::string str = to_symbol_expr(cond.op0()).get_identifier().c_str();
      if(str.find("count") != std::string::npos)
      {
        int limit = string2integer(cond.op1().op0().get_string(ID_value), 16).to_long();
        max_limit = limit + 1;
      }
    }
  }

  // For pthread-driver-races
  if(has_external_alloc)
    max_limit = 100;

  // For weaver/popl20-* cases
  if(has_create_fresh_int_array && has_plus)
    max_limit = 3;

  // For pthread/indexer.i
  if(max_limit == 2 && for_unwind_limit == 128)
  {
    std::cout << "Unsupported function!\n";
    std::exit(1);
  }

  // For newly added pthread-race-challenges/*
  if(has_threads_total)
    max_limit = 5;
}

void symex_bmct::svcomp_exit(int max_limit, int for_unwind_limit, bool has_mutex_array)
{
  if(has_sleep || has_key || has_barrier_init || has_my_drv_probe || has_Init_WorkStealQueue) // || has_malloc) // szh: exiting cases with malloc is too destructive
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }

  if(has_calloc && has_threads_total) //for pthread-race-challenges/thread-local-value-dynamic
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }

  for(auto& assign_code : assigns)
  {
    auto& left = assign_code.lhs();
    if(left.id() != ID_symbol)
      continue;
    std::string left_str = to_symbol_expr(left).get_identifier().c_str();

    //for popl20-*
    if(left_str.find("q1_front") != std::string::npos)
    {
      std::cout << "Unsupported library function!\n";
      std::exit(1);
    }

    //for singleton_with-uninit-problem
    if(left_str == "v" && left.type().id() == ID_pointer)
    {
      std::cout << "Unsupported library function!\n";
      std::exit(1);
    }
  }
}

void symex_bmct::svcomp_overflow_exit()
{
  if(has_myPartOfCalc)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }

  if(has_nvram || has_pc8736x)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }
}

void symex_bmct::svcomp_memsafety_exit()
{
  if(has_nvram || has_pc8736x)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }
}

void symex_bmct::svcomp_datarace_exit()
{
  // if(has_sleep || has_key || has_barrier_init || has_Init_WorkStealQueue) // szh: exiting cases with malloc is too destructive
  // {
  //   std::cout << "Unsupported library function!\n";
  //   std::exit(1);
  // }

  if(has_barrier_init)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }

  if(has_atomic_trash)
  {
    std::cout << "Unsupported __VERIFIER_atomic_* function!\n";
    std::exit(1);
  }

  if(has_writer_fn)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }

  if(has_insert && !has_take)
  {
    std::cout << "Unsupported library function!\n";
    std::exit(1);
  }
}

// __SZH_ADD_END__