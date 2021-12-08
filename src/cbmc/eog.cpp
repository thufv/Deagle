/*
 * eog.cpp
 *
 *  Created on: Jan 4, 2016
 *      Author: ylz86
 */

#include "eog.h"
#include <util/namespace.h>
#include <iostream>
#include <algorithm>
#include <string.h>



DEOG::DEOG(symex_target_equationt* equation) {
	// TODO Auto-generated constructor stub
	totaltime = current_time() - current_time();
	nodes_num = 0;
	optimize = false;
	m_equation = equation;
}

DEOG::~DEOG() {
	// TODO Auto-generated destructor stub
	clear();

	for (nodevt::iterator it = m_all_nodes.begin(); it != m_all_nodes.end(); it++) {
		delete (*it);
	}
}

// judge if there exists some circle among the graph, return true if there is no circle
bool DEOG::no_circle_judge() {
	nodest enabled_nodes;
	for (nodest::iterator it = m_nodes.begin(); it != m_nodes.end(); ) {
		if ((*it)->no_input_edge()) {
			enabled_nodes.push_back(*it);
			it = m_nodes.erase(it);
		}
		else {
			it++;
		}
	}

	//// __FHY_ADD_BEGIN__
#if output_reason
	std::cout << "************************No Circle Judge: ***********************************" << "\n";
#endif
	while(!enabled_nodes.empty()) {
		node* selected = enabled_nodes.front();

		// delete all the po edges of selected
		for (edgest::iterator it = selected->m_output_pos.begin();
				it != selected->m_output_pos.end();
				++it)
		{
		    //// __FHY_ADD_BEGIN__
#if output_reason
		    std::cout << "po: ";
            (*it)->outputx();
            std::cout << "\n";
#endif
            //// __FHY_ADD_END__

			node* son = (*it)->m_dst;
			m_pos.remove(*it);
			son->m_input_pos.remove(*it);

			if (son->no_input_edge()) {
				enabled_nodes.push_back(son);
				m_nodes.remove(son);
			}
		}

		// delete all the epo edges of selected
		for (edgest::iterator it = selected->m_output_epos.begin();
				it != selected->m_output_epos.end();
				++it)
		{
		    //// __FHY_ADD_BEGIN__
#if output_reason
            std::cout << "epo: ";
            (*it)->outputx();
            std::cout << "\n";
#endif
            //// __FHY_ADD_END__

            node* son = (*it)->m_dst;
			m_epos.remove(*it);
			son->m_input_epos.remove(*it);

			if (son->no_input_edge()) {
				enabled_nodes.push_back(son);
				m_nodes.remove(son);
			}
		}

		// delete all the rf edges of selected
		for (edgest::iterator it = selected->m_output_rfs.begin();
				it != selected->m_output_rfs.end();
				++it)
		{
		    //// __FHY_ADD_BEGIN__
#if output_reason
            if((*it)->m_cycle_valid)
                std::cout << "rf: ";
            else
                std::cout << "unvalid rf: ";
            (*it)->outputx();
            std::cout << "\n";
#endif
            //// __FHY_ADD_END__

			node* son = (*it)->m_dst;
			m_rfs.remove(*it);
			son->m_input_rfs.remove(*it);

			if (son->no_input_edge()) {
				enabled_nodes.push_back(son);
				m_nodes.remove(son);
			}
		}

		// delete all the na edges of selected
		for (edgest::iterator it = selected->m_output_nas.begin();
				it != selected->m_output_nas.end();
				++it)
		{
		    //// __FHY_ADD_BEGIN__
#if output_reason
            std::cout << "na: ";
            (*it)->outputx();
            std::cout << "\n";
#endif
            //// ___FHY_ADD_END__

			node* son = (*it)->m_dst;
			m_nas.remove(*it);
			son->m_input_nas.remove(*it);

			if (son->no_input_edge()) {
				enabled_nodes.push_back(son);
				m_nodes.remove(son);
			}
		}

		enabled_nodes.pop_front();
		selected->clear(m_all_nodes.size());
	}
#if output_reason
	std::cout << "**********************No Circle Judge End***********************************\n\n";
#endif
	return m_nodes.empty();
	//// __FHY_ADD_END__
}

