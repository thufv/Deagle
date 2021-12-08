/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include "boolbv.h"

/*******************************************************************\

Function: boolbvt::convert_array

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_array(const exprt &expr, bvt &bv)
{
  unsigned width=boolbv_width(expr.type());

  if(width==0)
    return conversion_failed(expr, bv);
    
  bv.reserve(width);

  if(expr.type().id()==ID_array)
  {
    assert(expr.has_operands());
    const exprt::operandst &operands=expr.operands();
    assert(!operands.empty());
    std::size_t op_width=width/operands.size();
    
    bool pthread_mutex = false;
    //__SZH_REMOVE_BEGIN__
    //if (width == 320)
    //	pthread_mutex = true;
    //__SZH_REMOVE_END__
    int m = 0;
    forall_expr(it, operands)
    {
      const bvt &tmp=convert_bv(*it);

      if(tmp.size()!=op_width)
        throw "convert_array: unexpected operand width";

      forall_literals(it2, tmp)
        bv.push_back(*it2);
      if (pthread_mutex && ++m == 4)
    	  break;
    }   

    return;
  }
  
  conversion_failed(expr, bv);
}

