#include <iostream>
#include "ast_node.h"
#include "fuzzer.h"
#include "emi_transformer.h"
using namespace std;


//void printProgram(ASTNode * tree) {
//  cout << "#include <omp.h>" << "\n\n";
//
//  cout << "int main(int argc, char* argv[]) {\n";
//  tree->print(cout, 1);
//  cout << "\treturn 0;\n";
//  cout << "}\n";
//}

ASTNode * generateBaseBlock() {
  ASTNode * constant_expr = new ASTNode();
  constant_expr->type = CONSTANT;
  constant_expr->ival = 0;

  ASTNode * ret_stmt = new ASTNode();
  ret_stmt->type = RET_STMT;
  ret_stmt->children.push_back(constant_expr);

  ASTNode * blk_stmt = new ASTNode();
  blk_stmt->type = BLK_STMT;
  blk_stmt->children.push_back(ret_stmt);
  
  return blk_stmt;
}

int main(int argc, char* argv[])
{
  ASTNode * root = generateBaseBlock();
  EMI_Transformer transformer(time(0));
  transformer.insertIncrementLoop(root, 7);
  transformer.processEMI(root);
  //transformer.transform(root);
  root->print(cout, 0);
  return 0;
}
