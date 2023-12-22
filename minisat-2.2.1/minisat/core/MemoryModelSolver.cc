// __SZH_ADD_BEGIN__

#include "MemoryModelSolver.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

#include <iostream>

#include <cat/cat_module.h>

#define append(lv1, lv2) {for(auto l : lv2) if(l.x != -1) lv1.push_back(l);}

#define push_need(lv, l) {if(l.x != -1) lv.push_back(l);}

#define RETURN_IF_TRUE(statement) {if(statement) return true;}

#define get_assignment(l) ((l).x == -1 ? l_True : value(l))

#define stl_contains(container, key) ((container).find(key) != (container).end())

#define output_lv(lv) {for (auto l: (lv)) std::cout << var(l) << "(" << sign(l) << ") ";}

const int OC_VERBOSITY = 0;

using namespace Minisat;

literal_vector mm_empty_reason;

MemoryModelSolver::MemoryModelSolver()
{
    Solver();
}

void MemoryModelSolver::save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_label_tablet& _oc_label_table, oc_location_mapt& _oc_location_map, std::map<std::string, int>& _oc_result_order, cat_modulet& _cat_module, std::map<std::string, unsigned>& _oc_thread_map)
{
    oc_edge_table = _oc_edge_table;
    oc_label_table = _oc_label_table;
    oc_location_map = _oc_location_map;
    oc_result_order = &_oc_result_order;
    oc_thread_map = _oc_thread_map;
    raw_cat_module = _cat_module;
    set_graph();
}

void MemoryModelSolver::build_sets(int arity, std::string kind, bool is_base) // return true if already set
{
    if(stl_contains(assigned_kinds, kind))
    {
        if(is_base)
            base_kinds.insert(kind_str2id[kind]);
        return;
    }
    assigned_kinds.insert(kind);
    
    int kind_id = numbered;
    numbered++;
    kind_str2id[kind] = kind_id;
    kind_id2str.push_back(kind);
    kind_arities.push_back(arity);
    num_bind_var.push_back(0);
    num_assigned_bind_var.push_back(0);

    binary_elements_to_lit.push_back(std::map<std::pair<int, int>, Lit>());
    unary_elements_to_lit.push_back(std::map<int, Lit>());

    if(is_base)
        base_kinds.insert(kind_id);

    if(OC_VERBOSITY >= 1)
        std::cout << kind << " is represented with " << kind_id << ", arity: " << arity << "\n";

    if(arity == 1)
    {
        unary_sets.resize(kind_id + 1);
        dangerous_unary_sets.resize(kind_id + 1);
        unary_sets[kind_id] = unary_sett(node_names.size());
        dangerous_unary_sets[kind_id] = unary_sett(node_names.size());
    }
    else if(arity == 2)
    {
        binary_sets.resize(kind_id + 1);
        dangerous_binary_sets.resize(kind_id + 1);
        binary_sets[kind_id] = binary_sett(node_names.size());
        dangerous_binary_sets[kind_id] = binary_sett(node_names.size());
    }
    
    propagate_forward.push_back(std::vector<mm_edget>());
    propagate_backward.push_back(std::vector<mm_edget>());
    subtrahend_to_subtract_ids.push_back(std::vector<int>());
    base_to_subtract_ids.push_back(std::vector<int>());
    axioms.push_back(raw_cat_module.get_axiom(kind));
    graph_updates.push_back(std::vector<int>());

    if(!stl_contains(raw_cat_module.propagate_forward, kind))
        return;
    
    for(auto& propagation: raw_cat_module.propagate_forward[kind])
    {
        std::string to = propagation.to;
        build_sets(raw_cat_module.get_arity(to), to, false);

        // for those include id : po? = po | id and po* = po+ | id
        if(propagation.link.link_type == QMARK || propagation.link.link_type == STAR)
        {
            int to_id = kind_str2id[to];
            for(int node = 0; node < int(node_names.size()); node++)
                add_binary_element(node, node, to_id, mm_empty_reason);
        }

        std::string another_kind = propagation.link.another_kind;

        int another_kind_id = -1;
        if(another_kind != "invalid")
        {
            build_sets(raw_cat_module.get_arity(another_kind), another_kind, false);
            another_kind_id = kind_str2id[another_kind];
        }

        auto link = propagation.link;
        mm_linkt new_link(link.link_type, link.link_position, another_kind_id);
        int from_id = kind_id, to_id = kind_str2id[to];

        if(new_link.link_type == rel_opt::MINUS && new_link.link_position == link_post::LEFT)
        {
            int subtract_id = subtract_pool.size();
            subtract_pool.push_back(subtractt(to_id, from_id, another_kind_id));
            subtrahend_to_subtract_ids[another_kind_id].push_back(subtract_id);
            new_link.subtract_id = subtract_id;
        }

        mm_edget new_edge(from_id, new_link, to_id);
        propagate_forward[from_id].push_back(new_edge);
        propagate_backward[to_id].push_back(new_edge);

        if(OC_VERBOSITY >= 1)
        {
            std::cout << "propagation from " << kind << "(" << from_id << ") to " << to << "(" << to_id << ")\n";
            std::cout << "another relation is " << another_kind << "(" << new_edge.link.another_kind << ")\n";
        }
    }
}

void MemoryModelSolver::build_base_to_subtract_ids()
{
    for(auto& base_kind : base_kinds)
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "base_kind: " << base_kind << ", whose full name is " << kind_id2str[base_kind] << "\n";
        
        std::vector<int> upward_kinds{base_kind};
        std::set<int> visited_upward_kinds{base_kind};
        while(!upward_kinds.empty())
        {
            int upward_kind = upward_kinds.back();
            upward_kinds.pop_back();
            for(int subtract_id : subtrahend_to_subtract_ids[upward_kind])
            {
                subtract_pool[subtract_id].subtrahend_bases.push_back(base_kind);
                base_to_subtract_ids[base_kind].push_back(subtract_id);

                if(OC_VERBOSITY >= 1)
                    std::cout << "\t" << kind_id2str[base_kind] << " is related to subtract: " << kind_id2str[subtract_pool[subtract_id].difference] << " = " << kind_id2str[subtract_pool[subtract_id].minuend] << " - " << kind_id2str[subtract_pool[subtract_id].subtrahend] << "\n";
            }
            for(auto& propagation : propagate_forward[upward_kind])
            {
                if(!stl_contains(visited_upward_kinds, propagation.to))
                {
                    upward_kinds.push_back(propagation.to);
                    visited_upward_kinds.insert(propagation.to);
                }
            }
        }
    }
}

