#ifndef _AST_H
#define _AST_H

// Based on LLVM tutorial: https://llvm.org/docs/tutorial/LangImpl02.html

#include <vector>
#include <string>

class OpenMpClause {};

// NodeAST - Base class for all AST nodes
class NodeAST {
  public:
    virtual ~NodeAST();
};

class ExprAST : public NodeAST {
};

class StmtAST : public NodeAST {
}; 

class OpenMpAST : public NodeAST {
};

class NumberExprAST : public ExprAST {
  int Val;

  public: 
    NumberExprAST(int Val) : Val(Val) {}
};

class VariableExprAST : public ExprAST {
  std::string Name;

  public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                                    std::unique_ptr<ExprAST> RHS)
                : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class AssignmentExprAST : public ExprAST {
  std::unique_ptr<VariableExprAST> LHS;
  std::unique_ptr<ExprAST> RHS;

  public:
    AssignmentExprAST(std::unique_ptr<VariableExprAST> LHS, std::unique_ptr<ExprAST> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}

};

class ExprStmtAST : StmtAST {
  std::unique_ptr<ExprAST> Exp;

  public:
    ExprStmtAST(std::unique_ptr<ExprAST> Exp) : Exp(std::move(Exp)) {}

};

class BlockStmtAST : StmtAST {
  std::vector<StmtAST> Statements;

  public:
    BlockStmtAST(std::vector<StmtAST> Statements)
            : Statements(std::move(Statements)) {}
};

class RetStmtAST : StmtAST {
  std::unique_ptr<ExprAST> Exp;

  public:
    RetStmtAST(std::unique_ptr<ExprAST> Exp) : Exp(std::move(Exp)) {}
};

// Only support integers as of now
class VariableDeclStmtAST : StmtAST {
  std::unique_ptr<VariableExprAST> Var;
  std::unique_ptr<ExprAST> AssignmentExpr;

  public:
    VariableDeclStmtAST(std::unique_ptr<VariableExprAST> Var) : Var(std::move(Var)) {}
    VariableDeclStmtAST(std::unique_ptr<VariableExprAST> Var, std::unique_ptr<ExprAST> AssignmentExpr) 
            : Var(std::move(Var)), AssignmentExpr(std::move(AssignmentExpr)) {}
};

class ForStmtAST : StmtAST {
  std::unique_ptr<VariableDeclStmtAST> Init;
  std::unique_ptr<ExprAST> Cond;
  std::unique_ptr<StmtAST> Step;
  std::unique_ptr<StmtAST> Body;

  public:
    ForStmtAST(std::unique_ptr<VariableDeclStmtAST> Init, std::unique_ptr<ExprAST> Cond,
                    std::unique_ptr<StmtAST> Step, std::unique_ptr<StmtAST> Body)
            : Init(std::move(Init)), Cond(std::move(Cond)), 
            Step(std::move(Step)), Body(std::move(Body)) {}
};

class OpenMpParallelAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpParallelAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class OpenMpSingleAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpSingleAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class OpenMpCriticalAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpCriticalAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class OpenMpMasterAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpMasterAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class OpenMpParallelForAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpParallelForAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class OpenMpTargetAST : OpenMpAST {
  std::vector<OpenMpClause> Clauses;
  std::unique_ptr<StmtAST> Body;

  public:
    OpenMpTargetAST(std::vector<OpenMpClause> Clauses, std::unique_ptr<StmtAST> Body)
            : Clauses(std::move(Clauses)), Body(std::move(Body)) {}
};

class ClauseOMP {
  public:
    virtual ~ClauseOMP();
};

class IfClauseOMP : public ClauseOMP {
  std::unique_ptr<ExprAST> Cond;

  public:
    IfClauseOMP(std::unique_ptr<ExprAST> Cond) : Cond(std::move(Cond)) {}
};

class NumThreadsClauseOMP : public ClauseOMP {
  int Num;

  public:
    NumThreadsClauseOMP(int num) : Num(num) {}
};

class CollapseClauseOMP : public ClauseOMP {
  int Num;

  public:
    CollapseClauseOMP(int num) : Num(num) {}
};

class ReductionClauseOMP : public ClauseOMP {
  std::string ReduceOp;
  std::string Var;

  public:
    ReductionClauseOMP(const std::string *ReduceOp, std::string *Var) : ReduceOp(ReduceOp), Var(Var) {}
};

#endif /* _AST_H */
