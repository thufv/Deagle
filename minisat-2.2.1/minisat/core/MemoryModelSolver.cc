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

#define RETURN_IF_TRUE(statement) {if(statement) return true;}

#define get_assignment(l) ((l).x == -1 ? l_True : value(l))

#define stl_contains(container, key) ((container).find(key) != (container).end())

#define output_lv(lv) {for (auto l: (lv)) std::cout << var(l) << "(" << sign(l) << ") ";}

const int MM_VERBOSITY = 0;

using namespace Minisat;

mm_reasont empty_reason;

MemoryModelSolver::MemoryModelSolver()
{
    Solver();
}

void MemoryModelSolver::save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_label_tablet& _oc_label_table, cat_modulet& _cat_module)
{
    oc_edge_table = _oc_edge_table;
    oc_label_table = _oc_label_table;
    raw_cat_module = _cat_module;
    set_nodes_and_kinds();
    set_may_sets();
    set_graph();
    set_neg_graph();
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

    labels_to_lit.push_back(std::map<int, Lit>());
    neglabels_to_lit.push_back(std::map<int, Lit>());
    edges_to_lit.push_back(std::map<std::pair<int, int>, Lit>());
    negedges_to_lit.push_back(std::map<std::pair<int, int>, Lit>());

    if(stl_contains(raw_cat_module.negative_relations, kind))
    {
        std::cout << kind << " needs negation\n";
        need_negation.push_back(true);
    }
    else
        need_negation.push_back(false);

    if(is_base)
        base_kinds.insert(kind_id);

    if(MM_VERBOSITY >= 1)
        std::cout << kind << " is represented with " << kind_id << ", arity: " << arity << "\n";

    if(arity == 1)
    {
        unary_sets.resize(kind_id + 1);
        neg_unary_sets.resize(kind_id + 1);
        dangerous_unary_sets.resize(kind_id + 1);
        unary_sets[kind_id] = unary_sett(node_num());
        neg_unary_sets[kind_id] = unary_sett(node_num());
        dangerous_unary_sets[kind_id] = unary_sett(node_num());
    }
    else if(arity == 2)
    {
        binary_sets.resize(kind_id + 1);
        neg_binary_sets.resize(kind_id + 1);
        dangerous_binary_sets.resize(kind_id + 1);
        neg_seq_components.resize(kind_id + 1);
        binary_sets[kind_id] = binary_sett(node_num());
        neg_binary_sets[kind_id] = binary_sett(node_num());
        dangerous_binary_sets[kind_id] = binary_sett(node_num());
        neg_seq_components[kind_id] = binary_sett(node_num());
    }
    
    propagate_forward.push_back(std::vector<mm_edget>());
    propagate_backward.push_back(std::vector<mm_edget>());
    axioms.push_back(raw_cat_module.get_axiom(kind));

    auto kind_it = raw_cat_module.propagate_forward.find(kind);
    if(kind_it == raw_cat_module.propagate_forward.end())
        return;
    auto propagations = kind_it->second;
    
    for(auto& propagation: propagations)
    {
        std::string to = propagation.to;
        build_sets(raw_cat_module.get_arity(to), to, false);

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
        mm_edget new_edge(from_id, new_link, to_id);
        propagate_forward[from_id].push_back(new_edge);
        propagate_backward[to_id].push_back(new_edge);

        if(MM_VERBOSITY >= 1)
        {
            std::cout << "propagation from " << kind << "(" << from_id << ") to " << to << "(" << to_id << ")\n";
            std::cout << "another relation is " << another_kind << "(" << new_edge.link.another_kind << ")\n";
        }
    }
}

void MemoryModelSolver::set_nodes_and_kinds()
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
    for (auto& unary_kind : raw_cat_module.unary_relations)
        build_sets(1, unary_kind, true);
    for (auto& binary_kind : raw_cat_module.binary_relations)
        build_sets(2, binary_kind, true);
}

void MemoryModelSolver::set_may_sets()
{
    for(auto& pair : raw_cat_module.unary_mays)
    {
        auto kind_str = pair.first;
        auto kind_id = kind_str2id[kind_str];

        if(int(unary_may_sets.size()) <= kind_id)
            unary_may_sets.resize(kind_id + 1);
        unary_may_sets[kind_id] = unary_may_sett(node_num());
        auto& unary_may_set = unary_may_sets[kind_id];

        auto& raw_may_set = pair.second;
        for(auto& node_str : raw_may_set)
            unary_may_set.set(get_node(node_str));
    }

    for(auto& pair : raw_cat_module.binary_mays)
    {
        auto kind_str = pair.first;
        auto kind_id = kind_str2id[kind_str];

        if(int(binary_may_sets.size()) <= kind_id)
            binary_may_sets.resize(kind_id + 1);
        binary_may_sets[kind_id] = binary_may_sett(node_num());
        auto& binary_may_set = binary_may_sets[kind_id];

        auto& raw_may_set = pair.second;
        for(auto& node_pair : raw_may_set)
            binary_may_set.set(get_node(node_pair.first), get_node(node_pair.second));
    }

    // build neg_seq_components
    for(int kind = 0; kind < int(propagate_forward.size()); kind++)
        for(auto& propagation: propagate_forward[kind])
        {
            if(propagation.link.link_type != rel_opt::SEQ || propagation.link.link_position != link_post::LEFT)
                continue;

            auto another_kind = propagation.link.another_kind;
            auto next_kind = propagation.to;

            if(!need_negation[next_kind])
                continue;

            if(int(seq_derivation_num.size()) <= next_kind)
                seq_derivation_num.resize(next_kind + 1);
            if(seq_derivation_num[next_kind].empty())
            {
                seq_derivation_num[next_kind].resize(node_num());
                for(auto& row : seq_derivation_num[next_kind])
                    row.resize(node_num());
            }
            if(int(left_may_seq.size()) <= kind)
                left_may_seq.resize(kind + 1);
            if(left_may_seq[kind].empty())
            {
                left_may_seq[kind].resize(node_num());
                for(auto& row : left_may_seq[kind])
                    row.resize(node_num());
            }

            if(int(right_may_seq.size()) <= another_kind)
                right_may_seq.resize(another_kind + 1);
            if(right_may_seq[another_kind].empty())
            {
                right_may_seq[another_kind].resize(node_num());
                for(auto& row : right_may_seq[another_kind])
                    row.resize(node_num());
            }

            for(auto pair : binary_may_sets[kind].get_elements())
            {
                for(auto another_pair : binary_may_sets[another_kind].get_elements())
                {
                    if(pair.second != another_pair.first)
                        continue;
                    auto node1 = pair.first;
                    auto node2 = pair.second; // aka another_pair.first
                    auto node3 = another_pair.second;

                    seq_derivation_num[next_kind][node1][node3]++;
                    left_may_seq[kind][node1][node2].push_back(node3);
                    right_may_seq[another_kind][node2][node3].push_back(node1);
                }
            }
        }
}

