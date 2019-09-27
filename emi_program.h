#ifndef _EMI_PROGRAM_H
#define _EMI_PROGRAM_H

#include "ast.h"

class EmiProgram {
  public:
    static EmiProgram* generateEmptyProgram();
    EmiProgram(FunctionDecl* main);
    void injectInputAssignment(std::string identifier, int inputVal);
    void injectStmt(Stmt* stmt);
    FunctionDecl* getMain();

  private:
    FunctionDecl* main;
    std::vector<std::pair<std::string, int>> inputs;
};

#endif /* _EMI_PROGRAM_H */
