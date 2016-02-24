#include <iterator>
#include <list>
#include <mutex>
#include <set>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "Distance.hpp"
#include "DSGraph.hpp"
#include "FuncIter.hpp"
#include "MinHS.hpp"
#include "WComps.hpp"
#include "rgds.hpp"
#include "../types/distance_info.hpp"
#include "../utility/setops.hpp"

rgds::result_t rgds::no_solution_res() {
    return rgds::result_t(std::set<IVertex>(), false);
}

unsigned int rgds::nthreads_active = 0;

std::mutex rgds::mutex;

rgds::result_t rgds::rgds(DSGraph DSG, std::set<IVertex> H,
        std::list< std::set<IVertex> > Fs, unsigned int k,
        std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
        const unsigned int ncores) {
    std::set<IVertex> VG = DSG.get_set_IVertices();

    if(k == 0) { 
        if(test_k_eq_0(VG, H, Fs)) {
            return rgds::result_t(D, true);
        } else {
            return rgds::no_solution_res();
        }
    }

    if(VG.size() <= 1) {
        return rgds::result_t(setops::union_new(D, VG), true);
    }

    IVertex v = rgds::choose_v_spd(DSG, spd_ord);
    std::set<IVertex> N1v = DSG.get_adj_IVertices(v);
    DSG.remove_IVertex(v);
    std::set<IVertex> VG_minus_v = DSG.get_set_IVertices();

    // try v \in GDS
    std::set<IVertex> remove1 = setops::union_new(H, N1v);
            // remove var when setops const
    std::set<IVertex> W1 = setops::setminus_new(VG_minus_v, remove1);
    std::list<DSGraph*>* comps1 = WComps(DSG, rgds::trans_I2B(DSG, W1)).get();
    rgds::result_t res1 = rgds::solve(comps1, setops::union_new(H, N1v),
            setops::filter_containing_v_new(Fs, v), k - 1, VG_minus_v, D,
            spd_ord, ncores);
    WComps::delete_comps_ptrs(comps1);
    if(res1.second) {
        std::pair<std::set<IVertex>::iterator, bool> ins = res1.first.insert(v);
        if(!ins.second) { throw std::runtime_error("failed to insert in D"); }
        return res1;
    }

    // try v \notin GDS
    std::set<IVertex> W2 = setops::setminus_new(VG_minus_v, H);
    std::list<DSGraph*>* comps2 = WComps(DSG, rgds::trans_I2B(DSG, W2)).get();
    if(!N1v.empty() && H.find(v) == H.end()) { Fs.push_back(N1v); }
    rgds::result_t res2 = rgds::solve(comps2, H, Fs, k, VG_minus_v, D, spd_ord,
            ncores);
    WComps::delete_comps_ptrs(comps2);
    return res2;
}

IVertex rgds::choose_v_spd(const DSGraph& dsg,
        const std::vector<IVertex>& ord) {
    // iterate through ord, return first v with v \in V(dsg)
    std::set<IVertex> vs = dsg.get_set_IVertices();
    for(std::vector<IVertex>::const_iterator ord_it = ord.begin();
            ord_it != ord.end(); ++ord_it) {
        if(vs.find(*ord_it) != vs.end()) { return *ord_it; }
    }
    throw std::runtime_error("none of DSG's vertices in spd_order");
}

rgds::result_t rgds::solve(std::list<DSGraph*>* comps, std::set<IVertex> H,
        std::list< std::set<IVertex> > Fs, unsigned int k,
        std::set<IVertex> VG, std::set<IVertex> D,
        const std::vector<IVertex>& spd_ord, const unsigned int ncores) {

    // check termination conditions
    if(comps->size() > k) { return rgds::no_solution_res(); }
    if(rgds::diams_exceeding(comps, k)) { return rgds::no_solution_res(); }

    std::set<IVertex> B = rgds::get_backland(VG, comps);

    FuncIter f(comps->size() + 1, Fs.size());
    while(true) {
        rgds::result_t res = rgds::try_f(f, comps, B, H, Fs, k, D, spd_ord,
                ncores);
        if(res.second) { return res; }
        if(f.max_reached()) { break; }
        f.increment();
    }

    return rgds::no_solution_res();
}

