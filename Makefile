CXX = clang++
CXXFLAGS = -std=c++26 -Wall -Werror -pedantic

all: main test

main: main.cc *.hh
	$(CXX) $(CXXFLAGS) main.cc -o main

test: test.cc *.hh
	$(CXX) $(CXXFLAGS) test.cc -o test
