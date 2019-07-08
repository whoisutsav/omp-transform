#ifndef __H
#define __H

#include <vector>
#include <string>

class Node {
  public:
    virtual ~Node();
};

class Expr : public Node {
};

class Stmt : public Node {
}; 


class IntLiteral : public Expr {
  int Val;

  public: 
    IntLiteral(int Val) : Val(Val) {}
};

class VarExpr : public Expr {
  std::string Name;

  public:
    VarExpr(const std::string &Name) : Name(Name) {}
};

class BinaryExpr : public Expr {
  char Op;
  std::unique_ptr<Expr> LHS, RHS;

  public:
    BinaryExpr(char op, std::unique_ptr<Expr> LHS,
                                    std::unique_ptr<Expr> RHS)
                : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class ExprStmt : Stmt {
  std::unique_ptr<Expr> Exp;

  public:
    ExprStmt(std::unique_ptr<Expr> Exp) : Exp(std::move(Exp)) {}

};

class CompoundStmt : Stmt {
  std::vector<std::unique_ptr<Stmt>> Statements;

  public:
    CompoundStmt(std::vector<std::unique_ptr<Stmt>> Statements)
            : Statements(std::move(Statements)) {}
};

class ReturnStmt : Stmt {
  std::unique_ptr<Expr> Exp;

  public:
    ReturnStmt(std::unique_ptr<Expr> Exp) : Exp(std::move(Exp)) {}
};

class DeclStmt : Stmt {
  std::unique_ptr<VarExpr> Var;
  std::unique_ptr<Expr> AssignmentExpr;

  public:
    DeclStmt(std::unique_ptr<VarExpr> Var) : Var(std::move(Var)) {}
    DeclStmt(std::unique_ptr<VarExpr> Var, std::unique_ptr<Expr> AssignmentExpr) 
            : Var(std::move(Var)), AssignmentExpr(std::move(AssignmentExpr)) {}
};

class ForStmt : Stmt {
  std::unique_ptr<DeclStmt> Init;
  std::unique_ptr<Expr> Cond;
  std::unique_ptr<Expr> Inc;
  std::unique_ptr<Stmt> Body;

  public:
    ForStmt(std::unique_ptr<DeclStmt> Init, std::unique_ptr<Expr> Cond,
                    std::unique_ptr<Stmt> Inc, std::unique_ptr<Stmt> Body)
            : Init(std::move(Init)), Cond(std::move(Cond)), 
            Inc(std::move(Inc)), Body(std::move(Body)) {}
};

/////////////////////////////////////////////////////////////////
// Open MP Constructs
/////////////////////////////////////////////////////////////////

class Directive : public Node {
};

class Clause {};

class Parallel : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    Parallel(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class Single : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    Single(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class Critical : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    Critical(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class Master : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    Master(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class ParallelFor : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    ParallelFor(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class Target : Directive {
  std::vector<std::unique_ptr<Clause>> Clauses;
  std::unique_ptr<Stmt> Body;

  public:
    Target(std::vector<std::unique_ptr<Clause>> Clauses, std::unique_ptr<Stmt> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class Clause {
  public:
    virtual ~Clause();
};

class IfClause : public Clause {
  std::unique_ptr<Expr> Cond;

  public:
    IfClause(std::unique_ptr<Expr> Cond) : Cond(std::move(Cond)) {}
};

class NumThreadsClause : public Clause {
  int Num;

  public:
    NumThreadsClause(int num) : Num(num) {}
};

class CollapseClause : public Clause {
  int Num;

  public:
    CollapseClause(int num) : Num(num) {}
};

class ReductionClause : public Clause {
  std::string ReduceOp;
  std::string Var;

  public:
    ReductionClause(const std::string *ReduceOp, std::string *Var) : ReduceOp(ReduceOp), Var(Var) {}
};

#endif /* __H */
