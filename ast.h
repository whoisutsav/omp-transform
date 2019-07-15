// Adapted from Clang AST (https://clang.llvm.org)

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
    static IntLiteral* create(int Val);
    int getValue() { return Val; }
    
};

class VarExpr : public Expr {
  std::string Name;

  public:
    VarExpr(const std::string &Name) : Name(Name) {}
    static VarExpr* create(std::string Name);
    std::string getName() { return Name; }
};

class BinaryExpr : public Expr {
  char Op;
  Expr* LHS;
  Expr* RHS;

  public:
    BinaryExpr(char op, Expr* LHS, Expr* RHS)
                : Op(op), LHS(LHS), RHS(RHS) {}
    static BinaryExpr* create(Expr* LHS, Expr* RHS, char op);
    Expr* getLHS() { return LHS; }
    Expr* getRHS() { return RHS; }
    char getOp() { return Op; }
};

class ExprStmt : public Stmt {
  Expr* Exp;

  public:
    ExprStmt(Expr* Exp) : Exp(Exp) {}
    static ExprStmt* create(Expr* expr);
    Expr* getExpr() { return Exp; }

};

class CompoundStmt : public Stmt {
    std::vector<Stmt*> Statements;

  public:
    CompoundStmt(std::vector<Stmt*> Statements)
            : Statements(Statements) {}
    std::vector<Stmt*>::iterator begin() {
      return Statements.begin();
    }
    std::vector<Stmt*> getStatements() { return Statements; }
};

class ReturnStmt : public Stmt {
  Expr* Exp;

  public:
    ReturnStmt(Expr* Exp) : Exp(Exp) {}
    Expr* getRetValue() { return Exp; }
};

class DeclStmt : public Stmt {
  VarExpr* Var;
  Expr* AssignmentExpr;

  public:
    DeclStmt(VarExpr* Var) : Var(Var) {}
    DeclStmt(VarExpr* Var, Expr* AssignmentExpr) 
            : Var(Var), AssignmentExpr(AssignmentExpr) {}
    static DeclStmt* create(VarExpr* var, Expr* assignmentExpr);
    VarExpr* getVar() { return Var; }
    Expr* getValue() { return AssignmentExpr; }

};

class ForStmt : public Stmt {
  DeclStmt* Init;
  Expr* Cond;
  Expr* Inc;
  Stmt* Body;

  public:
    ForStmt(DeclStmt* Init, Expr* Cond, Expr* Inc, Stmt* Body)
            : Init(Init), Cond(Cond), Inc(Inc), Body(Body) {}
    static ForStmt* create(DeclStmt* init, Expr* cond, Expr* inc, Stmt* body);
    DeclStmt* getInit() { return Init; }
    Expr* getCond() { return Cond; }
    Expr* getInc() { return Inc; }
    Stmt* getBody() { return Body; }
};

/////////////////////////////////////////////////////////////////
// Open MP Constructs
/////////////////////////////////////////////////////////////////

class Directive : public Node {
};

class Clause;

class Parallel : public Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Parallel(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Single : public Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Single(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Critical : public Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Critical(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Master : public Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    Master(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class ParallelFor : public Directive {
  std::vector<Clause*> Clauses;
  Stmt* Body;

  public:
    ParallelFor(std::vector<Clause*> Clauses, Stmt* Body)
            : Clauses(Clauses), Body(Body) {}
};

class Target : public Directive {
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
