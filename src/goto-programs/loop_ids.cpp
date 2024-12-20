/*******************************************************************\

Module: Loop IDs

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

/// \file
/// Loop IDs

#include "loop_ids.h"

#include <iostream>

#include <util/json_irep.h>
#include <util/xml_irep.h>

#include "goto_model.h"

void show_loop_ids(
  ui_message_handlert::uit ui,
  const goto_modelt &goto_model)
{
  show_loop_ids(ui, goto_model.goto_functions);
}

void show_loop_ids(
  ui_message_handlert::uit ui,
  const irep_idt &function_id,
  const goto_programt &goto_program)
{
  switch(ui)
  {
    case ui_message_handlert::uit::PLAIN:
    {
      for(const auto &instruction : goto_program.instructions)
      {
        if(instruction.is_backwards_goto())
        {
          std::cout << "Loop "
                    << goto_programt::loop_id(function_id, instruction) << ":"
                    << "\n";

          std::cout << "  " << instruction.source_location() << "\n";
        }
      }
      break;
    }
    case ui_message_handlert::uit::XML_UI:
    {
      for(const auto &instruction : goto_program.instructions)
      {
        if(instruction.is_backwards_goto())
        {
          std::string id =
            id2string(goto_programt::loop_id(function_id, instruction));

          xmlt xml_loop("loop", {{"name", id}}, {});
          xml_loop.new_element("loop-id").data=id;
          xml_loop.new_element() = xml(instruction.source_location());
          std::cout << xml_loop << "\n";
        }
      }
      break;
    }
    case ui_message_handlert::uit::JSON_UI:
      UNREACHABLE; // use function below
  }
}

void show_loop_ids_json(
  ui_message_handlert::uit ui,
  const irep_idt &function_id,
  const goto_programt &goto_program,
  json_arrayt &loops)
{
  PRECONDITION(ui == ui_message_handlert::uit::JSON_UI); // use function above

  for(const auto &instruction : goto_program.instructions)
  {
    if(instruction.is_backwards_goto())
    {
      std::string id =
        id2string(goto_programt::loop_id(function_id, instruction));

      loops.push_back(json_objectt(
        {{"name", json_stringt(id)},
         {"sourceLocation", json(instruction.source_location())}}));
    }
  }
}

void show_loop_ids(
  ui_message_handlert::uit ui,
  const goto_functionst &goto_functions)
{
  switch(ui)
  {
    case ui_message_handlert::uit::PLAIN:
    case ui_message_handlert::uit::XML_UI:
      for(const auto &f: goto_functions.function_map)
        show_loop_ids(ui, f.first, f.second.body);
      break;

    case ui_message_handlert::uit::JSON_UI:
      json_objectt json_result;
      json_arrayt &loops=json_result["loops"].make_array();

      for(const auto &f : goto_functions.function_map)
        show_loop_ids_json(ui, f.first, f.second.body, loops);

      std::cout << ",\n" << json_result;
      break;
  }
}