// find all circles starting from a node
void DEOG::find_cycle_from_node(node* v)
{
	if (m_reasons.size() > 400)
		return;

	// find if v exists in trace
	int j = -1;
	for (unsigned i = 0; i < m_trace.size(); i++) {
		if (m_trace[i] == v) {
			j = i;
			break;
		}
	}

	// if j != -1, there exists a cycle
	if (j != -1) {
		v->m_search_flag = true;
		exprtvt reason;

		for (unsigned i = j; i < m_trace_edge.size(); i++) {
			if (m_trace_edge[i]->m_type == edge::RF) {
				if (m_trace_edge[i]->m_cycle_valid)
					reason.push_back(to_rf_sel_symbol(m_trace_edge[i]));
			}
			else if (m_trace_edge[i]->m_type == edge::NA) {
				for (exprtvt::iterator it = m_trace_edge[i]->m_reasons.begin(); it != m_trace_edge[i]->m_reasons.end(); it++) {
					bool exist_flag = false;
					for (exprtvt::iterator jt = reason.begin(); jt != reason.end(); jt++) {
						if (*it == *jt) {
							exist_flag = true;
							break;
						}
					}
					if (!exist_flag) {
						reason.push_back(*it);
					}
				}
			}
		}

        //// __FHY_ADD_BEGIN__
        // ylz output trace
#if output_reason
        // output traces and kernel reasons which resulting in the circle
        std::cout << ++reason_num << " trace: ";
        for (unsigned i = j; i < m_trace.size(); i++) {
            std::cout << m_trace[i]->m_event->ssa_lhs.get_identifier() << "->";
        }
        std::cout << "\nreason: ";

        for (unsigned i = 0; i < reason.size(); i++) {
            std::cout << from_expr(reason[i]);
            std::cout << ", ";
        }
        std::cout << "\n";
#endif
        //// __FHY_ADD_END__

		m_reasons.push_back(reason);
		return;
	}

	if (v->m_search_flag)
		return;

	// else, there is no cycle, add v to trace
	m_trace.push_back(v);

	// search all the output edges of v
	for (edgest::iterator it = v->m_output_pos.begin(); it != v->m_output_pos.end(); it++) {
		m_trace_edge.push_back(*it);
		find_cycle_from_node((*it)->m_dst);
		m_trace_edge.pop_back();
	}
	for (edgest::iterator it = v->m_output_epos.begin(); it != v->m_output_epos.end(); it++) {
		m_trace_edge.push_back(*it);
		find_cycle_from_node((*it)->m_dst);
		m_trace_edge.pop_back();
	}
	for (edgest::iterator it = v->m_output_rfs.begin(); it != v->m_output_rfs.end(); it++) {
		if ((*it)->m_cycle_valid)
		{
			m_trace_edge.push_back(*it);
			find_cycle_from_node((*it)->m_dst);
			m_trace_edge.pop_back();
		}
	}
	for (edgest::iterator it = v->m_output_nas.begin(); it != v->m_output_nas.end(); it++) {
		m_trace_edge.push_back(*it);
		find_cycle_from_node((*it)->m_dst);
		m_trace_edge.pop_back();
	}
	m_trace.pop_back();
}

// compute all cycles of the graph
void DEOG::compute_all_cycles()
{
	// reset the search flag of all nodes
	for (nodest::iterator it = m_nodes.begin(); it != m_nodes.end(); it++) {
		(*it)->m_search_flag = false;
	}

	reason_num = 0;
	m_reasons.clear();
	for (nodest::iterator it = m_nodes.begin(); it != m_nodes.end(); it++) {
		if ((*it)->m_input_pos.size() == 0) {
			m_trace.clear();
			m_trace_edge.clear();
			find_cycle_from_node(*it);
		}
	}

	process_reasons();
}

bool reason_vec_compare(const exprtvt& r1, const exprtvt& r2) {
	return r1.size() < r2.size();
}

bool reason_compare(const exprt& r1, const exprt& r2) {
	return r1 < r2;
}

