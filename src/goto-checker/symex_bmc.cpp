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

void symex_bmct::get_interesting_codes()
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

    // std::cout << "The whole code of " << id << " is " << symbol.value.pretty() << "\n";

    while(!codes.empty())
    {
      auto code = codes.back();
      codes.pop_back();

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
      else if(code.get_statement() == ID_function_call)
        function_calls.push_back(to_code_function_call(code));
      else if(code.get_statement() == ID_assign)
        assigns.push_back(to_code_assign(code));
      // __WP_ADD_BEGIN__
      else if(code.get_statement() == ID_decl_block)
      {
        for(auto& element : to_code_block(code).statements())
          codes.push_back(element);
      }
      else if(code.get_statement() == ID_decl)
        decls.push_back(to_code_decl(code));
      else if(code.get_statement() == ID_ifthenelse)
      {
        ifthenelses.push_back(to_code_ifthenelse(code));
        codes.push_back(to_code_ifthenelse(code).then_case());
        codes.push_back(to_code_ifthenelse(code).else_case());
      }
      // __WP_ADD_END__
    }
  }
}

void symex_bmct::get_assume_upperbounds()
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
        update_assume_upperbound(variable, constant);
      }
      if((expr.id() == ID_ge || expr.id() == ID_gt) && right.id() == ID_symbol && left.id() == ID_constant) // c > v
      {
        std::string variable = to_symbol_expr(right).get_identifier().c_str();
        int constant = string2integer(left.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_gt)
          constant--;
        update_assume_upperbound(variable, constant);
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
        update_assume_upperbound(variable, constant);
      }
      if((expr.id() == ID_ge || expr.id() == ID_gt) && right.id() == ID_symbol && left.id() == ID_constant) // c > v
      {
        std::string variable = to_symbol_expr(right).get_identifier().c_str();
        int constant = string2integer(left.get_string(ID_value), 16).to_long();
        if(expr.id() == ID_gt)
          constant--;
        update_assume_upperbound(variable, constant);
      }
    }
  }
}