void MemoryModelSolver::set_graph()
{
    //create nodes for each event first
    for(auto pair: oc_label_table)
    {
        if(pair.first == "")
            continue;
        get_node(pair.first);
    }
    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;
        get_node(pair.first.first);
        get_node(pair.first.second);
    }

    //pre-create sets for labels and relation names
    for(auto pair: oc_label_table)
    {
        auto kind = pair.second.second;
        build_sets(1, kind, true);
    }
    for(auto pair: oc_edge_table)
    {
        auto kind = pair.second.second;
        build_sets(2, kind, true);
    }

    apply_literal_assignment();

    build_base_to_subtract_ids();

    //init or add unary relations (labels), guard first
    std::set<int> nodes_with_guard;
    for(auto pair: oc_label_table)
    {
        if(pair.first == "")
            continue;

        int node = get_node(pair.first);
        Lit& l = pair.second.first;
        int kind = kind_str2id[pair.second.second];

        if(pair.second.second != "guard")
            continue;

        nodes_with_guard.insert(node);

        if(l.x != -1) // reasonable
            init_unary_element(node, kind, l);
        else
        {
            if(add_unary_element(node, kind, mm_empty_reason)) //todo
            {
                std::cout << "WARNING: Inconsistency during set_graph\n";
                ok = false;
            }
        }
    }

    for(int node = 0; node < int(node_names.size()); node++)
        if(!stl_contains(nodes_with_guard, node))
        {
            if(OC_VERBOSITY >= 1)
                std::cout << node_names[node] << "(" << node << ") misses guard, add it\n";

            if(add_unary_element(node, kind_str2id["guard"], mm_empty_reason)) //todo
            {
                std::cout << "WARNING: Inconsistency during set_graph\n";
                ok = false;
            }
        }

    for(auto pair: oc_label_table)
    {
        if(pair.first == "")
            continue;

        int node = get_node(pair.first);
        Lit& l = pair.second.first;
        int kind = kind_str2id[pair.second.second];

        if(pair.second.second == "guard")
            continue;

        if(l.x != -1) // reasonable
            init_unary_element(node, kind, l);
        else
        {
            if(add_unary_element(node, kind, mm_empty_reason)) //todo
            {
                std::cout << "WARNING: Inconsistency during set_graph\n";
                ok = false;
            }
        }
    }

    //init or add binary relations (edges)
    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int node1 = get_node(pair.first.first);
        int node2 = get_node(pair.first.second);
        Lit& l = pair.second.first;
        int kind = kind_str2id[pair.second.second];

        if(l.x != -1) // reasonable
            init_binary_element(node1, node2, kind, l);
        else
        {
            if(add_binary_element(node1, node2, kind, mm_empty_reason)) //todo
            {
                std::cout << "WARNING: Inconsistency during set_graph\n";
                ok = false;
            }
        }
    }
}

lbool MemoryModelSolver::search(int nof_conflicts)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    //initialize nodes

    if(use_available_info())
    {
        std::cout << "WARNING: use_available_info find cycle at the very start\n";
        return l_False;
    }

    for (;;){
        CRef confl = propagate();
        if (confl != CRef_Undef){
            // CONFLICT
            conflicts++; conflictC++;
            if (decisionLevel() == 0) return l_False;

            learnt_clause.clear();
            analyze(confl, learnt_clause, backtrack_level);

            cancelUntil(backtrack_level);

            if (learnt_clause.size() == 1){
                uncheckedEnqueue(learnt_clause[0]);
            }else{
                CRef cr = ca.alloc(learnt_clause, true);
                learnts.push(cr);
                attachClause(cr);
                claBumpActivity(ca[cr]);
                uncheckedEnqueue(learnt_clause[0], cr);
            }

            varDecayActivity();
            claDecayActivity();

            if (--learntsize_adjust_cnt == 0){
                learntsize_adjust_confl *= learntsize_adjust_inc;
                learntsize_adjust_cnt    = (int)learntsize_adjust_confl;
                max_learnts             *= learntsize_inc;

                if (verbosity >= 1)
                    printf("| %9d | %7d %8d %8d | %8d %8d %6.0f | %6.3f %% |\n",
                           (int)conflicts,
                           (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]), nClauses(), (int)clauses_literals,
                           (int)max_learnts, nLearnts(), (double)learnts_literals/nLearnts(), progressEstimate()*100);
            }

        }else{
            // NO CONFLICT
            if ((nof_conflicts >= 0 && conflictC >= nof_conflicts) || !withinBudget()){
                // Reached bound on number of conflicts:
                progress_estimate = progressEstimate();
                cancelUntil(0);
                return l_Undef; }

            // Simplify the set of problem clauses:
            if (decisionLevel() == 0 && !simplify())
                return l_False;

            if (learnts.size()-nAssigns() >= max_learnts)
                // Reduce the set of learnt clauses:
                reduceDB();

            Lit next = lit_Undef;
            while (decisionLevel() < assumptions.size()){
                // Perform user provided assumption:
                Lit p = assumptions[decisionLevel()];
                if (value(p) == l_True){
                    // Dummy decision level:
                    newDecisionLevel();
                }else if (value(p) == l_False){
                    analyzeFinal(~p, conflict);
                    return l_False;
                }else{
                    next = p;
                    break;
                }
            }

            if (next == lit_Undef){
                // New variable decision:
                decisions++;
                next = pickBranchLit();

                if (next == lit_Undef)
                {
                    // szh: Final check
                    bool cycle = false;
                    for(auto& subtract : subtract_pool)
                    {
                        if(!can_propagate_subtract(subtract))
                            cycle = propagate_subtract(subtract);
                        if(cycle)
                            break;
                    }
                    if(cycle)
                    {
                        conflict_cycle++;
                        if (OC_VERBOSITY >= 1)
                        {
                            std::cout << "found cycle during final check! conflict clause is: \n";
                            for (auto l: conflict_clause)
                                std::cout << var(l) << "(" << sign(l) << ") ";
                            std::cout << "\n";
                            for(auto l: conflict_clause)
                                std::cout << toInt(get_assignment(l)) << " ";
                            std::cout << "\n";
                        }

                        auto &lv = conflict_clause;

                        vec<Lit> minisat_lv;
                        for (auto l: lv)
                            minisat_lv.push(~l);

                        confl = ca.alloc(minisat_lv, true);
                        conflicts++; conflictC++;
                        if (decisionLevel() == 0) return l_False;

                        learnt_clause.clear();
                        analyze(confl, learnt_clause, backtrack_level);

                        if(backtrack_level > 0)
                        {
                            bool all_same_level = true;
                            for(int i = 0; i < int(learnt_clause.size()); i++)
                                all_same_level = all_same_level && (level(var(learnt_clause[i])) == backtrack_level);
                            if(all_same_level)
                                backtrack_level--;
                        }

                        if(OC_VERBOSITY >= 1)
                        {
                            std::cout << "learnt clause is: \n";
                            for (int i = 0; i < int(learnt_clause.size()); i++)
                                std::cout << var(learnt_clause[i]) << "(" << sign(learnt_clause[i]) << ") ";
                            std::cout << "\nlevel:\n";
                            for (int i = 0; i < int(learnt_clause.size()); i++)
                                std::cout << level(var(learnt_clause[i])) << " ";
                            std::cout << "\n";
                            std::cout << "backtrace to level " << backtrack_level << "\n";
                        }

                        cancelUntil(backtrack_level);

                        if (learnt_clause.size() == 1){
                            uncheckedEnqueue(learnt_clause[0]);
                        }else{
                            CRef cr = ca.alloc(learnt_clause, true);
                            learnts.push(cr);
                            attachClause(cr);
                            claBumpActivity(ca[cr]);
                            uncheckedEnqueue(learnt_clause[0], cr);
                        }

                        varDecayActivity();
                        claDecayActivity();

                        if (--learntsize_adjust_cnt == 0){
                            learntsize_adjust_confl *= learntsize_adjust_inc;
                            learntsize_adjust_cnt    = (int)learntsize_adjust_confl;
                            max_learnts             *= learntsize_inc;

                            if (verbosity >= 1)
                                printf("| %9d | %7d %8d %8d | %8d %8d %6.0f | %6.3f %% |\n",
                                    (int)conflicts,
                                    (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]), nClauses(), (int)clauses_literals,
                                    (int)max_learnts, nLearnts(), (double)learnts_literals/nLearnts(), progressEstimate()*100);
                        }
                        continue;
                    }

                    // Model found:
                    return l_True;
                }
            }

            // Increase decision level and enqueue 'next'
            newDecisionLevel();
            uncheckedEnqueue(next);
        }
    }
}

