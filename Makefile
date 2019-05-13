CXX = clang++
CXXFLAGS = -Wall -Werror -std=c++11

all: omp_node.o main.o transformer.o
	$(CXX) $(CXXFLAGS) -o main main.o omp_node.o transformer.o 

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

omp_node.o: omp_node.h
	$(CXX) $(CXXFLAGS) -c omp_node.cpp

transformer.o: transformer.h
	$(CXX) $(CXXFLAGS) -c transformer.cpp

clean:
	rm -f main main.o omp_node.o transformer.o
