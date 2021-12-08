/*******************************************************************\

Module: Add constraints to equation encoding partial orders on events

Author: Michael Tautschnig, michael.tautschnig@cs.ox.ac.uk

\*******************************************************************/

#include <limits>

#include <util/i2string.h>
#include <util/arith_tools.h>
#include <util/simplify_expr.h>

#include "partial_order_concurrency.h"
#include <iostream>

/*******************************************************************\

Function: partial_order_concurrencyt::~partial_order_concurrencyt

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

partial_order_concurrencyt::partial_order_concurrencyt(
        const namespacet &_ns):ns(_ns)
{
}

/*******************************************************************\

Function: partial_order_concurrencyt::~partial_order_concurrencyt

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

partial_order_concurrencyt::~partial_order_concurrencyt()
{
}

/*******************************************************************\

Function: partial_order_concurrencyt::add_init_writes

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void partial_order_concurrencyt::add_init_writes(
        symex_target_equationt &equation)
{
    hash_set_cont<irep_idt, irep_id_hash> init_done;
    bool spawn_seen=false;

    symex_target_equationt::SSA_stepst init_steps;

    for(eventst::const_iterator
                e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end();
        e_it++)
    {
        if(is_spawn(e_it))
        {
            spawn_seen=true;
            continue;
        }
        else if(!is_shared_read(e_it) &&
                !is_shared_write(e_it))
            continue;

        const irep_idt &a=address(e_it);

        if(init_done.find(a)!=init_done.end()) continue;

        if(spawn_seen ||
           is_shared_read(e_it) ||
           !e_it->guard.is_true())
        {
            init_steps.push_back(symex_target_equationt::SSA_stept());
            symex_target_equationt::SSA_stept &SSA_step=init_steps.back();

            SSA_step.guard=true_exprt();
            // no SSA index, thus nondet value
            SSA_step.ssa_lhs=e_it->original_lhs_object;
            SSA_step.original_lhs_object=e_it->original_lhs_object;
            SSA_step.type=goto_trace_stept::SHARED_WRITE;
            SSA_step.atomic_section_id=0;
            SSA_step.source=e_it->source;
        }

        init_done.insert(a);
    }

    equation.SSA_steps.splice(equation.SSA_steps.begin(), init_steps);
}

/*******************************************************************\

Function: partial_order_concurrencyt::build_event_lists

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void partial_order_concurrencyt::build_event_lists(
        symex_target_equationt &equation)
{
    address_map.clear();

    add_init_writes(equation);

    // a per-thread counter
    std::map<unsigned, unsigned> counter;
    int nreads = 0, nwrites = 0;
    for(eventst::const_iterator
                e_it=equation.SSA_steps.begin();
        e_it!=equation.SSA_steps.end();
        e_it++)
    {
        if(is_shared_read(e_it) ||
           is_shared_write(e_it) ||
           is_spawn(e_it))
        {
            unsigned thread_nr=e_it->source.thread_nr;

            if(!is_spawn(e_it))
            {
                a_rect &a_rec=address_map[address(e_it)];

                if(is_shared_read(e_it)) {
                    a_rec.reads.push_back(e_it); nreads++; }
                else {// must be write
                    a_rec.writes.push_back(e_it); nwrites++; }
            }

            // maps an event id to a per-thread counter
            unsigned cnt=counter[thread_nr]++;
            numbering[e_it]=cnt;
        }
    }
    equation.events_num = nreads + nwrites;
//  std::cout << "reads num: " << nreads << "\n";
//  std::cout << "writes num: " << nwrites << "\n";
//
//  for (address_mapt::iterator it = address_map.begin(); it != address_map.end(); it++)
//  {
//	  std::cout << "Shared " << it->first << ": " << it->second.reads.size() << "R/" << it->second.writes.size() << "W\n";
//  }
}

/*******************************************************************\

Function: partial_order_concurrencyt::rw_clock_id

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

irep_idt partial_order_concurrencyt::rw_clock_id(
        event_it event,
        axiomt axiom)
{
//  if(event->is_shared_write())
//    return id2string(id(event))+"$wclk$"+i2string(axiom);
//  else if(event->is_shared_read())
//    return id2string(id(event))+"$rclk$"+i2string(axiom);
//  else
//    assert(false);
//// __FHY_ADD_BEGIN_210412__
    if(event->is_shared_write())
        return id2string(id(event))+"$wclk" + "_"+ i2string(event->source.thread_nr);
    else if(event->is_shared_read())
        return id2string(id(event))+"$rclk" + "_"+ i2string(event->source.thread_nr);
    else
        assert(false);
    //// __FHY_ADD_END_210412__

    return "";
}

/*******************************************************************\

Function: partial_order_concurrencyt::clock

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

symbol_exprt partial_order_concurrencyt::clock(
        event_it event,
        axiomt axiom)
{
    irep_idt identifier;
    assert(!numbering.empty());

    if(event->is_shared_write())
    {
        assert(is_shared_write(event));
        //// __FHY_ADD_BEGIN_210412__
        identifier=rw_clock_id(event, axiom) + "_" + i2string(numbering[event]);
        //// __FHY_ADD_end_210412__

    }
    else if(event->is_shared_read())
    {
        assert(is_shared_read(event));
        //// __FHY_ADD_BEGIN_210412__
        identifier=rw_clock_id(event, axiom) + "_" + i2string(numbering[event]);
        //// __FHY_ADD_end_210412__
    }
    else if(event->is_spawn())
    {
        assert(is_spawn(event));
        //// __FHY_ADD_BEGIN__
		//identifier=
		//		"t"+i2string(event->source.thread_nr+1)+"$"+
		//		i2string(numbering[event])+"$spwnclk$"+i2string(axiom);
        identifier = "t"+i2string(event->source.thread_nr+1)+"$"+
                     i2string(numbering[event])+"$spwnclk";
        //// __FHY_ADD_END__
    }
    else
    {
        assert(false);
    }

    return symbol_exprt(identifier, clock_type);
}

/*******************************************************************\

Function: partial_order_concurrencyt::is_shared_write

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

bool partial_order_concurrencyt::is_shared_write(event_it event) const
{
    if(!event->is_shared_write()) return false;
    const irep_idt identifier=event->original_lhs_object.get_identifier();
    if(identifier=="goto_symex::\\guard") return false;
    const symbolt &symbol=ns.lookup(identifier);
    return !symbol.is_thread_local;
}

/*******************************************************************\

Function: partial_order_concurrencyt::is_shared_read

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

bool partial_order_concurrencyt::is_shared_read(event_it event) const
{
    if(!event->is_shared_read())
        return false;
    const irep_idt identifier=event->original_lhs_object.get_identifier();
    if(identifier=="goto_symex::\\guard")
        return false;
    const symbolt &symbol=ns.lookup(identifier);
    return !symbol.is_thread_local;
}

/*******************************************************************\

Function: partial_order_concurrencyt::build_clock_type

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void partial_order_concurrencyt::build_clock_type(
        const symex_target_equationt &equation)
{
    assert(!numbering.empty());

//  mp_integer width=address_bits(numbering.size());
//  assert(width<std::numeric_limits<unsigned>::max());
//  clock_type=unsignedbv_typet(integer2unsigned(width));
//	clock_type = integer_typet();
    //// __FHY_ADD_BEGIN__
    clock_type = oc_typet();
    //// __FHY_ADD_END__
}

/*******************************************************************\

Function: partial_order_concurrencyt::before

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

exprt partial_order_concurrencyt::before(
        event_it e1, event_it e2, unsigned axioms)
{
    const axiomt axiom_bits[]={
            AX_SC_PER_LOCATION,
            AX_NO_THINAIR,
            AX_OBSERVATION,
            AX_PROPAGATION };

    exprt::operandst ops;
    ops.reserve(sizeof(axiom_bits)/sizeof(axiomt));

    for(int i=0; i<int(sizeof(axiom_bits)/sizeof(axiomt)); ++i)
    {
        const axiomt ax=axiom_bits[i];

        if((axioms & ax)==0) continue;

        if(e1->atomic_section_id!=0 &&
           e1->atomic_section_id==e2->atomic_section_id)
            ops.push_back(equal_exprt(clock(e1, ax), clock(e2, ax)));
        else
            ops.push_back(
                    binary_relation_exprt(clock(e1, ax), ID_lt, clock(e2, ax)));
    }

    assert(!ops.empty());

    return conjunction(ops);
}
/*******************************************************************\

Function: partial_order_concurrencyt::before_ssa

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

exprt partial_order_concurrencyt::before_ssa(unsigned ssa_id1, unsigned ssa_id2)
{
    irep_idt identifier_e1 = "clk" + i2string(ssa_id1);
    irep_idt identifier_e2 = "clk" + i2string(ssa_id2);
    symbol_exprt symbol_e1 = symbol_exprt(identifier_e1, clock_type);
    symbol_exprt symbol_e2 = symbol_exprt(identifier_e2, clock_type);
    return binary_relation_exprt(symbol_e1, ID_lt, symbol_e2);
}

/*******************************************************************\

Function: partial_order_concurrencyt::add_constraint

  Inputs: 

 Outputs:

 Purpose:

\*******************************************************************/

void partial_order_concurrencyt::add_constraint(
        symex_target_equationt &equation,
        const exprt &cond,
        const std::string &msg,
        const symex_targett::sourcet &source) const
{
    exprt tmp=cond;
    simplify(tmp, ns);
    equation.constraint(tmp, msg, source);
}

