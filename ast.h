#ifndef _AST_H
#define _AST_H

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
  Expr* LHS;
  Expr* RHS;

  public:
    BinaryExpr(char op, Expr* LHS, Expr* RHS)
                : Op(op), LHS(LHS), RHS(RHS) {}
};

class ExprStmt : Stmt {
  Expr* Exp;

  public:
    ExprStmt(Expr* Exp) : Exp(Exp) {}

};

class CompoundStmt : Stmt {
  public:
    // TODO - it appears we have made all other AST child node private, so probably 
    // this should be as well? Or make the others public. This is just a temporary
    // hack that it seems we should fix as soon as possible 
    std::vector<Stmt*> Statements;

    CompoundStmt(std::vector<Stmt*> Statements)
            : Statements(Statements) {}
    std::vector<Stmt*>::iterator begin() {
      return Statements.begin();
    }
};

class ReturnStmt : Stmt {
  Expr* Exp;

  public:
    ReturnStmt(Expr* Exp) : Exp(Exp) {}
};

// TODO - DeclStmt and ForStmt were made to derive from public
// classes in order to support polymorphism. Change needs to either
// be propagated to all classes, or we must fix some way with private
// classes
class DeclStmt : public Stmt {
  VarExpr* Var;
  Expr* AssignmentExpr;

  public:
    DeclStmt(VarExpr* Var) : Var(Var) {}
    DeclStmt(VarExpr* Var, Expr* AssignmentExpr) 
            : Var(Var), AssignmentExpr(AssignmentExpr) {}
};

class ForStmt : public Stmt {
  DeclStmt* Init;
  Expr* Cond;
  Expr* Inc;
  Stmt* Body;

  public:
    ForStmt(DeclStmt* Init, Expr* Cond, Expr* Inc, Stmt* Body)
            : Init(Init), Cond(Cond), Inc(Inc), Body(Body) {}
};

/////////////////////////////////////////////////////////////////
// Open MP Constructs
/////////////////////////////////////////////////////////////////

class Directive : public Node {
};

class Clause;

class Parallel : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Parallel(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Single : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Single(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Critical : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Critical(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Master : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Master(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class ParallelFor : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    ParallelFor(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Target : Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Target(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

/////////////////////////////////////////////////////////////////
// Open MP Clauses 
/////////////////////////////////////////////////////////////////

class Clause {
  public:
    virtual ~Clause();
};

class IfClause : public Clause {
  Expr* Cond;

  public:
    IfClause(Expr* Cond) : Cond(Cond) {}
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
    ReductionClause(const std::string& ReduceOp, std::string& Var) : ReduceOp(ReduceOp), Var(Var) {}
};

#endif /* _AST_H */
