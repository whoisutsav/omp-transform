#include <ctime>
#include <cstdlib>
#include "emi_transformer.h"
#include "fuzzer.h"
#include "ast_node.h"

vector<int> getRandomInsertPoints(int length) {
  vector<int> result;
  result[0] = length/2;
  return result;
}

ASTNode * EMI_Transformer::create_dead_fragment_modulo_input() {
  string emiVarName = var_base + to_string(input_set.size() + 1);
  int emiVarVal = 0;

  input_set.insert({emiVarName, emiVarVal});

  ASTNode* if_node =  new ASTNode();
  if_node->node_type = IF_STMT;

  ASTNode * expr = new ASTNode();
  expr->node_type = VAR;
  expr->sval = emiVarName;

  ASTNode * blk_stmt_node = new ASTNode();
  blk_stmt_node->node_type = BLK_STMT;
  blk_stmt_node->children.push_back(fuzzer.generateFragment());

  if_node->children.push_back(expr);
  if_node->children.push_back(blk_stmt_node);

  return if_node;
}


// inject code that is "dead-by-construction" (modulo input) 
void EMI_Transformer::dead_code_transform(ASTNode * node) {
  switch(node->node_type) {
    case(BLK_STMT):
      node->children.insert(node->children.begin(), create_dead_fragment_modulo_input()); 
      break;
    default:
      break;
  } 

  //for(ASTNode* i  : node->children)
  //  dead_code_transform(i); 

}

// convert expressions to new expressions that are equivalent (modulo input)
void EMI_Transformer::expression_transform(ASTNode * node) {
  // TODO
}

ASTNode * generate_assignment_stmt_node(string varName, int val) {
  ASTNode * var_node = new ASTNode();
  var_node->node_type = VAR;
  var_node->sval = varName;

  ASTNode * vdecl_node = new ASTNode();
  vdecl_node->node_type = VDECL;
  vdecl_node->ctype = INT;
  vdecl_node->children.push_back(var_node);

  ASTNode * const_node = new ASTNode();
  const_node->node_type = CONSTANT;
  const_node->ctype = INT;
  const_node->ival = val;

  ASTNode * assign_stmt_node = new ASTNode();
  assign_stmt_node->node_type = ASSIGN_STMT;
  assign_stmt_node->children.push_back(vdecl_node);
  assign_stmt_node->children.push_back(const_node);

  return assign_stmt_node;
}

void EMI_Transformer::transform(ASTNode * root) {
  if (root->node_type != BLK_STMT) {
    cerr << "EMI_Transformer::transformer must be called with node of type BLK_STMT\n";
    return;
  }

  dead_code_transform(root);
  expression_transform(root);
  
  // TODO not efficient 
  unordered_map<string, int>::const_iterator i;
  for (i = input_set.begin(); i != input_set.end(); ++i) {
    root->children.insert(root->children.begin(), generate_assignment_stmt_node(i->first, i->second)); 
  }

}