static double luby(double y, int x){

    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for (size = 1, seq = 0; size < x+1; seq++, size = 2*size+1);

    while (size-1 != x){
        size = (size-1)>>1;
        seq--;
        x = x % size;
    }

    return pow(y, seq);
}

lbool MemoryModelSolver::solve_()
{
    model.clear();
    conflict.clear();
    if (!ok) return l_False;

    solves++;

    max_learnts               = nClauses() * learntsize_factor;
    learntsize_adjust_confl   = learntsize_adjust_start_confl;
    learntsize_adjust_cnt     = (int)learntsize_adjust_confl;
    lbool   status            = l_Undef;

    if (verbosity >= 1){
        printf("============================[ Search Statistics ]==============================\n");
        printf("| Conflicts |          ORIGINAL         |          LEARNT          | Progress |\n");
        printf("|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |\n");
        printf("===============================================================================\n");
    }

    // Search:
    int curr_restarts = 0;
    while (status == l_Undef){
        double rest_base = luby_restart ? luby(restart_inc, curr_restarts) : pow(restart_inc, curr_restarts);
        status = search(rest_base * restart_first);
        if (!withinBudget()) break;
        curr_restarts++;
    }

    if (verbosity >= 1)
        printf("===============================================================================\n");


    if (status == l_True){
        // Extend & copy model:
        model.growTo(nVars());
        for (int i = 0; i < nVars(); i++) model[i] = value(i);

        // show model
        if(OC_VERBOSITY >= 1)
            show_model();
    }
    else if (status == l_False && conflict.size() == 0)
        ok = false;

    std::cout << "MemoryModelSolver finishes with " << conflicts << " conflicts, " << decisions << " decisions, and " << propagations << " propagations. ";
    std::cout << theory_propagation << " theory propagations and " << conflict_cycle << " cycles included.\n";

    cancelUntil(0);
    return status;
}

Var MemoryModelSolver::newVar (lbool upol, bool dvar)
{
    bind.push(-1);
    return Solver::newVar(upol, dvar);
}

CRef MemoryModelSolver::propagate()
{
    literals_to_assign.clear();

    CRef confl = CRef_Undef;
    int num_props = 0;
    watches.cleanAll();

    std::vector<binary_elementt> binary_elements_to_add;
    std::vector<unary_elementt> unary_elements_to_add;

    while (qhead < trail.size()){
        Lit            p   = trail[qhead++];     // 'p' is enqueued fact to propagate.
        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        //our method
        auto unary_range = lit_to_unary_elements.equal_range(p);
        for(auto it = unary_range.first; it != unary_range.second; it++)
            unary_elements_to_add.push_back(it->second);

        auto binary_range = lit_to_binary_elements.equal_range(p);
        for(auto it = binary_range.first; it != binary_range.second; it++)
            binary_elements_to_add.push_back(it->second);
        //out method ends

        for (i = j = (Watcher*)ws, end = i + ws.size();  i != end;){
            // Try to avoid inspecting the clause:
            Lit blocker = i->blocker;
            if (value(blocker) == l_True){
                *j++ = *i++; continue; }

            // Make sure the false literal is data[1]:
            CRef     cr        = i->cref;

            Clause&  c         = ca[cr];

            Lit      false_lit = ~p;
            if (c[0] == false_lit)
                c[0] = c[1], c[1] = false_lit;
            assert(c[1] == false_lit);
            i++;

            // If 0th watch is true, then clause is already satisfied.
            Lit     first = c[0];
            Watcher w     = Watcher(cr, first);
            if (first != blocker && value(first) == l_True){
                *j++ = w; continue; }

            // Look for new watch:
            for (int k = 2; k < c.size(); k++)
                if (value(c[k]) != l_False){
                    c[1] = c[k]; c[k] = false_lit;
                    watches[~c[1]].push(w);
                    goto NextClause; }

            // Did not find watch -- clause is unit under assignment:
            *j++ = w;
            if (value(first) == l_False){
                confl = cr;
                qhead = trail.size();
                // Copy the remaining watches:
                while (i < end)
                    *j++ = *i++;
            }else
                uncheckedEnqueue(first, cr);

        NextClause:;
        }
        ws.shrink(i - j);
    }

    //our method
    if(confl == CRef_Undef)
    {
        bool cycle = false;

        for(auto& bind_kind : finished_kinds)
        {
            for(auto subtract_id : base_to_subtract_ids[bind_kind])
            {
                auto& subtract = subtract_pool[subtract_id];
                if(can_propagate_subtract(subtract))
                    cycle = propagate_subtract(subtract); // what if true?
            }
        }
        finished_kinds.clear();

        for(auto& unary_element: unary_elements_to_add)
        {
            if(cycle)
                break;
            cycle = add_unary_element(unary_element);
        }

        for(auto& binary_element: binary_elements_to_add)
        {
            if(cycle)
                break;
            cycle = add_binary_element(binary_element);
        }

        if(!cycle)
            cycle = cycle_detection();

        if(cycle)
        {
            conflict_cycle++;
            if (OC_VERBOSITY >= 1)
            {
                std::cout << "found cycle! conflict clause is: \n";
                for (auto l: conflict_clause)
                    std::cout << var(l) << "(" << sign(l) << ") ";
                std::cout << "\n";
                for(auto l: conflict_clause)
                    std::cout << toInt(get_assignment(l)) << " ";
                std::cout << "\n";
            }

            auto &lv = conflict_clause;

            vec<Lit> minisat_lv;
            for (auto l: lv)
                minisat_lv.push(~l);

            confl = ca.alloc(minisat_lv, true);
        }
        else
            apply_literal_assignment();
    }
    //our method ends

    propagations += num_props;
    simpDB_props -= num_props;

    return confl;
}

