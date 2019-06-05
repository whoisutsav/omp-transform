#include <iostream>
#include "ast_node.h"
#include "fuzzer.h"
#include "emi_transformer.h"
using namespace std;


void printProgram(ostream &o, ASTNode * root, vector<int> inputs) {
  o << "//";
  for(int i=0; i<inputs.size(); i++) {
    o << " " << to_string(inputs[i]);
  }

  o << "\n";
  o << "#include <omp.h>" << "\n";
  o << "#include <assert.h>" << "\n";
  o << "#include <stdlib.h>" << "\n\n";

  o << "int main(int argc, char* argv[]) \n";

  root->print(o, 0);
}

ASTNode * generateBaseBlock() {
  ASTNode * constant_expr = new ASTNode();
  constant_expr->type = CONSTANT;
  constant_expr->ival = 0;

  ASTNode * assert_stmt = new ASTNode();
  assert_stmt->type = CUSTOM;
  assert_stmt->sval = "assert(my_counter == 1);";

  ASTNode * ret_stmt = new ASTNode();
  ret_stmt->type = RET_STMT;
  ret_stmt->children.push_back(constant_expr);

  ASTNode * blk_stmt = new ASTNode();
  blk_stmt->type = BLK_STMT;
  blk_stmt->children.push_back(assert_stmt);
  blk_stmt->children.push_back(ret_stmt);
  
  return blk_stmt;
}



int main(int argc, char* argv[])
{
  ASTNode * root = generateBaseBlock();
  EMI_Transformer transformer(time(0));
  transformer.insertIncrementLoop(root, 1);
  transformer.processEMI(root);
  printProgram(cout, root, transformer.get_inputs());
  return 0;
}
