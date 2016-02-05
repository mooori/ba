#ifndef SUBSETGEN_H
#define SUBSETGEN_H

#include <iostream>
#include <list>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../types/dstypes.hpp"

/**
 * @class SubsetGen
 * @brief Get all k-element subsets (i.e. combinations).
 */
template<typename T>
class SubsetGen {
public:

    /**
     * Set private attributes
     * @param population set of BVertices from which to draw k-size subsets.
     *     std::vector because []-operator must be used extensively. Values
     *     must be unique.
     */
    SubsetGen(std::vector<T>& population)
            : population(population)
            , ssets(std::list< std::set<T> >())
            { }

    /**
     * Print this->ssets
     */
    void print_ssets() {
        std::cout << "Number of subsets: " << this->ssets.size() << "\n";
        for(typename std::list< std::set<T> >::iterator l_it =
                    this->ssets.begin(); l_it != this->ssets.end(); ++l_it) {
            std::cout << "{ ";
            for(typename std::set<T>::iterator s_it = l_it->begin();
                    s_it != l_it->end(); ++s_it) {
                if(s_it != l_it->begin()) { std::cout << ", "; }
                std::cout << *s_it;
            }
            std::cout << " }\n";
        }
    }

    /**
     * Get all size k subsets of population
     * @throws std::runtime if k larger than size of population or < 0
     */
    std::list< std::set<T> > get_subsets(unsigned int k) {
        if(k > this->population.size() || k < 0) {
            throw std::runtime_error("subset must be smaller than population");
        }

        // reset ssets and s
        this->ssets = std::list< std::set<T> >();
        this->s = std::list<T>();

        this->generate_ssets(0, k);
        return this->ssets;
    }

private:

    /** Population from which to draw subsets, @see SubsetGen for why vector */
    std::vector<T>& population;

    /** List of subsets of size k, to be filled by @see get_subsets */
    std::list< std::set<T> > ssets;

    /** 
     * "Set" generate_ssets is working on. List since generate_ssets works
     *     with push_back and pop_back
     */
    std::list<T> s; 

    /**
     * Recursively generate all size k subsets of population.
     * @returns void, subsets are pushed_back to ssets
     */
    void generate_ssets(unsigned int offset, unsigned int k) {
        if(k == 0) {
            ssets.push_back(this->make_set(this->s));
            return;
        }
        for(unsigned int i = offset; i <= this->population.size() - k; ++i) {
            this->s.push_back(this->population[i]);
            this->generate_ssets(i + 1, k - 1);
            this->s.pop_back();
        }
    }

    /**
     * Create set based on contents of list
     * @throws std::runtime_error, if trying to add an element more than once
     */
    std::set<T> make_set(std::list<T> l) {
        std::set<T> res;
        for(typename std::list<T>::iterator l_it = l.begin();
                l_it != l.end(); ++l_it) {
            std::pair<typename std::set<T>::iterator, bool> ins =
                    res.insert(*l_it);
            if(!ins.second) {
                throw std::runtime_error("Element can be added set only once");
            }
        }
        return res;
    }
};

#endif
