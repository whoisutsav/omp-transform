CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++

all: emi_program.o loop_transformer.o code_generator.o util.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

loop_transformer.o: loop_transformer.h ast.h
	$(CXX) $(CXXFLAGS) -c loop_transformer.cpp 

emi_program.o: emi_program.h ast.h
	$(CXX) $(CXXFLAGS) -c emi_program.cpp

code_generator.o: code_generator.h ast.h
	$(CXX) $(CXXFLAGS) -c code_generator.cpp

util.o: code_generator.h ast.h
	$(CXX) $(CXXFLAGS) -c util.cpp

clean:
	rm -f emi_program.o loop_transformer.o code_generator.o util.o
