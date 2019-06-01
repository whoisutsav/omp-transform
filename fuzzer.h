#ifndef _FUZZER_H
#define _FUZZER_H

#include <cstdlib>
#include "ast_node.h"
using namespace std;

class Fuzzer {
  public:
    ASTNode* generateFragment();

    // TODO move into helper class
    static ASTNode* getConstExpr(int val);

    Fuzzer(int seed) {srand(seed);}; 

  private:
    void transform(ASTNode* root);
    const float alterPct = 0.7; 
    const float composePct = 0.7; 
    const float appendPct = 0.7;
};

#endif /* _FUZZER_H */