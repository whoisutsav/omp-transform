CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++

all: ast_node.o main.o fuzzer.o emi_transformer.o ast.o
	$(CXX) $(CXXFLAGS) -o main main.o ast_node.o fuzzer.o emi_transformer.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

loop_transformer.o: loop_transformer.h ast.h
	$(CXX) $(CXXFLAGS) -c loop_transformer.cpp 

emi_context.o: emi_context.h ast.h
	$(CXX) $(CXXFLAGS) -c emi_context.cpp

code_generator.o: code_generator.h ast.h
	$(CXX) $(CXXFLAGS) -c code_generator.cpp

clean:
	rm -f emi_context.o loop_transformer.o code_generator.o
