/*******************************************************************\

Module: Goto Programs with Functions

Author: Daniel Kroening

Date: June 2003

\*******************************************************************/

#ifndef CPROVER_GOTO_FUNCTIONS_H
#define CPROVER_GOTO_FUNCTIONS_H

#include "goto_program.h"
#include "goto_functions_template.h"

class goto_functionst:public goto_functions_templatet<goto_programt>
{
public:
	bool is_same_func(const namespacet &ns, irep_idt func_name1, irep_idt func_name2) {
		goto_programt& func1 = function_map[func_name1].body;
		goto_programt& func2 = function_map[func_name2].body;

		if (func1.instructions.size() != func2.instructions.size())
			return false;

		typedef std::list<goto_program_templatet<codet, exprt>::instructiont> instructions_t;
		instructions_t::iterator it = func1.instructions.begin();
		instructions_t::iterator jt = func2.instructions.begin();
		for (; it != func1.instructions.end(); it++, jt++) {
			std::string code1 = from_expr(ns, "", it->code);
			std::string code2 = from_expr(ns, "", jt->code);

			if (code1.find(as_string(func_name1).substr(3, func_name1.size() - 3), 0) != std::string::npos &&
				code2.find(as_string(func_name2).substr(3, func_name1.size() - 3), 0) != std::string::npos)
				continue;

			if (code1 != code2)
			{
				return false;
			}
		}
		return true;
	}
};

#define Forall_goto_functions(it, functions) \
  for(goto_functionst::function_mapt::iterator it=(functions).function_map.begin(); \
      it!=(functions).function_map.end(); it++)
 
#define forall_goto_functions(it, functions) \
  for(goto_functionst::function_mapt::const_iterator it=(functions).function_map.begin(); \
      it!=(functions).function_map.end(); it++)

void get_local_identifiers(
  const goto_function_templatet<goto_programt> &goto_function,
  std::set<irep_idt> &dest);

#endif
