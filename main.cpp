#include <iostream>
#include "omp_node.h"
using namespace std;


OMPNode* buildTree() {
  OMPNode* retstmt_node = new OMPNode();
  retstmt_node->type = OMPN_STMT;
  retstmt_node->stmt = "return 0;";

  OMPClause* omp_clause = new OMPClause();
  omp_clause->type = OMPC_NUM_THREADS;
  omp_clause->expr = "5";

  OMPNode* omp_node = new OMPNode();
  omp_node->type = OMPN_PARALLEL;
  omp_node->clauses.push_front(omp_clause);
  omp_node->children.push_front(retstmt_node);

  return omp_node;
}

int main()
{
  OMPNode * tree = buildTree();
  tree->print(cout);
}
