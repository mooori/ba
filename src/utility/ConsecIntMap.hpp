#ifndef CONSECINTMAP_H
#define CONSECINTMAP_H

#include <cstddef> // for size_t
#include <list>
#include <map>

/**
 * @brief Mapping of the elements of sequence container cont to a consecutive
 *     sequence of integers [0, cont.size()].
 */

template<typename T>
class ConsecIntMap {
public:
    ConsecIntMap() : map_(std::map<T, int>()) { }
    
    ConsecIntMap(std::list<T> vals) : map_(std::map<T, int>()) {
        this->populate(vals);
    }

    void populate(std::list<T> vals) {
        int ctr = 0;
        for(typename std::list<T>::iterator it = vals.begin();
                it != vals.end(); ++it) {
            pair<typename std::map<T, int>::iterator, bool> res =
                    this->map_.insert(pair<T, int>(*it, ctr));
            if(!res.second) { throw runtime_error("failed to insert in map"); }
            ++ctr;
        }
    }

    int find(const T& key) { 
        typename std::map<T, int>::iterator it = map_.find(key);
        if(it == map_.end()) { throw out_of_range("key not present"); }
        return it->second;
    }

    size_t size() { return map_.size(); }

private:
    std::map<T, int> map_;
};

#endif
