#ifndef _LOOP_TRANSFORMER_H
#define _LOOP_TRANSFORMER_H

#include "emi_program.h"

class LoopTransformer {
  public:
    static void emiLoopTransform(EmiProgram* program, std::string counterName, int numIterations); 
};

#endif /* _LOOP_TRANSFORMER_H */
