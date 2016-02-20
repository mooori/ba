# Preparation
- Download and extract `boost_1_60_0` (versions prior to `1_60` cause compiler
  to throw warnings, as they are not compatible with C++11)
- If directory `boost_1_60_0` is not located in project's root directory, adjust
  path to it in `CXXFLAGS` in `src/Makefile`
- If this repository is not located in same folder as
  `BA-shortest-paths-decomposition`, adjust `SPD_PATH` in `src/Makefile`
- Have a `g++` version that supports `-std=c++11`

# Compilation & Execution
Move to `src/`, then:

1. Build `BA-shortest-paths-decomposition` with `make spd`
2. Build this project with `make`

Graph parser accepts format used by Konect.

Call RGDS on a graph and provide `k`:
    `./dsSolver ./path/to/graph rgds k`

Calculate a graphs domination number and a minimum dominating set using brute
force (for `k \in [0, |V(G)|]`, try all size k subset of `V(G)`):
    `./dsSolver ./path/to/graph bf`

# Tests
See `test/Readme.markdown`.