bool subset(const exprtvt& r1, const exprtvt& r2) {
	unsigned i = 0, j = 0;
	for (i = 0; i < r1.size(); i++) {
		for (; j < r2.size(); j++) {
			if (r2[j] == r1[i]) {
				break;
			}
		}
		if (j == r2.size())
			return false;
		else
			j++;
	}
	return true;
}

// delete all the duplicate and ineffective reasons
void DEOG::process_reasons()
{
	// sort each reason by edge address
	for (unsigned i = 0; i < m_reasons.size(); i++) {
		sort(m_reasons[i].begin(), m_reasons[i].end(), reason_compare);
	}

	// sort reasons by reason size
	sort(m_reasons.begin(), m_reasons.end(), reason_vec_compare);
	
	// delete duplicate and ineffective reasons
	std::vector< exprtvt > effective_reasons;
	for (unsigned i = 0; i < m_reasons.size(); i++) {
		unsigned j;
		for (j = 0; j < effective_reasons.size(); j++) {
			if (subset(effective_reasons[j], m_reasons[i]))
				break;
		}
		if (j == effective_reasons.size()) {
			effective_reasons.push_back(m_reasons[i]);
		}
	}
	m_reasons.clear();
	m_reasons = effective_reasons;

    //// __FHY_ADD_BEGIN__
#if output_reason
    std::cout << "******************Kernel Reasons ******************\n\n";
    for (unsigned i = 0; i < m_reasons.size(); i++) {
        std::cout << i << ":";
        for (unsigned j = 0; j < m_reasons[i].size(); j++)
            std::cout << " (" << from_expr(m_reasons[i][j]) << ")";
        std::cout << "\n";
    }
    std::cout << "****************************************************\n\n";
#endif
    //// __FHY_ADD_END__
}

void DEOG::add_na_edge(node* nsrc, node* ndst, na_info& info,
		na_info_mapt& na_info_map, edgest& to_be_add_edges) {
	if (nsrc->m_output_epos.empty() && ndst->m_input_epos.empty()) {
		edge* e = new edge(nsrc, ndst, edge::NA);
		na_info_map[e] = info;
		to_be_add_edges.push_back(e);
	} else {
		nodest src_list, dst_list;
		if (nsrc->m_output_epos.empty()) {
			src_list.push_back(nsrc);
		} else {
			for (edgest::iterator pt = nsrc->m_output_epos.begin();
					pt != nsrc->m_output_epos.end(); pt++) {
				src_list.push_back((*pt)->m_dst);
			}
		}
		if (ndst->m_input_epos.empty()) {
			dst_list.push_back(ndst);
		}
		else {
			for (edgest::iterator pt = ndst->m_input_epos.begin();
					pt != ndst->m_input_epos.end(); pt++) {
				dst_list.push_back((*pt)->m_src);
			}
		}
		for (nodest::iterator pti = src_list.begin(); pti != src_list.end(); pti++) {
			for (nodest::iterator ptj = dst_list.begin(); ptj != dst_list.end(); ptj++) {
				add_na_edge(*pti, *ptj, info, na_info_map, to_be_add_edges);
			}
		}
	}
}

void DEOG::reason_merge(exprtvt& tmp, exprtvt& tmp1) {
	exprtvt result;
	int i = 0, j = 0;
	while(i < tmp.size() && j < tmp1.size())
	{
		if (tmp[i] == tmp1[j]) {
			result.push_back(tmp[i++]);
			j++;
		}
		else if (tmp[i] < tmp1[j]) {
			result.push_back(tmp[i++]);
		}
		else {
			result.push_back(tmp1[j++]);
		}
	}
	for (; i < tmp.size(); i++)
		result.push_back(tmp[i]);
	for (; j < tmp1.size(); j++)
		result.push_back(tmp1[j]);

	tmp = result;
}

void DEOG::update_pa_son(edge* e)
{
	node* src = e->m_src;
	node* dst = e->m_dst;

	std::vector<int> src_parents;
	std::vector<int> dst_sons;

	src->get_parents(src_parents);
	src_parents.push_back(src->m_id);

	dst->get_sons(dst_sons);
	dst_sons.push_back(dst->m_id);

	for (unsigned i = 0; i < src_parents.size(); i++)
	{
		node* tmp = m_vnodes[src_parents[i]];
		tmp->sons->pa_son_info.unionc(dst->sons->pa_son_info);
		tmp->sons->pa_son_info.set_true(dst->m_id);
	}
	for (unsigned i = 0; i < dst_sons.size(); i++)
	{
		node* tmp = m_vnodes[dst_sons[i]];
		tmp->parents->pa_son_info.unionc(src->parents->pa_son_info);
		tmp->parents->pa_son_info.set_true(src->m_id);
	}
}

