#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "PrePro1.hpp"
#include "../types/dstypes.hpp"
#include "../utility/setops.hpp"

PrePro1::PrePro1(DSGraph& dsg)
        :dsg(dsg)
        { }

void PrePro1::apply() {
    // Got to iterate over vertices and delete vertices at same time.
    // So don't do it via dsg.vertices(). Not clear what happens if you
    // delete vertex lying "ahead" of current vertex.
    // Instead: get set of BVertices, iterate over it. At beginning of loop
    // check if vertex still in dsg. If not, continue.
    std::set<BVertex> bverts = dsg.get_set_BVertices();
    for(std::set<BVertex>::iterator bverts_it = bverts.begin();
            bverts_it != bverts.end(); ++bverts_it) {
        BVertex v = *bverts_it;
        if(!dsg.contains_BVertex(v)) { continue; }
        IVertex v_I = dsg.get_IVertex(v);
        std::vector< std::set<BVertex> > nis = this->get_n_is(v);
        if(nis[2].empty()) {
            this->no_effect.insert(v_I);
            continue;    // do nothing if n3.empty()
        }

        // remove n3, n2
        // upd this->pre_H (what's not in graph cannot be in H)
        for(std::set<BVertex>::iterator it = nis[2].begin();
                it != nis[2].end(); ++it) {
            this->pre_H.erase(dsg.get_IVertex(*it));
            dsg.remove_BVertex(*it);
        }
        for(std::set<BVertex>::iterator it = nis[1].begin();
                it != nis[1].end(); ++it) {
            this->pre_H.erase(dsg.get_IVertex(*it));
            dsg.remove_BVertex(*it);
        }

        this->pre_D.insert(v_I);
        std::set<IVertex> nv_new_I = dsg.get_adj_IVertices(v_I);
        this->pre_H.erase(v_I);
        this->pre_H.insert(nv_new_I.begin(), nv_new_I.end());
        dsg.remove_BVertex(v);
    }
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