bool rgds::diams_exceeding(std::list<DSGraph*>* comps, unsigned int k) {
    unsigned int sum_diams = 0;
    for(std::list<DSGraph*>::iterator c_it = comps->begin(); 
            c_it != comps->end(); ++c_it) {
        distance_info di = Distance().get_info(**c_it);
        sum_diams += di.diameter;
    }
    return sum_diams > 3*k + comps->size();
}

bool rgds::test_k_eq_0(std::set<IVertex>& VG, std::set<IVertex>& H,
        std::list< std::set<IVertex> >& Fs) {
    if(!(setops::setminus_new(VG, H)).empty()) {
        return false;
    }
    if(!(setops::big_union_new(Fs)).empty()) {
        return false;
    }
    return true;
}

std::set<IVertex> rgds::get_backland(std::set<IVertex>& VG,
        std::list<DSGraph*>* comps) {
    // get set of all verts that are in any component
    std::set<IVertex> comps_vs;
    for(std::list<DSGraph*>::iterator c_it = comps->begin();
            c_it != comps->end(); ++c_it) {
        std::set<IVertex> c_Is = (*c_it)->get_set_IVertices();
                // remove var when setops const
        comps_vs = setops::union_new(comps_vs, c_Is);
    }
    return setops::setminus_new(VG, comps_vs);
}

rgds::result_t rgds::get_min_gds(DSGraph DSG, std::set<IVertex> H,
        std::list< std::set<IVertex> > Fs, unsigned int max_k,
        std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
        const unsigned int ncores) {
    for(unsigned int k = 0; k <= max_k; ++k) {
        rgds::result_t res = rgds::rgds(DSG, H, Fs, k, D, spd_ord, ncores);
        if(res.second) { return res; }
    }
    return rgds::result_t(std::set<IVertex>(), false);
}

std::set<IVertex>
        rgds::big_union_results(std::vector<rgds::result_t> res_vec) {
    std::set<IVertex> res;
    for(std::vector<rgds::result_t>::iterator r_it = res_vec.begin();
            r_it != res_vec.end(); ++r_it) {
        res = setops::union_new(res, (*r_it).first);
    }
    return res;
}

rgds::result_t rgds::try_f(FuncIter& f, std::list<DSGraph*>* comps,
        std::set<IVertex> B, std::set<IVertex> H,
        std::list< std::set<IVertex> > Fs, unsigned int k,
        std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
        const unsigned int ncores) {
    // vecs to hold comps and per comp: Fs, D, result of rgds
    unsigned int size = comps->size();
    std::vector<DSGraph*> comps_vec(comps->begin(), comps->end());
    std::vector< std::set<IVertex> > Fs_vec(Fs.begin(), Fs.end());
    std::vector< std::list< std::set<IVertex> > > comps_Fs(size,
            std::list< std::set<IVertex> >());
    std::list< std::set<IVertex> > Fs_B;

    // loop over FuncIter's function number to assign F-colors to comps
    for(unsigned int i = 0; i < Fs_vec.size(); ++i) {
        unsigned int comps_idx = f.get_digit_val(i);
        if(comps_idx < comps_vec.size()) {
            comps_Fs[comps_idx].push_back(Fs_vec[i]);
        } else if(comps_idx == comps_vec.size()) {
            Fs_B.push_back(Fs_vec[i]);
        } else {
            throw std::runtime_error("nonsense digit in fnumber");
        }
    }

    // find minGDS for all components
    /**
    std::pair<std::vector<rgds::result_t>, bool> comps_res(
            std::pair<std::vector<rgds::result_t>, bool>
            (std::vector<rgds::result_t>(comps_vec.size(),
            rgds::no_solution_res()), false));
    work_comps(comps_vec, H, comps_Fs, k, D, spd_ord, ncores, comps_res);
    if(!comps_res.second) { return rgds::no_solution_res(); }
    */
    std::pair<std::vector<rgds::result_t>, bool> comps_res =
            rgds::concurrently_work_comps(comps_vec, H, comps_Fs, k, D,
            spd_ord, ncores);
    if(!comps_res.second) { return rgds::no_solution_res(); }


    // solve MinHS for backland
    // intersect f^-1(B) colors with B
    for(std::list< std::set<IVertex> >::iterator Fs_B_it = Fs_B.begin();
            Fs_B_it != Fs_B.end(); ++Fs_B_it) {
        bool was_empty = Fs_B_it->empty();
        *Fs_B_it = setops::inters_new(*Fs_B_it, B);
        if(!was_empty && Fs_B_it->empty()) { return rgds::no_solution_res(); }
    }
    MinHS HS(Fs_B, k);
    rgds::result_t HS_res = HS.get();
    if(!HS_res.second) { return rgds::no_solution_res(); }

    // sum up GDS resp. backland k's
    unsigned int sum_k = 0;
    for(unsigned int i = 0; i < comps_res.first.size(); ++i) {
        sum_k += comps_res.first[i].first.size();
    }
    sum_k += HS_res.first.size();

    if(sum_k > k) { return rgds::no_solution_res(); }
    std::set<IVertex> D_res = rgds::big_union_results(comps_res.first);
    D_res = setops::union_new(D_res, D);
    D_res = setops::union_new(D_res, HS_res.first);
    return rgds::result_t(D_res, true);
}

