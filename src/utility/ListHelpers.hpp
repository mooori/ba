#ifndef LISTHELPERS_H
#define LISTHELPERS_H

#include <list>

/**
 * Functions to help dealing with lists.
 */

template<typename T>
class ListHelpers {
public:
    
    /** Constructor, just creating "empty object" */
    ListHelpers() { }

    /**
     * Init list by array.
     * @param arr
     * @param size arr's size (since arr is in fact only ptr to arr[0])
     */
    std::list<T> init_by_arr(T arr[], size_t size) {
        std::list<T> l;
        for(unsigned int i = 0; i < size; i++) {
            l.push_back(arr[i]);
        }
        return l;
    }
};

#endif
