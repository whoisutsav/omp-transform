#ifndef _PROGRAM_WRAPPER_H
#define _PROGRAM_WRAPPER_H

#include "ast.h"

class ProgramWrapper {
  public:
    ProgramWrapper(FunctionDecl* main);
    static ProgramWrapper* create();

    Expr* addExpectedInput(int value);
    FunctionDecl* getMain();
    std::vector<int> getExpectedInputs();

  private:
    FunctionDecl* main;
    std::vector<int> expectedInputs;
};

#endif /* _PROGRAM_WRAPPER_H */
