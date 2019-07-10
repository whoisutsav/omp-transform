#include "loop_transformer.h"
#include "emi_context.h"

LoopTransformer::LoopTransformer(EMIContext* context) : context(context) {
  counterIdentifier = "loopCounter";
  counterVal = 0;
  VarDeclStmt* varDeclStmt = ASTHelper::generateVarDeclStmt(ASTHelper::generateVarRef(counterIdentifier), ASTHelper::generateIntLiteral(0));
  context->injectNode(std::unique_ptr(varDeclStmt));
}

struct LoopParams {
  int initialValue;
  int step;
  char op; 
  int predRHS;
}

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

void LoopTransformer::addLoop() {
  Stmt* body = ASTHelper::generateBinOp(
                  ASTHelper::generateVarRef(counterIdentifier),
                  ASTHelper::generateBinOp(ASTHelper::generateVarRef(counterIdentifier), ASTHelper::generateIntLiteral(1), '+'),
                  '=');
  Node* current = body;

  int iterations;
  int incCount=1;
  while((iterations = rand() % 10) != 0) {
    LoopParams params = randLoopParameters(iterations);  
    std::string emiVarName = context->addInput(params.initialValue);

    std::string iteratorRef = "i" + std::to_string(i);
    Stmt* init = ASTHelper::generateVarDeclStmt(ASTHelper::generateVarRef(iteratorRef), ASTHelper::generateVarRef(emiVarName));
    Expr* cond = ASTHelper::generateBinOp(ASTHelper::generateVarRef(iteratorRef), ASTHelper::generateIntLiteral(params.predRHS), params.op);
    Expr* inc = ASTHelper::generateBinOp(
                    ASTHelper::generateVarRef(iteratorRef), 
                    ASTHelper::generateBinOp(ASTHelper::generateVarRef(iteratorRef), ASTHelper::generateIntLiteral(params.step), '+'),
                    '=');

    ForStmt* forStmt = ASTHelper::generateForStmt(init, cond, inc, current);

    current = forStmt;
    incCount *= iterations;
  }
  // TODO wrap in parallelFor Directive
  context->injectNode(current);
  counterVal += incCount;
}

int LoopTransformer::getCounterValue() {
  return counterValue;
}
