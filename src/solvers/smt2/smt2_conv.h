/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SOLVER_SMT2_CONV_H
#define CPROVER_SOLVER_SMT2_CONV_H

#include <sstream>
#include <set>

#include <util/hash_cont.h>
#include <util/std_expr.h>
#include <util/byte_operators.h>

#include <solvers/prop/prop_conv.h>
#include <solvers/flattening/boolbv_width.h>
#include <solvers/flattening/pointer_logic.h>

// __FHY_ADD_BEGIN__
#define use_node_guard 0
// __FHY_ADD_END__


class typecast_exprt;
class constant_exprt;
class index_exprt;
class member_exprt;

class smt2_convt:public prop_convt
{
public:
	typedef enum { GENERIC, BOOLECTOR, CVC3, CVC4, MATHSAT, OPENSMT, YICES, Z3 } solvert;
	
	smt2_convt(
			const namespacet &_ns,
			const std::string &_benchmark,
			const std::string &_notes,
			const std::string &_logic,
			solvert _solver,
			std::ostream &_out):
			prop_convt(_ns),
			use_FPA_theory(false),
			use_datatypes(false), //todo: switched
			use_array_of_bool(false),
			emit_set_logic(true),
			out(_out),
			benchmark(_benchmark),
			notes(_notes),
			logic(_logic),
			solver(_solver),
			boolbv_width(_ns),
			pointer_logic(_ns),
			no_boolean_variables(0)
	{
		// We set some defaults differently
		// for some solvers.
		
		switch(solver)
		{
			case GENERIC:
				break;
			
			case BOOLECTOR:
				break;
			
			case CVC3:
				break;
			
			case CVC4:
				break;
			
			case MATHSAT:
				break;
			
			case OPENSMT:
				break;
			
			case YICES:
				break;
			
			case Z3:
				use_array_of_bool=true;
				emit_set_logic=false;
				break;
		}
		
		write_header();
	}
	
	//// __FHY_ADD_BEGIN__
	smt2_convt(
			const namespacet &_ns,
			const std::string &_benchmark,
			const std::string &_notes,
			const std::string &_logic,
			solvert _solver,
			std::ostream &_out,
			std::multimap<
					std::pair<std::string,std::string>,
					std::pair<std::string, std::string> > &_oclt_symbols,
			std::map<std::string, exprt> &_oclt_node_guard_map):
			prop_convt(_ns),
			use_FPA_theory(false),
            use_datatypes(false), //todo: switched
			use_array_of_bool(false),
			emit_set_logic(true),
			out(_out),
			benchmark(_benchmark),
			notes(_notes),
			logic(_logic),
			solver(_solver),
			boolbv_width(_ns),
			pointer_logic(_ns),
			no_boolean_variables(0)
	{
		// We set some defaults differently
		// for some solvers.
		
		switch(solver)
		{
			case GENERIC:
				break;
			
			case BOOLECTOR:
				break;
			
			case CVC3:
				break;
			
			case CVC4:
				break;
			
			case MATHSAT:
				break;
			
			case OPENSMT:
				break;
			
			case YICES:
				break;
			
			case Z3:
				use_array_of_bool = true;
				emit_set_logic = false;
				break;
		}
		
		this->oclt_type_table = _oclt_symbols;
		this->oclt_node_guard_map = _oclt_node_guard_map;
		write_header();
	}

	void set_oclt_node_guard();

	//// __FHY_ADD_END__
	
	virtual ~smt2_convt() { }
	virtual resultt dec_solve();
	
	bool use_FPA_theory;
	bool use_datatypes;
	bool use_array_of_bool;
	bool emit_set_logic;
	
	// overloading interfaces
	virtual literalt convert(const exprt &expr);
	virtual void set_to(const exprt &expr, bool value);
	virtual exprt get(const exprt &expr) const;
	virtual std::string decision_procedure_text() const { return "SMT2"; }
	virtual void print_assignment(std::ostream &out) const;
	virtual tvt l_get(literalt l) const;
	virtual void set_assumptions(const bvt &bv) { assumptions=bv; }

protected:
	std::ostream &out;
	std::string benchmark, notes, logic;
	solvert solver;
	
