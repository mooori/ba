#ifndef CONSECINTMAP_H
#define CONSECINTMAP_H

#include <cstddef> // for size_t
#include <list>
#include <map>

using namespace std;

/**
 * @brief Mapping of the elements of sequence container cont to a consecutive
 *     sequence of integers [0, cont.size()].
 */

template<typename T>
class ConsecIntMap {
public:
    ConsecIntMap() : map_(map<T, int>()) { }
    
    ConsecIntMap(list<T> vals) : map_(map<T, int>()) {
        this->populate(vals);
    }

    void populate(list<T> vals) {
        int ctr = 0;
        for(typename list<T>::iterator it = vals.begin();
                it != vals.end(); ++it) {
            pair<typename map<T, int>::iterator, bool> res =
                    this->map_.insert(pair<T, int>(*it, ctr));
            if(!res.second) { throw runtime_error("failed to insert in map"); }
            ++ctr;
        }
    }

    int find(const T& key) { 
        typename map<T, int>::iterator it = map_.find(key);
        if(it == map_.end()) { throw out_of_range("key not present"); }
        return it->second;
    }

    size_t size() { return map_.size(); }

private:
    map<T, int> map_;
};

#endif
