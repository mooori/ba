# ONLY tasks to run directly from test/:
# 	- install
# 	- clean
# OHTERWISE, use makefile in projects root dir

# Based on Makefile provided by googletest

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = ../lib/googletest_1.7.0
UTIL_DIR = ../src/utility

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
clean :
	rm -f $(GTEST_DIR)/make/gtest.a $(GTEST_DIR)/make/gtest_main.a \
	$(GTEST_DIR)/make/*.o

# Builds gtest.a and gtest_main.a.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc \
			-o $(GTEST_DIR)/make/gtest-all.o

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc
			-o $(GTEST_DIR)/make/gtest_main.o

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $(GTEST_DIR)/make/$@ $(GTEST_DIR)/make/$^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $(GTEST_DIR)/make/$@ $(GTEST_DIR)/make/$^

# Tests
utility_parser.o : utility_parser.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c utility_parser.cpp

# needs boost
utility_listHelpers.o : utility_listHelpers.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -I ../boost_1_59_0 utility_listHelpers.cpp

utility_consecIntMap.o : utility_consecIntMap.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c utility_consecIntMap.cpp

conn_comps.o : conn_comps.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -I ../boost_1_59_0 conn_comps.cpp

main.o : main.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp

test : utility_parser.o $(UTIL_DIR)/Parser.o \
		utility_listHelpers.o \
		utility_consecIntMap.o \
		conn_comps.o \
		main.o \
		$(GTEST_DIR)/make/gtest.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
