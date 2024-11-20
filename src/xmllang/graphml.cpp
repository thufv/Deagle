/*******************************************************************\

Module: Read/write graphs as GraphML

Author: Michael Tautschnig, mt@eecs.qmul.ac.uk

\*******************************************************************/

/// \file
/// Read/write graphs as GraphML

#include "graphml.h"

#include <util/message.h>

// include last to make sure #define stack(x) of parser.h does not
// collide with std::stack included by graph.h
#include "xml_parser.h"

#include <fstream>
#include "sha256.h"

typedef std::map<std::string, graphmlt::node_indext> name_mapt;

static graphmlt::node_indext add_node(
  const std::string &name,
  name_mapt &name_to_node,
  graphmlt &graph)
{
  std::pair<name_mapt::iterator, bool> entry=
    name_to_node.insert(std::make_pair(name, 0));
  if(entry.second)
    entry.first->second=graph.add_node();

  return entry.first->second;
}

static bool build_graph_rec(
  const xmlt &xml,
  name_mapt &name_to_node,
  std::map<std::string, std::map<std::string, std::string> > &defaults,
  graphmlt &dest,
  std::string &entrynode)
{
  if(xml.name=="node")
  {
    const std::string node_name=xml.get_attribute("id");

    const graphmlt::node_indext n=
      add_node(node_name, name_to_node, dest);

    graphmlt::nodet &node=dest[n];
    node.node_name=node_name;
    node.is_violation=false;
    node.has_invariant=false;

    for(xmlt::elementst::const_iterator
        e_it=xml.elements.begin();
        e_it!=xml.elements.end();
        e_it++)
    {
      assert(e_it->name=="data");

      if(e_it->get_attribute("key")=="violation" &&
         e_it->data=="true")
        node.is_violation=e_it->data=="true";
      else if(e_it->get_attribute("key")=="entry" &&
              e_it->data=="true")
        entrynode=node_name;
    }
  }
  else if(xml.name=="edge")
  {
    const std::string source=xml.get_attribute("source");
    const std::string target=xml.get_attribute("target");

    const graphmlt::node_indext s=add_node(source, name_to_node, dest);
    const graphmlt::node_indext t=add_node(target, name_to_node, dest);

    // add edge and annotate
    xmlt xml_w_defaults=xml;

    std::map<std::string, std::string> &edge_defaults=defaults["edge"];
    for(std::map<std::string, std::string>::const_iterator
        it=edge_defaults.begin();
        it!=edge_defaults.end();
        ++it)
    {
      bool found=false;
      for(xmlt::elementst::const_iterator
          e_it=xml.elements.begin();
          e_it!=xml.elements.end() && !found;
          ++e_it)
        found=e_it->get_attribute("key")==it->first;

      if(!found)
      {
        xmlt &d=xml_w_defaults.new_element("data");
        d.set_attribute("key", it->first);
        d.data=it->second;
      }
    }

    dest[s].out[t].xml_node=xml_w_defaults;
    dest[t].in[s].xml_node=xml_w_defaults;
  }
  else if(xml.name=="graphml" ||
          xml.name=="graph")
  {
    for(xmlt::elementst::const_iterator
        e_it=xml.elements.begin();
        e_it!=xml.elements.end();
        e_it++)
      // recursive call
      if(build_graph_rec(
          *e_it,
          name_to_node,
          defaults,
          dest,
          entrynode))
        return true;
  }
  else if(xml.name=="key")
  {
    for(xmlt::elementst::const_iterator
        e_it=xml.elements.begin();
        e_it!=xml.elements.end();
        ++e_it)
      if(e_it->name=="default")
        defaults[xml.get_attribute("for")][xml.get_attribute("id")]=
          e_it->data;
  }
  else if(xml.name=="data")
  {
    // ignored
  }
  else
  {
    UNREACHABLE;
    return true;
  }

  return false;
}

