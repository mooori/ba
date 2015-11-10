CC=g++
# -O2 optimization required for boost
CFLAGS=-c -Wall -O2 -I boost_1_59_0

all: dsSolver

dsSolver: main.o Parser.o
	$(CC) src/main.o src/utility/Parser.o -o dsSolver

main.o:
	$(CC) $(CFLAGS) src/main.cpp -o src/main.o

Parser.o:
	$(CC) $(CFLAGS) src/utility/Parser.cpp -o src/utility/Parser.o

clean:
	rm dsSolver src/*.o src/utility/*.o /test/*.o test/test

# after running `make test` execute `test/test` to actually run tests
test: dsSolver test_utility.o
	$(CC) src/utility/Parser.o test/utility_parser.o -o test/test

test_utility.o:
	$(CC) $(CFLAGS) test/utility_parser.cpp -o test/utility_parser.o
