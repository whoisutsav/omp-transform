#ifndef _AST_HELPER_H
#define _AST_HELPER_H

class ASTHelper {
  public:
    static VarDeclStmt* generateVarDeclStmt(std::string ident, int val);
}
