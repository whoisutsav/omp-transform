#include "ast.h"
#include "emi_program.h"

EmiProgram::EmiProgram(FunctionDecl* main) : main(main) {
  current = main->getBody()->getStatements().begin();
}

EmiProgram* EmiProgram::generateEmptyProgram() {
  FunctionDecl* main = FunctionDecl::create(
                 "int",
                 "main",
                 { 
                    ParamDecl::create("int", VarExpr::create("argc")), 
                    ParamDecl::create("char**", VarExpr::create("argv")),
                 },
                 CompoundStmt::create());

  EmiProgram* context = new EmiProgram(main);
    
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

  main->getBody()->getStatements().insert(current, declStmt);
  if(current != main->getBody()->getStatements().end()) current++;
}

void EmiProgram::injectStmt(Stmt* stmt) {
  main->getBody()->getStatements().insert(current, stmt);
  if(current != main->getBody()->getStatements().end()) current++;
}

FunctionDecl* EmiProgram::getMain() {
  return main;
}
