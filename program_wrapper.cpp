#include "ast.h"
#include "program_wrapper.h"

ProgramWrapper::ProgramWrapper(FunctionDecl* main) : main(main) { }

ProgramWrapper* ProgramWrapper::create() {
  FunctionDecl* main = FunctionDecl::create(
                 "int",
                 "main",
                 { 
                    ParamDecl::create("int", VarExpr::create("argc")), 
                    ParamDecl::create("char**", VarExpr::create("argv")),
                 },
                 CompoundStmt::create({}));

  ProgramWrapper* program = new ProgramWrapper(main);
    
  return program;
}

Expr* ProgramWrapper::addExpectedInput(int val) {
  expectedInputs.push_back(val);

  return CallExpr::create(
                  "atoi",
                  {
                    ArrayExpr::create(VarExpr::create("argv"), IntLiteral::create(expectedInputs.size()))
                  }); 
}

FunctionDecl* ProgramWrapper::getMain() {
  return main;
}

std::vector<int> ProgramWrapper::getExpectedInputs() {
  return expectedInputs;
}