static bool build_graph(
  const xmlt &xml,
  graphmlt &dest,
  graphmlt::node_indext &entry)
{
  assert(dest.empty());

  name_mapt name_to_node;
  std::map<std::string, std::map<std::string, std::string> > defaults;
  std::string entrynode;

  const bool err=
    build_graph_rec(
      xml,
      name_to_node,
      defaults,
      dest,
      entrynode);

  for(std::size_t i=0; !err && i<dest.size(); ++i)
  {
    const graphmlt::nodet &n=dest[i];

    INVARIANT(!n.node_name.empty(), "node should be named");
  }

  assert(!entrynode.empty());
  name_mapt::const_iterator it=name_to_node.find(entrynode);
  assert(it!=name_to_node.end());
  entry=it->second;

  return err;
}

bool read_graphml(
  std::istream &is,
  graphmlt &dest,
  graphmlt::node_indext &entry,
  message_handlert &message_handler)
{
  xmlt xml;

  if(parse_xml(is, "", message_handler, xml))
    return true;

  return build_graph(xml, dest, entry);
}

bool read_graphml(
  const std::string &filename,
  graphmlt &dest,
  graphmlt::node_indext &entry,
  message_handlert &message_handler)
{
  xmlt xml;

  if(parse_xml(filename, message_handler, xml))
    return true;

  return build_graph(xml, dest, entry);
}

