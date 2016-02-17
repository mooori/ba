# based on paper "Recursive make considered harmful"
# http://aegis.sourceforge.net/auug97.pdf

CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -O2 -I ../boost_1_60_0    # -O2 req for boost

SDIRS := main types utility visitors

SRC :=

include $(patsubst %,%/sdir.mk,$(SDIRS))

OBJ := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))
DEP := $(patsubst %.cpp,%.d,$(filter %.cpp,$(SRC)))

# path to BA-shortest-paths (relative to this makefile)
SPD_PATH := ../../BA-shortest-paths-decomposition
SPD_EXCL := $(SPD_PATH)/build/Main.o
SPD_OBJ_ALL := $(wildcard $(SPD_PATH)/build/*.o)
SPD_OBJ_LINK := $(filter-out $(SPD_EXCL),$(SPD_OBJ_ALL))

dsSolver: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(SPD_OBJ_LINK) $(LIBS)

include $(OBJ:.o=.d)

%.d: %.cpp
	./depend.sh 'dirname $*.cpp' $(CXXFLAGS) $*.cpp > $@

.PHONY: clean
clean: 
	rm -f $(DEP) $(OBJ) dsSolver

# build BA-shortes-paths-decomposition
.PHONY: spd
spd:
	$(MAKE) -C $(SPD_PATH)