void MemoryModelSolver::cancelUntil(int level)
{
    if (decisionLevel() > level)
    {
        for (int c = trail.size()-1; c >= trail_lim[level]; c--)
        {
            Var x  = var(trail[c]);
            assigns [x] = l_Undef;

            if(bind[x] != -1)
                num_assigned_bind_var[bind[x]]--;

            if (phase_saving > 1 || ((phase_saving == 1) && c > trail_lim.last()))
                polarity[x] = sign(trail[c]);
            insertVarOrder(x); 
        }
        qhead = trail_lim[level];
        trail.shrink(trail.size() - trail_lim[level]);
        trail_lim.shrink(trail_lim.size() - level);

        //Our new operation
        pop_scope(level);
    }
}

bool MemoryModelSolver::can_propagate_subtract(subtractt& subtract)
{
    for(auto base_kind : subtract.subtrahend_bases)
        if(num_assigned_bind_var[base_kind] < num_bind_var[base_kind])
            return false;
    return true;
}

bool MemoryModelSolver::propagate_subtract(subtractt& subtract)
{
    int minuend = subtract.minuend, subtrahend = subtract.subtrahend, difference = subtract.difference;
    if(kind_arities[minuend] == 1)
    {
        auto& minuend_set = unary_sets[minuend];
        auto& subtrahend_set = unary_sets[subtrahend];
        for(int node : minuend_set.all_elements())
        {
            if(subtrahend_set.contains(node))
                continue;
            RETURN_IF_TRUE(add_unary_element(node, difference, minuend_set.get_reason(node)));
        }
    }
    if(kind_arities[minuend] == 2)
    {
        auto& minuend_set = binary_sets[minuend];
        auto& subtrahend_set = binary_sets[subtrahend];
        for(int node1 = 0; node1 < int(node_names.size()); node1++)
        {
            for(auto node2 : minuend_set.get_outs(node1))
            {
                if(subtrahend_set.contains(node1, node2))
                    continue;
                RETURN_IF_TRUE(add_binary_element(node1, node2, difference, minuend_set.get_reason(node1, node2)));
            }
        }
    }
    return false;
}

void MemoryModelSolver::uncheckedEnqueue(Lit p, CRef from)
{
    Solver::uncheckedEnqueue(p, from);

    int bind_kind = bind[var(p)];
    if(bind_kind != -1)
    {
        num_assigned_bind_var[bind_kind]++;
        if(OC_VERBOSITY >= 1)
            std::cout << kind_id2str[bind_kind] << " has one more assigned bind var, now " << num_assigned_bind_var[bind_kind] << "/" << num_bind_var[bind_kind] << "\n";
        if(num_assigned_bind_var[bind_kind] >= num_bind_var[bind_kind])
        {
            finished_kinds.push_back(bind_kind);
            if(OC_VERBOSITY >= 1)
                std::cout << kind_id2str[bind_kind] << " has all bind var assigned\n";
        }
    }
}

void MemoryModelSolver::newDecisionLevel()
{
    trail_lim.push(trail.size());
    push_scope();
}

void MemoryModelSolver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel)
{
    int pathC = 0;
    Lit p     = lit_Undef;

    // Generate conflict clause:
    //
    out_learnt.push();      // (leave room for the asserting literal)
    int index   = trail.size() - 1;

    do{
        assert(confl != CRef_Undef); // (otherwise should be UIP)
        Clause& c = ca[confl];

        if (c.learnt())
            claBumpActivity(c);

        for (int j = (p == lit_Undef) ? 0 : 1; j < c.size(); j++){
            Lit q = c[j];

            if (!seen[var(q)] && level(var(q)) > 0){
                varBumpActivity(var(q));
                seen[var(q)] = 1;
                if (level(var(q)) >= decisionLevel())
                    pathC++;
                else
                    out_learnt.push(q);
            }
        }

        // Select next clause to look at:
        while (!seen[var(trail[index--])]);
        p     = trail[index+1];
        confl = reason(var(p));
        seen[var(p)] = 0;
        pathC--;

    }while (pathC > 0);
    out_learnt[0] = ~p;

    // Simplify conflict clause:
    //
    int i, j;
    out_learnt.copyTo(analyze_toclear);
    if (ccmin_mode == 2){
        uint32_t abstract_level = 0;
        for (i = 1; i < out_learnt.size(); i++)
            abstract_level |= abstractLevel(var(out_learnt[i])); // (maintain an abstraction of levels involved in conflict)

        for (i = j = 1; i < out_learnt.size(); i++)
            if (reason(var(out_learnt[i])) == CRef_Undef || !litRedundant(out_learnt[i], abstract_level))
                out_learnt[j++] = out_learnt[i];

    }else if (ccmin_mode == 1){
        for (i = j = 1; i < out_learnt.size(); i++){
            Var x = var(out_learnt[i]);

            if (reason(x) == CRef_Undef)
                out_learnt[j++] = out_learnt[i];
            else{
                Clause& c = ca[reason(var(out_learnt[i]))];
                for (int k = 1; k < c.size(); k++)
                    if (!seen[var(c[k])] && level(var(c[k])) > 0){
                        out_learnt[j++] = out_learnt[i];
                        break; }
            }
        }
    }else
        i = j = out_learnt.size();

    max_literals += out_learnt.size();
    out_learnt.shrink(i - j);
    tot_literals += out_learnt.size();

    // Find correct backtrack level:
    //
    if (out_learnt.size() == 1)
        out_btlevel = 0;
    else{
        int max_i = 1;
        // Find the first literal assigned at the next-highest level:
        for (int i = 2; i < out_learnt.size(); i++)
            if (level(var(out_learnt[i])) > level(var(out_learnt[max_i])))
                max_i = i;
        // Swap-in this literal at index 1:
        Lit p             = out_learnt[max_i];
        out_learnt[max_i] = out_learnt[1];
        out_learnt[1]     = p;
        out_btlevel       = level(var(p));
    }

    for (int j = 0; j < analyze_toclear.size(); j++) seen[var(analyze_toclear[j])] = 0;    // ('seen[]' is now cleared)
}