void MemoryModelSolver::set_graph()
{
    // apply_literal_assignment();

    for(auto pair: oc_label_table)
    {
        if(pair.first == "")
            continue;

        int node = get_node(pair.first);
        Lit& l = pair.second.first;
        int kind = kind_str2id[pair.second.second];

        if(l.x != -1) // reasonable
            init_label(node, kind, l);
        else
        {
            labels_to_lit[kind][node] = l;
            if(add_label(node, kind, empty_reason)) //todo
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
            init_edge(node1, node2, kind, l);
        else
        {
            edges_to_lit[kind][std::make_pair(node1, node2)] = l;
            if(add_edge(node1, node2, kind, empty_reason)) //todo
            {
                std::cout << "WARNING: Inconsistency during set_graph\n";
                ok = false;
            }
        }
    }
}

void MemoryModelSolver::set_neg_graph()
{
    for(int kind = 0; kind < int(labels_to_lit.size()); kind++)
    {
        if(!need_negation[kind])
            continue;

        auto& labels_to_lit_this_kind = labels_to_lit[kind];
        for(auto& pair : labels_to_lit_this_kind)
        {
            auto node = pair.first;
            auto lit = pair.second;
            if(lit.x != -1) // not always hold, sometimes negated
            {
                auto neg_lit = ~lit;
                init_neglabel(node, kind, neg_lit);
            }
        }
    }

    for(int kind = 0; kind < int(edges_to_lit.size()); kind++)
    {
        if(!need_negation[kind])
            continue;

        auto& edges_to_lit_this_kind = edges_to_lit[kind];
        for(auto& pair : edges_to_lit_this_kind)
        {
            auto node1 = pair.first.first;
            auto node2 = pair.first.second;
            auto lit = pair.second;
            if(lit.x != -1) // not always hold, sometimes negated
            {
                auto neg_lit = ~lit;
                init_negedge(node1, node2, kind, neg_lit);
            }
        }
    }
}

bool MemoryModelSolver::simplify()
{
    assert(decisionLevel() == 0);

    if (!ok || propagate() != CRef_Undef)
        return ok = false;

    if (nAssigns() == simpDB_assigns || (simpDB_props > 0))
        return true;

    // Remove satisfied clauses:
    removeSatisfied(learnts);
    if (remove_satisfied)        // Can be turned off.
        removeSatisfied(clauses);
    checkGarbage();
    rebuildOrderHeap();

    simpDB_assigns = nAssigns();
    simpDB_props   = clauses_literals + learnts_literals;   // (shouldn't depend on stats really, but it will do for now)

    return true;
}

lbool MemoryModelSolver::search(int nof_conflicts)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    //initialize nodes

    if(use_available_info(starts == 1))
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

        negation_check();

        // show model
        if(MM_VERBOSITY >= 1)
            show_model();
    }
    else if (status == l_False && conflict.size() == 0)
        ok = false;

    std::cout << "MemoryModelSolver finishes with " << conflicts << " conflicts, " << decisions << " decisions, and " << propagations << " propagations. ";
    std::cout << theory_propagation << " theory propagations and " << conflict_cycle << " cycles included.\n";

    cancelUntil(0);
    return status;
}

