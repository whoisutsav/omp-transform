CXX = clang++
CXXFLAGS = -Wall -std=c++11

all: ast_node.o main.o fuzzer.o emi_transformer.o
	$(CXX) $(CXXFLAGS) -o main main.o ast_node.o fuzzer.o emi_transformer.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

ast_node.o: ast_node.h
	$(CXX) $(CXXFLAGS) -c ast_node.cpp

fuzzer.o: fuzzer.h
	$(CXX) $(CXXFLAGS) -c fuzzer.cpp

emi_transformer.o: emi_transformer.h
	$(CXX) $(CXXFLAGS) -c emi_transformer.cpp

clean:
	rm -f main main.o ast_node.o fuzzer.o emi_transformer.o
