CC = g++

# -O2 optimization required for boost
CFLAGS = -c -Wall -O2 -I boost_1_59_0

all: dsSolver

dsSolver: Parser.o main.o
	$(CC) src/main.o src/utility/Parser.o -o dsSolver

main.o:
	$(CC) $(CFLAGS) src/main.cpp -o src/main.o

Parser.o:
	$(CC) $(CFLAGS) src/utility/Parser.cpp -o src/utility/Parser.o

clean:
	rm -f dsSolver src/*.o src/utility/*.o /test/*.o test/test

# after running `make test` execute `test/test` to actually run tests
test: dsSolver 
	$(MAKE) -C test/ test