void rgds::work_comps(
        std::vector<DSGraph*>& comps_vec, std::set<IVertex> H,
        std::vector< std::list< std::set<IVertex> > > comps_Fs,
        unsigned int k, std::set<IVertex> D,
        const std::vector<IVertex>& spd_ord, const unsigned int ncores,
        std::pair<std::vector<rgds::result_t>, bool>& res) {
    for(unsigned int i = 0; i < comps_vec.size(); ++i) {
        // deref comps ok, as rgds call by val and setops::*_new used
        DSGraph* C = comps_vec[i];
        std::list< std::set<IVertex> > Fs_C = comps_Fs[i];

        // intersect each F in Fs_C with V(C)
        std::set<IVertex> VC = C->get_set_IVertices();
        for(std::list< std::set<IVertex> >::iterator f_it = Fs_C.begin();
                f_it != Fs_C.end(); ++f_it) {
            bool was_empty = f_it->empty();
            *f_it = setops::inters_new(*f_it, VC);
            if(!was_empty && f_it->empty()) { res.second = false; return; }
        }
        std::set<IVertex> H_C = setops::inters_new(H, VC);
        rgds::result_t res_C = rgds::get_min_gds(*C, H_C, Fs_C, k, D, spd_ord,
                ncores);
        if(!res_C.second) { res.second = false; return; }
        res.first[i] = res_C;
    }
    res.second = true;

    // this function may be executed in thread, so try decr nthreads_active
    std::unique_lock<std::mutex> lock{rgds::mutex};
    if(rgds::nthreads_active > 0) { --rgds::nthreads_active; }
    lock.unlock();
    return;
}