// add a po or na edge to the graph
void DEOG::add_po_edge(edge* e)
{
	add_edge(e);

	update_pa_son(e);
}

// compute w events from a pa_son_set
void DEOG::get_var_nodes(pa_son_set& set, const std::vector<int>& vec, nodevt& nodes)
{
	bit_int& info = set.pa_son_info;
	for (unsigned i = 0; i < vec.size(); i++) {
		if (info.is_true(vec[i]))
			nodes.push_back(m_vnodes[vec[i]]);
	}
}

void DEOG::compute_son_reason(node* src, node* dst, exprtvt& son_reason) {
	std::map<node*, int> weight;
	std::map<node*, edgest> parents;
	std::vector<node*> src_sons;
	src_sons.push_back(src);
	weight[src] = 1;
	int m = 0;
	unsigned i;
	for (i = 0; i < src_sons.size(); i++) {
		node* xsrc = src_sons[i];
		edgest::iterator it;
		m++;

		for (it = xsrc->m_output_rfs.begin(); it != xsrc->m_output_rfs.end(); it++) {
			edge* e = *it;
			if (e->m_dst == dst && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc] + 1;
				parents[e->m_dst].push_back(e);
				break;
			}
			if (is_son_of(e->m_dst, dst) && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc] + 1;
				parents[e->m_dst].push_back(e);
				src_sons.push_back(e->m_dst);
			}
		}

		if (it != xsrc->m_output_rfs.end())
			break;

		for (it = xsrc->m_output_nas.begin(); it != xsrc->m_output_nas.end(); it++) {
			edge* e = *it;
			if (e->m_dst == dst && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc] + 1;
				parents[e->m_dst].push_back(e);
				break;
			}
			if (is_son_of(e->m_dst, dst) && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc] + 1;
				parents[e->m_dst].push_back(e);
				src_sons.push_back(e->m_dst);
			}
		}

		if (it != xsrc->m_output_nas.end())
			break;

		for (it = xsrc->m_output_pos.begin(); it != xsrc->m_output_pos.end(); it++) {
			edge* e = *it;
			if (e->m_dst == dst && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc];
				parents[e->m_dst].push_back(e);
				break;
			}
			if (is_son_of(e->m_dst, dst) && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc];
				parents[e->m_dst].push_back(e);
				src_sons.push_back(e->m_dst);
			}
		}

		if (it != xsrc->m_output_pos.end())
			break;

		for (it = xsrc->m_output_epos.begin(); it != xsrc->m_output_epos.end(); it++) {
			edge* e = *it;
			if (e->m_dst == dst && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc];
				parents[e->m_dst].push_back(e);
				break;
			}
			if (is_son_of(e->m_dst, dst) && weight[e->m_dst] == 0) {
				weight[e->m_dst] = weight[xsrc];
				parents[e->m_dst].push_back(e);
				src_sons.push_back(e->m_dst);
			}
		}

		if (it != xsrc->m_output_epos.end())
			break;
	}

	node* node_tmp = dst;
	while (weight[node_tmp] > 1) {
		edgest& edgest_tmp = parents[node_tmp];

		edgest::iterator it;
		for (it = edgest_tmp.begin(); it != edgest_tmp.end(); it++) {
			if (weight[(*it)->m_src] <= weight[node_tmp])
				break;
		}
		assert(it != edgest_tmp.end());

		if ((*it)->m_type != edge::RF || (*it)->m_cycle_valid)
			reason_merge(son_reason, (*it)->m_reasons);

		node_tmp = (*it)->m_src;
	}
}

