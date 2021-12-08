// __SZH_ADD_BEGIN__

#include <iostream>
#include <string>

#include "ClosureTypes.h"
#include "Closure.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

using namespace Minisat;

//bool closure_edget::operator<(const closure_edget& right) const
//{
//    if(kind != right.kind)
//        return kind < right.kind;
//    if(n != right.n)
//        return n < right.n;
//    return reason < right.reason;
//}
bool closure_edget::operator==(const closure_edget& right) const { return from == right.from && to == right.to && kind == right.kind && reason == right.reason; }
//bool closure_edget::operator!=(const closure_edget& right) const { return !(*this == right);}

Minisat::closure_nodet::closure_nodet(std::string _name, int _address, int id) : name(_name), address(_address), atomic_parent(id), guard_lighted(false), is_write(false), is_read(false)
{
    atomic_items = std::set<int>{id};
    atomic_in = std::set<int>{id};
    atomic_out = std::set<int>{id};
}

// __SZH_ADD_END__