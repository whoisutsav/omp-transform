#include "ast.h"
#include "emi_program.h"

EmiProgram::EmiProgram(FunctionDecl* main) : main(main) { }

EmiProgram* EmiProgram::generateEmptyProgram() {
  FunctionDecl* main = FunctionDecl::create(
                 "int",
                 "main",
                 { 
                    ParamDecl::create("int", VarExpr::create("argc")), 
                    ParamDecl::create("char**", VarExpr::create("argv")),
                 },
                 CompoundStmt::create({}));

  EmiProgram* context = new EmiProgram(main);
    
  return context;
}

void EmiProgram::injectInputAssignment(std::string identifier, int val) {
  inputs.push_back({identifier, val});
  int cliIndex = inputs.size();

  // TODO change to support command line input
  DeclStmt* declStmt = DeclStmt::create(
                  "int",
                  VarExpr::create(identifier),
                  CallExpr::create(
                          "atoi",
                          {ArrayExpr::create(VarExpr::create("argv"), IntLiteral::create(cliIndex))}
                  ));

  main->getBody()->getStatements().push_back(declStmt);
}

void EmiProgram::injectStmt(Stmt* stmt) {
  main->getBody()->getStatements().push_back(stmt);
}

FunctionDecl* EmiProgram::getMain() {
  return main;
}