void DEOG::compute_rw_map(node* n, address_rw_mapt& rw_map_dst_son_r, address_rw_mapt& rw_map_dst_son_w)
{
	std::vector<int> son_rw;
	n->sons->pa_son_info.get_one_bits(son_rw);
	for (unsigned i = 0; i < son_rw.size(); i++)
	{
		node* tmp = m_vnodes[son_rw[i]];
		if (tmp->is_r())
			rw_map_dst_son_r[tmp->address()].push_back(tmp);
		else if (tmp->is_w())
			rw_map_dst_son_w[tmp->address()].push_back(tmp);
	}
}

bool DEOG::is_redundant(node* src, node* dst, edgest& to_be_add_edges) {
	for (edgest::iterator it = to_be_add_edges.begin(); it != to_be_add_edges.end(); it++)
	{
		node* esrc = (*it)->m_src;
		node* edst = (*it)->m_dst;

		if ((esrc == src || is_son_of(src, esrc)) && (edst == dst || is_son_of(edst, dst)))
			return true;
	}
	return false;
}

// add a rf edge to the graph
void DEOG::add_rf_edge(edge* e)
{
	edgest to_be_add_edges;
	to_be_add_edges.push_back(e);

	na_info_mapt na_info_map;

	// obtain parents and sons of esrc and edst
	node* esrc = e->m_src;
	node* edst = e->m_dst;

	// compute the nas due to RF e
	nodevt src_son_w_same;
	get_var_nodes(*(esrc->sons), m_addr_nodes_map_w[esrc->address()], src_son_w_same);

	for (nodevt::iterator it = src_son_w_same.begin(); it != src_son_w_same.end(); it++) {
		// it should not be contained in dst's sons and src's parents
		node* n = *it;
		if(is_redundant(edst, n, to_be_add_edges))
			continue;
		if (!is_son_of(edst, n) && n != edst && n != esrc) {
			na_info info(esrc, n, e, n);
			add_na_edge(edst, n, info, na_info_map, to_be_add_edges);
		}
	}

	nodevt dst_parent_w_same;
	get_var_nodes(*(edst->parents), m_addr_nodes_map_w[edst->address()], dst_parent_w_same);
	for (nodevt::iterator it = dst_parent_w_same.begin(); it != dst_parent_w_same.end(); it++) {
		// it should not be contained in src's parent and dst's sons
		node* n = *it;
		if(is_redundant(n, esrc, to_be_add_edges))
			continue;
		if (!is_son_of(n, esrc) && n != esrc && n != edst) {
			na_info info(n, edst, e, n);
			add_na_edge(n, esrc, info, na_info_map, to_be_add_edges);
		}
	}

	while (!to_be_add_edges.empty()) {

		edge* tmp = to_be_add_edges.front();
		to_be_add_edges.pop_front();

		node* src = tmp->m_src;
		node* dst = tmp->m_dst;

		if (tmp->m_type == edge::NA) {
			compute_edge_reason(tmp, na_info_map[tmp]);
			if (optimize && tmp->m_reasons.size() > 5) {
				delete tmp;
				continue;
			}
		}

		if (tmp->m_type == edge::RF && is_po_son_of(src, dst)) {
			tmp->m_cycle_valid = false;
			add_edge(tmp);
			continue;
		}

		// compute rw events of esrc and edst
		nodevt src_parent_w;
		get_var_nodes(*(src->parents), m_addr_nodes_map_w["all"], src_parent_w);
		if (src->is_w())
			src_parent_w.push_back(src);

		address_rw_mapt rw_map_dst_son_r, rw_map_dst_son_w;
		compute_rw_map(dst, rw_map_dst_son_r, rw_map_dst_son_w);

		for (nodevt::iterator it = src_parent_w.begin(); it != src_parent_w.end(); it++) {
			nodevt son_w_list = rw_map_dst_son_w[(*it)->address()];
			nodevt son_r_list = rw_map_dst_son_r[(*it)->address()];

			if (dst->same_address(*it)) {
				if (dst->is_r())
					son_r_list.push_back(dst);
				if (dst->is_w() && dst)
					son_w_list.push_back(dst);
			}

			for (unsigned i = 0; i < son_r_list.size(); i++) {
				node* r = son_r_list[i];
				if (r == dst || r == *it)
					continue;

				for (edgest::iterator j = r->m_input_rfs.begin(); j != r->m_input_rfs.end(); j++) {
					node* w= (*j)->m_src;

					if (is_son_of(*it, w))
						continue;

					if(is_redundant(*it, w, to_be_add_edges))
						continue;

					na_info info(*it, src, dst, r, tmp, *j, *it); // *it
					add_na_edge(*it, w, info, na_info_map, to_be_add_edges);
				}
			}

			for (unsigned i = 0; i < son_w_list.size(); i++) {
				node* w = son_w_list[i];
				if (w == dst || w == *it)
					continue;

				for (edgest::iterator j = (*it)->m_output_rfs.begin(); j != (*it)->m_output_rfs.end(); j++) {
					node* r= (*j)->m_dst;

					if (is_son_of(r, w))
						continue;

					if(is_redundant(r, w, to_be_add_edges))
						continue;

					na_info info(*it, src, dst, w, tmp, *j, w); // w
					add_na_edge(r, w, info, na_info_map, to_be_add_edges);
				}
			}
		}

		add_edge(tmp);
		update_pa_son(tmp);
	}
}

