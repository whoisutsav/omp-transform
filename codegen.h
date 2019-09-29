#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"

class Codegen {
  public:
    static std::string generate(Node* root);
    static std::string generate(int indentLevel, Node* root);

};

#endif /* _CODEGEN_H */

