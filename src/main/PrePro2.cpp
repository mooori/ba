#include <list>
#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "PrePro2.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/setops.hpp"

PrePro2::PrePro2(DSGraph& dsg)
        :dsg(dsg)
        { }

std::vector< std::set<BVertex> >
        PrePro2::get_n_is(const std::set<IVertex> set_vw) const {
    std::set<IVertex>::const_iterator vw_it = set_vw.begin();
    BVertex v = dsg.get_BVertex(*vw_it);
    BVertex w = dsg.get_BVertex(*(++vw_it));

    // v = N(v), nv_sq = n[v]
    std::set<BVertex> Nv, Nw, Nvw;
    Nv = dsg.get_adj_BVertices(v);
    Nw = dsg.get_adj_BVertices(w);
    Nvw = setops::union_new(Nv, Nw);
    Nvw.erase(v); Nvw.erase(w);
    std::set<BVertex> Nvw_sq(Nvw);
    Nvw_sq.insert(v); Nvw_sq.insert(w);

    // for each u in Nvw, check if it is in N1vw or not
    std::set<BVertex> N1vw, not_N1vw;
    for(std::set<BVertex>::iterator it = Nvw.begin(); it != Nvw.end(); ++it) {
        BVertex u = *it;
        std::set<BVertex> Nu = dsg.get_adj_BVertices(u);
        if((setops::setminus_new(Nu, Nvw_sq)).size() > 0) {
            N1vw.insert(u);
        } else {
            not_N1vw.insert(u);
        }
    }

    // for each u in not_N1vw, add it to N2 or N3
    std::set<BVertex> N2vw, N3vw;
    for(std::set<BVertex>::iterator it = not_N1vw.begin();
            it != not_N1vw.end(); ++it) {
        BVertex u = *it;
        std::set<BVertex> Nu = dsg.get_adj_BVertices(u);
        if((setops::inters_new(Nu, N1vw)).size() > 0) {
            N2vw.insert(u);
        } else {
            N3vw.insert(u);
        }
    }

    return std::vector< std::set<BVertex> >({ N1vw, N2vw, N3vw });
}
