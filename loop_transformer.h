#ifndef _LOOP_TRANSFORMER_H
#define _LOOP_TRANSFORMER_H

#include "emi_context.h"

class LoopTransformer {
  public:
    LoopTransformer(EMIContext* context); 
    void addLoop();
    int getCounterValue();

  private:
    EMIContext* context;
    std::string counterIdentifier;
    int counterVal;
}

#endif /* _LOOP_TRANSFORMER_H */
