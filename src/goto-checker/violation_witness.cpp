#include "violation_witness.h"

#include "bmc_util.h"

#include <goto-programs/goto_trace.h>
#include <goto-symex/symex_target_equation.h>
#include <solvers/decision_procedure.h>
#include <util/config.h>
#include <util/arith_tools.h>
#include <util/find_symbols.h>
#include <util/namespace.h>
#include <util/options.h>
#include <util/pointer_expr.h>
#include <util/symbol.h>
#include <util/ui_message.h>
#include <util/version.h>
#include <xmllang/sha256.h>
#include <xmllang/graphml.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace
{
struct locationt
{
  unsigned line;
  unsigned column;
};

struct waypointt
{
  std::string type;
  unsigned thread;
  locationt location;
};

enum class violationt
{
  reach,
  overflow,
  dereference,
  free,
  memtrack,
  datarace
};

const char *specification(violationt violation)
{
  switch(violation)
  {
  case violationt::reach:
    return "CHECK( init(main()), LTL(G ! call(reach_error())) )";
  case violationt::overflow:
    return "CHECK( init(main()), LTL(G ! overflow) )";
  case violationt::dereference:
    return "CHECK( init(main()), LTL(G valid-deref) )";
  case violationt::free:
    return "CHECK( init(main()), LTL(G valid-free) )";
  case violationt::memtrack:
    return "CHECK( init(main()), LTL(G valid-memtrack) )";
  case violationt::datarace:
    return "CHECK( init(main()), LTL(G ! data-race) )";
  }
  throw std::runtime_error("unknown violation property");
}

violationt classify_violation(
  const goto_trace_stept &failure,
  const optionst &options)
{
  const std::string property = id2string(failure.property_id);
  const std::string property_class =
    id2string(failure.pc->source_location().get_property_class());
  if(property.find("alloc.assertion.") != std::string::npos)
    throw std::runtime_error("allocation-size check is not an SV-COMP property");
  if(options.get_bool_option("datarace") &&
     property.find("nodatarace.assertion.") == 0)
    return violationt::datarace;
  if(options.get_bool_option("signed-overflow-check") ||
     options.get_bool_option("unsigned-overflow-check"))
  {
    if(property_class == "overflow")
      return violationt::overflow;
  }
  else if(options.get_bool_option("pointer-check"))
  {
    if(property_class == "pointer dereference")
      return violationt::dereference;
    if(property_class == "precondition_instance")
      return violationt::free;
    if(property_class == "memory-leak")
      return violationt::memtrack;
  }
  else if(!options.get_bool_option("datarace"))
    return violationt::reach;
  throw std::runtime_error("failed assertion is not a supported property");
}

const exprt &without_typecasts(const exprt &value)
{
  const exprt *current = &value;
  while(current->id() == ID_typecast)
    current = &current->op0();
  return *current;
}

bool same_object(const exprt &lhs, const exprt &rhs)
{
  const exprt &left = without_typecasts(lhs);
  const exprt &right = without_typecasts(rhs);
  if(left.id() == ID_annotated_pointer_constant &&
     right.id() == ID_annotated_pointer_constant)
  {
    const std::size_t offset_bits =
      config.ansi_c.pointer_width - config.bv_encoding.object_bits;
    const auto object_id = [offset_bits](const exprt &value) {
      return bvrep2integer(
               to_annotated_pointer_constant_expr(value).get_value(),
               config.ansi_c.pointer_width,
               false) >>
             offset_bits;
    };
    return object_id(left) == object_id(right);
  }
  return false;
}

bool contains_object(const exprt &value, const exprt &object)
{
  if(same_object(value, object))
    return true;
  for(const auto &operand : value.operands())
    if(contains_object(operand, object))
      return true;
  return false;
}

std::string basename(const std::string &path)
{
  const std::size_t slash = path.find_last_of("/\\");
  return path.substr(slash == std::string::npos ? 0 : slash + 1);
}

std::string quote(const std::string &value)
{
  std::ostringstream out;
  out << '"';
  for(unsigned char c : value)
  {
    if(c == '"' || c == '\\')
      out << '\\' << c;
    else if(c == '\n')
      out << "\\n";
    else if(c == '\r')
      out << "\\r";
    else if(c == '\t')
      out << "\\t";
    else if(c < 0x20)
      out << "\\x" << std::hex << std::setw(2) << std::setfill('0')
          << static_cast<unsigned>(c) << std::dec;
    else
      out << c;
  }
  return out.str() + '"';
}

std::string uuid()
{
  std::random_device random;
  unsigned char bytes[16];
  for(auto &byte : bytes)
    byte = static_cast<unsigned char>(random());
  bytes[6] = static_cast<unsigned char>((bytes[6] & 0x0f) | 0x40);
  bytes[8] = static_cast<unsigned char>((bytes[8] & 0x3f) | 0x80);
  std::ostringstream out;
  for(unsigned i = 0; i < 16; ++i)
  {
    if(i == 4 || i == 6 || i == 8 || i == 10)
      out << '-';
    out << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<unsigned>(bytes[i]);
  }
  return out.str();
}

std::string creation_time()
{
  const std::time_t now = std::time(nullptr);
  const std::tm *utc = std::gmtime(&now);
  if(utc == nullptr)
    throw std::runtime_error("cannot determine creation time");
  char buffer[32];
  if(std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", utc) == 0)
    throw std::runtime_error("cannot format creation time");
  return buffer;
}

bool word_char(char c)
{
  return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

class source_filet
{
public:
  explicit source_filet(const std::string &path) : input_path(path)
  {
    std::ifstream input(path, std::ios::binary);
    if(!input)
      throw std::runtime_error("cannot read task input file: " + path);
    const std::string content(
      (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    hash = ly::Sha256().getHexMessageDigest(content);
    std::istringstream lines_input(content);
    std::string line;
    std::string logical_file = path;
    unsigned logical_line = 1;
    const std::regex marker(
      R"marker(^\s*#\s*(?:line\s+)?([0-9]+)\s+"([^"]+)")marker");
    while(std::getline(lines_input, line))
    {
      if(!line.empty() && line.back() == '\r')
        line.pop_back();
      lines.push_back(line);
      std::smatch match;
      if(std::regex_search(line, match, marker))
      {
        logical_locations.emplace_back(logical_file, logical_line);
        logical_line = static_cast<unsigned>(std::stoul(match[1].str()));
        logical_file = match[2].str();
      }
      else
      {
        logical_locations.emplace_back(logical_file, logical_line);
        ++logical_line;
      }
    }
  }

  std::string hash;

  locationt expression_location(const source_locationt &source) const
  {
    if(source.is_nil() || source.get_line().empty())
      throw std::runtime_error("violation has no source line");
    const std::string file = id2string(source.get_file());
    const unsigned requested_line =
      static_cast<unsigned>(std::stoul(id2string(source.get_line())));
    std::vector<unsigned> candidates;
    if(basename(file) == basename(input_path) &&
       requested_line >= 1 && requested_line <= lines.size())
      candidates.push_back(requested_line);
    for(unsigned line = 1; line <= logical_locations.size(); ++line)
      if(basename(logical_locations[line - 1].first) == basename(file) &&
         logical_locations[line - 1].second == requested_line &&
         std::find(candidates.begin(), candidates.end(), line) ==
           candidates.end())
        candidates.push_back(line);
    if(candidates.size() != 1)
      throw std::runtime_error("cannot uniquely map violation to task input");

    const unsigned line = candidates.front();
    const std::string &text = lines[line - 1];
    const std::string column_text = id2string(source.get_column());
    if(!column_text.empty())
    {
      const unsigned column = static_cast<unsigned>(std::stoul(column_text));
      if(column == 0 || column > text.size())
        throw std::runtime_error("violation column is outside task input");
      return {line, column};
    }
    return {line, 0};
  }

  locationt call_location(
    const source_locationt &source,
    const std::string &function,
    bool closing_parenthesis) const
  {
    if(source.is_nil())
      throw std::runtime_error("call has no source location");
    const std::string source_file = id2string(source.get_file());
    const std::string source_line = id2string(source.get_line());
    if(source_line.empty())
      throw std::runtime_error("call has no source line");
    const unsigned requested_line = static_cast<unsigned>(std::stoul(source_line));
    std::vector<unsigned> candidates;
    if(basename(source_file) == basename(input_path) &&
       requested_line >= 1 && requested_line <= lines.size())
      candidates.push_back(requested_line);
    for(unsigned line = 1; line <= logical_locations.size(); ++line)
    {
      const auto &logical = logical_locations[line - 1];
      if(basename(logical.first) == basename(source_file) &&
         logical.second == requested_line &&
         std::find(candidates.begin(), candidates.end(), line) ==
           candidates.end())
        candidates.push_back(line);
    }

    std::vector<std::pair<locationt, locationt>> matches;
    for(unsigned line : candidates)
    {
      const std::string &text = lines[line - 1];
      std::size_t pos = text.find(function);
      while(pos != std::string::npos)
      {
        const std::size_t end = pos + function.size();
        if((pos == 0 || !word_char(text[pos - 1])) &&
           (end == text.size() || !word_char(text[end])))
        {
          std::size_t open = end;
          while(open < text.size() &&
                std::isspace(static_cast<unsigned char>(text[open])))
            ++open;
          if(open < text.size() && text[open] == '(')
          {
            const locationt function_location{
              line, static_cast<unsigned>(pos + 1)};
            if(closing_parenthesis)
              matches.emplace_back(
                function_location, find_closing_parenthesis(line, open));
            else
              matches.emplace_back(function_location, function_location);
          }
        }
        pos = text.find(function, pos + 1);
      }
    }
    if(matches.empty())
      throw std::runtime_error(
        "cannot locate " + function + " in task input");

    std::size_t selected = 0;
    if(matches.size() != 1)
    {
      const std::string source_column = id2string(source.get_column());
      if(source_column.empty())
        throw std::runtime_error(
          "cannot distinguish repeated " + function + " calls");
      const unsigned column = static_cast<unsigned>(std::stoul(source_column));
      std::vector<std::size_t> matching_columns;
      for(std::size_t i = 0; i < matches.size(); ++i)
        if(matches[i].first.column == column)
          matching_columns.push_back(i);
      if(matching_columns.size() != 1)
        throw std::runtime_error(
          "cannot distinguish repeated " + function + " calls");
      selected = matching_columns.front();
    }

    const locationt result = matches[selected].second;
    if(closing_parenthesis)
      return matches.size() == 1 ? locationt{result.line, 0} : result;

    // A line-only target may resolve to an earlier statement on the same line.
    const std::string &text = lines[result.line - 1];
    const auto first = text.find_first_not_of(" \t");
    if(first == std::string::npos || first + 1 == result.column)
      return {result.line, 0};
    return result;
  }

private:
  locationt find_closing_parenthesis(unsigned start_line, std::size_t open) const
  {
    unsigned depth = 0;
    for(unsigned line = start_line; line <= lines.size(); ++line)
    {
      const std::string &text = lines[line - 1];
      for(std::size_t column = line == start_line ? open : 0;
          column < text.size(); ++column)
      {
        if(text[column] == '(')
          ++depth;
        else if(text[column] == ')' && --depth == 0)
          return {line, static_cast<unsigned>(column + 1)};
      }
      if(line - start_line > 32)
        break;
    }
    throw std::runtime_error("unterminated pthread_create call");
  }

  std::string input_path;
  std::vector<std::string> lines;
  std::vector<std::pair<std::string, unsigned>> logical_locations;
};

const goto_trace_stept *memtrack_loss_step(
  const goto_tracet &trace,
  const source_filet &source)
{
  exprt leaked_object;
  leaked_object.make_nil();
  for(const auto &step : trace.steps)
  {
    if(!step.is_assignment() || step.full_lhs.id() != ID_symbol ||
       step.full_lhs_value.is_nil() ||
       step.full_lhs_value.type().id() != ID_pointer)
      continue;
    const irep_idt identifier = to_symbol_expr(step.full_lhs).get_identifier();
    if(identifier == "__CPROVER_memory_leak")
      leaked_object = step.full_lhs_value;
  }
  if(leaked_object.is_nil())
    throw std::runtime_error("failed leak object is absent from trace");
  if(without_typecasts(leaked_object).id() != ID_annotated_pointer_constant)
    throw std::runtime_error("leaked object has no concrete pointer identity");

  std::map<irep_idt, exprt> pointers;
  for(const auto &step : trace.steps)
  {
    if(!step.is_assignment() || step.full_lhs_value.is_nil())
      continue;
    if(step.full_lhs_value.type().id() != ID_pointer)
    {
      if(contains_object(step.full_lhs_value, leaked_object))
        throw std::runtime_error("leaked object is stored in an aggregate");
      continue;
    }
    if(step.full_lhs.id() != ID_symbol)
    {
      if(same_object(step.full_lhs_value, leaked_object))
        throw std::runtime_error("leaked object has a compound pointer alias");
      continue;
    }
    const irep_idt identifier = to_symbol_expr(step.full_lhs).get_identifier();
    const std::string name = id2string(identifier);
    if(name.compare(0, 8, "malloc::") == 0 ||
       name.compare(0, 12, "goto_symex::") == 0 ||
       name.find("__CPROVER") != std::string::npos ||
       name.find("::$tmp::") != std::string::npos)
      continue;
    if(name.find("::") != std::string::npos)
    {
      if(same_object(step.full_lhs_value, leaked_object))
        throw std::runtime_error("leaked object has a local pointer alias: " + name);
      continue;
    }
    const auto previous = pointers.find(identifier);
    if(previous != pointers.end() &&
       same_object(previous->second, leaked_object) &&
       !same_object(step.full_lhs_value, leaked_object))
    {
      bool aliased = false;
      for(const auto &entry : pointers)
        if(entry.first != identifier &&
           same_object(entry.second, leaked_object))
          aliased = true;
      if(!aliased)
      {
        try
        {
          source.expression_location(step.pc->source_location());
          return &step;
        }
        catch(const std::exception &)
        {
          throw std::runtime_error("lost pointer has no task-input location");
        }
      }
    }
    pointers[identifier] = step.full_lhs_value;
  }
  throw std::runtime_error("cannot identify the lost allocation in trace");
}

std::string data_model()
{
  if(config.ansi_c.int_width == 32 && config.ansi_c.long_int_width == 32 &&
     config.ansi_c.pointer_width == 32)
    return "ILP32";
  if(config.ansi_c.int_width == 32 && config.ansi_c.long_int_width == 64 &&
     config.ansi_c.pointer_width == 64)
    return "LP64";
  throw std::runtime_error("unsupported data model for YAML witness");
}

std::vector<waypointt> datarace_targets(
  const goto_trace_stept &failure,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const source_filet &source)
{
  // One assertion can group several event pairs at the same source lines.
  // Select a true race from this assertion, not an arbitrary failing property.
  const auto symbols = find_symbol_identifiers(failure.cond_expr);
  for(std::size_t i = 0; i < equation.numbered_dataraces.size(); ++i)
  {
    const symbol_exprt race("race" + std::to_string(i), bool_typet());
    if(symbols.count(race.get_identifier()) && solver.get(race).is_true())
    {
      std::vector<waypointt> targets;
      const auto &events = equation.numbered_dataraces[i];
      for(const auto event : {events.first, events.second})
        targets.push_back(
          {"target",
           event->source.thread_nr,
           source.expression_location(event->source.pc->source_location())});
      return targets;
    }
  }
  throw std::runtime_error("cannot identify the conflicting accesses");
}

mp_integer spawn_order(
  const symex_target_equationt::witness_spawn_eventt &spawn,
  const symex_target_equationt &equation,
  const decision_proceduret &solver)
{
  if(spawn.clock.is_not_nil())
  {
    const auto order = equation.oc_result_order.find(
      id2string(to_symbol_expr(spawn.clock).get_identifier()));
    if(order != equation.oc_result_order.end())
      return order->second;
    const auto value = numeric_cast<mp_integer>(solver.get(spawn.clock));
    if(value.has_value())
      return *value;
  }
  throw std::runtime_error("cannot determine thread creation order");
}

std::vector<const symex_target_equationt::witness_spawn_eventt *> required_spawns(
  const std::vector<waypointt> &targets,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  bool order_by_model)
{
  std::map<unsigned, const symex_target_equationt::witness_spawn_eventt *> spawns;
  for(const auto &spawn : equation.witness_spawn_events)
    if(solver.get(spawn.guard).is_true())
      spawns[spawn.child_thread] = &spawn;

  std::vector<const symex_target_equationt::witness_spawn_eventt *> ancestors;
  std::set<unsigned> needed_threads;
  for(const auto &target : targets)
  {
    for(unsigned thread = target.thread;
        thread != 0 && needed_threads.insert(thread).second;)
    {
      const auto found = spawns.find(thread);
      if(found == spawns.end())
        throw std::runtime_error("cannot register target thread");
      ancestors.push_back(found->second);
      thread = found->second->creator_thread;
    }
  }
  std::reverse(ancestors.begin(), ancestors.end());
  if(order_by_model)
  {
    std::map<unsigned, mp_integer> order;
    for(const auto *spawn : ancestors)
      order.emplace(spawn->child_thread, spawn_order(*spawn, equation, solver));
    std::stable_sort(
      ancestors.begin(), ancestors.end(),
      [&order](const symex_target_equationt::witness_spawn_eventt *left,
               const symex_target_equationt::witness_spawn_eventt *right) {
        return order.at(left->child_thread) < order.at(right->child_thread);
      });
  }
  return ancestors;
}

std::string thread_entry(
  unsigned thread,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const namespacet &ns)
{
  // The entry routine can be indirect, and the race may be inside a helper.
  for(const auto &step : equation.SSA_steps)
    if(step.is_function_call() && step.source.thread_nr == thread &&
       step.source.function_id == "__spawned_thread" &&
       solver.get(step.guard).is_true())
      return id2string(ns.lookup(step.called_function).base_name);
  throw std::runtime_error("cannot identify thread entry function");
}

std::string render_datarace_graphml(
  const goto_trace_stept &failure,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const namespacet &ns,
  const optionst &options)
{
  const std::string input_path = options.get_option("filename");
  const source_filet source(input_path);
  const auto targets = datarace_targets(failure, equation, solver, source);
  graphmlt graph;
  graph.key_values["sourcecodelang"] = "C";
  graph.key_values["specification"] = specification(violationt::datarace);
  const auto add_node = [&graph]() {
    const auto node = graph.add_node();
    graph[node].node_name = "N" + std::to_string(node);
    graph[node].is_violation = false;
    graph[node].has_invariant = false;
    return node;
  };
  const auto add_edge = [&](graphmlt::node_indext from,
                            graphmlt::node_indext to,
                            const graphmlt::key_valuest &values) {
    xmlt edge("edge");
    edge.set_attribute("source", graph[from].node_name);
    edge.set_attribute("target", graph[to].node_name);
    for(const auto &value : values)
    {
      xmlt &data = edge.new_element("data");
      data.set_attribute("key", value.first);
      data.data = value.second;
    }
    graph[from].out[to].xml_node = edge;
    graph[to].in[from].xml_node = edge;
  };
  auto current = add_node();
  const auto append = [&](const graphmlt::key_valuest &values) {
    const auto next = add_node();
    add_edge(current, next, values);
    current = next;
  };
  append({{"originfile", input_path},
          {"threadId", "0"},
          {"createThread", "0"},
          {"enterFunction", "main"}});
  std::set<unsigned> registered{0};
  for(const auto *spawn : required_spawns(targets, equation, solver, true))
  {
    if(!registered.count(spawn->creator_thread))
      throw std::runtime_error("thread creator is not registered");
    const auto location =
      source.expression_location(spawn->call_site.pc->source_location());
    const std::string child = std::to_string(spawn->child_thread);
    append({{"originfile", input_path},
            {"startline", std::to_string(location.line)},
            {"threadId", std::to_string(spawn->creator_thread)},
            {"createThread", child}});
    append({{"originfile", input_path},
            {"threadId", child},
            {"enterFunction",
             thread_entry(spawn->child_thread, equation, solver, ns)}});
    registered.insert(spawn->child_thread);
  }
  // End at the actual conflicting accesses. Race encoding may relax read
  // values, so neither trace assignments nor its synthetic assertion belong
  // here. Unspecified intermediate steps are left to the witness validator.
  const auto access = [&](const waypointt &target) {
    return graphmlt::key_valuest{
      {"originfile", input_path},
      {"startline", std::to_string(target.location.line)},
      {"threadId", std::to_string(target.thread)}};
  };
  // GraphML has no unordered target segment: allow both access orders.
  const auto prefix = current;
  append(access(targets[0]));
  append(access(targets[1]));
  const auto violation = current;
  graph[violation].is_violation = true;
  current = prefix;
  append(access(targets[1]));
  add_edge(current, violation, access(targets[0]));
  std::ostringstream out;
  write_graphml(graph, out, input_path, options);
  return out.str();
}

std::string render(
  const std::string &input_path,
  const source_filet &source,
  const std::vector<waypointt> &waypoints,
  violationt violation)
{
  const bool has_registered_threads =
    std::any_of(waypoints.begin(), waypoints.end(), [](const waypointt &waypoint) {
      return waypoint.type == "function_enter";
    });
  std::ostringstream out;
  out << "- entry_type: violation_sequence\n"
      << "  metadata:\n"
      << "    format_version: "
      << quote(violation == violationt::memtrack ? "2.3" : "2.2") << '\n'
      << "    uuid: " << quote(uuid()) << '\n'
      << "    creation_time: " << quote(creation_time()) << '\n'
      << "    producer:\n"
      << "      name: \"Deagle\"\n"
      << "      version: " << quote(DEAGLE_VERSION) << '\n'
      << "    task:\n"
      << "      input_files:\n"
      << "        - " << quote(input_path) << '\n'
      << "      input_file_hashes:\n"
      << "        " << quote(input_path) << ": " << quote(source.hash) << '\n'
      << "      specification: " << quote(specification(violation)) << '\n'
      << "      data_model: " << data_model() << '\n'
      << "      language: C\n"
      << "  content:\n";
  bool target_segment = false;
  for(const auto &waypoint : waypoints)
  {
    // The two race targets must share a final segment, without ordering them.
    if(!target_segment)
      out << "    - segment:\n";
    target_segment = waypoint.type == "target";
    out << "      - waypoint:\n"
        << "          type: " << waypoint.type << '\n'
        << "          action: follow\n";
    if(has_registered_threads)
      out << "          thread_id: " << waypoint.thread << '\n';
    out << "          location:\n"
        << "            file_name: " << quote(input_path) << '\n'
        << "            line: " << waypoint.location.line << '\n';
    if(waypoint.location.column != 0)
      out << "            column: " << waypoint.location.column << '\n';
  }
  return out.str();
}

void write_atomically(const std::string &path, const std::string &content)
{
  if(path.empty() || path == "-")
    throw std::runtime_error("--yaml-witness requires a file path");
  const std::string temporary = path + ".tmp." + uuid();
  {
    std::ofstream output(temporary, std::ios::binary);
    if(!output)
      throw std::runtime_error("cannot open witness output");
    output << content;
    if(!output)
    {
      std::remove(temporary.c_str());
      throw std::runtime_error("cannot write witness output");
    }
  }
  if(std::rename(temporary.c_str(), path.c_str()) != 0)
  {
    std::remove(temporary.c_str());
    throw std::runtime_error("cannot publish witness output");
  }
}
} // namespace

void output_yaml_violation_witness(
  const goto_tracet &trace,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const namespacet &,
  const optionst &options,
  ui_message_handlert &message_handler)
{
  const std::string output_path = options.get_option("yaml-witness");
  if(output_path.empty())
    return;

  messaget log(message_handler);
  std::remove(output_path.c_str());
  try
  {
    const std::string input_path = options.get_option("filename");
    source_filet source(input_path);
    if(trace.steps.empty() || !trace.get_last_step().is_assert() ||
       trace.get_last_step().cond_value)
      throw std::runtime_error("failed assertion is missing from trace");

    const goto_trace_stept &failure = trace.get_last_step();
    const violationt violation = classify_violation(failure, options);
    const irep_idt property = failure.property_id;
    unsigned target_thread = failure.thread_nr;
    locationt target_location;
    std::vector<waypointt> targets;
    if(violation == violationt::reach)
    {
      const SSA_stept *target_call = nullptr;
      for(const auto &step : equation.SSA_steps)
      {
        if(step.is_assert() && step.get_property_id() == property &&
           solver.get(step.cond_handle).is_false())
          break;
        if(step.is_function_call() && step.source.thread_nr == target_thread &&
           step.called_function == "reach_error" &&
           solver.get(step.guard_handle).is_true())
          target_call = &step;
        if(step.is_function_return() && step.source.thread_nr == target_thread &&
           step.called_function == "reach_error" &&
           solver.get(step.guard_handle).is_true())
          target_call = nullptr;
      }
      if(target_call == nullptr)
        throw std::runtime_error("no active reach_error call for failed property");
      target_location = source.call_location(
        target_call->source.pc->source_location(), "reach_error", false);
    }
    else if(violation == violationt::free)
      target_location =
        source.call_location(failure.pc->source_location(), "free", false);
    else if(violation == violationt::memtrack)
    {
      const goto_trace_stept *loss = memtrack_loss_step(trace, source);
      target_thread = loss->thread_nr;
      target_location = source.expression_location(loss->pc->source_location());
    }
    else if(violation == violationt::datarace)
      targets = datarace_targets(failure, equation, solver, source);
    else
      target_location = source.expression_location(failure.pc->source_location());
    if(violation != violationt::datarace)
      targets.push_back({"target", target_thread, target_location});

    const auto ancestors = required_spawns(
      targets, equation, solver, violation == violationt::datarace);
    std::map<unsigned, unsigned> witness_threads{{0, 0}};
    std::vector<waypointt> waypoints;
    for(const auto *spawn : ancestors)
    {
      const auto creator = witness_threads.find(spawn->creator_thread);
      if(creator == witness_threads.end())
        throw std::runtime_error("thread creator is not registered");
      waypoints.push_back(
        {"function_enter",
         creator->second,
         source.call_location(
           spawn->call_site.pc->source_location(), "pthread_create", true)});
      witness_threads[spawn->child_thread] =
        static_cast<unsigned>(witness_threads.size());
    }
    for(auto target : targets)
    {
      target.thread = witness_threads.at(target.thread);
      waypoints.push_back(target);
    }
    write_atomically(
      output_path, render(input_path, source, waypoints, violation));
    log.status() << "YAML witness written to " << output_path << messaget::eom;
  }
  catch(const std::exception &error)
  {
    log.warning() << "YAML witness missing: " << error.what() << messaget::eom;
  }
}

void output_violation_witness(
  const goto_tracet &trace,
  const symex_target_equationt &equation,
  const decision_proceduret &solver,
  const namespacet &ns,
  const optionst &options,
  ui_message_handlert &message_handler)
{
  const bool datarace =
    !trace.steps.empty() && trace.get_last_step().is_assert() &&
    !trace.get_last_step().cond_value &&
    id2string(trace.get_last_step().property_id).find("nodatarace.assertion.") == 0;
  if(!datarace)
    output_graphml(trace, ns, options);
  else if(!options.get_option("graphml-witness").empty())
  {
    const std::string path = options.get_option("graphml-witness");
    if(path != "-")
      std::remove(path.c_str());
    try
    {
      const auto content = render_datarace_graphml(
        trace.get_last_step(), equation, solver, ns, options);
      if(path == "-")
        std::cout << content;
      else
        write_atomically(path, content);
    }
    catch(const std::exception &error)
    {
      messaget log(message_handler);
      log.warning() << "GraphML witness missing: " << error.what() << messaget::eom;
    }
  }
  output_yaml_violation_witness(
    trace, equation, solver, ns, options, message_handler);
}
