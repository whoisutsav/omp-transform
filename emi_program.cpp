#include "ast.h"
#include "emi_program.h"

EmiProgram::EmiProgram(CompoundStmt* root, CompoundStmt* mainBody) : root(root), mainBody(mainBody) {
  current = mainBody->getStatements().begin();
}

EmiProgram* EmiProgram::generateEmptyProgram() {
  CompoundStmt* root = CompoundStmt::create(); 
  CompoundStmt* mainBody = CompoundStmt::create(); 

  root->getStatements().push_back(mainBody);

  EmiProgram* context = new EmiProgram(root, mainBody);
    
  return context;
}

void EmiProgram::injectInputAssignment(std::string identifier, int val) {
  inputs.push_back({identifier, val});

  // TODO change to support command line input
  DeclStmt* declStmt = DeclStmt::create(
                  "int",
                  VarExpr::create(identifier),
                  IntLiteral::create(val)
                  );

  mainBody->getStatements().insert(current, declStmt);
  if(current != mainBody->getStatements().end()) current++;
}

void EmiProgram::injectStmt(Stmt* stmt) {
  mainBody->getStatements().insert(current, stmt);
  if(current != mainBody->getStatements().end()) current++;
}