bool write_graphml(const graphmlt &src, std::ostream &os, std::string filename, const optionst& options)
{
  xmlt graphml("graphml");
  graphml.set_attribute(
    "xmlns:xsi",
    "http://www.w3.org/2001/XMLSchema-instance");
  graphml.set_attribute(
    "xmlns",
    "http://graphml.graphdrawing.org/xmlns");

  // <key attr.name="originFileName" attr.type="string" for="edge"
  //      id="originfile">
  //   <default>"&lt;command-line&gt;"</default>
  // </key>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "originFileName");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "originfile");

    if(src.key_values.find("programfile")!=src.key_values.end())
      key.new_element("default").data=src.key_values.at("programfile");
    else
      key.new_element("default").data="<command-line>";
  }

  // // <key attr.name="invariant" attr.type="string" for="node" id="invariant"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "invariant");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "node");
  //   key.set_attribute("id", "invariant");
  // }

  // // <key attr.name="invariant.scope" attr.type="string" for="node"
  // //     id="invariant.scope"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "invariant.scope");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "node");
  //   key.set_attribute("id", "invariant.scope");
  // }

  // <key attr.name="isViolationNode" attr.type="boolean" for="node"
  //      id="violation">
  //     <default>false</default>
  // </key>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "isViolationNode");
    key.set_attribute("attr.type", "boolean");
    key.set_attribute("for", "node");
    key.set_attribute("id", "violation");

    key.new_element("default").data="false";
  }

  // <key attr.name="isEntryNode" attr.type="boolean" for="node" id="entry">
  //     <default>false</default>
  // </key>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "isEntryNode");
    key.set_attribute("attr.type", "boolean");
    key.set_attribute("for", "node");
    key.set_attribute("id", "entry");

    key.new_element("default").data="false";
  }

  // <key attr.name="isSinkNode" attr.type="boolean" for="node" id="sink">
  //     <default>false</default>
  // </key>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "isSinkNode");
    key.set_attribute("attr.type", "boolean");
    key.set_attribute("for", "node");
    key.set_attribute("id", "sink");

    key.new_element("default").data="false";
  }

  // <key attr.name="enterLoopHead" attr.type="boolean" for="edge"
  //      id="enterLoopHead">
  //   <default>false</default>
  // </key>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "enterLoopHead");
    key.set_attribute("attr.type", "boolean");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "enterLoopHead");

    key.new_element("default").data="false";
  }

  // <key attr.name="cyclehead" attr.type="boolean" for="node"
  //      id="cyclehead">
  //   <default>false</default>
  // </key>
  {
    xmlt &key = graphml.new_element("key");
    key.set_attribute("attr.name", "cyclehead");
    key.set_attribute("attr.type", "boolean");
    key.set_attribute("for", "node");
    key.set_attribute("id", "cyclehead");

    key.new_element("default").data = "false";
  }

  // <key attr.name="threadId" attr.type="string" for="edge" id="threadId"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "threadId");
    key.set_attribute("attr.type", "int");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "threadId");

    key.new_element("default").data = "0";
  }

  // <key attr.name="createThread" attr.type="string"
  //      for="edge" id="createThread"/>
  {
    xmlt &key = graphml.new_element("key");
    key.set_attribute("attr.name", "createThread");
    key.set_attribute("attr.type", "int");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "createThread");

    key.new_element("default").data="0";
  }

  // <key attr.name="sourcecodeLanguage" attr.type="string" for="graph"
  //      id="sourcecodelang"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "sourcecodeLanguage");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "sourcecodelang");
  }

  // <key attr.name="programFile" attr.type="string" for="graph"
  //      id="programfile"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "programFile");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "programfile");
  }

  // <key attr.name="programHash" attr.type="string" for="graph"
  //      id="programhash"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "programHash");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "programhash");
  }

  // <key attr.name="specification" attr.type="string" for="graph"
  //      id="specification"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "specification");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "specification");
  }

  // <key attr.name="architecture" attr.type="string" for="graph"
  //      id="architecture"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "architecture");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "architecture");
  }

  // <key attr.name="producer" attr.type="string" for="graph"
  //      id="producer"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "producer");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "producer");
  }

  // <key attr.name="creationtime" attr.type="string" for="graph"
  //      id="creationtime"/>
  {
    xmlt &key = graphml.new_element("key");
    key.set_attribute("attr.name", "creationtime");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "creationtime");
  }

  // <key attr.name="startline" attr.type="int" for="edge" id="startline"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "startline");
    key.set_attribute("attr.type", "int");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "startline");
  }

  // <key attr.name="control" attr.type="string" for="edge" id="control"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "control");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "control");
  }

  // <key attr.name="assumption" attr.type="string" for="edge" id="assumption"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "assumption");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "edge");
    key.set_attribute("id", "assumption");
  }

  // <key attr.name="assumption.resultfunction" attr.type="string" for="edge"
  //      id="assumption.resultfunction"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "assumption.resultfunction");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "edge");
  //   key.set_attribute("id", "assumption.resultfunction");
  // }

  // <key attr.name="assumption.scope" attr.type="string" for="edge"
  //      id="assumption.scope"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "assumption.scope");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "edge");
  //   key.set_attribute("id", "assumption.scope");
  // }

  // <key attr.name="enterFunction" attr.type="string" for="edge"
  //      id="enterFunction"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "enterFunction");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "edge");
  //   key.set_attribute("id", "enterFunction");
  // }

  // <key attr.name="returnFromFunction" attr.type="string" for="edge"
  //      id="returnFrom"/>
  // {
  //   xmlt &key=graphml.new_element("key");
  //   key.set_attribute("attr.name", "returnFromFunction");
  //   key.set_attribute("attr.type", "string");
  //   key.set_attribute("for", "edge");
  //   key.set_attribute("id", "returnFrom");
  // }

  // <key attr.name="witness-type" attr.type="string" for="graph"
  //      id="witness-type"/>
  {
    xmlt &key=graphml.new_element("key");
    key.set_attribute("attr.name", "witness-type");
    key.set_attribute("attr.type", "string");
    key.set_attribute("for", "graph");
    key.set_attribute("id", "witness-type");
  }

  xmlt &graph=graphml.new_element("graph");
  graph.set_attribute("edgedefault", "directed");

  // __SZH_ADD_BEGIN__

  // <data key="witness-type">violation_witness</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "witness-type");
    data.data="violation_witness";
  }

  // <data key="sourcecodelang">C</data>
  // {
  //   xmlt &data=graph.new_element("data");
  //   data.set_attribute("key", "sourcecodelang");
  //   data.data="C";
  // }

  // <data key="producer">Deagle 4.1.0</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "producer");
    data.data="Deagle 4.1.0";
  }

  // <data key="specification">CHECK( init(main()), LTL(G ! call(__VERIFIER_error())) )</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "specification");

    bool enable_unreach_call = !options.get_bool_option("no-assertions");
    bool enable_datarace = options.get_bool_option("datarace");
    bool enable_overflow = options.get_bool_option("signed-overflow-check") || options.get_bool_option("signed-overflow-check");
    bool enable_memsafety = options.get_bool_option("pointer-check") || options.get_bool_option("alloc-check") || options.get_bool_option("memory-leak-check");

    if(enable_unreach_call)
      data.data="CHECK( init(main()), LTL(G ! call(reach_error())) )";
    else if(enable_datarace)
      data.data="CHECK( init(main()), LTL(G ! data-race) )";
    else if(enable_overflow)
      data.data="CHECK( init(main()), LTL(G ! overflow) )";
    else if(enable_memsafety)
    {
      data.data="CHECK( init(main()), LTL(G valid-free) )\n";
      data.data+="CHECK( init(main()), LTL(G valid-deref) )\n";
      data.data+="CHECK( init(main()), LTL(G valid-memtrack) )\n";
    }
  }

  // <data key="programfile">../../sv-benchmarks/c/bitvector/s3_clnt_3_true-unreach-call.BV.c.cil.c</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "programfile");
    data.data=filename;
  }

  // <data key="programhash">c53759a150934ef20fe7ff264c4326cfaaab3702</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "programhash");

    std::ifstream file(filename);
    std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    ly::Sha256 sha256;

    data.data=sha256.getHexMessageDigest(file_content);
  }

  // <data key="architecture">32bit</data>
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", "architecture");
    data.data="32bit";
  }

  // <data key="creationtime">2016-12-24T13:15:32+02:00</data>
  {
      time_t now = time(0) + 8 * 3600;
      tm* gm = gmtime(&now);
      char time_data[100];
      strftime(time_data, 100, "%Y-%m-%dT%H:%M:%S+08:00", gm);
      std::string data_str = time_data;

      xmlt &data=graph.new_element("data");
      data.set_attribute("key", "creationtime");
      data.data=data_str;
  }
  // __SZH_ADD_END__

  for(const auto &kv : src.key_values)
  {
    xmlt &data=graph.new_element("data");
    data.set_attribute("key", kv.first);
    data.data=kv.second;
  }

  bool entry_done=false;
  for(graphmlt::node_indext i=0; i<src.size(); ++i)
  {
    const graphmlt::nodet &n=src[i];

    // <node id="A12"/>
    xmlt &node=graph.new_element("node");
    node.set_attribute("id", n.node_name);

    // <node id="A1">
    //     <data key="entry">true</data>
    // </node>
    if(!entry_done && n.node_name!="sink")
    {
      xmlt &entry=node.new_element("data");
      entry.set_attribute("key", "entry");
      entry.data="true";

      entry_done=true;
    }

    // <node id="A14">
    //     <data key="violation">true</data>
    // </node>
    if(n.is_violation)
    {
      xmlt &entry=node.new_element("data");
      entry.set_attribute("key", "violation");
      entry.data="true";
    }

    // if(n.has_invariant)
    // {
    //   xmlt &val=node.new_element("data");
    //   val.set_attribute("key", "invariant");
    //   val.data=n.invariant;

    //   xmlt &val_s=node.new_element("data");
    //   val_s.set_attribute("key", "invariant.scope");
    //   val_s.data=n.invariant_scope;
    // }

    for(graphmlt::edgest::const_iterator
        e_it=n.out.begin();
        e_it!=n.out.end();
        ++e_it)
      graph.new_element(e_it->second.xml_node);
  }

  os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
  os << graphml;

  return !os.good();
}