extern std::vector<std::string> write_order;

std::vector<std::string> MemoryModelSolver::get_write_order()
{
    return write_order;
}

bool Minisat::MemoryModelSolver::add_unary_element(unary_elementt elem)
{
    return add_unary_element(elem.node, elem.kind, elem.reason);
}

bool Minisat::MemoryModelSolver::add_unary_element(int node, int kind, literal_vector& reason)
{
    if(unary_sets[kind].contains(node)) // if this already exists
        return false;

    if(OC_VERBOSITY >= 1)
        std::cout << "adding unary element " << node_names[node] << "(" << node << ") " << kind_id2str[kind] << "\n";

    if(axioms[kind] == EMPTY)
    {
        conflict_clause = reason;
        return true;
    }

    auto& unary_set = unary_sets[kind];
    unary_set.set(node, reason);
    mm_trail.push_back(unary_trail(kind, node));

    for(auto& propagation: propagate_forward[kind])
    {
        auto& next_kind = propagation.to;
        auto& link = propagation.link;
        if(link.link_type == ALT)
        {
            RETURN_IF_TRUE(add_unary_element(node, next_kind, reason))
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            if(another_unary_set.contains(node))
            {
                auto another_reason = another_unary_set.get_reason(node);
                append(another_reason, reason);
                RETURN_IF_TRUE(add_unary_element(node, next_kind, another_reason))
            }
        }
        else if(link.link_type == MINUS)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            if(link.link_position == LEFT)
            {
                if(another_unary_set.contains(node))
                    continue;

                auto& subtract = subtract_pool[link.subtract_id];
                if(can_propagate_subtract(subtract))
                    RETURN_IF_TRUE(add_unary_element(node, next_kind, reason)) // todo: is reason that simple?
            }
            // else if(link.link_position == RIGHT)
            // {
            //     assert(!another_unary_set.contains(node));
            // }
        }
        else if(link.link_type == TIMES)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            auto& another_all_elements = another_unary_set.all_elements();
            for(int another_element : another_all_elements)
            {
                auto another_reason = another_unary_set.get_reason(another_element);
                append(another_reason, reason);
                if(link.link_position == LEFT)
                    RETURN_IF_TRUE(add_binary_element(node, another_element, next_kind, another_reason))
                else
                    RETURN_IF_TRUE(add_binary_element(another_element, node, next_kind, another_reason))
            }
        }
        else if(link.link_type == BRACKET)
        {
            RETURN_IF_TRUE(add_binary_element(node, node, next_kind, reason))
        }
        else if(link.link_type == GUARD_ENABLEMENT)
        {
            auto& guard_unary_set = unary_sets[kind];
            auto& raw_kind = link.another_kind;
            auto& raw_binary_set = binary_sets[raw_kind];
            //when then node is left
            auto& node_outs = raw_binary_set.get_outs(node);
            for(auto another_node : node_outs)
            {
                if(guard_unary_set.contains(another_node))
                {
                    auto raw_reason = raw_binary_set.get_reason(node, another_node);
                    append(raw_reason, reason);
                    append(raw_reason, guard_unary_set.get_reason(another_node))
                    RETURN_IF_TRUE(add_binary_element(node, another_node, next_kind, raw_reason));
                }
            }
            //when the node is right
            auto& node_ins = raw_binary_set.get_ins(node);
            for(auto another_node : node_ins)
            {
                if(guard_unary_set.contains(another_node))
                {
                    auto raw_reason = raw_binary_set.get_reason(another_node, node);
                    append(raw_reason, reason);
                    append(raw_reason, guard_unary_set.get_reason(another_node))
                    RETURN_IF_TRUE(add_binary_element(another_node, node, next_kind, raw_reason));
                }
            }
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in unary element's propagation.\n";
        }
    }
    return false;
}

bool Minisat::MemoryModelSolver::add_binary_element(binary_elementt elem)
{
    return add_binary_element(elem.node1, elem.node2, elem.kind, elem.reason);
}

