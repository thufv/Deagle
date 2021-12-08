/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_PROP_SMT2_DEC_H
#define CPROVER_PROP_SMT2_DEC_H

/*! \defgroup gr_smt2 SMT-LIB 2.x Interface */

#include <fstream>

#include "smt2_conv.h"

class smt2_temp_filet
{
public:
  smt2_temp_filet();
  ~smt2_temp_filet();

protected:  
  std::ofstream temp_out;
  std::string temp_out_filename, temp_result_filename;
};

/*! \brief Decision procedure interface for various SMT 2.x solvers
    \ingroup gr_smt2
*/
class smt2_dect:protected smt2_temp_filet, public smt2_convt
{
public:
  smt2_dect(
    const namespacet &_ns,
    const std::string &_benchmark,
    const std::string &_notes,
    const std::string &_logic,
    solvert _solver):
    smt2_temp_filet(),
    smt2_convt(_ns, _benchmark, _notes, _logic, _solver, temp_out)
  {
  }
  
	//// __FHY_ADD_BEGIN__
	smt2_dect(
			const namespacet &_ns,
			const std::string &_benchmark,
			const std::string &_notes,
			const std::string &_logic,
			solvert _solver,
			std::multimap<
					std::pair<std::string,std::string>,
					std::pair<std::string, std::string> > &_oclt_symbols,
          std::map<std::string, exprt> &_oclt_node_guard_map):
			smt2_temp_filet(),
			smt2_convt(_ns, _benchmark, _notes, _logic, _solver, temp_out, _oclt_symbols, _oclt_node_guard_map)
	{
	}
	//// __FHY_ADD_END__

  virtual resultt dec_solve();
  virtual std::string decision_procedure_text() const;
  
  // yes, we are incremental!
  virtual bool has_set_assumptions() const { return true; }

  std::vector<std::string> smt2_graph_name;
  std::vector<std::set<int> > smt2_graph_ins;
  std::vector<std::set<int> > smt2_graph_outs;
  std::map<std::string, int> name_to_id;

  std::vector<std::string> smt2_graph_name_sorted;
  std::map<std::string, int> name_to_id_sorted;

  std::vector<irep_idt> write_order;

  void create_graph(std::istream& in);

protected:
  resultt read_result(std::istream &in);
};

#endif
