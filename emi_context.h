#ifndef _EMI_CONTEXT_H
#define _EMI_CONTEXT_H

#include <ctime>
#include <cstdlib>
#include "ast.h"

class EMIContext {
  public:
    EMIContext(Node* root, CompoundStmt* main);
    std::string addInput(int val);
    void injectStmt(Stmt* stmt);

  private:
    Node* root;
    CompoundStmt* main;
    std::vector<std::pair<std::string, int>> inputs;
    std::vector<std::unique_ptr<Stmt>>::iterator current;
};

#endif /* _EMI_CONTEXT_H */