bool Minisat::MemoryModelSolver::add_binary_element(int node1, int node2, int kind, literal_vector& reason)
{
    if(binary_sets[kind].contains(node1, node2)) // if this already exists
        return false;

    if(OC_VERBOSITY >= 1)
    {
        std::cout << "adding binary element " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << "\n";
        std::cout << "\twith reason: \n\t";
        for (auto l: reason)
            std::cout << var(l) << "(" << sign(l) << ") ";
        std::cout << "\n";
    }

    if(dangerous_binary_sets[kind].contains(node1, node2))
    {
        conflict_clause = reason;
        append(conflict_clause, dangerous_binary_sets[kind].get_reason(node1, node2));

        if(OC_VERBOSITY >= 1)
        {
            std::cout << "but dangerous binary element " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is already there\n";
            // std::cout << "current model is:\n";
            // show_model();
        }
        
        return true;
    }

    if(axioms[kind] == ACYCLIC)
        graph_updates[kind].push_back(node2); //record updates, waiting cycle_detection to find cycles

    auto& binary_set = binary_sets[kind];
    binary_set.set(node1, node2, reason);
    mm_trail.push_back(binary_trail(kind, node1, node2));

    for(auto& propagation: propagate_forward[kind])
    {
        if(OC_VERBOSITY >= 1)
        {
            std::cout << "employing propagation rule: ";
            std::cout << kind_id2str[propagation.from] << " " << propagation.link.link_type_name() << " " << kind_id2str[propagation.to] << "\n";
        }

        auto& next_kind = propagation.to;
        auto& link = propagation.link;
        if(link.link_type == ALT || link.link_type == QMARK)
        {
            RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, reason))
        }
        else if(link.link_type == FLIP)
        {
            RETURN_IF_TRUE(add_binary_element(node2, node1, next_kind, reason))
        }
        else if(link.link_type == SEQ)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_set = binary_sets[another_kind];

            auto& next_dangerous_binary_set = dangerous_binary_sets[next_kind];

            if(link.link_position == LEFT)
            {
                auto& node2_outs = another_binary_set.get_outs(node2);
                for(auto node3_id : node2_outs)
                {
                    auto another_reason = another_binary_set.get_reason(node2, node3_id);
                    append(another_reason, reason);
                    RETURN_IF_TRUE(add_binary_element(node1, node3_id, next_kind, another_reason))
                }

                auto& node1_dangerous_outs = next_dangerous_binary_set.get_outs(node1);
                for(auto node3_id : node1_dangerous_outs)
                {
                    auto dangerous_reason = next_dangerous_binary_set.get_reason(node1, node3_id);
                    append(dangerous_reason, reason);
                    RETURN_IF_TRUE(add_dangerous_binary_element(node2, node3_id, another_kind, dangerous_reason))
                }
            }
            if(link.link_position == RIGHT)
            {
                auto& node1_ins = another_binary_set.get_ins(node1);
                for(auto node0_id : node1_ins)
                {
                    auto another_reason = another_binary_set.get_reason(node0_id, node1);
                    append(another_reason, reason);
                    RETURN_IF_TRUE(add_binary_element(node0_id, node2, next_kind, another_reason))
                }

                auto& node2_dangerous_ins = next_dangerous_binary_set.get_ins(node2);
                for(auto node0_id : node2_dangerous_ins)
                {
                    auto dangerous_reason = next_dangerous_binary_set.get_reason(node0_id, node2);
                    append(dangerous_reason, reason);
                    RETURN_IF_TRUE(add_dangerous_binary_element(node0_id, node1, another_kind, dangerous_reason))
                }
            }
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_set = binary_sets[another_kind];
            if(another_binary_set.contains(node1, node2))
            {
                auto another_reason = another_binary_set.get_reason(node1, node2);
                append(another_reason, reason);
                RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, another_reason))
            }
        }
        else if(link.link_type == MINUS)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_set = binary_sets[another_kind];
            if(link.link_position == LEFT)
            {
                if(another_binary_set.contains(node1, node2))
                    continue;

                auto& subtract = subtract_pool[link.subtract_id];
                if(can_propagate_subtract(subtract))
                    RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, reason)) // todo: is reason that simple?
            }
            // else if(link.link_position == RIGHT)
            // {
            //     assert(!another_binary_set.contains(node1, node2));
            // }
        }
        else if(link.link_type == PLUS || link.link_type == STAR)
        {
            auto& next_binary_set = binary_sets[next_kind];
            auto& next_dangerous_binary_set = dangerous_binary_sets[next_kind];
            if(!next_binary_set.contains(node1, node2))
            {
                RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, reason))
                std::vector<std::pair<std::pair<int, int>, literal_vector>> waiting_list;
                waiting_list.push_back(std::make_pair(std::make_pair(node1, node2), reason));
                while(!waiting_list.empty())
                {
                    int n1 = waiting_list.back().first.first;
                    int n2 = waiting_list.back().first.second;
                    auto r = waiting_list.back().second;
                    waiting_list.pop_back();

                    auto& n2_outs = next_binary_set.get_outs(n2);
                    auto& n1_ins = next_binary_set.get_ins(n1);
                    for(auto& n3 : n2_outs)
                    {
                        if(next_binary_set.contains(n1, n3))
                            continue;
                        
                        auto another_r = next_binary_set.get_reason(n2, n3);
                        append(another_r, r);
                        RETURN_IF_TRUE(add_binary_element(n1, n3, next_kind, another_r))
                        waiting_list.push_back(std::make_pair(std::make_pair(n1, n3), another_r));
                    }
                    for(auto& n0 : n1_ins)
                    {
                        if(next_binary_set.contains(n0, n2))
                            continue;

                        auto another_r = next_binary_set.get_reason(n0, n1);
                        append(another_r, r);
                        RETURN_IF_TRUE(add_binary_element(n0, n2, next_kind, another_r))
                        waiting_list.push_back(std::make_pair(std::make_pair(n0, n2), another_r));
                    }
                }
            }

            auto& node1_dangerous_outs = next_dangerous_binary_set.get_outs(node1);
            for(auto node3_id : node1_dangerous_outs)
            {
                auto dangerous_reason = next_dangerous_binary_set.get_reason(node1, node3_id);
                append(dangerous_reason, reason);
                RETURN_IF_TRUE(add_dangerous_binary_element(node2, node3_id, next_kind, dangerous_reason))
            }
            auto& node2_dangerous_ins = next_dangerous_binary_set.get_ins(node2);
            for(auto node0_id : node2_dangerous_ins)
            {
                auto dangerous_reason = next_dangerous_binary_set.get_reason(node0_id, node2);
                append(dangerous_reason, reason);
                RETURN_IF_TRUE(add_dangerous_binary_element(node0_id, node1, next_kind, dangerous_reason))
            }
            
        }
        else if(link.link_type == GUARD_ENABLEMENT)
        {
            auto& guard_kind = link.another_kind;
            auto& guard_unary_set = unary_sets[guard_kind];

            if(guard_unary_set.contains(node1) && guard_unary_set.contains(node2))
            {
                auto raw_reason = reason;
                append(raw_reason, guard_unary_set.get_reason(node1));
                append(raw_reason, guard_unary_set.get_reason(node2))
                RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, raw_reason));
            }
        }
        else if(link.link_type == INT)
        {
            if(thread_map[node1] == thread_map[node2])
                RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, reason))
        }
        else if(link.link_type == EXT)
        {
            if(thread_map[node1] != thread_map[node2])
                RETURN_IF_TRUE(add_binary_element(node1, node2, next_kind, reason))
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in binary element's forward propagation.\n";
        }
    }

    return false;
}

bool Minisat::MemoryModelSolver::add_dangerous_unary_element(int node, int kind, literal_vector& reason)
{
    //todo
    return false;
}

