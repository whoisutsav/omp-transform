#ifndef _LOOP_TRANSFORMER_H
#define _LOOP_TRANSFORMER_H

#include "emi_program.h"

class LoopTransformer {
  public:
    LoopTransformer(EmiProgram* program); 
    void addLoop();
    int getCounterValue();
    // CHANGE TO:
    //static void addLoop(EmiProgram* program, std::string counterName, int numIterations);
    // (Or return number of iterations)

  private:
    EmiProgram* program;
    std::string counterIdentifier;
    int counterVal;
};

#endif /* _LOOP_TRANSFORMER_H */
