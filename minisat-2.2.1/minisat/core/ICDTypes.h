// __SZH_ADD_BEGIN__
#ifndef ICDTYPES_H
#define ICDTYPES_H

#include <iostream>
#include <vector>
#include <bitset>
#include <set>

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"
#include "../../minisat/core/GraphTypes.h"

namespace Minisat
{

struct ICD_reasont // the reason for a single edge in ICD is one literal or two
{
	int num;
	Minisat::Lit l1;
	Minisat::Lit l2;
	ICD_reasont(): num(0) {}
	ICD_reasont(Minisat::Lit _l1)
	{
		num = 1;
		l1 = _l1;
		// if(_l1 == Minisat::Lit(0))
		// {
		// 	num = 0;
		// }
		// else
		// {
		// 	num = 1;
		// 	l1 = _l1;
		// }
	}
	ICD_reasont(Minisat::Lit _l1, Minisat::Lit _l2): num(2), l1(_l1), l2(_l2) {}
	ICD_reasont(ICD_reasont r1, ICD_reasont r2)
	{
		if(r1.num == 0)
		{
			num = r2.num;
			l1 = r2.l1;
			l2 = r2.l2;
			return;
		}
		if(r2.num == 0)
		{
			num = r1.num;
			l1 = r1.l1;
			l2 = r1.l2;
			return;
		}
		if(r1.num != 1 || r2.num != 1)
			std::cout << "ERROR: two reasons are both > 1\n";
		num = 2;
		l1 = r1.l1;
		l2 = r2.l1;
	}
	bool operator==(const ICD_reasont& right) const
	{
		return num == right.num && (num > 0 ? l1 == right.l1 : true) && (num > 1 ? l2 == right.l2 : true);
	}
	friend literal_vector& operator<<(literal_vector& vec, ICD_reasont& reason)
	{
		if(reason.num > 0)
			vec.push_back(reason.l1);
		if(reason.num > 1)
			vec.push_back(reason.l2);
		return vec;
	}
	ICD_reasont operator~()
	{
		switch(num)
		{
		case 0:
			return ICD_reasont();
		case 1:
			return ICD_reasont(~l1);
		case 2:
		default:
			return ICD_reasont(~l1, ~l2);
		}
	}
	
	bool add(Minisat::Lit l_more)
	{
		switch(num)
		{
		case 0:
			num = 1;
			l1 = l_more;
			return true;
		case 1:
			num = 2;
			l2 = l_more;
			return true;
		default:
			return false;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, ICD_reasont reason)
	{
		switch(reason.num)
		{
		case 0:
			os << "true0";
			break;
		case 1:
			os << reason.l1.x;
			break;
		case 2:
			os << reason.l1.x << " and " << reason.l2.x;
			break;
		default:
			os << "error";
			break;
		}
		return os;
	}

	bool operator<(const ICD_reasont right) const
	{
		if(num != right.num)
			return num < right.num;
		switch(num)
		{
		case 1:
			return l1 < right.l1;
		case 2:
			if(l1 != right.l1)
				return l1 < right.l1;
			return l2 < right.l2;
		case 0:
		default:
			return false;
		}
	}
};

struct ICD_nodet;

struct ICD_edget
{
    int from;
	int to;
	edge_kindt kind;
    ICD_reasont reason;

	ICD_edget(int _from = -1, int _to = -1, edge_kindt _kind = OC_NA) : from(_from), to(_to), kind(_kind), reason(ICD_reasont()) {}
	ICD_edget(int _from, int _to, edge_kindt _kind, ICD_reasont _reason) : from(_from), to(_to), kind(_kind), reason(_reason) {}
	//bool operator<(const ICD_edget& right) const;
	bool operator==(const ICD_edget& right) const;
	//bool operator!=(const ICD_edget& right) const;
};

struct ICD_fathert
{
	ICD_nodet* n;
	ICD_reasont reason;
	edge_kindt kind;
	ICD_fathert(): n(nullptr), reason(ICD_reasont()), kind(OC_PO) {}
	ICD_fathert(ICD_nodet* _n, ICD_reasont _reason, edge_kindt _kind)
		: n(_n), reason(_reason), kind(_kind) {}
};

struct ICD_nodet
{
    std::string name;

	std::vector<ICD_edget> out;
	std::vector<ICD_edget> in;

	//about atomic
	int atomic_parent;
	std::set<int> atomic_in;
	std::set<int> atomic_out;

	ICD_nodet(std::string _name, int id);

	//about all cycles
	int cost;
	std::vector<ICD_fathert> valid_out;
	std::vector<ICD_fathert> valid_in;
	int valid_in_num;
	std::vector<literal_vector> all_reasons;

	ICD_fathert backward_father;
	ICD_fathert forward_father;
};

}
#endif // ICDTYPES_H
// __SZH_ADD_END__