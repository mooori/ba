#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "PrePro1.hpp"
#include "../types/dstypes.hpp"
#include "../utility/setops.hpp"

PrePro1::PrePro1(DSGraph& dsg)
        :dsg(dsg)
        { }

void PrePro1::run() {
    this->init();
    this->finish();
    return;
}

std::vector< std::set<BVertex> > PrePro1::get_n_is(const BVertex v) const {
    std::set<BVertex> nv, not_n1, n1, n2, n3;
    nv = dsg.get_adj_BVertices(v);

    // Get N1: for each u in nv: if u has neighbour not in N[v], add u to N1
    for(std::set<BVertex>::iterator it = nv.begin(); it != nv.end(); ++it) {
        BVertex u = *it;
        std::set<BVertex> nu = dsg.get_adj_BVertices(u);
        std::set<BVertex> nu_red = setops::setminus_new(nu, nv);
        nu_red.erase(v);
        if(!nu_red.empty()) { n1.insert(u); }
        else { not_n1.insert(u); }
    }

    // Get N2, N3: for each u in not_n1
    // - if u has neighbour in N1: n2.insert(u)
    // - else n3.insert(u)
    for(std::set<BVertex>::iterator it = not_n1.begin(); it != not_n1.end();
            ++it) {
        BVertex u = *it;
        std::set<BVertex> nu = dsg.get_adj_BVertices(u);
        if(!setops::inters_new(nu, n1).empty()) { n2.insert(u); }
        else { n3.insert(u); }
    }

    return std::vector< std::set<BVertex> >{ n1, n2, n3 };
}

void PrePro1::init() {
    // for v in V(G)
    //     - calc v's n_is and store them in this->m
    //     - if n3 != {}, add v to pre_D
    for(std::pair<BVertex_it, BVertex_it> it = this->dsg.vertices();
            it.first != it.second; ++it.first) {
        BVertex bv = *it.first;
        std::vector< std::set<BVertex> > nis = this->get_n_is(bv);
        this->m[this->dsg.get_IVertex(bv)] = nis;
        if(!nis[2].empty()) { pre_D.insert(dsg.get_IVertex(bv)); }
    }
    return;
}

void PrePro1::finish() {
    // for each v in pre_D
    //     - if v no longer in dsg: continue
    //     - remove v from pre_H (what's in pre_D will be deleted)
    //     - remove n2(v), n3(v) of v from dsg, pre_H, pre_D
    //     - add N(v) to pre_H
    //     - remove v from dsg
    for(std::set<IVertex>::iterator d_it = pre_D.begin(); d_it != pre_D.end();
            ++d_it) {
        IVertex v = *d_it;
        this->pre_H.erase(v);
        if(!this->dsg.contains_IVertex(v)) { continue; }
        std::vector< std::set<BVertex> > nis = this->m[v];
        
        // remove n2(v), n3(v) from dsg
        this->remove_ni(nis[1]);
        this->remove_ni(nis[2]);

        // add n(v) (what is remaining of it after removing n2, n3) to pre_H
        std::set<IVertex> nv = this->dsg.get_adj_IVertices(v);
        this->pre_H.insert(nv.begin(), nv.end());

        // remove v fromd dsg
        dsg.remove_IVertex(v);
    }
    return;
}

void PrePro1::remove_ni(const std::set<BVertex>& ni) {
    for(std::set<BVertex>::iterator it = ni.begin(); it != ni.end(); ++it) {
        if(!this->dsg.contains_BVertex(*it)) { continue; }
        IVertex v = this->dsg.get_IVertex(*it);
        this->pre_H.erase(v);
        this->pre_D.erase(v);
        this->dsg.remove_IVertex(v);
    }
    return;
}
