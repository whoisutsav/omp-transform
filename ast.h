// Adapted from Clang AST (https://clang.llvm.org)

#ifndef _AST_H
#define _AST_H

#include <vector>
#include <string>

class Node {
  public:
    virtual ~Node() = default;
};

class Expr : public Node {
};

class Stmt : public Node {
}; 


class IntLiteral : public Expr {
  int Val;

  public: 
    IntLiteral(int Val) : Val(Val) {}
    static IntLiteral* create(int Val) { return new IntLiteral(Val); }
    int getValue() { return Val; }
    
};

class VarExpr : public Expr {
  std::string Name;

  public:
    VarExpr(const std::string &Name) : Name(Name) {}
    static VarExpr* create(std::string Name) { return new VarExpr(Name); }
    std::string getName() { return Name; }
};

class ArrayExpr : public Expr {
  Expr* Base;
  Expr* Index;

  public:
    ArrayExpr(Expr* Base, Expr* Index) : Base(Base), Index(Index) {}
    static ArrayExpr* create(Expr* Base, Expr* Index) { return new ArrayExpr(Base, Index); } 
    Expr* getBase() { return Base; }
    Expr* getIndex() { return Index; }
};

class BinaryExpr : public Expr {
  char Op;
  Expr* LHS;
  Expr* RHS;

  public:
    BinaryExpr(Expr* LHS, Expr* RHS, char Op)
                : Op(Op), LHS(LHS), RHS(RHS) {}
    static BinaryExpr* create(Expr* LHS, Expr* RHS, char Op) { return new BinaryExpr(LHS, RHS, Op); }
    Expr* getLHS() { return LHS; }
    Expr* getRHS() { return RHS; }
    char getOp() { return Op; }
};

class CallExpr: public Expr {
  std::string Name;
  std::vector<Expr*> Args;

  public:
    CallExpr(const std::string &Name, std::vector<Expr*> Args) : Name(Name), Args(std::move(Args)) {}
    static CallExpr* create(std::string Name, std::vector<Expr*> Args) { return new CallExpr(Name, Args); }
    std::string getName() { return Name; }
    std::vector<Expr*> getArgs() { return Args; }
};

class ExprStmt : public Stmt {
  Expr* Exp;

  public:
    ExprStmt(Expr* Exp) : Exp(Exp) {}
    static ExprStmt* create(Expr* Exp) { return new ExprStmt(Exp); }
    Expr* getExpr() { return Exp; }

};

class CompoundStmt : public Stmt {
    std::vector<Stmt*> Statements;

  public:
    CompoundStmt(std::vector<Stmt*> Statements)
            : Statements(std::move(Statements)) {}
    static CompoundStmt* create(std::vector<Stmt*> Statements) { return new CompoundStmt(Statements); }
    std::vector<Stmt*>& getStatements() { return Statements; }
};

class ReturnStmt : public Stmt {
  Expr* Exp;

  public:
    ReturnStmt(Expr* Exp) : Exp(Exp) {}
    Expr* getRetValue() { return Exp; }
};

class DeclStmt : public Stmt {
  std::string Type;
  VarExpr* Var;
  Expr* Init;

  public:
    DeclStmt(std::string Type, VarExpr* Var) : Type(Type), Var(Var) {}
    DeclStmt(std::string Type, VarExpr* Var, Expr* Init) 
            : Type(Type), Var(Var), Init(Init) {}
    static DeclStmt* create(std::string Type, VarExpr* Var, Expr* Init) { return new DeclStmt(Type, Var, Init); }
    std::string getType() { return Type; }
    VarExpr* getVar() { return Var; }
    Expr* getValue() { return Init; }
};

class ForStmt : public Stmt {
  DeclStmt* Init;
  Expr* Cond;
  Expr* Inc;
  Stmt* Body;

  public:
    ForStmt(DeclStmt* Init, Expr* Cond, Expr* Inc, Stmt* Body)
            : Init(Init), Cond(Cond), Inc(Inc), Body(Body) {}
    static ForStmt* create(DeclStmt* Init, Expr* Cond, Expr* Inc, Stmt* Body) { return new ForStmt(Init, Cond, Inc, Body); }
    DeclStmt* getInit() { return Init; }
    Expr* getCond() { return Cond; }
    Expr* getInc() { return Inc; }
    Stmt* getBody() { return Body; }
};

class ParamDecl : public Node {
  std::string Type;
  VarExpr* Var;

  public:
    ParamDecl(std::string Type, VarExpr* Var) : Type(Type), Var(Var) {}
    static ParamDecl* create(std::string Type, VarExpr* Var) { return new ParamDecl(Type, Var); }
    std::string getType() { return Type; }
    VarExpr* getVar() { return Var; }
};

class FunctionDecl : public Stmt {
  std::string ReturnType;
  std::string Name;
  std::vector<ParamDecl*> Params;
  CompoundStmt* Body;

  public:
    FunctionDecl(std::string ReturnType, std::string Name, std::vector<ParamDecl*> Params, CompoundStmt* Body)
            : ReturnType(ReturnType), Name(Name), Params(std::move(Params)), Body(Body) {}
    static FunctionDecl* create(std::string ReturnType, std::string Name, std::vector<ParamDecl*> Params, CompoundStmt* Body) { return new FunctionDecl(ReturnType, Name, Params, Body); }
    std::string getReturnType() { return ReturnType; }
    std::string getName() { return Name; }
    std::vector<ParamDecl*>& getParams() { return Params; }
    CompoundStmt* getBody() { return Body; }
};

/////////////////////////////////////////////////////////////////
// Open MP Constructs
/////////////////////////////////////////////////////////////////

class Directive : public Stmt {
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
            : Clauses(std::move(Clauses)), Body(Body) {}
    static ParallelFor* create(std::vector<Clause*> Clauses, Stmt* Body) { return new ParallelFor(Clauses, Body); }
    Stmt* getBody() { return Body; }
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
