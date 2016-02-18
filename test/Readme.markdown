# Preparation
- `boost_1_60_0` and a version of `g++` supporting `-std=c++11`, see
  `src/Readme.markdown`
- If `boost_1_60_0` is not located in project's root dir, adjust path to boost
  given to `I` flag in `test/Makefile`.
- Download `googletest` from [here](https://github.com/google/googletest/tree/master/googletest) (tested with Version 1.7.0)
- Place `googletest` in `lib/` or adjust `GTEST_DIR` in `test/Makefile`

# Compilation & Execution
Move to `test/`

1. Build `googletest` with `make install`
2. Build tests with `make test`

Then run tests by executing `./test`.
