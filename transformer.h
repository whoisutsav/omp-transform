#ifndef _TRANSFORMER_H
#define _TRANSFORMER_H

#include <ctime>
#include <cstdlib>
#include "omp_node.h"
using namespace std;

class Transformer {
  public:
    void transform(OMPNode* root);
    Transformer(float alter, float compose, float append) 
            : alterPct{alter}, composePct{compose}, appendPct{append}
            {srand(time(0));}; 

  private:
    const float alterPct; 
    const float composePct; 
    const float appendPct;
};

#endif /* _TRANSFORMER_H */
