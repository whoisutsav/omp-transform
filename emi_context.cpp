#include "ast.h"
#include "emi_context.h"

EMIContext::EMIContext(Node* root, CompoundStmt* main) : root(root), main(main) {
  current = main->Statements.begin();
}

std::string EMIContext::addInput(int val) {
  string result = "emi" + std::to_string(Inputs.size() + 1);
  inputs.insert({result, val});

  VarDeclStmt* varDeclStmt = ASTHelper::generateVarDeclStmt(inputs[i].first, inputs[i].second);

  main->Statements.insert(current, std::unique_ptr(varDeclStmt));
  if(current != main->Statements.end()) current++;

  return result;
}

void EMIContext::injectNode(Node* node) {
  main->Statements.insert(current, std::unique_ptr(node));
  if(current != main->Statements.end()) current++;
}