CRef MemoryModelSolver::propagate()
{
    literals_to_assign.clear();
    assigned_literals.clear();

    CRef confl = CRef_Undef;
    int num_props = 0;
    watches.cleanAll();

    std::vector<labelt> labels_to_add;
    std::vector<edget> edges_to_add;
    std::vector<labelt> neglabels_to_add;
    std::vector<edget> negedges_to_add;

    while (qhead < trail.size()){
        Lit            p   = trail[qhead++];     // 'p' is enqueued fact to propagate.
        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        //our method
        auto& correspond_label = get_correspond_label(p);
        labels_to_add.insert(labels_to_add.end(), correspond_label.begin(), correspond_label.end());
        auto& correspond_neglabel = get_correspond_neglabel(p);
        neglabels_to_add.insert(neglabels_to_add.end(), correspond_neglabel.begin(), correspond_neglabel.end());
        auto& correspond_edge = get_correspond_edge(p);
        edges_to_add.insert(edges_to_add.end(), correspond_edge.begin(), correspond_edge.end());
        auto& correspond_negedge = get_correspond_negedge(p);
        negedges_to_add.insert(negedges_to_add.end(), correspond_negedge.begin(), correspond_negedge.end());
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

    bool one_more_time = false;
    //our method
    if(confl == CRef_Undef)
    {
        bool cycle = false;

        for(auto& label: labels_to_add)
        {
            if(cycle)
                break;
            cycle = add_label(label);
        }

        for(auto& edge: edges_to_add)
        {
            if(cycle)
                break;
            cycle = add_edge(edge);
        }

        for(auto& neglabel: neglabels_to_add)
        {
            if(cycle)
                break;
            cycle = add_neglabel(neglabel);
        }

        for(auto& negedge: negedges_to_add)
        {
            if(cycle)
                break;
            cycle = add_negedge(negedge);
        }

        if(cycle)
        {
            conflict_cycle++;

            // sort and deduplicate
            std::set<Minisat::Lit> ls(conflict_clause.begin(), conflict_clause.end());
            conflict_clause.assign(ls.begin(), ls.end());
            
            if (MM_VERBOSITY >= 1)
            {
                std::cout << "found cycle! conflict clause is: \n";
                for (auto l: conflict_clause)
                    std::cout << pretty_lit(l) << " ";
                std::cout << "\n";
            }

            auto &lv = conflict_clause;

            vec<Lit> minisat_lv;
            for (auto l: lv)
                minisat_lv.push(~l);

            confl = ca.alloc(minisat_lv, true);
        }
        else if(!literals_to_assign.empty())
        {
            one_more_time = true;
            apply_literal_assignment();
        }
    }
    //our method ends

    propagations += num_props;
    simpDB_props -= num_props;

    if(one_more_time)
    {
        if(MM_VERBOSITY >= 1)
            std::cout << "one more time\n";
        assert(confl == CRef_Undef);
        return propagate();
    }

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

bool Minisat::MemoryModelSolver::add_label(int node, int kind, mm_reasont& reason)
{
    if(unary_sets[kind].contains(node)) // if this already exists
        return false;

    if(MM_VERBOSITY >= 1)
        std::cout << "adding " << kind_id2str[kind] << " label " << node << " " << "\n";

    if(axioms[kind] == EMPTY)
    {
        conflict_clause = reason.get_full_reason();
        return true;
    }

    auto& unary_set = unary_sets[kind];
    unary_set.set(node, reason);
    mm_reasont& this_reason = unary_set.get_reason(node);
    mm_trail.push_back(unary_trail(kind, node));

    for(auto& propagation: propagate_forward[kind])
    {
        auto& next_kind = propagation.to;
        auto& link = propagation.link;
        if(link.link_type == ALT)
        {
            RETURN_IF_TRUE(add_label(node, next_kind, reason))
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            if(another_unary_set.contains(node))
            {
                mm_reasont& another_reason = another_unary_set.get_reason(node);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_label(node, next_kind, next_reason));
            }
        }
        else if(link.link_type == SUB && link.link_position == LEFT)
        {
            auto& another_kind = link.another_kind;
            auto& another_neg_unary_set = neg_unary_sets[another_kind];
            auto& another_unary_may_set = unary_may_sets[another_kind];
            if(another_neg_unary_set.contains(node) || !another_unary_may_set.contains(node))
            {
                mm_reasont& another_reason = another_neg_unary_set.get_reason(node);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_label(node, next_kind, next_reason))
            }
        }
        else if(link.link_type == SUB && link.link_position == RIGHT)
        {
            if(!need_negation[next_kind] || !unary_may_sets[next_kind].contains(node))
                continue;

            RETURN_IF_TRUE(add_neglabel(node, next_kind, reason))
        }
        else if(link.link_type == PROD)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            auto& another_all_elements = another_unary_set.all_elements();
            for(int another_element : another_all_elements)
            {
                mm_reasont& another_reason = another_unary_set.get_reason(another_element);
                mm_reasont next_reason(&this_reason, &another_reason);
                if(link.link_position == LEFT)
                    RETURN_IF_TRUE(add_edge(node, another_element, next_kind, next_reason))
                else
                    RETURN_IF_TRUE(add_edge(another_element, node, next_kind, next_reason))
            }
        }
        else if(link.link_type == BRACKET)
        {
            RETURN_IF_TRUE(add_edge(node, node, next_kind, this_reason))
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in label's propagation.\n";
        }
    }
    return false;
}