void DEOG::compute_edge_reason(edge* na, na_info& info)
{
	exprtvt reason;
	if (info.flag) {
		merge_reason(info.less_src_parent,
				info.less_src,
				info.less_dst,
				info.less_dst_son,
				info.less_e,
				reason);
	}
	else {
		compute_son_reason(info.less_src, info.less_dst, reason);
	}

	reason.push_back(to_rf_sel_symbol(info.rf));

	exprt& guard = info.mid_event->m_event->guard;
	if (!guard.is_true()) {
		if (guard.has_operands() && guard.id() == ID_and) {
			for (unsigned i = 0; i < guard.operands().size(); i++)
            {
                reason.push_back(guard.operands()[i]);
            }
		}
		else
        {
            reason.push_back(guard);
        }
	}

	na->m_reasons = reason;
}

void DEOG::merge_reason(node* src_parent, node* src, node* dst, node* dst_son, edge* e, exprtvt& reason)
{
	compute_son_reason(src_parent, src, reason);
	exprtvt son_reason;
	compute_son_reason(dst, dst_son, son_reason);
	reason_merge(reason, son_reason);
	reason_merge(reason, e->m_reasons);
}


void DEOG::clear() {
	for (nodest::iterator it = m_nodes.begin(); it != m_nodes.end(); it++){
		(*it)->clear(m_all_nodes.size());
	}
	for (edgest::iterator it = m_pos.begin(); it != m_pos.end(); it++)
		delete (*it);
	for (edgest::iterator it = m_epos.begin(); it != m_epos.end(); it++)
		delete (*it);
	for (edgest::iterator it = m_rfs.begin(); it != m_rfs.end(); it++)
		delete (*it);
	for (edgest::iterator it = m_nas.begin(); it != m_nas.end(); it++)
		delete (*it);
	m_nodes.clear();
	m_vnodes.clear();
	m_pos.clear();
	m_epos.clear();
	m_rfs.clear();
	m_nas.clear();
	m_reasons.clear();
	m_trace.clear();
	m_trace_edge.clear();
	m_addr_nodes_map.clear();
	m_addr_nodes_map_r.clear();
	m_addr_nodes_map_w.clear();
	nodes_num = 0;
}

// judge if n2 is a son of n1
bool DEOG::is_son_of(const node* n1, const node* n2) {
	return (n1 == n2 || n1->sons->pa_son_info.is_true(n2->m_id));
}

// judge if n2 is a son of n1
bool DEOG::is_po_son_of(node* n1, const node* n2) {
	return (n1 == n2 || n1->po_sons.is_true(n2->m_id));
}

void DEOG::init_pa_sons() {
	for (nodevt::iterator it = m_all_nodes.begin(); it != m_all_nodes.end(); it++) {
		(*it)->parents = new pa_son_set(m_all_nodes.size());
		(*it)->sons = new pa_son_set(m_all_nodes.size());
	}
}

exprt DEOG::to_rf_sel_symbol(edge* e)
{
	assert(e->m_type == edge::RF);
	return m_equation->edge_symbol_map[e];
}
