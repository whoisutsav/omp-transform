CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++

all: testgen 

testgen: main.o program_wrapper.o loop_injector.o codegen.o 
	$(CXX) $(CXXFLAGS) -o testgen main.o program_wrapper.o loop_injector.o codegen.o 

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

loop_injector.o: loop_injector.h ast.h
	$(CXX) $(CXXFLAGS) -c loop_injector.cpp 

program_wrapper.o: program_wrapper.h ast.h
	$(CXX) $(CXXFLAGS) -c program_wrapper.cpp

codegen.o: codegen.h ast.h
	$(CXX) $(CXXFLAGS) -c codegen.cpp

clean:
	rm -f testgen main.o program_wrapper.o loop_injector.o codegen.o 
