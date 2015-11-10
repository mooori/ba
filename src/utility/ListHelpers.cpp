#include <vector>
#include <list>
#include <boost/assign/list_of.hpp>

#include "./ListHelpers.hpp"

using namespace std;

template<typename T>
ListHelpers<T>::ListHelpers() { }

template<typename T>
list<T> ListHelpers<T>::init_by_arr(T arr[], size_t size) {
    list<T> l;
    for(unsigned int i = 0; i < size; i++) {
        l.push_back(arr[i]);
    }
    return l;
}

template class ListHelpers<int>;
template class ListHelpers< pair<int,int> >;
