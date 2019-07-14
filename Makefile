CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++

all: ast_node.o main.o fuzzer.o emi_transformer.o ast.o
	$(CXX) $(CXXFLAGS) -o main main.o ast_node.o fuzzer.o emi_transformer.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

loop_transformer.o: loop_transformer.h 
	$(CXX) $(CXXFLAGS) -c loop_transformer.cpp 

ast.o: ast.h 
	$(CXX) $(CXXFLAGS) -c ast.cpp 

emi_context.o: emi_context.h
	$(CXX) $(CXXFLAGS) -c emi_context.cpp

clean:
	rm -f ast.o 
