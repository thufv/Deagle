// __SZH_ADD_BEGIN__

#include "ClosureSolver.h"
#include "GraphTypes.h"

#include "../../minisat/mtl/Vec.h"
#include "../../minisat/mtl/Heap.h"
#include "../../minisat/mtl/Alg.h"
#include "../../minisat/core/SolverTypes.h"
#include "../../minisat/mtl/Sort.h"

const int OC_VERBOSITY = 0;

using namespace Minisat;

literal_vector closure_empty_reason;

inline bool oc_reasonable(edge_kindt kind) //for initially existing edges
{
    return kind == OC_RF;
}

ClosureSolver::ClosureSolver()
{
    conflict_cycle = 0;
    theory_propagation = 0;
    Solver();
}

void ClosureSolver::init()
{
    graph.init(this);
    set_graph();
}

void ClosureSolver::save_raw_graph(oc_edge_tablet& _oc_edge_table, oc_guard_mapt& _oc_guard_map, oc_location_mapt& _oc_location_map, std::map<std::string, int>& _oc_result_order)
{
    oc_edge_table = _oc_edge_table;
    oc_guard_map = _oc_guard_map;
    oc_location_map = _oc_location_map;
    oc_result_order = &_oc_result_order;

    init();
}

void ClosureSolver::set_graph()
{
    for(auto pair: oc_edge_table) //init nodes first
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        graph.get_node(pair.first.first);
        graph.get_node(pair.first.second);
    }

    // for(auto pair: oc_guard_map)
    // {
    //     auto write_name = pair.first;
    //     graph.get_node(write_name);
    // }

    for(auto pair: oc_location_map)
    {
        auto write_name = pair.first;
        auto write_location = pair.second;
        int id = graph.get_node(write_name);
        graph.nodes[id].location = write_location;

        // std::cout << "write " << id << "(" << write_name << ")'s location is " << write_location << "\n";
    }

    for(auto pair: oc_guard_map)
    {
        auto write_name = pair.first;

        int u = graph.get_node(write_name);
        auto& guard_literal = pair.second;

        if(graph.add_guard_literal(guard_literal, u))
        {
            std::cout << "WARNING: find cycle during set_graph\n";
            ok = false;
        }

        if(OC_VERBOSITY >= 1)
            std::cout << "guard of " << u << "(" << write_name << ") is " << var(guard_literal) << "(" << sign(guard_literal) << ")\n";
    }

    for(auto pair: oc_edge_table) //race first 
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int u = graph.get_node(pair.first.first);
        int v = graph.get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind != OC_RACE)
            continue;

        Lit& l = pair.second.first;
        graph.init_race(u, v, l);
    }

    for(auto pair: oc_edge_table) //atomic po second
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

    graph.atomic_remove_self();

    for(auto pair: oc_edge_table) // others last
    {
        if(pair.first.first == "" || pair.first.second == "")
            continue;

        int u = graph.get_node(pair.first.first);
        int v = graph.get_node(pair.first.second);
        edge_kindt kind = str_to_kind(pair.second.second);

        if(kind == OC_RACE || kind == OC_APO)
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
            if(graph.activate_edge(u, v, kind, closure_empty_reason))
            {
                std::cout << "WARNING: Find cycle during set_graph\n";
                ok = false;
            }
        }
    }

    if(OC_VERBOSITY >= 1)
        std::cout << "apply literal assignment after setting graph\n";

    apply_literal_assignment();

    //sort atomic_items by write and read
    graph.refine_atomic_atoms();

    if(OC_VERBOSITY >= 1) //show apo info
    {
        for(int i = 0; i < int(graph.nodes.size()); i++)
        {
            auto& node = graph.nodes[i];
            std::cout << i << "'s representative is " << graph.union_find_parent(i) << ", has atomic_in: ";
            for(auto in: node.atomic_in)
                std::cout << in << " ";
            std::cout << " and atomic_out: ";
            for(auto out: node.atomic_out)
                std::cout << out << " ";
            std::cout << "\n";

            if(graph.union_find_parent(i) == i)
            {
                std::cout << i << " is a representative with atomic_items: ";
                for(auto item: node.atomic_items)
                    std::cout << item << " ";
                std::cout << "\n";

                std::cout << "including atomic_reads: ";
                for(auto item: node.atomic_reads)
                    std::cout << item << " ";
                std::cout << "\n";

                std::cout << "and atomic_writes: ";
                for(auto item: node.atomic_writes)
                    std::cout << item << " ";
                std::cout << "\n";
            }
        }
    }
}

