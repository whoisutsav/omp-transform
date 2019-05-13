#include <iostream>
#include <string>
#include "ast_node.h"

void OmpNode::print() const {
      string out = "#pragma omp parallel";

      if (num_threads_clause)
        out += " num_threads(5)";

      out += " {\n";

      cout << out;

      list<AstNode *>::const_iterator i;
      for (i = children.begin(); i != children.end(); ++i)
             (*i)->print(); 

      cout << "}\n"; 
}

void StmtNode::print() const {
      cout << stmt << "\n";
}