bool Minisat::MemoryModelSolver::add_neglabel(int node, int kind, mm_reasont& reason)
{
    if(!need_negation[kind])
        return false;

    if(neg_unary_sets[kind].contains(node)) // if this already exists
        return false;

    if(MM_VERBOSITY >= 1)
        std::cout << "adding " << kind_id2str[kind] << " negated label " << node << " " << "\n";

    auto& neg_unary_set = neg_unary_sets[kind];
    neg_unary_set.set(node, reason);
    mm_reasont& this_reason = neg_unary_set.get_reason(node);
    mm_trail.push_back(unary_trail(kind, node, false));

    if(axioms[kind] == NOT_EMPTY)
    {
        int existing_num = neg_unary_set.get_element_num();
        int maximal_num = unary_may_sets[kind].get_elements().size();
        if(existing_num >= maximal_num)
        {
            mm_reasont total_reason;
            for(int n = 0; n < node_num(); n++)
                total_reason.push_source_unless_trivial(&neg_unary_set.get_reason(n));
            conflict_clause = total_reason.get_full_reason();
            return true;
        }
    }

    for(auto& propagation: propagate_forward[kind])
    {
        auto& next_kind = propagation.to;
        auto& link = propagation.link;
        if(link.link_type == ALT)
        {
            if(!need_negation[next_kind] || !unary_may_sets[next_kind].contains(node))
                continue;

            auto& another_kind = link.another_kind;
            auto& another_neg_unary_set = neg_unary_sets[another_kind];
            auto& another_unary_may_set = unary_may_sets[another_kind];
            if(another_neg_unary_set.contains(node) || !another_unary_may_set.contains(node))
            {
                mm_reasont& another_reason = another_neg_unary_set.get_reason(node);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_neglabel(node, next_kind, next_reason))
            }
        }
        else if(link.link_type == AND)
        {
            if(!need_negation[next_kind] || !unary_may_sets[next_kind].contains(node))
                continue;
            RETURN_IF_TRUE(add_neglabel(node, next_kind, this_reason))
        }
        else if(link.link_type == SUB && link.link_position == LEFT)
        {
            if(!need_negation[next_kind] || !unary_may_sets[next_kind].contains(node))
                continue;
            RETURN_IF_TRUE(add_neglabel(node, next_kind, this_reason))
        }
        else if(link.link_type == SUB && link.link_position == RIGHT)
        {
            auto& another_kind = link.another_kind;
            auto& another_unary_set = unary_sets[another_kind];
            if(another_unary_set.contains(node))
            {
                mm_reasont& another_reason = another_unary_set.get_reason(node);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_label(node, next_kind, next_reason))
            }
        }
        else if(link.link_type == PROD)
        {
            if(!need_negation[next_kind])
                continue;

            for(int another_node = 0; another_node < node_num(); another_node++)
            {
                if(link.link_position == LEFT)
                {
                    if(!binary_may_sets[next_kind].contains(node, another_node))
                        continue;
                    RETURN_IF_TRUE(add_negedge(node, another_node, next_kind, this_reason))
                }
                else if(link.link_position == RIGHT)
                {
                    if(!binary_may_sets[next_kind].contains(node, another_node))
                        continue;
                    RETURN_IF_TRUE(add_negedge(another_node, node, next_kind, this_reason))
                }
            }
        }
        else if(link.link_type == BRACKET)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node, node))
                continue;
            RETURN_IF_TRUE(add_negedge(node, node, next_kind, this_reason));
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in negated label's propagation.\n";
        }
    }
    return false;
}

bool Minisat::MemoryModelSolver::add_edge(int node1, int node2, int kind, mm_reasont& reason)
{
    if(binary_sets[kind].contains(node1, node2)) // if this already exists
        return false;

    if(MM_VERBOSITY >= 1)
    {
        std::cout << "adding " << kind_id2str[kind] << " edge " << node1 << " " << node2 << " " << "\n";
        auto full_reason = reason.get_full_reason();
        if(!full_reason.empty())
        {
            std::cout << "\twith reason: \n\t";
            for (auto l: full_reason)
                std::cout << var(l) << "(" << sign(l) << ") ";
            std::cout << "\n";
        }
    }

    if(dangerous_binary_sets[kind].contains(node1, node2))
    {
        conflict_clause = reason.get_full_reason();
        auto dangerous_reason = dangerous_binary_sets[kind].get_reason(node1, node2).get_full_reason();
        append(conflict_clause, dangerous_reason);

        if(MM_VERBOSITY >= 1)
        {
            std::cout << "but dangerous edge " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is already there\n";
            // std::cout << "current model is:\n";
            // show_model();
        }
        
        return true;
    }

    auto& binary_set = binary_sets[kind];
    binary_set.set(node1, node2, reason);
    mm_reasont& this_reason = binary_set.get_reason(node1, node2);
    mm_trail.push_back(binary_trail(kind, node1, node2));

    for(auto& propagation: propagate_forward[kind])
    {
        auto& next_kind = propagation.to;
        auto& link = propagation.link;
        if(link.link_type == ALT)
        {
            RETURN_IF_TRUE(add_edge(node1, node2, next_kind, reason))
        }
        else if(link.link_type == FLIP)
        {
            RETURN_IF_TRUE(add_edge(node2, node1, next_kind, reason))
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
                    mm_reasont& another_reason = another_binary_set.get_reason(node2, node3_id);
                    mm_reasont next_reason(&this_reason, &another_reason);
                    RETURN_IF_TRUE(add_edge(node1, node3_id, next_kind, next_reason))
                }

                auto& node1_dangerous_outs = next_dangerous_binary_set.get_outs(node1);
                for(auto node3_id : node1_dangerous_outs)
                {
                    mm_reasont& dangerous_reason = next_dangerous_binary_set.get_reason(node1, node3_id);
                    mm_reasont next_reason(&this_reason, &dangerous_reason);
                    RETURN_IF_TRUE(add_dangerous_edge(node2, node3_id, another_kind, next_reason))
                }
            }
            if(link.link_position == RIGHT)
            {
                auto& node1_ins = another_binary_set.get_ins(node1);
                for(auto node0_id : node1_ins)
                {
                    mm_reasont& another_reason = another_binary_set.get_reason(node0_id, node1);
                    mm_reasont next_reason(&this_reason, &another_reason);
                    RETURN_IF_TRUE(add_edge(node0_id, node2, next_kind, next_reason))
                }

                auto& node2_dangerous_ins = next_dangerous_binary_set.get_ins(node2);
                for(auto node0_id : node2_dangerous_ins)
                {
                    mm_reasont& dangerous_reason = next_dangerous_binary_set.get_reason(node0_id, node2);
                    mm_reasont next_reason(&this_reason, &dangerous_reason);
                    RETURN_IF_TRUE(add_dangerous_edge(node0_id, node1, another_kind, next_reason))
                }
            }
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_set = binary_sets[another_kind];
            if(another_binary_set.contains(node1, node2))
            {
                mm_reasont& another_reason = another_binary_set.get_reason(node1, node2);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_edge(node1, node2, next_kind, next_reason))
            }
        }
        else if(link.link_type == SUB && link.link_position == LEFT)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_may_set = binary_may_sets[another_kind];
            auto& another_neg_binary_set = neg_binary_sets[another_kind];
            if(another_neg_binary_set.contains(node1, node2) || !another_binary_may_set.contains(node1, node2))
            {
                mm_reasont& another_reason = another_neg_binary_set.get_reason(node1, node2);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_edge(node1, node2, next_kind, next_reason))
            }
        }
        else if(link.link_type == SUB && link.link_position == RIGHT)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node1, node2))
                continue;
            RETURN_IF_TRUE(add_negedge(node1, node2, next_kind, this_reason))
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in edge's forward propagation.\n";
        }
    }

    return false;
}