void symex_bmct::get_const_values()
{
  // __WP_ADD_BEGIN__
  for(auto& assign_code : assigns)
  {

    if(assign_code.lhs().id() != ID_symbol)
      continue;
    // if(assign_code.rhs().id() != ID_constant)
    //   continue;

    auto& left = to_symbol_expr(assign_code.lhs());
    // if(!left.type().get_bool("#constant"))
    //   continue;
    exprt simp_right = simplify_expr(assign_code.rhs(), ns);

    // std::cout << "left: " << left.pretty() << "\n" << "simp_right: " << simp_right.pretty() << "\n";
    // for (auto& const_value: const_values)
    //   std::cout << "const_value_id: " << const_value.first.pretty() << "\nconst_value: \n" << const_value.second << "\n";

    if(simp_right.id() == ID_typecast)
      simp_right = simp_right.op0();
    if(simp_right.id() != ID_constant &&
      !(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant"))
      // &&!(simp_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_right)) != const_values.end())
      )
      continue;

    if (simp_right.id() == ID_constant)
      const_values[left] = string2integer(simp_right.get_string(ID_value), 16).to_long();
    else if(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant"))
      const_values[left] = const_values[to_symbol_expr(simp_right)];
    // else if(simp_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_right)) != const_values.end())
    //   const_values[left] = const_values[to_symbol_expr(simp_right)];
  }

  for(auto& decl_code : decls)
  {
    // std::cout << "decl_const_value: " << decl_code.pretty() << "\n";

    if(decl_code.operands().size() == 1)
      continue;
    if(decl_code.op0().id() != ID_symbol)
      continue;

    auto& left = to_symbol_expr(decl_code.op0());
    // if(!left.type().get_bool("#constant"))
    //   continue;
    exprt simp_right = simplify_expr(decl_code.op1(), ns);

    // std::cout << "left: " << left.pretty() << "\n" << "simp_right: " << simp_right.pretty() << "\n";
    // for (auto& const_value: const_values)
    //   std::cout << "const_value_id: " << const_value.first.pretty() << "\nconst_value: \n" << const_value.second << "\n";


    if(simp_right.id() == ID_typecast)
      simp_right = simp_right.op0();
    if(simp_right.id() != ID_constant &&
      !(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant"))
      // &&!(simp_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_right)) != const_values.end())
      )
      continue;

    if (simp_right.id() == ID_constant)
      const_values[left] = string2integer(simp_right.get_string(ID_value), 16).to_long();
    else if(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant"))
      const_values[left] = const_values[to_symbol_expr(simp_right)];
    // else if(simp_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_right)) != const_values.end())
    //   const_values[left] = const_values[to_symbol_expr(simp_right)];
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

      if (expr.id() == ID_side_effect)
      {
        // std::cout << "expr_side_effect: " << expr.pretty() << "\n";

        if (static_cast<const codet &>(expr).get_statement() != ID_assign) {
          // std::cout << "static_cast<const codet &>(expr).get_statement(): " << static_cast<const codet &>(expr).get_statement() << "\n";
          continue;
        }

        if(expr.op0().id() != ID_symbol) {
          // std::cout << "expr.op0().id(): " << expr.op0().id() << "\n";
          continue;
        }

        exprt simp_right = simplify_expr(expr.op1(), ns);
        if(simp_right.id() == ID_typecast)
          simp_right = simp_right.op0();
        if(simp_right.id() != ID_constant &&
          !(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant")))
          continue;

        auto& left = to_symbol_expr(expr.op0());
        // if(!left.type().get_bool("#constant"))
        //   continue;

        if (simp_right.id() == ID_constant)
          const_values[left] = string2integer(simp_right.get_string(ID_value), 16).to_long();
        else if(simp_right.id() == ID_symbol && simp_right.type().get_bool("#constant"))
          const_values[left] = const_values[to_symbol_expr(simp_right)];

        // std::cout << "left: " << left.pretty() << "\n" << "right_value: " << right_value << "\n";

        continue;
      }
    }
  }
  // __WP_ADD_END__
}

int symex_bmct::for_unwind_limit_single(code_fort& code)
{
  // std::cout << "It's in for_unwind_limit_single.\n";

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
  // std::cout << "op0 " << op0.pretty() << " op1:" << op1 .pretty() << " op2:" << op2.pretty() << "\n";
  //// __FHY_ADD_END__

  if (op1.id() != ID_lt && op1.id() != ID_le && op1.id() != ID_gt && op1.id() != ID_ge)
  {
    return 0;
  }
  std::string type_str, value_str;

  //std::cout << type_str << "\n";

  //auto* init_left = &op0.op0();
  //auto* init_right = &op0.op1();
  auto* cond_left = &op1.op0();
  auto* cond_right = &op1.op1();
  exprt simp_cond_right = simplify_expr(*cond_right, ns);
  auto* iter_left = &op2.op0();

  // std::cout << iter_left->pretty() << "\n==========================\n";
  
  int a0 = 0;
  bool is_find_inital = false;
  auto iter_var_name = to_symbol_expr(*iter_left).get_identifier().c_str();

  // for (auto& const_value: const_values)
  //   std::cout << "const_value_id: " << const_value.first.pretty() << "\nconst_value: \n" << const_value.second << "\n";

  for (auto& const_value : const_values) {
    if (const_value.first.get_identifier().c_str() == iter_var_name) {
      a0 = const_value.second;
      is_find_inital = true;
      break;
    }
  }

  // for (int i = assigns.size() - 1; i >= 0; --i) {

  //   if(assigns[i].lhs().id() != ID_symbol)
  //     continue;
  //   if(assigns[i].rhs().id() != ID_constant)
  //     continue;
    
  //   auto& assign_left = to_symbol_expr(assigns[i].lhs());

  //   std::cout << assigns[i].lhs().pretty() << "\n";

  //   if(!assign_left.type().get_bool("#constant"))
  //     continue;
    
  //   if (assign_left.get_identifier().c_str() == iter_var_name) {
  //     a0 = const_values[assign_left];
  //     is_find_inital = true;
  //     break;
  //   }
  // }

  if (!is_find_inital) {
    std::cout << "No Find Inital\n";
    return 0;
  }

  // __SZH_ADD_BEGIN__
  // if(init_right->id() == ID_typecast)
  //     init_right = &init_right->op0();

  if(simp_cond_right.id() == ID_typecast)
      simp_cond_right = simp_cond_right.op0();
  // __SZH_ADD_END__

  // std::cout << "cond_left:" << cond_left->pretty() << "\ncond_right:" << simp_cond_right.pretty() << "\n";

  if (cond_left->id() == ID_symbol && 
      iter_left->id() == ID_symbol && 
      *cond_left == *iter_left &&
      (simp_cond_right.id() == ID_constant ||
      (simp_cond_right.id() == ID_symbol && simp_cond_right.type().get_bool("#constant")) ||
      (simp_cond_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_cond_right)) != const_values.end())||
      (simp_cond_right.id() == ID_symbol && assume_upperbounds.find(to_symbol_expr(simp_cond_right).get_identifier().c_str()) != assume_upperbounds.end())))
  {
      int a1;

      if (simp_cond_right.id() == ID_constant)
        a1 = string2integer(simp_cond_right.get_string(ID_value), 16).to_long();
      else if(simp_cond_right.id() == ID_symbol && simp_cond_right.type().get_bool("#constant"))
        a1 = const_values[to_symbol_expr(simp_cond_right)];
      else if (simp_cond_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_cond_right)) != const_values.end())
        a1 = const_values[to_symbol_expr(simp_cond_right)];
      else
        a1 = assume_upperbounds[to_symbol_expr(simp_cond_right).get_identifier().c_str()];
      
      // std::cout << "a0:" << a0 << ";  a1:" << a1 << "\n";

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
          // std::cout << format(code) << " is a decrement one\n";
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


// __WP_ADD_BEGIN__

int symex_bmct::while_unwind_limit_single(code_whilet& code) {
  int limit = 0;

  exprt op1 = code.cond();

  if (op1.id() != ID_lt && op1.id() != ID_le && op1.id() != ID_gt && op1.id() != ID_ge)
    return 0;

  auto* cond_left = &op1.op0();
  auto* cond_right = &op1.op1();
  exprt simp_cond_right = simplify_expr(*cond_right, ns);

  if (cond_left->id() != ID_symbol)
    return 0;
  
  if(simp_cond_right.id() == ID_typecast)
      simp_cond_right = simp_cond_right.op0();
  
  if(simp_cond_right.id() != ID_constant &&
      !(simp_cond_right.id() == ID_symbol && simp_cond_right.type().get_bool("#constant")) &&
      !(simp_cond_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_cond_right)) != const_values.end()) &&
      !(simp_cond_right.id() == ID_symbol && assume_upperbounds.find(to_symbol_expr(simp_cond_right).get_identifier().c_str()) != assume_upperbounds.end()))
      return 0;
  
  int a1;

  if (simp_cond_right.id() == ID_constant)
    a1 = string2integer(simp_cond_right.get_string(ID_value), 16).to_long();
  else if(simp_cond_right.id() == ID_symbol && simp_cond_right.type().get_bool("#constant"))
    a1 = const_values[to_symbol_expr(simp_cond_right)];
  else if (simp_cond_right.id() == ID_symbol && const_values.find(to_symbol_expr(simp_cond_right)) != const_values.end())
    a1 = const_values[to_symbol_expr(simp_cond_right)];
  else
    a1 = assume_upperbounds[to_symbol_expr(simp_cond_right).get_identifier().c_str()];

  int a0 = 0;
  bool is_find_inital = false;

  if(cond_left->id() != ID_symbol)
  {
    std::cout << "No Find Cond-variable\n";
    return 0;
  }
  auto cond_var_name = to_symbol_expr(*cond_left).get_identifier().c_str();

  // for (auto& const_value: const_values)
  //   std::cout << "const_value_id: " << const_value.first.pretty() << "\nconst_value: \n" << const_value.second << "\n";

  for (auto& const_value : const_values) {
    if (const_value.first.get_identifier().c_str() == cond_var_name) {
      a0 = const_value.second;
      is_find_inital = true;
      break;
    }
  }

  if (!is_find_inital) {
    std::cout << "No Find Initial\n";
    return 0;
  }

  codet& body = code.body();
  std::vector<codet> body_codes;
  body_codes.push_back(to_code(body));

  // std::cout << "The whole code of " << id << " is " << symbol.value.pretty() << "\n";

  std::vector<code_expressiont> body_expressions;
  while(!body_codes.empty())
  {
    auto code = body_codes.back();
    body_codes.pop_back();

    if(code.get_statement() == ID_block)
    {
      for(auto& element : to_code_block(code).statements())
        body_codes.push_back(element);
    }
    else if(code.get_statement() == ID_expression)
      body_expressions.push_back(to_code_expression(code));
  }

  std::vector<exprt> exprs;
  for(auto& expression_code : body_expressions)
    exprs.push_back(expression_code.expression());
  
  while(!exprs.empty())
  {
    auto op2 = exprs.back();
    exprs.pop_back();
    for(auto& operand : op2.operands())
      exprs.push_back(operand);
    
    if (op2.id() != ID_side_effect)
      continue;

    if (op2.op0().id() != ID_symbol || to_symbol_expr(op2.op0()).get_identifier().c_str() != cond_var_name)
      continue;

    // for (int k = a0; k < a1; k += a2)
    if (op1.id() == ID_lt || op1.id() == ID_le) {
        limit = a1 - a0;
        if (op1.id() == ID_lt)
            limit--;
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
        } else
            limit = 0;
    }

    // for (int k = a0; k > a1; k -= a2)
    if (op1.id() == ID_gt || op1.id() == ID_ge) {
        limit = a0 - a1;
        if (op1.id() == ID_gt)
            limit--;
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
        } else
            limit = 0;
      }

      if (limit)
        return limit;
  }

  return 0;
}

