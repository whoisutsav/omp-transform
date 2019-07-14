#ifndef _AST_HELPER_H
#define _AST_HELPER_H

class ASTHelper {
  public:
    static DeclStmt* createDeclStmt(VarExpr* var, Expr* assignmentExpr);
    static VarExpr* createVarExpr(std::string identifier);
    static IntLiteral* createIntLiteral(int val);
    static BinaryExpr* createBinaryExpr(Expr* LHS, Expr* RHS, char op);
    static ForStmt* createForStmt(DeclStmt* init, Expr* cond, Expr* inc, Stmt* body); 
    static Stmt* createExprStmt(Expr* expr);
};

#endif
