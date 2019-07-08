#include "emi_context.h"

std::string EMIContext::addInput(int val) {
  string result = "emi" + std::to_string(Inputs.size() + 1);

  Inputs.insert({result, val});

  return result;
}

Node* EMIContext::getRoot() {
  return root;
}

CompoundStmt* EMIContext::getMain() {
  return main;
}

Node* EMIContext::generateEMIAST() {
  for(int i=generateCount; i<inputs.size(); i++) {
    VarDeclStmt* varDeclStmt = ASTHelper::generateVarDeclStmt(inputs[i].first, inputs[i].second);
    main->Statements.insert(main->Statements.begin(),
                    std::unique_ptr(varDeclStmt));
    numGenerated++;
  }  

  return root;
}

