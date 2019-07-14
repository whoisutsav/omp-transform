#include "loop_transformer.h"
#include "emi_context.h"
#include "ast_helper.h"

LoopTransformer::LoopTransformer(EMIContext* context) : context(context) {
  counterIdentifier = "loopCounter";
  counterVal = 0;
  DeclStmt* declStmt = ASTHelper::createDeclStmt(ASTHelper::createVarExpr(counterIdentifier), ASTHelper::createIntLiteral(0));
  context->injectStmt(declStmt);
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
  Stmt* body = ASTHelper::createExprStmt(
                  ASTHelper::createBinaryExpr(
                    ASTHelper::createVarExpr(counterIdentifier),
                    ASTHelper::createBinaryExpr(ASTHelper::createVarExpr(counterIdentifier), ASTHelper::createIntLiteral(1), '+'),
                    '='
                  ));
  Stmt* current = body;

  std::vector<int> nest = randNest();
  int incCount=1;
  for(int i=0; i<nest.size(); i++) {
    LoopParams params = randLoopParameters(nest[i]);  
    std::string emiVarName = context->addInput(params.initialValue);

    std::string iteratorRef = "i" + std::to_string(i);
    DeclStmt* init = ASTHelper::createDeclStmt(ASTHelper::createVarExpr(iteratorRef), ASTHelper::createVarExpr(emiVarName));
    Expr* cond = ASTHelper::createBinaryExpr(ASTHelper::createVarExpr(iteratorRef), ASTHelper::createIntLiteral(params.predRHS), params.op);
    Expr* inc = ASTHelper::createBinaryExpr(
                    ASTHelper::createVarExpr(iteratorRef), 
                    ASTHelper::createBinaryExpr(ASTHelper::createVarExpr(iteratorRef), ASTHelper::createIntLiteral(params.step), '+'),
                    '=');

    ForStmt* forStmt = ASTHelper::createForStmt(init, cond, inc, current);

    current = forStmt;
    incCount *= nest[i];
  }
  // TODO wrap in parallelFor Directive
  context->injectStmt(current);
  counterVal += incCount;
}

int LoopTransformer::getCounterValue() {
  return counterVal;
}
