# For gtest-part: Based on Makefile provided by googletest
# To make own tests: Based on http://aegis.sourceforge.net/auug97.pdf

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = ../lib/googletest_1.7.0

CXX = g++

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS = -isystem $(GTEST_DIR)/include

CXXFLAGS = -g -Wall -Wextra -pthread

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# install gtest
install : gtest.a gtest_main.a

# cleans gtest files (i.e. file created by install)
.PHONY: clean-gtest
clean-gtest :
	rm -f $(GTEST_DIR)/make/gtest.a $(GTEST_DIR)/make/gtest_main.a \
	$(GTEST_DIR)/make/*.o

# Builds gtest.a and gtest_main.a.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc \
			-o $(GTEST_DIR)/make/$@

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc \
			-o $(GTEST_DIR)/make/$@

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $(GTEST_DIR)/make/$@ $(GTEST_DIR)/make/$^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $(GTEST_DIR)/make/$@ $(addprefix $(GTEST_DIR)/make/,$^)

# Tests
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
LIBS := ../boost_1_59_0

test: $(OBJ) $(GTEST_DIR)/make/gtest.a ../src/utility/Parser.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

include $(OBJ:.o=.d)

./%.o: ./%.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I ../boost_1_59_0 -c $<

%.d: %.cpp
	./depend.sh $(CXXFLAGS) $*.cpp > $@

.PHONY: clean
clean: 
	rm -f *.o *.d test
