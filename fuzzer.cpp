#include <ctime>
#include <cstdlib>
#include "fuzzer.h"
#include "ast_node.h"

float getRandomPct() {
  return (rand() % 1000)/1000.0;
}

ASTNode* getConstExpr(int val) {
  ASTNode * const_node = new ASTNode();
  const_node->node_type = CONSTANT;
  const_node->ival = val;

  return const_node;
}

void alterNode(ASTNode * node) {
  if(!node->isDirective()) return;


  if(node->node_type == OMPN_PARALLEL) {
    if(node->clauses.find(OMPC_NUM_THREADS) == node->clauses.end()) 
      node->clauses.insert({OMPC_NUM_THREADS, getConstExpr(1)});

    if(node->clauses.find(OMPC_IF) == node->clauses.end())
      node->clauses.insert({OMPC_IF, getConstExpr(1)});
  }
}

ASTNode * getInvariantDirective() {
  float n = getRandomPct();
  ASTNode * node = new ASTNode();
  if (n < 0.33) {
    node->node_type = OMPN_PARALLEL;
    node->clauses.insert({OMPC_NUM_THREADS, getConstExpr(1)});
  } else if (0.33 <= n < 0.66) {
    node->node_type = OMPN_CRITICAL;
  } else {
    node->node_type = OMPN_MASTER;
  }

  return node;
}

void composeNode(ASTNode * node) {
  if(!node->isDirective()) return;
  if(node->node_type == OMPN_PARALLEL || node->node_type == OMPN_CRITICAL || node->node_type == OMPN_MASTER) {
    ASTNode * inner_node = getInvariantDirective();
    inner_node->children = node->children;
    node->children.assign(1, inner_node); 
  }
}

ASTNode* basicBlkStmt() {
  ASTNode* constant_node = new ASTNode();
  constant_node->node_type = CONSTANT;
  constant_node->ival = 3;

  ASTNode* exp_stmt_node = new ASTNode();
  exp_stmt_node->node_type = EXP_STMT;
  exp_stmt_node->children.push_back(constant_node);

  ASTNode * blk_stmt_node = new ASTNode();
  blk_stmt_node->node_type = BLK_STMT;
  blk_stmt_node->children.push_back(exp_stmt_node);

  return blk_stmt_node;
}

ASTNode * createDisjointNode() {
  float n = getRandomPct();
  ASTNode * node = new ASTNode();
  if (n < 0.33) {
    node->node_type = OMPN_PARALLEL;
    node->clauses.insert({OMPC_NUM_THREADS, getConstExpr(5)});
  } else if (0.33 <= n < 0.66) {
    node->node_type = OMPN_CRITICAL;
  } else {
    node->node_type = OMPN_MASTER;
  }

  ASTNode* stmt_node = basicBlkStmt();
  node->children.clear();
  node->children.push_back(stmt_node);

  return node;
}

void appendNode(ASTNode * node) {
  if(!node->isDirective()) return;
  node->children.push_back(createDisjointNode());
}


ASTNode* buildTree() {
  ASTNode* stmt_node = basicBlkStmt();;

  ASTNode* ast_node = new ASTNode();
  ast_node->node_type = OMPN_PARALLEL;
  ast_node->children.push_back(stmt_node);
  ast_node->clauses.insert({OMPC_NUM_THREADS, getConstExpr(5)});

  return ast_node;
}


// TODO destroy tree properly
ASTNode* Fuzzer::generateFragment() {
  ASTNode * tree = buildTree();
  transform(tree);
  return tree;
}


void Fuzzer::transform(ASTNode* root) {
  if (getRandomPct() < alterPct) alterNode(root);
  if (getRandomPct() < composePct) composeNode(root);
  //if (getRandomPct() < appendPct) appendNode(root);
  for (ASTNode* n : root->children)
    transform(n);
}

