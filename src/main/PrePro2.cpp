#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "PrePro2.hpp"
#include "SubsetGen.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/setops.hpp"

PrePro2::PrePro2(DSGraph& dsg)
        :dsg(dsg)
        ,orig_nverts(dsg.num_vertices())
        { }

void PrePro2::run() {
    if(this->dsg.num_vertices() < 2) { return; }
    this->init();
    this->finish();
    return;
}

std::vector< std::set<BVertex> >
        PrePro2::get_n_is(const std::set<IVertex> set_vw) const {
    std::set<IVertex>::const_iterator vw_it = set_vw.begin();
    BVertex v = this->dsg.get_BVertex(*vw_it);
    BVertex w = this->dsg.get_BVertex(*(++vw_it));

    // v = N(v), nv_sq = n[v]
    std::set<BVertex> Nv, Nw, Nvw;
    Nv = this->dsg.get_adj_BVertices(v);
    Nw = this->dsg.get_adj_BVertices(w);
    Nvw = setops::union_new(Nv, Nw);
    Nvw.erase(v); Nvw.erase(w);
    std::set<BVertex> Nvw_sq(Nvw);
    Nvw_sq.insert(v); Nvw_sq.insert(w);

    // for each u in Nvw, check if it is in N1vw or not
    std::set<BVertex> N1vw, not_N1vw;
    for(std::set<BVertex>::iterator it = Nvw.begin(); it != Nvw.end(); ++it) {
        BVertex u = *it;
        std::set<BVertex> Nu = this->dsg.get_adj_BVertices(u);
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
        std::set<BVertex> Nu = this->dsg.get_adj_BVertices(u);
        if((setops::inters_new(Nu, N1vw)).size() > 0) {
            N2vw.insert(u);
        } else {
            N3vw.insert(u);
        }
    }

    return std::vector< std::set<BVertex> >({ N1vw, N2vw, N3vw });
}

void PrePro2::init() {
    std::set<IVertex> VG = this->dsg.get_set_IVertices();
    std::vector<IVertex> VG_vec(VG.begin(), VG.end());
    SubsetGen<IVertex> sg(VG_vec);
    std::list< std::set<IVertex> > sets = sg.get_subsets(2);

    for(std::list< std::set<IVertex> >::iterator it = sets.begin();
            it != sets.end(); ++it) {
        std::set<IVertex> set_vw = *it;
        std::vector< std::set<BVertex> > nis = this->get_n_is(set_vw);
        if(this->rule_2_applicable(nis)) {
            this->applicable.push_back(set_vw);
            this->m_n_is[set_vw] = nis;
        }
    }
    return;
}

void PrePro2::finish() {
    for(std::list< std::set<IVertex> >::iterator it = this->applicable.begin();
            it != this->applicable.end(); ++it) {
        std::set<IVertex> set_vw = *it;
        std::set<IVertex>::iterator vw_it = set_vw.begin();
        IVertex vI = *vw_it; IVertex wI = *(++vw_it);
        
        if(this->skip(vI, wI)) { continue; }

        BVertex vB = this->dsg.get_BVertex(vI);
        BVertex wB = this->dsg.get_BVertex(wI);

        if(this->in_case_1(set_vw)) {
            std::set<BVertex> N3vw = this->m_n_is[set_vw][2];
            bool Nv_incl_N3vw = this->N_includes_N3vw(vB, N3vw);
            bool Nw_incl_N3vw = this->N_includes_N3vw(wB, N3vw);

            if(Nv_incl_N3vw && Nw_incl_N3vw) { this->finish_case_1_1(set_vw); }
            else if(Nv_incl_N3vw) { this->finish_symmetric(set_vw, vB); }
            else if(Nw_incl_N3vw) { this->finish_symmetric(set_vw, wB); }
            else { throw std::runtime_error("mistakenly in case 1"); }
        }

        else {    // if not case 1, it's case 2
            this->finish_case_2(set_vw);
        }
        this->worked.insert(vI); this->worked.insert(wI);
    }
    return;
}

bool PrePro2::skip(IVertex v, IVertex w) {
    // skip if either v or w has been removed already
    if(!this->dsg.contains_IVertex(v) || !this->dsg.contains_IVertex(w)) {
        return true;
    }

    // skip if either vertex in set has already been worked
    if(this->worked.find(v) != this->worked.end() || this->worked.find(w) !=
            this->worked.end()) {
        return true;
    }

    // skip if N3vw <= 1
    if(this->m_n_is[std::set<IVertex>({v, w})][2].size() <= 1) {
        return true;
    }

    // skip if to either v or w gadget as per case 1_1 attached
    if(this->vs_gadgeted.find(this->dsg.get_BVertex(v)) != this->vs_gadgeted.end()
            || this->vs_gadgeted.find(this->dsg.get_BVertex(w)) !=
            this->vs_gadgeted.end()) {
        return true;
    }

    return false;
}

bool PrePro2::
        rule_2_applicable(std::vector< std::set<BVertex> >& nis) const {
    // check if any u from N2 dominates N3
    for(std::set<BVertex>::iterator it = nis[1].begin(); it != nis[1].end();
            ++it) {
        std::set<BVertex> Nu = this->dsg.get_adj_BVertices(*it);
        if(setops::setminus_new(nis[2], Nu).empty()) { return false; }
    }

    // check if any u from N2 dominates N3
    for(std::set<BVertex>::iterator it = nis[2].begin(); it != nis[2].end();
            ++it) {
        std::set<BVertex> Nu = this->dsg.get_adj_BVertices(*it);
        std::set<BVertex> diff = setops::setminus_new(nis[2], Nu);
        diff.erase(*it);    // to take into accout that u itself in N3
        if(diff.empty()) { return false; }
    }

    // if this is reached, no single vertex from N2 \cup N3 dominates N3
    return true;
}

