# based on paper "Recursive make considered harmful"
# http://aegis.sourceforge.net/auug97.pdf

CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -O2 -I ../boost_1_60_0    # -O2 req for boost

SDIRS := main types utility visitors

SRC :=

include $(patsubst %,%/sdir.mk,$(SDIRS))

OBJ := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))
DEP := $(patsubst %.cpp,%.d,$(filter %.cpp,$(SRC)))

dsSolver: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LIBS)

include $(OBJ:.o=.d)

%.d: %.cpp
	./depend.sh 'dirname $*.cpp' $(CXXFLAGS) $*.cpp > $@

.PHONY: clean
clean: 
	rm -f $(DEP) $(OBJ) dsSolver
