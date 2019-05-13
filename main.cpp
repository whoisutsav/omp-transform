#include <iostream>
#include "ast_node.h"
using namespace std;


AstNode* buildTree() {
  StmtNode* retstmt_node = new StmtNode();
  retstmt_node->type = STMT;
  retstmt_node->stmt = "return 0;";

  OmpNode* omp_node = new OmpNode();
  omp_node->type = OMP_PARALLEL;
  omp_node->num_threads_clause = true;
  omp_node->num_threads_value = 1;
  omp_node->children.push_front(retstmt_node);

  return omp_node;
}

int main()
{
  AstNode * tree = buildTree();
  tree->print();
}
