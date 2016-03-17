#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include "add_ds.hpp"
#include "../main/DSGraph.hpp"
#include "../types/dstypes.hpp"

void add_ds::add_ds(DSGraph& dsg, unsigned int k) {
    // cannot add dom set of size 0
    if(k < 1) { throw std::runtime_error("cannot add dom set of size 0"); }

    // make random number generators
    // (see cppreference)
    std::random_device rd_verts;    // to pick k random verts from V(G)
    std::mt19937 gen_verts(rd_verts());
    std::uniform_int_distribution<unsigned int>
            dis_verts(0, dsg.num_vertices() - 1);

    std::random_device rd_nconns;    // how many edges add to dom set verts
    std::mt19937 gen_nconns(rd_nconns());
    std::uniform_int_distribution<unsigned int> dis_nconns(1, k);
    
    std::random_device rd_dsidx;    // random index for dom set vector
    std::mt19937 gen_dsidx(rd_dsidx());
    std::uniform_int_distribution<unsigned int> dis_dsidx(0, k - 1);

    // randomly pick k vertices to become dom set
    std::set<IVertex> VG = dsg.get_set_IVertices();
    std::set<IVertex> ds_set;    // set: easier to get k distinct vals
    while(true) {
        unsigned int v_idx = dis_verts(gen_verts);
        std::set<IVertex>::iterator v_it = VG.begin();
        std::advance(v_it, v_idx);
        IVertex v = *v_it;
        ds_set.insert(v);    // just insert till size k reached
        if(ds_set.size() == k) { break; }
    }
    std::vector<IVertex> ds_vec(ds_set.begin(), ds_set.end());

    // for each vertex (except those in ds):
    // guess to how many verts from ds adjacent and add edges
    // don't make sure its really different verts.
    // it'a at least one, that's enough...
    for(std::set<IVertex>::iterator it = VG.begin(); it != VG.end(); ++it) {
        if(ds_set.find(*it) != ds_set.end()) { continue; }
        unsigned int nconns = dis_nconns(gen_nconns);
        IVertex v = *it;

        // add nconns edges
        for(unsigned int i = 0; i < nconns; ++i) {
            IVertex dsv = ds_vec[dis_dsidx(gen_dsidx)];
            if(dsg.in_adj_list_I(dsv, v)) { continue; }
            dsg.add_IEdge(IEdge(v, dsv));
        }
    }
    return;
}