// __WP_ADD_END__
#include <util/format_expr.h>
void symex_bmct::unwind_suggest()
{
  get_interesting_codes();
  get_assume_upperbounds();
  get_const_values();

  // __WP_ADD_BEGIN__
  for(auto& code_for : fors)
  {
    int new_unwind_limit = for_unwind_limit_single(code_for);
    if(new_unwind_limit != 0)
      single_unwind_limit[code_for.source_location()] = new_unwind_limit + 1;
  }

  for(auto& code_while : whiles)
  {
    int new_unwind_limit = while_unwind_limit_single(code_while);
    if(new_unwind_limit != 0)
      single_unwind_limit[code_while.source_location()] = new_unwind_limit + 1;
  }
  // __WP_ADD_END__
}

void symex_bmct::update_assume_upperbound(std::string& id, int value)
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

void symex_bmct::show_unwind_suggest(const goto_functionst &goto_functions)
{
  for(const auto& f : goto_functions.function_map)
  {
    auto& function_id = f.first;
    auto& goto_program = f.second.body;
    for(const auto& instruction : goto_program.instructions)
    {
      if(instruction.is_backwards_goto())
      {
        std::cout << "Loop "
                  << goto_programt::loop_id(function_id, instruction) << ":"
                  << "\n";

        std::cout << "  " << instruction.source_location() << "\n";

        auto& original_code = instruction.code();
        if(single_unwind_limit.find(original_code.source_location()) != single_unwind_limit.end())
        {
          int unwind_limit = single_unwind_limit[original_code.source_location()];
          std::cout << "Suggested unwind limit: " << unwind_limit << "\n";
        }
        else
          std::cout << "No suggested unwind limit\n";
        // __SZH_ADD_END__
      }
    }
  }
}

// __SZH_ADD_END__