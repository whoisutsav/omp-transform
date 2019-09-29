#ifndef _LOOP_INJECTOR_H
#define _LOOP_INJECTOR_H 

#include "program_wrapper.h"

class LoopInjector {
  public:
    static void injectOmpLoopAndCounterAssertion(ProgramWrapper* program, bool useTarget, int seed); 
};

#endif /* _LOOP_INJECTOR_H */
