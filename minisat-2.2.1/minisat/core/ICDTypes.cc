// __SZH_ADD_BEGIN__

#include <iostream>
#include <string>

#include "ICDTypes.h"
#include "ICD.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

using namespace Minisat;

//bool ICD_edget::operator<(const ICD_edget& right) const
//{
//    if(kind != right.kind)
//        return kind < right.kind;
//    if(n != right.n)
//        return n < right.n;
//    return reason < right.reason;
//}
bool ICD_edget::operator==(const ICD_edget& right) const { return from == right.from && to == right.to && kind == right.kind && reason == right.reason; }
//bool ICD_edget::operator!=(const ICD_edget& right) const { return !(*this == right);}

Minisat::ICD_nodet::ICD_nodet(std::string _name, int id) : name(_name), atomic_parent(id)
{
    atomic_in = std::set<int>{id};
    atomic_out = std::set<int>{id};
}

// __SZH_ADD_END__