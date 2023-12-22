// __SZH_ADD_BEGIN__

#include "ICDSolver.h"
#include "GraphTypes.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

const int OC_VERBOSITY = 0;

using namespace Minisat;

ICD_reasont ICD_empty_reason;

inline bool oc_reasonable(edge_kindt kind)
{
    return kind == OC_RF || kind == OC_WS || kind == OC_FR;
}

ICDSolver::ICDSolver()
{
    conflict_cycle = 0;
    Solver();
}

void ICDSolver::init()
{
    graph.init(this);
}

void ICDSolver::setRawGraph(oc_edge_tablet& _oc_edge_table)
{
    oc_edge_table = _oc_edge_table;
}

void ICDSolver::set_graph()
{
    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int u = graph.get_node(pair.first.first);
        int v = graph.get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind != OC_APO)
            continue;

        if(OC_VERBOSITY >= 1)
            std::cout << "initing " << u << "(" << pair.first.first << ") " << v << "(" << pair.first.second << ") " << kind_to_str(kind) << "\n";

        graph.activate_apo(u, v);
    }

    for(auto pair: oc_edge_table)
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int u = graph.get_node(pair.first.first);
        int v = graph.get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind == OC_APO)
            continue;

        if(OC_VERBOSITY >= 1)
            std::cout << "initing " << u << "(" << pair.first.first << ") " << v << "(" << pair.first.second << ") " << kind_to_str(kind) << "\n";

        if(oc_reasonable(kind))
        {
            Lit& l = pair.second.first;
            graph.init_reasonable_edge(u, v, kind, l);
        }
        else
        {
            if(graph.activate_edge(u, v, kind, ICD_empty_reason))
            {
                std::cout << "ERROR: find cycle during set_graph\n";
                ok = false;
            }
        }
    }

    if(OC_VERBOSITY >= 1) //show apo info
    {
        for(int i = 0; i < graph.nodes.size(); i++)
        {
            auto& node = graph.nodes[i];
            std::cout << i << "'s representative is " << graph.union_find_parent(i) << ", has atomic_in: ";
            for(auto in: node.atomic_in)
                std::cout << in << " ";
            std::cout << " and atomic_out: ";
            for(auto out: node.atomic_out)
                std::cout << out << " ";
            std::cout << "\n";
        }
    }
}

lbool ICDSolver::search(int nof_conflicts)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    //initialize nodes

    if(graph.use_available_info())
    {
        std::cout << "ERROR: use_available_info find cycle at the very start\n";
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
                    // Model found:
                    return l_True;
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

lbool ICDSolver::solve_()
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
    }else if (status == l_False && conflict.size() == 0)
        ok = false;

    graph.show_edges();

    std::cout << "ICDSolver finishes with " << conflicts << " conflicts, " << decisions << " decisions, and " << propagations << " propagations. " << conflict_cycle << " cycles included.\n";

    cancelUntil(0);
    return status;
}

CRef ICDSolver::propagate()
{
    literals_to_assign.clear();

    CRef confl = CRef_Undef;
    int num_props = 0;
    watches.cleanAll();

    std::vector<ICD_edget> edges_to_add;

    while (qhead < trail.size()){
        Lit            p   = trail[qhead++];     // 'p' is enqueued fact to propagate.
        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        //our method
        auto decide_entry = graph.get_decide_entry(p);
        if(decide_entry.first.first != -1)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << var(p) << "(" << sign(p) << ") is related to an edge (" << toInt(assigns[var(p)]) << ")\n";

            ICD_reasont reason(p);
            edges_to_add.push_back(ICD_edget(decide_entry.first.first, decide_entry.first.second, decide_entry.second, reason));
        }
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

        for(auto& edge: edges_to_add)
        {
            cycle = graph.activate_edge(edge.from, edge.to, edge.kind, edge.reason);

            if(decisionLevel() == 0 && edge.kind == OC_RF)
            {
                if(edge.reason.num != 1)
                    std::cout << "ERROR: the reason of an inactive_edge to remove has num " << edge.reason.num << "\n";
                
                graph.remove_inactive_edges(edge.from, edge.to, edge.reason.l1);
            }

            if(cycle)
                break;
        }

        if(cycle)
        {
            conflict_cycle++;

            auto conflict_lv = graph.conflict_literals_vector.back();
            graph.conflict_literals_vector.pop_back();

            if (OC_VERBOSITY >= 1)
            {
                std::cout << "found cycle! conflict clause is: \n";
                for (auto l: conflict_lv)
                    std::cout << var(l) << "(" << sign(l) << " aka " << l.x << ") ";
                std::cout << "\n";
            }

            auto &lv = conflict_lv;

            vec<Lit> minisat_lv;
            for (auto l: lv)
                minisat_lv.push(~l);

            confl = ca.alloc(minisat_lv, true);
        }
        else if(!literals_to_assign.empty())//add literals propagated by unit edge
        {
            one_more_time = true;
            for(auto p : literals_to_assign)
            {
                if(OC_VERBOSITY >= 1)
                {
                    std::cout << var(p.first) << "(" << sign(p.first) << ") is now assigned\n";
                    std::cout << "its value was " << toInt(value(p.first)) << "\n";
                }

                auto& raw_learnt_clause = p.second;

                vec<Lit> learnt_clause;

//                for(auto lit: raw_learnt_clause)
//                    learnt_clause.push(lit);

                learnt_clause.push(p.first);

                for(auto lit: raw_learnt_clause)
                    if(lit != p.first)
                        learnt_clause.push(lit);

                CRef cr = ca.alloc(learnt_clause, true);
                //learnts.push(cr);
                //attachClause(cr);
                //claBumpActivity(ca[cr]);
                if(value(learnt_clause[0]) == l_Undef)
                    uncheckedEnqueue(learnt_clause[0], cr);
            }
        }

        literals_to_assign.clear();
    }
    //our method ends

    propagations += num_props;
    simpDB_props -= num_props;

    if(one_more_time)
    {
        if(OC_VERBOSITY >= 1)
            std::cout << "one more time\n";
        assert(confl == CRef_Undef);
        return propagate();
    }

    return confl;
}

void ICDSolver::cancelUntil(int level)
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
        graph.pop_scope(level);
    }
}

void ICDSolver::newDecisionLevel()
{
    trail_lim.push(trail.size());
    graph.push_scope();
}

void ICDSolver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel)
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

void ICDSolver::assign_literal(Lit l, literal_set& lv)
{
    literals_to_assign.push_back(std::make_pair(l, lv));
}

void ICDSolver::assign_literal(Lit l, literal_vector& lv)
{
    literal_set ls;
    for(auto l : lv)
        ls.insert(l);
    literals_to_assign.push_back(std::make_pair(l, ls));
}

// __SZH_ADD_END__