bool Minisat::MemoryModelSolver::add_negedge(int node1, int node2, int kind, mm_reasont& reason)
{
    if(!need_negation[kind])
        return false;

    if(neg_binary_sets[kind].contains(node1, node2) || !binary_may_sets[kind].contains(node1, node2)) // if this already exists
        return false;

    if(MM_VERBOSITY >= 1)
    {
        std::cout << "adding " << kind_id2str[kind] << " negated edge " << node1 << " " << node2 << " " << "\n";
        auto full_reason = reason.get_full_reason();
        if(!full_reason.empty())
        {
            std::cout << "\twith reason: \n\t";
            for (auto l: full_reason)
                std::cout << var(l) << "(" << sign(l) << ") ";
            std::cout << "\n";
        }
    }

    auto& neg_binary_set = neg_binary_sets[kind];
    neg_binary_set.set(node1, node2, reason);
    mm_reasont& this_reason = neg_binary_set.get_reason(node1, node2);
    mm_trail.push_back(binary_trail(kind, node1, node2, false));

    if(axioms[kind] == NOT_EMPTY)
    {
        int existing_num = neg_binary_set.get_element_num();
        int maximal_num = binary_may_sets[kind].get_elements().size();
        if(existing_num >= maximal_num)
        {
            mm_reasont total_reason;
            for(int n1 = 0; n1 < node_num(); n1++)
                for(int n2 = 0; n2 < node_num(); n2++)
                    total_reason.push_source_unless_trivial(&neg_binary_set.get_reason(n1, n2));
            conflict_clause = total_reason.get_full_reason();
            return true;
        }
    }

    std::vector<edget> new_neq_seq_edges;

    for(auto& propagation: propagate_forward[kind])
    {
        auto& next_kind = propagation.to;
        auto& link = propagation.link;

        if(link.link_type == ALT)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node1, node2))
                continue;

            auto& another_kind = link.another_kind;
            auto& another_neg_binary_set = neg_binary_sets[another_kind];
            auto& another_binary_may_set = binary_may_sets[another_kind];
            if(another_neg_binary_set.contains(node1, node2) || !another_binary_may_set.contains(node1, node2))
            {
                mm_reasont& another_reason = another_neg_binary_set.get_reason(node1, node2);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_negedge(node1, node2, next_kind, next_reason))
            }
        }
        else if(link.link_type == FLIP)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node2, node1))
                continue;

            RETURN_IF_TRUE(add_negedge(node2, node1, next_kind, this_reason))
        }
        else if(link.link_type == SEQ)
        {
            if(!need_negation[next_kind])
                continue;

            auto& another_kind = link.another_kind;
            auto& another_neg_binary_set = neg_binary_sets[another_kind];

            if(link.link_position == LEFT)
            {
                for(auto node3 : left_may_seq[kind][node1][node2])
                {
                    bool need_to_consider = !another_neg_binary_set.contains(node2, node3) || (kind == another_kind && node1 == node2 && node2 == node3);
                    if(!need_to_consider)
                        continue;
                    auto& neg_reason_set = neg_seq_components[next_kind].get_reason(node1, node3);
                    neg_reason_set.real_size++;
                    bool non_trivial = neg_reason_set.push_source_unless_trivial(&this_reason);
                    mm_neg_seq_trail.push_back(neg_seq_trailt(next_kind, node1, node3, non_trivial));

                    if(neg_reason_set.real_size >= seq_derivation_num[next_kind][node1][node3])
                    {
                        if(!binary_may_sets[next_kind].contains(node1, node3))
                            continue;
                        edget neg_seq_edge(node1, node3, next_kind, neg_reason_set);
                        new_neq_seq_edges.push_back(neg_seq_edge);
                    }
                }
            }
            else // right
            {
                for(auto node0 : right_may_seq[kind][node1][node2])
                {
                    bool need_to_consider = !another_neg_binary_set.contains(node0, node1) || (kind == another_kind && node0 == node1 && node1 == node2);
                    if(!need_to_consider)
                        continue;
                    auto& neg_reason_set = neg_seq_components[next_kind].get_reason(node0, node2);
                    neg_reason_set.real_size++;
                    bool non_trivial = neg_reason_set.push_source_unless_trivial(&this_reason);
                    mm_neg_seq_trail.push_back(neg_seq_trailt(next_kind, node0, node2, non_trivial));

                    if(neg_reason_set.real_size >= seq_derivation_num[next_kind][node0][node2])
                    {
                        if(!binary_may_sets[next_kind].contains(node0, node2))
                            continue;
                        edget neg_seq_edge(node0, node2, next_kind, neg_reason_set);
                        new_neq_seq_edges.push_back(neg_seq_edge);
                    }
                }
            }
        }
        else if(link.link_type == AND)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node1, node2))
                continue;

            RETURN_IF_TRUE(add_negedge(node1, node2, next_kind, this_reason))
        }
        else if(link.link_type == SUB && link.link_position == LEFT)
        {
            if(!need_negation[next_kind] || !binary_may_sets[next_kind].contains(node1, node2))
                continue;

            RETURN_IF_TRUE(add_negedge(node1, node2, next_kind, this_reason))
        }
        else if(link.link_type == SUB && link.link_position == RIGHT)
        {
            auto& another_kind = link.another_kind;
            auto& another_binary_set = binary_sets[another_kind];
            if(another_binary_set.contains(node1, node2))
            {
                mm_reasont& another_reason = another_binary_set.get_reason(node1, node2);
                mm_reasont next_reason(&this_reason, &another_reason);
                RETURN_IF_TRUE(add_edge(node1, node2, next_kind, next_reason))
            }
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in edge's forward propagation.\n";
        }
    }

    for(auto& neg_seq_edge : new_neq_seq_edges)
        RETURN_IF_TRUE(add_negedge(neg_seq_edge))

    return false;
}

