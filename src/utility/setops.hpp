#ifndef SETOPS_H
#define SETOPS_H

#include <list>
#include <stdexcept>
#include <set>
#include <utility>

/**
 * @brief Operations on sets (all functions are function templates).
 *
 * Functions with trailing `_new` return ref to a newly constructed set, which
 * means input sets are not changed. Use `_new` functions if original input
 * sets are required later on. Drawbacks: slower and requires more memory.
 *
 * Functions with trailing `_inp` modify input sets and return ref to one
 * of the input sets. E.g. for `union` elements of smaller set are added to
 * larger set. This means input sets are changed, though it's faster and 
 * requires less memory.
 */

namespace setops {

/**
 * Union of two sets, input sets unchanged.
 */
template<typename T>
std::set<T> union_new(std::set<T>& s1, std::set<T>& s2) {
    std::set<T> res;
    
    // insert s1 to res
    for(typename std::set<T>::iterator s1_it = s1.begin(); s1_it != s1.end(); 
            ++s1_it) {
        std::pair<typename std::set<T>::iterator, bool> ins =
                res.insert(*s1_it);
        if(!ins.second) {
            throw std::runtime_error("failed to insert in set");
        }
    }

    // insert s2 to res
    // return val of insert not checked, as for union it's ok that some
    // elements cannot be inserted, as they are already there
    for(typename std::set<T>::iterator s2_it = s2.begin(); s2_it != s2.end();
            ++s2_it) {
        res.insert(*s2_it);
    }

    return res;
}

/**
 * Union of two sets, input changed
 */
template<typename T>
std::set<T> union_inp(std::set<T>& s1, std::set<T>& s2) {
    // build union by inserting elements of smaller set to bigger set
    std::set<T>& res = s1.size() >= s2.size() ? s1 : s2;
    std::set<T>& small = s1.size() < s2.size() ? s1 : s2;

    // insert small to res
    // return val of insert not checked, as for union it's ok that some
    // elements cannot be inserted, as they are already there
    for(typename std::set<T>::iterator small_it = small.begin();
            small_it != small.end(); ++small_it) {
        res.insert(*small_it);
    }

    return res;
}

/**
 * Union of all sets (\bigcup) contained in list - input unchanged
 * @param l list of sets
 * @returns union of all sets in l
 */
template<typename T>
std::set<T> big_union_new(std::list< std::set<T> >& l) {
    // iterate over c, insert each element to set<T> res
    std::set<T> res;

    for(typename std::list< std::set<T> >::iterator l_it = l.begin();
            l_it != l.end(); ++l_it) {
        std::set<T>& s = *l_it;
        for(typename std::set<T>::iterator s_it = s.begin();
                s_it != s.end(); ++s_it) {
            // duplicates expected, so don't check return val of insert
            res.insert(*s_it);    
        }
    }

    return res;
}

/**
 * @see big_union_new, only diff: inputs changed
 */
template<typename T>
std::set<T> big_union_inp(std::list< std::set<T> >& l) {
    // find biggest set bs, then add all other set's els to bs

    // return empty set if l.size() == 0 (avoid invalid set ref later)
    if(l.size() == 0) { return std::set<T>(); }

    // get and store iterator to biggest set, to know which one to skip later
    // (comparison of sets with == would be too expensive)
    typename std::list< std::set<T> >::iterator bs_pos = l.begin();
    for(typename std::list< std::set<T> >::iterator l_it = l.begin();
            l_it != l.end(); ++l_it) {
        if(l_it->size() > bs_pos->size()) { bs_pos = l_it; }
    }

    // insert els of sets (except those of bs itself, of course)
    for(typename std::list< std::set<T> >::iterator l_it2 = l.begin();
            l_it2 != l.end(); ++l_it2) {
        if(l_it2 == bs_pos) { continue; }
        std::set<T>& s = *l_it2;
        for(typename std::set<T>::iterator s_it = s.begin();
                s_it != s.end(); ++s_it) {
            // duplicates expected, so don't check ret val of insert
            bs_pos->insert(*s_it);
        }
    }

    return *bs_pos;
}

/**
 * Intersection of two sets, input sets unchanged
 */
template<typename T>
std::set<T> inters_new(std::set<T>& s1, std::set<T>& s2) {
    // iterate through elements of smaller set, each el that's also in
    // bigger is added to res
    std::set<T> res;
    std::set<T>& big = s1.size() >= s2.size() ? s1 : s2;
    std::set<T>& small = s1.size() < s2.size() ? s1 : s2;

    for(typename std::set<T>::iterator small_it = small.begin();
            small_it != small.end(); ++small_it) {
        if(big.find(*small_it) == big.end()) { continue; }
        std::pair<typename std::set<T>::iterator, bool> ins = 
                res.insert(*small_it);
        if(!ins.second) {
            throw std::runtime_error("failed to insert in set");
        }
    }

    return res;
}

/**
 * Intersection of two sets, input changed
 */
template<typename T>
std::set<T> inters_inp(std::set<T>& s1, std::set<T>& s2) {
    // iterate through elements of smaller set, remove those which are not in
    // bigger as well
    std::set<T>& big = s1.size() >= s2.size() ? s1 : s2;
    std::set<T>& small = s1.size() < s2.size() ? s1 : s2;

    for(typename std::set<T>::iterator small_it = small.begin();
            small_it != small.end(); ) {
        if(big.find(*small_it) == big.end()) {
            typename std::set<T>::iterator del = small_it;
            ++small_it;
            small.erase(del);    // c++98 erase only returns void
        } else {
            ++small_it;
        }
    }

    return small;
}

/**
 * s1\s2, input sets unchanged
 */
template<typename T>
std::set<T> setminus_new(std::set<T>& s1, std::set<T>& s2) {
    // iterate through s1, add each el that is not in s2 to res
    std::set<T> res;

    for(typename std::set<T>::iterator s1_it = s1.begin();
            s1_it != s1.end(); ++s1_it) {
        if(s2.find(*s1_it) != s2.end()) { continue; }
        std::pair<typename std::set<T>::iterator, bool> ins =
                res.insert(*s1_it);
        if(!ins.second) {
            throw std::runtime_error("failed to insert in set");
        }
    }

    return res;
}

/**
 * s1\s2, input changed
 */
template<typename T>
std::set<T> setminus_inp(std::set<T>& s1, std::set<T>& s2) {
    // iterate through s1, erase what's in s2
    for(typename std::set<T>::iterator s1_it = s1.begin();
            s1_it != s1.end(); ) {
        if(s2.find(*s1_it) != s2.end()) {
            typename std::set<int>::iterator del = s1_it;
            ++s1_it;
            s1.erase(del);    // c++98 erase only returns void
        } else {
            ++s1_it;
        }
    }
    return s1;
}

/**
 * Remove sets that contain v from list of sets - input list unchanged
 * @param l list of sets
 * @param v element to look for
 * @returns l less than elements of l that contain v
 */
template<typename T>
std::list< std::set<T> > filter_containing_v_new(
        std::list< std::set<T> >& l, T v) {
    // iterate through l, add each el of l not containing v to res
    std::list< std::set<T> > res;

    for(typename std::list< std::set<T> >::iterator l_it = l.begin();
            l_it != l.end(); ++l_it ) {
        if(l_it->find(v) != l_it->end()) { continue; }
        res.push_back(*l_it);
    }

    return res;
}

/**
 * @see filter_containing_v_new, only diff: input list may be modified
 */
template<typename T>
void filter_containing_v_inp(
        std::list< std::set<T> >& l, T v) {
    // iterate through l, del each el that contains v
    for(typename std::list< std::set<T> >::iterator l_it = l.begin();
            l_it != l.end(); ) {
        if(l_it->find(v) != l_it->end()) {
            l_it = l.erase(l_it);
        } else {
            ++l_it;
        }
    }
    return;
}

} // namespace

#endif