bool Minisat::MemoryModelSolver::add_dangerous_binary_element(int node1, int node2, int kind, literal_vector& reason)
{
    auto& dangerous_binary_set = dangerous_binary_sets[kind];
    auto& binary_set = binary_sets[kind];

    if(dangerous_binary_set.contains(node1, node2)) // if this already exists
        return false;

    if(OC_VERBOSITY >= 1)
    {
        std::cout << "adding dangerous binary element " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << "\n";
        std::cout << "\twith reason: \n\t";
        for (auto l: reason)
            std::cout << var(l) << "(" << sign(l) << ") ";
        std::cout << "\n";
    }

    if(binary_set.contains(node1, node2))
    {
        if(OC_VERBOSITY >= 1)
        {
            std::cout << "but binary element " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is already there\n";
            // std::cout << "current model is:\n";
            // show_model();
        }

        conflict_clause = reason;
        append(conflict_clause, binary_set.get_reason(node1, node2));
        return true;
    }

    dangerous_binary_set.set(node1, node2, reason);
    mm_dangerous_trail.push_back(binary_trail(kind, node1, node2));

    auto& binary_elements_to_lit_this_kind = binary_elements_to_lit[kind];
    if(binary_elements_to_lit_this_kind.find(std::make_pair(node1, node2)) != binary_elements_to_lit_this_kind.end())
    {
        auto prevented_lit = binary_elements_to_lit_this_kind[std::make_pair(node1, node2)];
        if(prevented_lit != lit_Error && get_assignment(prevented_lit) != l_Undef)
        {
            literal_vector unit_lv;
            for(auto l: reason)
                unit_lv.push_back(~l);
            unit_lv.push_back(~prevented_lit);

            if(OC_VERBOSITY >= 1)
            {
                std::cout << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is set to false\n";
                std::cout << "related to lit " << var(prevented_lit) << "(" << sign(prevented_lit) << ") \n";
            }

            assign_literal(~prevented_lit, unit_lv);
        }
        
    }

    for(auto& propagation: propagate_backward[kind])
    {
        if(OC_VERBOSITY >= 1)
        {
            std::cout << "employing propagation rule in the backward manner: ";
            std::cout << kind_id2str[propagation.from] << " " << propagation.link.link_type_name() << " " << kind_id2str[propagation.to] << "\n";
        }

        auto& last_kind = propagation.from;
        auto& last_binary_set = binary_sets[last_kind];
        auto& link = propagation.link;
        if(link.link_type == ALT || link.link_type == QMARK)
        {
            RETURN_IF_TRUE(add_dangerous_binary_element(node1, node2, last_kind, reason))
        }
        else if(link.link_type == FLIP)
        {
            RETURN_IF_TRUE(add_dangerous_binary_element(node2, node1, last_kind, reason))
        }
        else if(link.link_type == SEQ)
        {
            auto& another_kind = link.another_kind;
            if(link.link_position == LEFT)
            {
                auto& node1_outs = last_binary_set.get_outs(node1);
                for(auto node3_id : node1_outs)
                {
                    auto last_reason = last_binary_set.get_reason(node3_id, node2);
                    append(last_reason, reason);
                    RETURN_IF_TRUE(add_dangerous_binary_element(node3_id, node2, another_kind, last_reason))
                }
            }
            if(link.link_position == RIGHT)
            {
                auto& node2_ins = last_binary_set.get_ins(node2);
                for(auto node3_id : node2_ins)
                {
                    auto last_reason = last_binary_set.get_reason(node1, node3_id);
                    append(last_reason, reason);
                    RETURN_IF_TRUE(add_dangerous_binary_element(node1, node3_id, another_kind, last_reason))
                }
            }
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            if(last_binary_set.contains(node1, node2))
            {
                auto last_reason = last_binary_set.get_reason(node1, node2);
                append(last_reason, reason);
                RETURN_IF_TRUE(add_dangerous_binary_element(node1, node2, another_kind, last_reason))
            }
        }
        else if(link.link_type == MINUS)
        {
            // todo
        }
        else if(link.link_type == PLUS || link.link_type == STAR)
        {
            RETURN_IF_TRUE(add_dangerous_binary_element(node1, node2, last_kind, reason))

            auto& node1_outs = last_binary_set.get_outs(node1);
            for(auto node3_id : node1_outs)
            {
                auto last_reason = last_binary_set.get_reason(node3_id, node2);
                append(last_reason, reason);
                RETURN_IF_TRUE(add_dangerous_binary_element(node3_id, node2, kind, last_reason))
            }

            auto& node2_ins = last_binary_set.get_ins(node2);
            for(auto node3_id : node2_ins)
            {
                auto last_reason = last_binary_set.get_reason(node1, node3_id);
                append(last_reason, reason);
                RETURN_IF_TRUE(add_dangerous_binary_element(node1, node3_id, kind, last_reason))
            }
        }
        else if(link.link_type == TIMES)
        {
            if(link.link_position == LEFT)
            {
                auto& left_kind = last_kind;
                auto& right_kind = link.another_kind;
                if(unary_sets[left_kind].contains(node1) && unary_sets[right_kind].contains(node2))
                {
                    conflict_clause = reason;
                    std::cout << "unexpected conflict during TIMES backward propagation\n";
                    return true;
                }
            }
        }
        else if(link.link_type == GUARD_ENABLEMENT)
        {
            // todo
        }
        else if(link.link_type == INT)
        {
            // todo
        }
        else if(link.link_type == EXT)
        {
            // todo
        }
        else if(link.link_type == BRACKET)
        {
            // todo
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in binary element's backward propagation.\n";
        }
    }

    return false;
}

std::vector<int> dfs_stack;
bool dfs_in_stack[100005];
bool visited[100005];

bool Minisat::MemoryModelSolver::dfs(int kind, int node)
{
    if(dfs_in_stack[node])
    {
        literal_vector reason;

        int start = 0;
        for(int i = 0; i < int(dfs_stack.size()); i++)
            if(dfs_stack[i] == node)
            {
                start = i;
                break;
            }
        for(int i = start; i < int(dfs_stack.size()); i++)
        {
            int from = dfs_stack[i];
            int to;
            if(i + 1 < int(dfs_stack.size()))
                to = dfs_stack[i + 1];
            else
                to = dfs_stack[start];
            append(reason, binary_sets[kind].get_reason(from, to));
        }

        conflict_clause = reason;
        return true;
    }
    else if(visited[node])
        return false;
    
    visited[node] = true;

    dfs_stack.push_back(node);
    dfs_in_stack[node] = true;

    for(auto& out_node : binary_sets[kind].get_outs(node))
        RETURN_IF_TRUE(dfs(kind, out_node))

    dfs_stack.pop_back();
    dfs_in_stack[node] = false;
    return false;
}