bool Minisat::MemoryModelSolver::add_dangerous_label(int node, int kind, mm_reasont& reason)
{
    //todo
    return false;
}

bool Minisat::MemoryModelSolver::add_dangerous_edge(int node1, int node2, int kind, mm_reasont& reason)
{
    auto& dangerous_binary_set = dangerous_binary_sets[kind];
    auto& binary_set = binary_sets[kind];

    if(dangerous_binary_set.contains(node1, node2)) // if this already exists
        return false;

    if(MM_VERBOSITY >= 1)
    {
        std::cout << "adding dangerous edge " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << "\n";
        auto full_reason = reason.get_full_reason();
        if(!full_reason.empty())
        {
            std::cout << "\twith reason: \n\t";
            for (auto l: full_reason)
                std::cout << var(l) << "(" << sign(l) << ") ";
            std::cout << "\n";
        }
    }

    if(binary_set.contains(node1, node2))
    {
        conflict_clause = reason.get_full_reason();
        auto positive_reason = binary_set.get_reason(node1, node2).get_full_reason();
        append(conflict_clause, positive_reason);

        if(MM_VERBOSITY >= 1)
        {
            std::cout << "but edge " << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is already there\n";
            // std::cout << "current model is:\n";
            // show_model();
        }

        return true;
    }

    dangerous_binary_set.set(node1, node2, reason);
    mm_reasont& this_reason = dangerous_binary_set.get_reason(node1, node2);
    mm_dangerous_trail.push_back(binary_trail(kind, node1, node2));

    auto& edges_to_lit_this_kind = edges_to_lit[kind];
    auto it = edges_to_lit_this_kind.find(std::make_pair(node1, node2));
    if(it != edges_to_lit_this_kind.end())
    {
        auto prevented_lit = it->second;
        if(prevented_lit != lit_Error && get_assignment(prevented_lit) == l_Undef)
        {
            literal_vector unit_lv;
            for(auto l: reason.get_full_reason())
                unit_lv.push_back(~l);
            unit_lv.push_back(~prevented_lit);

            if(MM_VERBOSITY >= 1)
            {
                std::cout << node_names[node1] << "(" << node1 << ") " << node_names[node2] << "(" << node2 << ") " << kind_id2str[kind] << " is set to false\n";
                std::cout << "related to lit " << var(prevented_lit) << "(" << sign(prevented_lit) << ") \n";
            }

            assign_literal(~prevented_lit, unit_lv);
        }
        
    }

    for(auto& propagation: propagate_backward[kind])
    {
        auto& last_kind = propagation.from;
        auto& last_binary_set = binary_sets[last_kind];
        auto& link = propagation.link;
        if(link.link_type == ALT)
        {
            RETURN_IF_TRUE(add_dangerous_edge(node1, node2, last_kind, reason))
        }
        else if(link.link_type == FLIP)
        {
            RETURN_IF_TRUE(add_dangerous_edge(node2, node1, last_kind, reason))
        }
        else if(link.link_type == SEQ)
        {
            auto& another_kind = link.another_kind;
            if(link.link_position == LEFT)
            {
                auto& node1_outs = last_binary_set.get_outs(node1);
                for(auto node3_id : node1_outs)
                {
                    mm_reasont& last_reason = last_binary_set.get_reason(node3_id, node2);
                    mm_reasont next_reason(&last_reason, &this_reason);
                    RETURN_IF_TRUE(add_dangerous_edge(node3_id, node2, another_kind, next_reason))
                }
            }
            if(link.link_position == RIGHT)
            {
                auto& node2_ins = last_binary_set.get_ins(node2);
                for(auto node3_id : node2_ins)
                {
                    mm_reasont& last_reason = last_binary_set.get_reason(node1, node3_id);
                    mm_reasont next_reason(&last_reason, &this_reason);
                    RETURN_IF_TRUE(add_dangerous_edge(node1, node3_id, another_kind, next_reason))
                }
            }
        }
        else if(link.link_type == AND)
        {
            auto& another_kind = link.another_kind;
            if(last_binary_set.contains(node1, node2))
            {
                mm_reasont& last_reason = last_binary_set.get_reason(node1, node2);
                mm_reasont next_reason(&last_reason, &this_reason);
                RETURN_IF_TRUE(add_dangerous_edge(node1, node2, another_kind, next_reason))
            }
        }
        else if(link.link_type == SUB)
        {
            // todo
        }
        else if(link.link_type == PROD)
        {
            // todo
        }
        else if(link.link_type == BRACKET)
        {
            // todo
        }
        else
        {
            std::cout << "WARNING: link_type " << link.link_type << " should not be in edge's backward propagation.\n";
        }
    }

    return false;
}

