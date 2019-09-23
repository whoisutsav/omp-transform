#ifndef _EMI_PROGRAM_H
#define _EMI_PROGRAM_H

#include "ast.h"

class EmiProgram {
  public:
    static EmiProgram* generateEmptyProgram();
    EmiProgram(CompoundStmt* root, CompoundStmt* mainBody);
    void injectInputAssignment(std::string identifier, int inputVal);
    void injectStmt(Stmt* stmt);

  private:
    CompoundStmt* root;
    CompoundStmt* mainBody;
    std::vector<Stmt*>::iterator current;
    std::vector<std::pair<std::string, int>> inputs;
};

#endif /* _EMI_PROGRAM_H */
