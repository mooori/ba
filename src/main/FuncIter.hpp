#ifndef FUNCITER_H
#define FUNCITER_H

#include <vector>

/**
 * @brief Class to iterate over all functions \mathcal{F} \rightarrow
 * \mathcal{C} \cup {B}.
 *
 * Use it in do-while loop, otherwise if radix=1 the 1st and only execution
 * is skipped.
 *
 * Assumption: both \mathcal{F} and \mathcal{C} are given as lists of
 * elements.
 *
 * Basic idea: There are |\mathcal{C} \cup {B}|^|\mathcal{F}| functions, so 
 * count a |\mathcal{F}| digit number with radix |\mathcal{C} \cup {B}| from
 * zero to its max value. Each number in the range [0, max] represents a
 * function, with digit at position i representing f(\mathcal{F}[i]).
 *
 * Example: \mathcal{F} = { F0, F1, F2 }, \mathcal{C} = { C0 }. So FuncIter
 * is 3 digit number with radix 2 (because of |\mathcal{C} \cup {B}|).
 * Number 101 then represents the function f(F0) = B, f(F1) = C0, f(F2) = C0
 */

class FuncIter {
public:

    /** 
     * Constructor, init function number at zero value.
     * @param radix number components + 1 for backland
     * @param ndigs number of digits for the function number (number colors)
     * @throws std::runtime_error if either r or n <= 0
     */
    FuncIter(int r, int n);

    /**
     * @returns true if fnumber is at max value, given radix and ndigs
     */
    bool max_reached();

    /**
     * Increment fnumber by one. Call max_reached first, as increment throws
     *     error if called on max fnumber.
     * @throws std::runtime_error if called on max fnumber
     */
    void increment();

    /**
     * Get digit value at position idx (wrapper for fnumber[idx])
     */
    int get_digit_val(int idx);


private:
    /** function number's radix */
    int radix;

    /** number of digits for the function number */
    int ndigs;

    /** function number */
    std::vector<int> fnumber;
};

#endif
