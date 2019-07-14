#include "ast.h"
#include "emi_context.h"

EMIContext::EMIContext(Node* root, CompoundStmt* main) : root(root), main(main) {
  current = main->begin();
}

std::string EMIContext::addInput(int val) {
  std::string identifier = "emi" + std::to_string(inputs.size() + 1);
  inputs.push_back({identifier, val});

  // TODO change to support command line input
  DeclStmt* declStmt = ASTHelper::generateDeclStmt(
                  ASTHelper::generateVarExpr(identifier),
                  ASTHelper::generateIntLiteral(val)
                  );

  main->Statements.insert(current, std::unique_ptr<Stmt*> (declStmt));
  if(current != main->Statements.end()) current++;

  return identifier;
}

void EMIContext::injectStmt(Stmt* stmt) {
  main->Statements.insert(current, std::unique_ptr<Stmt*> (stmt));
  if(current != main->Statements.end()) current++;
}

