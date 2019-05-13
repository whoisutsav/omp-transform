#include <ctime>
#include <cstdlib>
#include "transformer.h"
#include "omp_node.h"

float getRandomPct() {
  return (rand() % 1000)/1000.0;
}

void alterNode(OMPNode * node) {
  if(!node->isDirective()) return;

  if(node->type == OMPN_PARALLEL) {
    if(node->clauses.find(OMPC_NUM_THREADS) == node->clauses.end()) 
      node->clauses.insert({OMPC_NUM_THREADS, "1"});

    if(node->clauses.find(OMPC_IF) == node->clauses.end())
      node->clauses.insert({OMPC_IF, "1"});
  }
}

OMPNode * getInvariantDirective() {
  float n = getRandomPct();
  OMPNode * node = new OMPNode();
  if (n < 0.33) {
    node->type = OMPN_PARALLEL;
    node->clauses.insert({OMPC_NUM_THREADS, "1"});
  } else if (0.33 <= n < 0.66) {
    node->type = OMPN_CRITICAL;
  } else {
    node->type = OMPN_MASTER;
  }

  return node;
}

void composeNode(OMPNode * node) {
  if(!node->isDirective()) return;
  if(node->type == OMPN_PARALLEL || node->type == OMPN_CRITICAL || node->type == OMPN_MASTER) {
    OMPNode * inner_node = getInvariantDirective();
    inner_node->children = node->children;
    node->children.assign(1, inner_node); 
  }
}

OMPNode * createDisjointNode() {
  float n = getRandomPct();
  OMPNode * node = new OMPNode();
  if (n < 0.33) {
    node->type = OMPN_PARALLEL;
    node->clauses.insert({OMPC_NUM_THREADS, "5"});
  } else if (0.33 <= n < 0.66) {
    node->type = OMPN_CRITICAL;
  } else {
    node->type = OMPN_MASTER;
  }

  OMPNode* stmt_node = new OMPNode();
  stmt_node->type = OMPN_STMT;
  stmt_node->stmt = "1+1;";
  node->children.assign(1, stmt_node);

  return node;
}


void appendNode(OMPNode * node) {
  if(!node->isDirective()) return;
  node->children.push_back(createDisjointNode());
}

void Transformer::transform(OMPNode* root) {
  if (getRandomPct() < alterPct) alterNode(root);
  if (getRandomPct() < composePct) composeNode(root);
  //if (getRandomPct() < appendPct) appendNode(root);
  list<OMPNode *>::const_iterator j;
  for (j = root->children.begin(); j != root->children.end(); ++j)
    transform(*j);
}

