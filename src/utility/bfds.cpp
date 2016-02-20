#include <list>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "bfds.hpp"
#include "helpers.hpp"
#include "Parser.hpp"
#include "../main/DSGraph.hpp"
#include "../main/SubsetGen.hpp"

std::set<IVertex> bfds::get_min_ds(const DSGraph& dsg) {
    std::set<IVertex> V = dsg.get_set_IVertices();
    std::vector<IVertex> V_vec(V.begin(), V.end());
    SubsetGen<IVertex> sg(V_vec);

    for(unsigned int k = 0; k <= V.size(); ++k) {
        std::list< std::set<IVertex> > ssets = sg.get_subsets(k);

        for(std::list< std::set<IVertex> >::iterator s_it = ssets.begin();
                s_it != ssets.end(); ++s_it) {
            if(helpers::is_ds(dsg, *s_it)) {
                return *s_it;
            }
        }
    }
    // V(G) is a ds in any case
    throw std::runtime_error("failed to recognize V(G) as dom set");
}
