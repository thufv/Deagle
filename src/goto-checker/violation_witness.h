#ifndef CPROVER_GOTO_CHECKER_VIOLATION_WITNESS_H
#define CPROVER_GOTO_CHECKER_VIOLATION_WITNESS_H

class decision_proceduret;
class goto_tracet;
class namespacet;
class optionst;
class symex_target_equationt;
class ui_message_handlert;

void output_violation_witness(
  const goto_tracet &trace,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const namespacet &ns,
  const optionst &options,
  ui_message_handlert &message_handler);

#endif