std::pair<std::vector<rgds::result_t>, bool> rgds::concurrently_work_comps(
        std::vector<DSGraph*>& comps_vec, std::set<IVertex> H,
        std::vector< std::list< std::set<IVertex> > > comps_Fs,
        unsigned int k, std::set<IVertex> D,
        const std::vector<IVertex>& spd_ord, unsigned int ncores) {
    // handle casses where no concurrency needed
    std::pair<std::vector<rgds::result_t>, bool> full_res(
            std::vector<rgds::result_t>(comps_vec.size(),
            rgds::no_solution_res()), false);
            // pass to work_comps when no threads are created
    if(ncores == 1 || comps_vec.size() < 2) {
        work_comps(comps_vec, H, comps_Fs, k, D, spd_ord, ncores, full_res);
        return full_res;
    }

    // check if nthreads < ncores - 1, no sense to start only 1 thread
    // if yes check how many new threads to start
    std::unique_lock<std::mutex> lock{rgds::mutex};
    unsigned int nstart = 0;
    if(rgds::nthreads_active < ncores - 1) {
        nstart = std::min((unsigned int) comps_vec.size(),
                ncores - rgds::nthreads_active);
        rgds::nthreads_active += nstart;
        lock.unlock();
    } else {
        lock.unlock();
        work_comps(comps_vec, H, comps_Fs, k, D, spd_ord, ncores, full_res);
        return full_res;
    }

    // make vectors of comps and Fs to be handled by threads
    unsigned int n_per_t = comps_vec.size() / nstart;
    unsigned int rest = comps_vec.size() % nstart;
    std::vector< std::vector<DSGraph*> > t_comps(nstart);
    std::vector< std::vector< std::list< std::set<IVertex> > > >t_Fs(nstart);
    std::vector<DSGraph*>::iterator it_C = comps_vec.begin();
    std::vector< std::list< std::set<IVertex> > >::iterator it_F =
            comps_Fs.begin();
    for(unsigned int i = 0; i < nstart; ++i) {
        unsigned int nadd = n_per_t;
        if(i < rest) { ++nadd; }    // to distr rest across threads
        std::vector<DSGraph*>::iterator start_C = it_C;
        std::advance(it_C, nadd);
        t_comps[i] = std::vector<DSGraph*>(start_C, it_C);
        std::vector< std::list< std::set<IVertex> > >::iterator
                start_F = it_F;
        std::advance(it_F, nadd);
        t_Fs[i] = std::vector< std::list< std::set<IVertex> > >(start_F, it_F);
    }

    // ts_res[i] to hold work_comps return value of i-th thread
    // init ts_res[i].first to have right size, to avoid expensive resizing
    std::vector< std::pair<std::vector<rgds::result_t>, bool> > t_res(nstart);
    for(unsigned int i = 0; i < nstart; ++i) {
        t_res[i] = std::pair<std::vector<rgds::result_t>, bool>(
                std::vector<rgds::result_t>(t_comps[i].size(),
                rgds::no_solution_res()), false);
    }

    // start threads
    std::vector<std::thread> threads(nstart);
    for(unsigned int i = 0; i < nstart; ++i) {
        threads[i] = std::thread(rgds::work_comps, std::ref(t_comps[i]), H,
                t_Fs[i], k, D, std::ref(spd_ord), ncores, std::ref(t_res[i]));
    }

    // join threads
    // dont decrement nthreads_active; done in function executed by threads
    for(unsigned int i = 0; i < nstart; ++i) {
        threads[i].join();
    }

    // concatenate threads' results
    // if any thread returned false, return 'no_solution'
    std::vector<rgds::result_t> C_res(comps_vec.size(), rgds::no_solution_res());
    std::pair<std::vector<rgds::result_t>, bool> 
            no_solution(std::vector<rgds::result_t>(), false);
    unsigned int j = 0;
    for(unsigned int i = 0; i< nstart; ++i) {
        if(!t_res[i].second) { return no_solution; }
        for(std::vector<rgds::result_t>::iterator it = t_res[i].first.begin();
                it != t_res[i].first.end(); ++it) {
            C_res[j++] = *it;
        }
    }

    return std::pair<std::vector<rgds::result_t>, bool>(C_res, true);
}

std::set<BVertex> rgds::trans_I2B(const DSGraph& DSG,
        const std::set<IVertex>& icol) {
    std::set<BVertex> bcol;
    for(std::set<IVertex>::const_iterator icol_it = icol.begin();
            icol_it != icol.end(); ++icol_it) {
        std::pair<std::set<BVertex>::iterator, bool> ins =
                bcol.insert(DSG.get_BVertex(*icol_it));
        if(!ins.second) {
            throw std::runtime_error("failed to insert BVertex in set");
        }
    }
    return bcol;
}