bool PrePro2::in_case_1(const std::set<IVertex>& set_vw) {
    std::set<IVertex>::const_iterator vw_it = set_vw.begin();
    BVertex v = this->dsg.get_BVertex(*vw_it);
    BVertex w = this->dsg.get_BVertex(*(++vw_it));
    std::set<BVertex> Nv = this->dsg.get_adj_BVertices(v);
    std::set<BVertex> Nw = this->dsg.get_adj_BVertices(w);
    std::set<BVertex> N3vw = this->m_n_is[set_vw][2];

    // check if v dominates N3vw
    std::set<BVertex> diff1 = setops::setminus_new(N3vw, Nv);
    diff1.erase(v);
    if(diff1.empty()) { return true; }

    // check if w dominates N3vw
    std::set<BVertex> diff2 = setops::setminus_new(N3vw, Nw);
    diff2.erase(w);
    if(diff2.empty()) { return true; }

    // neither v nor w can dominate N3vw
    return false;
}

bool PrePro2::N_includes_N3vw(const BVertex z, const std::set<BVertex>& N3vw)
        const {
    std::set<BVertex> Nz = this->dsg.get_adj_BVertices(z);
    return std::includes(Nz.begin(), Nz.end(), N3vw.begin(), N3vw.end());
}

void PrePro2::finish_case_1_1(const std::set<IVertex>& set_vw) {
    std::vector< std::set<BVertex> > nis = this->m_n_is[set_vw];

    // remove N3vw
    this->remove_verts(nis[2]);

    std::set<IVertex>::iterator vw_it = set_vw.begin();
    IVertex vI = *vw_it; IVertex wI = *(++vw_it);
    BVertex vB = this->dsg.get_BVertex(vI);
    BVertex wB = this->dsg.get_BVertex(wI);

    // remove N2vw \cap Nv \cap Nw
    std::set<BVertex> Nv_B = this->dsg.get_adj_BVertices(vB);
    std::set<BVertex> Nw_B = this->dsg.get_adj_BVertices(wB);
    std::set<BVertex> inters = setops::inters_new(Nv_B, Nw_B);
    this->remove_verts(setops::inters_new(nis[1], inters));

    // add gadget verts
    IVertex z1 = this->next_free_IVertex();
    this->dsg.add_IVertex(z1);
    this->dsg.add_IEdge(IEdge(vI, z1)); this->dsg.add_IEdge(IEdge(wI, z1));
    this->m_gadget[z1] = set_vw;

    IVertex z2 = this->next_free_IVertex();
    this->dsg.add_IVertex(z2);
    this->dsg.add_IEdge(IEdge(vI, z2)); this->dsg.add_IEdge(IEdge(wI, z2));
    this->m_gadget[z2] = set_vw;

    this->vs_gadgeted.insert(vB); this->vs_gadgeted.insert(wB);
}

void PrePro2::finish_symmetric(const std::set<IVertex>& set_vw, BVertex z) {
    std::vector< std::set<BVertex> > nis = this->m_n_is[set_vw];

    // remove N3vw
    this->remove_verts(nis[2]);

    // remove intersection of Nz and N2vw
    std::set<BVertex> Nz = this->dsg.get_adj_BVertices(z);
    this->remove_verts(setops::inters_new(Nz, nis[1]));

    IVertex zI = this->dsg.get_IVertex(z);
    this->add_N_to_pre_H(z);
    this->dsg.remove_BVertex(z);

    this->pre_D.insert(zI);
    this->pre_H.erase(zI);

    return;
}

void PrePro2::finish_case_2(const std::set<IVertex>& set_vw) {
    std::vector< std::set<BVertex> > nis = this->m_n_is[set_vw];

    // remove N2vw and N3vw
    this->remove_verts(nis[1]);
    this->remove_verts(nis[2]);

    std::set<IVertex>::iterator vw_it = set_vw.begin();
    IVertex v = *vw_it;
    IVertex w = *(++vw_it);

    this->pre_D.insert(v); this->pre_D.insert(w);
    this->pre_H.erase(v); this->pre_H.erase(w);

    this->add_N_to_pre_H(this->dsg.get_BVertex(v));
    this->add_N_to_pre_H(this->dsg.get_BVertex(w));
    this->dsg.remove_IVertex(v);
    this->dsg.remove_IVertex(w);

    return;
}

void PrePro2::remove_verts(const std::set<BVertex>& s) {
    for(std::set<BVertex>::const_iterator it = s.begin(); it != s.end(); ++it) {
        if(!this->dsg.contains_BVertex(*it)) { continue; }
        IVertex v = this->dsg.get_IVertex(*it);
        this->pre_H.erase(v);
        this->pre_D.erase(v);
        this->dsg.remove_IVertex(v);
    }
    return;
}

void PrePro2::add_N_to_pre_H(BVertex z) {
    std::set<IVertex> Nz = this->dsg.get_adj_IVertices(this->dsg.get_IVertex(z));
    for(std::set<IVertex>::iterator it = Nz.begin(); it != Nz.end(); ++it) {
        this->pre_H.insert(*it);
    }
}

unsigned int PrePro2::next_free_IVertex() const {
    return this->orig_nverts + this->m_gadget.size();
}
