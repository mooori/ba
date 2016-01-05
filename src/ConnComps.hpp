#ifndef CONNCOMPS_H
#define CONNCOMPS_H

#include <vector>
#include <list>
#include <stdexcept>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include "types/dstypes.hpp"
#include "utility/ConsecIntMap.hpp"

/** 
 * @brief Find a graph's connected components. Graph must have interior
 *     vertex property original_id.
 * 
 * @par Why `subgraph` is not used
 * Any operation on a graph of type `subgraph` affects all its ancestors
 *     in the hierarchy-tree.
 *
 */

template<class Graph>
class ConnComps {
public:

    ConnComps() {}

    /**
     * @param g Graph
     * @return list of pointers to g's connected components
     */
    std::vector<Graph*> find(Graph g) {
        std::vector<int> component(num_vertices(g));
        int num = connected_components(g, &component[0]);
        std::vector<Graph*> conn_comps(num);
        this->construct_subgraphs(g, conn_comps, component, num);
        return conn_comps;
    }

protected:

    /**
     * Construct (induced) subgraphs, as new, independent objects
     * @param g original graph
     * @param conn_comps vector of pointers to subgraphs to be created
     * @param component component[i] is vertex i's component
     * @param num the number of g's components
     */
    void construct_subgraphs(Graph g, std::vector<Graph*>& conn_comps,
            std::vector<int> component, int num) {
        std::vector< std::list<int> > vs_per_comp = 
                get_verts_per_comp(component, num);
        std::vector< ConsecIntMap<int> > vid_maps(num);
        
        // create comps' graph objects, maps of vertex-id's (since BGL 
        // requires vertex-id's of G with |V(G)|=n to be [0,n])
        // and assign OriginalId vertex-prop
        for(int i = 0; i < num; ++i) {
            conn_comps[i] = new Graph(vs_per_comp[i].size());
            vid_maps[i].populate(vs_per_comp[i]);
            this->set_vprop_origid(conn_comps[i], vs_per_comp[i]);
        }

        // Add each edge e in E(G) to the components it verices are in
        typedef typename boost::graph_traits<Graph>::edge_iterator
                edge_iterator_t;
        for(std::pair<edge_iterator_t, edge_iterator_t> eit = edges(g);
                eit.first != eit.second; ++eit.first) {
            int src = source(*eit.first, g);
            int tar = target(*eit.first, g);
            int comp_idx = component[src];
            add_edge(vid_maps[comp_idx].find(src),
                    vid_maps[comp_idx].find(tar), *conn_comps[comp_idx]);
        }

        return;
    }

    /**
     * For each connected component C, get V(C)
     * @param component component[i] is vertex i's component
     * @param num number of connected components
     * @return a vector of vertex-id lists, with i-th element being the
     *     vertices in the i-th component
     */
    std::vector< std::list<int> > get_verts_per_comp(std::vector<int> component,
            int num) {
        std::vector< std::list<int> > res(num);
        for(unsigned int i = 0;
                i < sizeof(component)/sizeof(component[0]); ++i) {
            res[component[i]].push_back(i);
        }
        return res;
    }

    /**
     * Set VertexProp OriginalId for all vertices
     * @param g pointer to graph
     * @param orig_ids list of original ids
     */
    void set_vprop_origid(Graph* g, const std::list<int>& orig_ids) {
        typedef typename boost::graph_traits<Graph>::vertex_iterator vert_it_t;
        typename boost::property_map<Graph, vertex_original_id_t>::type
                pm_orig_id = get(vertex_original_id, *g);
        std::pair<vert_it_t, vert_it_t> vit;
        std::list<int>::const_iterator lit = orig_ids.begin();

        for(vit = vertices(*g); vit.first != vit.second; ++vit.first) {
            put(pm_orig_id, *vit.first, *lit++);
        }

        // if lit doesn't point to end, something is wrong!
        if(lit != orig_ids.end()) {
            throw std::runtime_error("num edges in g != |orig_ids|");
        }

        return;
    }
};

#endif
