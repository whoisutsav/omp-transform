#include <iostream>
#include "omp_node.h"
#include "transformer.h"
using namespace std;


OMPNode* buildTree() {
  OMPNode* stmt_node = new OMPNode();
  stmt_node->type = OMPN_STMT;
  stmt_node->stmt = "5+3;";

  OMPNode* omp_node = new OMPNode();
  omp_node->type = OMPN_PARALLEL;
  omp_node->children.push_front(stmt_node);
  omp_node->clauses.insert({OMPC_NUM_THREADS, "5"});

  return omp_node;
}

void printProgram(OMPNode * tree) {
  cout << "#include <omp.h>" << "\n\n";

  cout << "int main(int argc, char* argv[]) {\n";
  tree->print(cout, 1);
  cout << "\treturn 0;\n";
  cout << "}\n";
}

int main()
{
  OMPNode * tree = buildTree();

  Transformer * myTransformer = new Transformer(0.7, 0.7, 0.7);
  myTransformer->transform(tree);

  printProgram(tree);
}
