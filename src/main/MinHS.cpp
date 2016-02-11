#include <list>
#include <set>
#include <utility>
#include <vector>

#include "MinHS.hpp"
#include "SubsetGen.hpp"
#include "../types/dstypes.hpp"
#include "../utility/setops.hpp"

MinHS::MinHS(const std::list< std::set<IVertex> > col_list, const int max_k)
        : max_k(max_k)
        , col_list(col_list)
        , col_vec(std::vector< std::set<IVertex> >(col_list.begin(),
                col_list.end()))
        { }

std::pair<std::set<IVertex>, bool> MinHS::get() {
    std::set<IVertex> pop = this->build_pop();
    std::vector<IVertex> pop_vec(pop.begin(), pop.end());
    
    // handle edge cases
    if(pop.empty()) { 
        return std::pair<std::set<IVertex>, bool>(std::set<IVertex>(), true);
    }
    if(this->max_k == 0) {    // max_k==0 AND pop empty are handled above
        return std::pair<std::set<IVertex>, bool>(std::set<IVertex>(), false);
    }
    

    // for k in [0, max_k]
    //   for each size k subset s of pop: return (s, true) if s is HS
    SubsetGen<IVertex> SG(pop_vec);
    for(unsigned int k = 0; k <= this->max_k; ++k) {
        std::list< std::set<IVertex> > ssets = SG.get_subsets(k);
        for(std::list< std::set<IVertex> >::iterator s_it = ssets.begin();
                s_it != ssets.end(); ++s_it) {
            if(this->each_color_hit(*s_it)) {
                return std::pair<std::set<IVertex>, bool>(*s_it, true);
            }
        }
    }
    return std::pair<std::set<IVertex>, bool>(std::set<IVertex>(), false);
}

std::set<IVertex> MinHS::build_pop() {
    std::set<IVertex> universe = setops::big_union_new(this->col_list);
    this->m_v_cidx.clear();
    
    // add each v in universe to m
    for(std::set<IVertex>::iterator u_it = universe.begin();
            u_it != universe.end(); ++u_it) {
        IVertex v = *u_it;
        // keys come out of set, so no need to check m_ins.first
        map_t::iterator m_it = (this->m_v_cidx.insert(std::pair<IVertex, 
                std::set<unsigned int> >(v, std::set<unsigned int>()))).first;

        // for each color F, check if v \in F, if yes insert in m[v]
        for(unsigned int i = 0; i < this->col_vec.size(); ++i) {
            if(this->col_vec[i].find(v) != this->col_vec[i].end()) {
                std::pair<std::set<unsigned int>::iterator, bool> c_idx_ins =
                        (*m_it).second.insert(i);
                if(!c_idx_ins.second) {
                    throw std::runtime_error("failed to insert color index");
                }
            }
        }
    }

    std::set<IVertex> pop;
    // for each v in universe: if no v' of its type is in pop, add v to pop
    for(std::set<IVertex>::iterator u_it = universe.begin();
            u_it != universe.end(); ++u_it) {
        IVertex v = *u_it;
        bool to_add = true;

        for(std::set<IVertex>::iterator p_it = pop.begin();
                p_it != pop.end(); ++p_it) {
            if(this->m_v_cidx[*p_it] == this->m_v_cidx[v]) {
                to_add = false;
                break;
            }
        }
        if(to_add) { pop.insert(v); }    // keys for pop.insert out of set
    }
    return pop;
}

bool MinHS::each_color_hit(const std::set<IVertex>& s) {
    // for each v in s, set hit[cidx] true for all colors v is in
    std::vector<bool> hit(this->col_vec.size(), false);
    for(std::set<IVertex>::const_iterator s_it = s.begin();
            s_it != s.end(); ++s_it) {
        std::set<unsigned int> cidxs = this->m_v_cidx[*s_it];
        for(std::set<unsigned int>::iterator cidx_it = cidxs.begin();
                cidx_it != cidxs.end(); ++cidx_it) {
            hit[*cidx_it] = true;
        }
    }

    // check if each entry in hit is true
    for(std::vector<bool>::iterator hit_it = hit.begin();
            hit_it != hit.end(); ++hit_it) {
        if(*hit_it == false) { return false; }
    }
    return true;
}