void Minisat::MemoryModelSolver::push_scope()
{
    mm_trail_lim.push_back(mm_trail.size());
    mm_dangerous_trail_lim.push_back(mm_dangerous_trail.size());
    mm_neg_seq_trail_lim.push_back(mm_neg_seq_trail.size());
}

void Minisat::MemoryModelSolver::pop_scope(int new_level)
{
    for (int c = mm_trail.size() - 1; c >= mm_trail_lim[new_level]; c--)
    {
        auto t = mm_trail[c];
        if(t.arity == 1)
        {
            if(t.pos)
                unary_sets[t.kind].unset(t.node1);
            else
                neg_unary_sets[t.kind].unset(t.node1);
        }
        else if(t.arity == 2)
        {
            if(t.pos)
                binary_sets[t.kind].unset(t.node1, t.node2);
            else
                neg_binary_sets[t.kind].unset(t.node1, t.node2);
        }
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

    for(int c = mm_neg_seq_trail.size() - 1; c >= mm_neg_seq_trail_lim[new_level]; c--)
    {
        auto t = mm_neg_seq_trail[c];
        auto& reason = neg_seq_components[t.kind].get_reason(t.node1, t.node2);
        reason.real_size--;
        if(t.non_trivial)
            reason.pop_source();
    }
    mm_neg_seq_trail.resize(mm_neg_seq_trail_lim[new_level]);
    mm_neg_seq_trail_lim.resize(new_level);
}

bool Minisat::MemoryModelSolver::use_available_info(bool is_first)
{
    if(is_first)
    {
        for(int kind = 0; kind < int(kind_arities.size()); kind++)
            if(axioms[kind] == rel_axiomt::NOT_EMPTY)
            {
                int arity = kind_arities[kind];
                if((arity == 1 && unary_may_sets[kind].get_elements().empty()) || (arity == 2 && binary_may_sets[kind].get_elements().empty()))
                {
                    std::cout << "A not-empty relation's may set is empty! Never consistent!\n";
                    conflict_clause = literal_vector();
                    return true;
                }
            }

        for(int kind = 0; kind < int(kind_arities.size()); kind++)
        {
            if(kind_arities[kind] == 2 && axioms[kind] == rel_axiomt::EMPTY)
            {
                for(int node1 = 0; node1 < node_num(); node1++)
                    for(int node2 = 0; node2 < node_num(); node2++)
                        RETURN_IF_TRUE(add_dangerous_edge(node1, node2, kind, empty_reason))
            }
            if(kind_arities[kind] == 1 && axioms[kind] == rel_axiomt::EMPTY)
            {
                for(int node = 0; node < node_num(); node++)
                    RETURN_IF_TRUE(add_dangerous_label(node, kind, empty_reason))
            }
            if(axioms[kind] == rel_axiomt::IRREFLEXIVE)
            {
                for(int node = 0; node < node_num(); node++)
                    RETURN_IF_TRUE(add_dangerous_edge(node, node, kind, empty_reason))
            }
        }
    }

    for(auto it = lit_to_labels.begin(); it != lit_to_labels.end();)
    {
        auto& lit = it->first;
        auto& label = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(MM_VERBOSITY >= 1)
                std::cout << label.node << " " << label.kind << " should be initially added\n";
            RETURN_IF_TRUE(add_label(label))

            it = lit_to_labels.erase(it);
            continue;
        }
        else if(is_first)
            set_correspond_label(lit, label);

        it++;
    }

    for(auto it = lit_to_neglabels.begin(); it != lit_to_neglabels.end();)
    {
        auto& lit = it->first;
        auto& label = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(MM_VERBOSITY >= 1)
                std::cout << label.node << " " << label.kind << " (negation) should be initially added\n";
            RETURN_IF_TRUE(add_neglabel(label))

            it = lit_to_neglabels.erase(it);
            continue;
        }
        else if(is_first)
            set_correspond_neglabel(lit, label);

        it++;
    }

    for(auto it = lit_to_edges.begin(); it != lit_to_edges.end();)
    {
        auto& lit = it->first;
        auto& edge = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(MM_VERBOSITY >= 1)
                std::cout << edge.node1 << " " << edge.node2 << " " << edge.kind << " should be initially added\n";
            RETURN_IF_TRUE(add_edge(edge))

            it = lit_to_edges.erase(it);
            continue;
        }
        else if(is_first)
            set_correspond_edge(lit, edge);

        it++;
    }

    for(auto it = lit_to_negedges.begin(); it != lit_to_negedges.end();)
    {
        auto& lit = it->first;
        auto& edge = it->second;
        if(get_assignment(lit) == l_True)
        {
            if(MM_VERBOSITY >= 1)
                std::cout << edge.node1 << " " << edge.node2 << " " << edge.kind << " (negation) should be initially added\n";
            RETURN_IF_TRUE(add_negedge(edge))

            it = lit_to_negedges.erase(it);
            continue;
        }
        else if(is_first)
            set_correspond_negedge(lit, edge);

        it++;
    }

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
                std::cout << elem << " ";
            std::cout << "\n";

            if(need_negation[id])
            {
                std::cout << kind_id2str[id] << " is unary, with the following negated elements:\n\t";
                auto& unary_set = neg_unary_sets[id];
                for(auto& elem : unary_set.all_elements())
                    std::cout << elem << " ";
                std::cout << "\n";
            }
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
                    std::cout << "(" << node1 << ", " << node2 << ") ";
                std::cout << "\n";
            }

            if(need_negation[id])
            {
                std::cout << kind_id2str[id] << " is binary, with the following negated elements:\n";
                auto& binary_set = neg_binary_sets[id];
                for(int node1 = 0; node1 < int(binary_set.size()); node1++)
                {
                    auto& out = binary_set.get_outs(node1);
                    if(out.empty())
                        continue;
                    std::cout << "\t";
                    for(auto& node2 : out)
                        std::cout << "(" << node1 << ", " << node2 << ") ";
                    std::cout << "\n";
                }
            }
        }
    }
}

