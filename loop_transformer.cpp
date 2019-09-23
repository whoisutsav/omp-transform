#include "loop_transformer.h"
#include "emi_program.h"
#include "util.h"

LoopTransformer::LoopTransformer(EmiProgram* program) : program(program) {
  counterIdentifier = "loopCounter";
  counterVal = 0;
  DeclStmt* declStmt = DeclStmt::create("int", VarExpr::create(counterIdentifier), IntLiteral::create(0));
  program->injectStmt(declStmt);
}

struct LoopParams {
  int initialValue;
  int step;
  char op; 
  int predRHS;
};

LoopParams randLoopParameters(int numIterations) {
  switch(rand() % 5) {
    case 0:
      return {INT_MAX, -1, '>', INT_MAX - numIterations};
    case 1:
      return {INT_MIN, 1, '<', INT_MIN + numIterations};
    case 2:
      return {INT_MAX - numIterations, 1, '<', INT_MAX};
    case 3:
      return {INT_MIN + numIterations, -1, '>', INT_MIN};
    default:
      return {0, 1, '<', numIterations};
  }
}

std::vector<int> randNest() {
  return {1, 1, 3, 5, 2};
}

void LoopTransformer::addLoop() {
  Stmt* body = ExprStmt::create(
                  BinaryExpr::create(
                    VarExpr::create(counterIdentifier),
                    BinaryExpr::create(VarExpr::create(counterIdentifier), IntLiteral::create(1), '+'),
                    '='
                  ));
  Stmt* current = body;

  std::vector<int> nest = randNest();
  int incCount=1;
  for(int i=0; i<nest.size(); i++) {
    LoopParams params = randLoopParameters(nest[i]);  
    std::string emiVar = generateRandomAlphaNumericString(10); 
    program->injectInputAssignment(emiVar, params.initialValue);

    std::string iteratorRef = "i" + std::to_string(i);
    DeclStmt* init = DeclStmt::create("int", VarExpr::create(iteratorRef), VarExpr::create(emiVar));
    Expr* cond = BinaryExpr::create(VarExpr::create(iteratorRef), IntLiteral::create(params.predRHS), params.op);
    Expr* inc = BinaryExpr::create(
                    VarExpr::create(iteratorRef), 
                    BinaryExpr::create(VarExpr::create(iteratorRef), IntLiteral::create(params.step), '+'),
                    '=');

    ForStmt* forStmt = ForStmt::create(init, cond, inc, current);

    current = forStmt;
    incCount *= nest[i];
  }
  // TODO wrap in parallelFor Directive
  program->injectStmt(current);
  counterVal += incCount;
}

int LoopTransformer::getCounterValue() {
  return counterVal;
}
