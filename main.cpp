#include <iostream>
#include "omp_node.h"
using namespace std;


OMPNode* buildTree() {
  OMPNode* retstmt_node = new OMPNode();
  retstmt_node->type = OMPN_STMT;
  retstmt_node->stmt = "return 0;";

  OMPNode* omp_node = new OMPNode();
  omp_node->type = OMPN_PARALLEL;
  omp_node->children.push_front(retstmt_node);
  omp_node->clauses.insert({OMPC_NUM_THREADS, "5"});

  return omp_node;
}

int main()
{
  OMPNode * tree = buildTree();
  tree->print(cout);
}
