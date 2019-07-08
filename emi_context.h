#ifndef _EMI_CONTEXT_H
#define _EMI_CONTEXT_H

#include <ctime>
#include <cstdlib>
#include "ast.h"

class EMIContext {
  public:
    EMIContext(Node* root, CompoundStmt* main) : root(root), main(main) {}
    std::string addInput(int val);
    Node* getRoot();
    CompoundStmt* getMain();
    Node* generateEMIAST();

  private:
    Node* root;
    CompoundStmt* main;
    vector<std::pair<string, int>> inputs;
    int numGenerated;
};

#endif /* _EMI_CONTEXT_H */
