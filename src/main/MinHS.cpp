#include <list>
#include <set>
#include <utility>
#include <vector>

#include "MinHS.hpp"
#include "../types/dstypes.hpp"
#include "../utility/setops.hpp"

MinHS::MinHS(const std::list< std::set<IVertex> > col_list, const int max_k)
        : max_k(max_k)
        , col_list(col_list)
        , col_vec(std::vector< std::set<IVertex> >(col_list.begin(),
                col_list.end()))
        { }

std::set<IVertex> MinHS::build_pop() {
    std::set<IVertex> universe = setops::big_union_new(this->col_list);
    typedef std::map<IVertex, std::set<unsigned int> > map_t;
    map_t m;
    
    // add each v in universe to m
    for(std::set<IVertex>::iterator u_it = universe.begin();
            u_it != universe.end(); ++u_it) {
        IVertex v = *u_it;
        // keys come out of set, so no need to check m_ins.first
        map_t::iterator m_it = (m.insert(std::pair<IVertex, 
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
            if(m[*p_it] == m[v]) {
                to_add = false;
                break;
            }
        }
        if(to_add) { pop.insert(v); }    // keys for pop.insert out of set
    }
    return pop;
}