lbool ClosureSolver::search(int nof_conflicts)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    //initialize nodes

    if(graph.use_available_info())
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

lbool ClosureSolver::solve_()
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

        graph.final_check();
        graph.show_rf();
        graph.show_model();
    }
    else if (status == l_False && conflict.size() == 0)
        ok = false;

    std::cout << "ClosureSolver finishes with " << conflicts << " conflicts, " << decisions << " decisions, and " << propagations << " propagations. " << theory_propagation << " theory propagations and " << conflict_cycle << " cycles included.\n";

    cancelUntil(0);
    return status;
}

CRef ClosureSolver::propagate()
{
    literals_to_assign.clear();
    assigned_literals.clear();

    CRef confl = CRef_Undef;
    int num_props = 0;
    watches.cleanAll();

    std::vector<closure_edget> edges_to_add;
    std::vector<int> guards_to_light;

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

            literal_vector reason(1, p);
            edges_to_add.push_back(closure_edget(decide_entry.first.first, decide_entry.first.second, decide_entry.second,reason));
        }

        auto guard_nodes = graph.check_guard_literal(p);
        for(auto guard_node: guard_nodes)
        {
            if(OC_VERBOSITY >= 1)
                std::cout << var(p) << "(" << sign(p) << ") is related to a guard of " << guard_node << "\n";

            guards_to_light.push_back(guard_node);
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
                graph.remove_rf(edge.from, edge.to, edge.reason[0]);
            }

            if(cycle)
                break;
        }

        if(!cycle)
        {
            for(auto& n: guards_to_light)
            {
                cycle = graph.light_guard(n);
                if(cycle)
                    break;
            }
        }

        if(cycle)
        {
            graph.triplets_to_check.clear();

            conflict_cycle++;
            if (OC_VERBOSITY >= 1)
            {
                std::cout << "found cycle! conflict clause is: \n";
                for (auto l: graph.conflict_lv)
                    std::cout << var(l) << "(" << sign(l) << ") ";
                std::cout << "\n";
            }

            auto &lv = graph.conflict_lv;

            vec<Lit> minisat_lv;
            for (auto l: lv)
                minisat_lv.push(~l);

            confl = ca.alloc(minisat_lv, true);
        }
        else if(!literals_to_assign.empty())//add literals propagated by unit edge
        {
            one_more_time = true;
            apply_literal_assignment();
        }

        literals_to_assign.clear();
        assigned_literals.clear();
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

void ClosureSolver::cancelUntil(int level)
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

void ClosureSolver::newDecisionLevel()
{
    trail_lim.push(trail.size());
    graph.push_scope();
}

void ClosureSolver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel)
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

void ClosureSolver::show_rf()
{
    graph.show_rf();
}

void ClosureSolver::assign_literal(Lit l, literal_set& lv)
{
    if(assigned_literals.find(l) != assigned_literals.end())
        return;

    literals_to_assign.push_back(std::make_pair(l, lv));
    assigned_literals.insert(l);
}

void ClosureSolver::assign_literal(Lit l, literal_vector& lv)
{
    if(assigned_literals.find(l) != assigned_literals.end())
        return;

    literal_set ls(lv.begin(), lv.end());
    assign_literal(l, ls);
}

void ClosureSolver::apply_literal_assignment()
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
    assigned_literals.clear();
}

extern std::vector<std::string> write_order;

std::vector<std::string> ClosureSolver::get_write_order()
{
    return write_order;
}

void ClosureSolver::addOC(std::string from, std::string to, std::string type, Lit related_lit)
{
    oc_edge_table.push_back(std::make_pair(std::make_pair(from, to), std::make_pair(related_lit, type)));
}

void ClosureSolver::addGuard(std::string node, Lit guard)
{
    oc_guard_map[node] = guard;
}

void ClosureSolver::setRawGraph(oc_edge_tablet& _oc_edge_table, oc_guard_mapt& _oc_guard_map)
{
    oc_edge_table = _oc_edge_table;
    oc_guard_map = _oc_guard_map;
}

// __SZH_ADD_END__