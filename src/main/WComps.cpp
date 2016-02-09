#include <list>
#include <queue>

#include "DSGraph.hpp"
#include "WComps.hpp"
#include "../utility/setops.hpp"

WComps::WComps(const DSGraph& DSG, const std::set<BVertex>& W)
        :DSG(DSG)
        ,W(W)
        ,discovered(std::set<BVertex>())
        { }

std::list<DSGraph*>* WComps::get() {
    this->discovered = std::set<BVertex>();
    std::list<DSGraph*>* comps = new std::list<DSGraph*>();
    std::set<BVertex> W_todo = this->W;

    while(!W_todo.empty()) {
        std::queue<BVertex> Q;
        std::set<IVertex> nonW;
        Q.push(*W_todo.begin());
        discovered.insert(*W_todo.begin());
        W_todo.erase(W_todo.begin());
        DSGraph* C = new DSGraph();
        comps->push_back(C);
        C->add_IVertex(this->DSG.get_IVertex(Q.front()));

        while(!Q.empty()) {
            BVertex v_B = Q.front();
            Q.pop();
            W_todo.erase(v_B);
            if((this->W.find(v_B)) != this->W.end()) {
                this->expand_W(C, Q, W_todo, v_B);
            } else {
                nonW.insert(this->DSG.get_IVertex(v_B));
                this->expand_nonW(C, Q, W_todo, v_B);
            }
        }
        add_nonW_nonW_edges(C, nonW);
    }

    return comps;
}

void WComps::expand_W(DSGraph* C, std::queue<BVertex>& Q,
        std::set<BVertex>& W_todo, const BVertex v_B) {
    std::set<BVertex> n1 = this->DSG.get_adj_BVertices(v_B);
    IVertex v_I = this->DSG.get_IVertex(v_B);
    for(std::set<BVertex>::iterator it = n1.begin(); it != n1.end(); ++it) {
        BVertex u_B = *it;
        IVertex u_I = this->DSG.get_IVertex(u_B);
        if(!C->contains_IVertex(u_I)) { C->add_IVertex(u_I); }
        if(!C->in_adj_list_I(v_I, u_I)) { C->add_IEdge(IEdge(v_I, u_I)); }
        if((this->discovered.insert(u_B)).second) { Q.push(u_B); }
        W_todo.erase(u_B);
    }
    return;
}

void WComps::expand_nonW(DSGraph* C, std::queue<BVertex>& Q,
        std::set<BVertex>& W_todo, const BVertex v_B) {
    std::set<BVertex> n1 = this->DSG.get_adj_BVertices(v_B);
    std::set<BVertex> n1_cap_W = setops::inters_new(n1, this->W);
    IVertex v_I = this->DSG.get_IVertex(v_B);
    for(std::set<BVertex>::iterator it = n1_cap_W.begin();
            it != n1_cap_W.end(); ++it) {
        BVertex u_B = *it;
        IVertex u_I = this->DSG.get_IVertex(u_B);
        if(!C->contains_IVertex(u_I)) { C->add_IVertex(u_I); }
        if(!C->in_adj_list_I(v_I, u_I)) { C->add_IEdge(IEdge(v_I, u_I)); }
        if((this->discovered.insert(u_B)).second) { Q.push(u_B); }
        W_todo.erase(u_B);
    }
    return;
}

void WComps::delete_comps_ptrs(std::list<DSGraph*>* l) {
    for(std::list<DSGraph*>::iterator l_it = l->begin();
            l_it != l->end(); ++l_it) {
        delete *l_it;
    }
    delete l;
    return;
}

void WComps::add_nonW_nonW_edges(DSGraph* C, std::set<IVertex>& nonW) {
    // for each v in nonW: add edges to v's nonW neighbours in G \cap V(C)
    for(std::set<IVertex>::const_iterator nonW_it = nonW.begin();
            nonW_it != nonW.end(); ++nonW_it) {
        IVertex v = *nonW_it;
        std::set<IVertex> n1_G = this->DSG.get_adj_IVertices(v);
        std::set<IVertex> n1_nonW_C = setops::inters_new(n1_G, nonW);
        for(std::set<IVertex>::iterator n1_nonW_C_it = n1_nonW_C.begin();
                n1_nonW_C_it != n1_nonW_C.end(); ++n1_nonW_C_it) {
            IVertex u = *n1_nonW_C_it;
            if(!C->in_adj_list_I(v, u)) { C->add_IEdge(IEdge(v, u)); }
        }
    }
    return;
}