void MemoryModelSolver::negation_check()
{
    for(int id = 0; id < int(kind_id2str.size()); id++)
    {
        if(!need_negation[id])
            continue;

        std::cout << "Checking negation's validity: " << kind_id2str[id] << "\n";

        if(kind_arities[id] == 1)
        {
            for(int node = 0; node < node_num(); node++)
            {
                bool in_may_set = unary_may_sets[id].contains(node);
                bool exists = unary_sets[id].contains(node);
                bool neg_exists = neg_unary_sets[id].contains(node);
                if(exists && neg_exists)
                    std::cout << "\tWarning: " << node << " exists in both\n";
                if(in_may_set && !exists && !neg_exists)
                    std::cout << "\tWarning: " << node << " exists in neither, though it is in may_set\n";
                if(!in_may_set && exists)
                    std::cout << "\tWarning: " << node << "is not in may_set but exists positively\n";
                if(!in_may_set && neg_exists)
                    std::cout << "\tWarning: " << node << "is not in may_set but exists negatively\n";
            }
        }
        if(kind_arities[id] == 2)
        {
            for(int node1 = 0; node1 < node_num(); node1++)
                for(int node2 = 0; node2 < node_num(); node2++)
                {
                    bool in_may_set = binary_may_sets[id].contains(node1, node2);
                    bool exists = binary_sets[id].contains(node1, node2);
                    bool neg_exists = neg_binary_sets[id].contains(node1, node2);
                    if(exists && neg_exists)
                        std::cout << "\tWarning: " << node1 << " " << node2 << " exists in both\n";
                    if(in_may_set && !exists && !neg_exists)
                        std::cout << "\tWarning: " << node1 << " " << node2 << " exists in neither, though it is in may_set\n";
                    if(!in_may_set && exists)
                        std::cout << "\tWarning: " << node1 << " " << node2 << "is not in may_set but exists positively\n";
                    if(!in_may_set && neg_exists)
                        std::cout << "\tWarning: " << node1 << " " << node2 << "is not in may_set but exists negatively\n";
                }
        }
    }
}

void MemoryModelSolver::assign_literal(Lit l, literal_vector& lv)
{
    if(stl_contains(assigned_literals, l))
        return;

    literals_to_assign.push_back(std::make_pair(l, lv));
    assigned_literals.insert(l);
}

void MemoryModelSolver::apply_literal_assignment()
{
    for(auto p : literals_to_assign)
    {
        if(MM_VERBOSITY >= 1)
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
            if(MM_VERBOSITY >= 1)
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
                if(MM_VERBOSITY >= 1)
                {
                    std::cout << learnt_clause[0].x << " is really assigned\n";

                    std::cout << "theory propagation:\n";
                    for (int i = 0; i < int(learnt_clause.size()); i++)
                        std::cout << pretty_lit(~learnt_clause[i]) << " ";
                    std::cout << "\n";
                }

                uncheckedEnqueue(learnt_clause[0], cr);
                theory_propagation++;
            }
        }

    }

    literals_to_assign.clear();
    assigned_literals.clear();
}

std::string MemoryModelSolver::pretty_lit(Minisat::Lit l)
{
    auto& correspond_label = get_correspond_label(l);
    if(!correspond_label.empty())
    {
        auto& label = correspond_label[0];
        return "(" + kind_id2str[label.kind] + " " + node_names[label.node] + ")";
    }
    auto& correspond_edge = get_correspond_edge(l);
    if(!correspond_edge.empty())
    {
        auto& edge = correspond_edge[0];
        return "(" + kind_id2str[edge.kind] + " " + node_names[edge.node1] + " " + node_names[edge.node2] + ")";
    }
    return "unknown";
}

// __SZH_ADD_END__