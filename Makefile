CXX = clang++
CXXFLAGS = -Wall -Werror

all: ast_node.o main.o
	$(CXX) $(CXXFLAGS) -o main main.o ast_node.o 

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

ast_node.o: ast_node.h
	$(CXX) $(CXXFLAGS) -c ast_node.cpp

clean:
	rm -f main main.o ast_node.o 