	bvt assumptions;
	boolbv_widtht boolbv_width;
	//// __FHY_ADD_BEGIN__
	/*
	 * oclt_symbolt:
	 * < <event1_identifier, event2_identifier>, <relation type, literal> > , for example:
	 * 		RF: < <wevent_clk, revent_clk>, <"rf1", "rf-order"> >
	 * 		PO: < <event1_clk, event2_clk>, <"", "po"> >
	 */
	typedef symex_target_equationt::SSA_stepst eventst;
	typedef eventst::const_iterator event_it;
	typedef std::multimap<std::pair<std::string, std::string>, std::pair<std::string, std::string> > oclt_symbolt;
	typedef std::multimap<std::pair<std::string, std::string>, std::pair<std::string, std::string> >::iterator oclt_ite;
	oclt_symbolt oclt_type_table;
    typedef std::map<std::string, exprt> node_guard_mapt;
    node_guard_mapt oclt_node_guard_map;
	//// __FHY_ADD_END__
	
	void write_header();
	void write_footer();
	
	// new stuff
	void convert_expr(const exprt &);
	void convert_type(const typet &);
	void convert_literal(const literalt);
	
	// tweaks for arrays
	bool use_array_theory(const exprt &);
	void flatten_array(const exprt &);
	void unflatten_array(const exprt &);
	
	// specific expressions go here
	void convert_byte_update(const byte_update_exprt &expr);
	void convert_byte_extract(const byte_extract_exprt &expr);
	void convert_typecast(const typecast_exprt &expr);
	void convert_struct(const struct_exprt &expr);
	void convert_union(const union_exprt &expr);
	void convert_constant(const constant_exprt &expr);
	void convert_relation(const exprt &expr);
	void convert_is_dynamic_object(const exprt &expr);
	void convert_plus(const plus_exprt &expr);
	void convert_minus(const minus_exprt &expr);
	void convert_div(const div_exprt &expr);
	void convert_mult(const mult_exprt &expr);
	//// __FHY_ADD_BEGIN__
	std::string type2id(const typet &type) const;
	std::string floatbv_suffix(const exprt &expr) const;
	void convert_floatbv(const exprt &expr);
	//// __FHY_ADD_END__
	void convert_floatbv_plus(const exprt &expr);
	void convert_floatbv_minus(const exprt &expr);
	void convert_floatbv_div(const exprt &expr);
	void convert_floatbv_mult(const exprt &expr);
	void convert_mod(const mod_exprt &expr);
	void convert_index(const index_exprt &expr);
	void convert_member(const member_exprt &expr);
	void convert_overflow(const exprt &expr);
	void convert_with(const with_exprt &expr);
	void convert_update(const exprt &expr);
	
	std::string convert_identifier(const irep_idt &identifier);
	
	// helpers for floating-point numbers
	void is_nan(const floatbv_typet &, const char *);
	void is_zero(const floatbv_typet &, const char *);
	void is_equal(const floatbv_typet &, const char *, const char *);
	
	// auxiliary methods
	void find_symbols(const exprt &expr);
	void find_symbols(const typet &type);
	void find_symbols_rec(const typet &type, std::set<irep_idt> &recstack);
	
	constant_exprt parse_literal(const irept &, const typet &type);
	exprt parse_struct(const irept &s, const struct_typet &type);
	exprt parse_union(const irept &s, const union_typet &type);
	exprt parse_array(const irept &s, const array_typet &type);
	exprt parse_rec(const irept &s, const typet &type);
	
	// flattens any non-bitvector type into a bitvector,
	// e.g., booleans, vectors, structs, arrays, ...
	// unflatten() does the opposite.
	typedef enum { BEGIN, END } wheret;
	void flatten2bv(const exprt &);
	void unflatten(wheret, const typet &, unsigned nesting=0);
	
	// pointers
	pointer_logict pointer_logic;
	void convert_address_of_rec(
			const exprt &expr, const pointer_typet &result_type);
	
	void define_object_size(const irep_idt &id, const exprt &expr);
	
	// keeps track of all non-Boolean symbols and their value
	struct identifiert
	{
		typet type;
		exprt value;
		
		identifiert()
		{
			type.make_nil();
			value.make_nil();
		}
	};
	
	typedef hash_map_cont<irep_idt, identifiert, irep_id_hash>
			identifier_mapt;
	
	identifier_mapt identifier_map;
	
	// for modeling structs as Z3 datatype, enabled when
	// use_datatype is set
	typedef std::map<typet, std::string> datatype_mapt;
	datatype_mapt datatype_map;
	
	// for replacing various defined expressions:
	//
	// ID_array_of
	// ID_array
	// ID_string_constant
	
	typedef std::map<exprt, irep_idt> defined_expressionst;
	defined_expressionst defined_expressions;
	
	defined_expressionst object_sizes;
	
	typedef std::set<std::string> smt2_identifierst;
	smt2_identifierst smt2_identifiers;
	
	// Boolean part
	unsigned no_boolean_variables;
	std::vector<bool> boolean_assignment;

public:
    oclt_symbolt& get_oclt_type_table() {return oclt_type_table;}

};

#endif
