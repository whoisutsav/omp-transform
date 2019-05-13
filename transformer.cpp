#include "transformer.h"
#include "omp_node.h"

void alterNode(OMPNode * node) {
  if(!node->isDirective()) return;

  if(node->type == OMPN_PARALLEL) {
    if(node->clauses.find(OMPC_NUM_THREADS) == node->clauses.end()) 
      node->clauses.insert({OMPC_NUM_THREADS, "1"});

    if(node->clauses.find(OMPC_IF) == node->clauses.end())
      node->clauses.insert({OMPC_IF, "1"});
  }
}

void composeNode(OMPNode * node) {
  if(node->type == OMPN_PARALLEL) {
    OMPNode * critical_node = new OMPNode();
    critical_node->type = OMPN_CRITICAL;
    critical_node->children = node->children;
    node->children.assign(1, critical_node); 
  }
}

void appendNode(OMPNode * node) {
  // Do nothing for now
}

void Transformer::transform(OMPNode* root) {
  alterNode(root);
  composeNode(root);
  //list<OMPNode *>::const_iterator j;
  //for (j = children.begin(); j != children.end(); ++j)
  //  (*j)->print(o);
}