bool Minisat::MemoryModelSolver::cycle_detection(int kind)
{
    bool has_cycle = false;
    for(int starting_node : graph_updates[kind])
    {
        if(dfs(kind, starting_node))
        {
            has_cycle = true;
            break;
        }
    }

    graph_updates[kind].clear();
    dfs_stack.clear();
    memset(dfs_in_stack, 0, sizeof(bool) * node_names.size());
    memset(visited, 0, sizeof(bool) * node_names.size());

    return has_cycle;
}

// todo: sometimes it misses cycles
bool Minisat::MemoryModelSolver::cycle_detection()
{
    for(int kind = 0; kind < int(kind_arities.size()); kind++)
        if(axioms[kind] == ACYCLIC)
        {
            RETURN_IF_TRUE(cycle_detection(kind));
        }
    return false;
}

void Minisat::MemoryModelSolver::push_scope()
{
    mm_trail_lim.push_back(mm_trail.size());
    mm_dangerous_trail_lim.push_back(mm_dangerous_trail.size());
}

void Minisat::MemoryModelSolver::pop_scope(int new_level)
{
    for (int c = mm_trail.size() - 1; c >= mm_trail_lim[new_level]; c--)
    {
        auto t = mm_trail[c];
        if(t.arity == 1)
            unary_sets[t.kind].unset(t.node1);
        else if(t.arity == 2)
            binary_sets[t.kind].unset(t.node1, t.node2);
    }
    mm_trail.resize(mm_trail_lim[new_level]);
    mm_trail_lim.resize(new_level);

    for (int c = mm_dangerous_trail.size() - 1; c >= mm_dangerous_trail_lim[new_level]; c--)
    {
        auto t = mm_dangerous_trail[c];
        if(t.arity == 1)
            dangerous_unary_sets[t.kind].unset(t.node1);
        else if(t.arity == 2)
            dangerous_binary_sets[t.kind].unset(t.node1, t.node2);
    }
    mm_dangerous_trail.resize(mm_dangerous_trail_lim[new_level]);
    mm_dangerous_trail_lim.resize(new_level);
}

bool Minisat::MemoryModelSolver::use_available_info()
{
    for(int kind = 0; kind < int(kind_arities.size()); kind++)
    {
        if(kind_arities[kind] == 2 && axioms[kind] == rel_axiomt::EMPTY)
        {
            for(int node1 = 0; node1 < int(node_names.size()); node1++)
                for(int node2 = 0; node2 < int(node_names.size()); node2++)
                    RETURN_IF_TRUE(add_dangerous_binary_element(node1, node2, kind, mm_empty_reason))
        }
        if(kind_arities[kind] == 1 && axioms[kind] == rel_axiomt::EMPTY)
        {
            for(int node = 0; node < int(node_names.size()); node++)
                RETURN_IF_TRUE(add_dangerous_unary_element(node, kind, mm_empty_reason))
        }
        if(axioms[kind] == rel_axiomt::IRREFLEXIVE)
        {
            for(int node = 0; node < int(node_names.size()); node++)
                RETURN_IF_TRUE(add_dangerous_binary_element(node, node, kind, mm_empty_reason))
        }
    }

    for(auto it = lit_to_unary_elements.begin(); it != lit_to_unary_elements.end();)
    {
        auto& lit = it->first;
        auto& unary_element = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << unary_element.node << " " << unary_element.kind << " should be initially added\n";
            RETURN_IF_TRUE(add_unary_element(unary_element))

            it = lit_to_unary_elements.erase(it);
        }
        else
            it++;
    }

    for(auto it = lit_to_binary_elements.begin(); it != lit_to_binary_elements.end();)
    {
        auto& lit = it->first;
        auto& binary_element = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << binary_element.node1 << " " << binary_element.node2 << " " << binary_element.kind << " should be initially added\n";
            RETURN_IF_TRUE(add_binary_element(binary_element))

            it = lit_to_binary_elements.erase(it);
        }
        else
            it++;
    }

    RETURN_IF_TRUE(cycle_detection())

    apply_literal_assignment();

    return false;
}

void MemoryModelSolver::show_model()
{
    for(int id = 0; id < int(kind_id2str.size()); id++)
    {
        if(kind_arities[id] == 1)
        {
            std::cout << kind_id2str[id] << " is unary, with the following elements:\n\t";
            auto& unary_set = unary_sets[id];
            for(auto& elem : unary_set.all_elements())
                std::cout << node_names[elem] << " ";
            std::cout << "\n";
        }
        if(kind_arities[id] == 2)
        {
            std::cout << kind_id2str[id] << " is binary, with the following elements:\n";
            auto& binary_set = binary_sets[id];
            for(int node1 = 0; node1 < int(binary_set.size()); node1++)
            {
                auto& out = binary_set.get_outs(node1);
                if(out.empty())
                    continue;
                std::cout << "\t";
                for(auto& node2 : out)
                    std::cout << "(" << node_names[node1] << ", " << node_names[node2] << ") ";
                std::cout << "\n";
            }
        }
    }
}

void MemoryModelSolver::assign_literal(Lit l, literal_vector& lv)
{
    literals_to_assign.push_back(std::make_pair(l, lv));
}

void MemoryModelSolver::apply_literal_assignment()
{
    for(auto p : literals_to_assign)
    {
        if(OC_VERBOSITY >= 1)
        {
            std::cout << var(p.first) << "(" << sign(p.first) << ") is now assigned\n";
            std::cout << "its value was " << toInt(value(p.first)) << "\n";
        }

        auto& raw_learnt_clause = p.second;

        vec<Lit> learnt_clause;

//      for(auto lit: raw_learnt_clause)
//          learnt_clause.push(lit);

        learnt_clause.push(p.first);

        for(auto lit: raw_learnt_clause)
            if(lit != p.first)
                learnt_clause.push(lit);


        if (learnt_clause.size() == 1)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << learnt_clause[0].x << " is really assigned\n";
            uncheckedEnqueue(learnt_clause[0]);
            theory_propagation++;
        }
        else
        {
            CRef cr = ca.alloc(learnt_clause, true);
            learnts.push(cr);
            attachClause(cr);
            claBumpActivity(ca[cr]);
            if(value(learnt_clause[0]) == l_Undef)
            {
                if(OC_VERBOSITY >= 1)
                    std::cout << learnt_clause[0].x << " is really assigned\n";

                uncheckedEnqueue(learnt_clause[0], cr);
                theory_propagation++;
            }
        }

    }

    literals_to_assign.clear();
}

// __SZH_ADD_END__