#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include "ast.h"

class CodeGenerator {
  public:
    static std::string generate(Node* root);

};

#endif /* _CODE_GENERATOR_H */

