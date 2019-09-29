#include "loop_injector.h"
#include "ast.h"
#include "program_wrapper.h"
#include <math.h>

struct ForLoop {
  int initialValue;
  int step;
  std::string condOp; 
  int condValue;
};

ForLoop randLoop(int iterations) {
  switch(rand() % 7) {
    case 0:  /* Decrement from int max */
      return {INT_MAX, -1, ">", INT_MAX - iterations};
    case 1: /* Decrement to int min */
      return {INT_MIN + iterations, -1, ">", INT_MIN};
    case 2: /* Increment from int min */
      return {INT_MIN, 1, "<", INT_MIN + iterations};
    case 3: /* Increment to int max */
      return {INT_MAX - iterations, 1, "<", INT_MAX};
    case 4: /* Increment across zero */
      return {-1*(iterations/2), 1, "<", -1*(iterations/2) + iterations}; 
    case 5: /* Decrement across zero */
      return {(iterations/2), -1, ">", (iterations/2) - iterations}; 
    default: /* Increment from zero */
      return {0, 1, "<", iterations};
  }
}

DeclStmt* init(std::string var, Expr* init) {
   return DeclStmt::create( "int", VarExpr::create(var), init);
}

Expr* increment(std::string var, int step) {
    return BinaryExpr::create(VarExpr::create(var), BinaryExpr::create(VarExpr::create(var), IntLiteral::create(step), "+"), "=");
}

Expr* cond(std::string var, std::string condOp, int condValue) {
    return BinaryExpr::create(VarExpr::create(var), IntLiteral::create(condValue), condOp);
}

ForStmt* nest(Stmt* body, std::string counter, Expr* initExp, int step, std::string condOp, int condValue) {
    return ForStmt::create( 
                      init(counter, initExp), 
                      cond(counter, condOp, condValue),
                      increment(counter, step),
                    body);
}

Stmt* accumulateBody(std::string accumulator) {
  return Atomic::create(ExprStmt::create(increment(accumulator, 1)));
}


Expr* assertCall(std::string var, int val) {
  return CallExpr::create("assert", {BinaryExpr::create(VarExpr::create(var), IntLiteral::create(val), "==")});
}

Stmt* loopNest(ProgramWrapper* prog, Stmt* body, int nestDepth, int iterations) {
  for(int i=0; i<nestDepth; i++) {
    ForLoop loop = randLoop(iterations);

    std::string counter = "i" + std::to_string(i);
    Expr* init = prog->addExpectedInput(loop.initialValue);
    body = nest(body, counter, init, loop.step, loop.condOp, loop.condValue); 
  }

  return body;
} 

Stmt* parallelFor(Stmt* body, int nestDepth, std::string accumulator) {
  std::vector<Clause*> clauses;
  switch(rand() % 2) {
    case 1:
      clauses.push_back(CollapseClause::create(nestDepth));
      break;
    default:
      clauses.push_back(ReductionClause::create("+", accumulator)); 
  }
  return ParallelFor::create(clauses, body); 
}

Stmt* target(Stmt* body) {
  return Target::create({}, body);
}

void LoopInjector::injectOmpLoopAndCounterAssertion(ProgramWrapper* prog, bool useTarget, int seed) {
  srand(seed);
  int nestDepth = (rand() % 10) + 3;
  int iterations = 3;

  /* Accumulator */

  std::string accumulator = "myTotal";
  Stmt* accumulatorInit = init(accumulator, IntLiteral::create(0));

  /* Loop body */

  Stmt* body = accumulateBody(accumulator);
  body = loopNest(prog, body, nestDepth, iterations);
  body = parallelFor(body, nestDepth, accumulator);
  if (useTarget) body = target(body);

  /* Assertion */
  Stmt* assert = ExprStmt::create(assertCall(accumulator, pow(iterations, nestDepth)));

  /* Inject */
  prog->getMain()->getBody()->getStatements().push_back(accumulatorInit);
  prog->getMain()->getBody()->getStatements().push_back(body);
  prog->getMain()->getBody()->getStatements().push_back(assert);
}

