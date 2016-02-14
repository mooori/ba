#include <vector>
#include <stdexcept>

#include "FuncIter.hpp"

FuncIter::FuncIter(unsigned int r, unsigned int n)
        : radix(r)
        , ndigs(n)
        , fnumber(std::vector<unsigned int>(ndigs, 0))
        {
            if(r <= 0 || n < 0) { 
                throw std::runtime_error("FuncIter: invalid init values");
            }
        }


bool FuncIter::max_reached() const {
    // iterate through fnumber, return false on seeing digit < radix - 1
    for(std::vector<unsigned int>::const_iterator it = this->fnumber.begin();
            it != this->fnumber.end(); ++it) {
        if(*it < this->radix - 1) { return false; }
    }
    return true;
}

void FuncIter::increment() {
    // iterator through fnumber starting at begin()
    // on getting digit < radix - 1: increment digit
    // else digit is at max value, so set it to 0 and move on
    for(std::vector<unsigned int>::iterator it = this->fnumber.begin(); 
            it != this->fnumber.end(); ++it) {
        if(*it < this->radix - 1) {
            (*it)++;
            return;
        } else {    // digit at position it is at max value
            *it = 0;
        }
    }
    // if this is reached, fnumber is already at max value
    throw std::runtime_error("cannot increment max function number");
}

unsigned int FuncIter::get_digit_val(unsigned int idx) const {
    return this->fnumber[idx];
}
