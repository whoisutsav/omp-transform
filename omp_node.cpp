#include <ostream>
#include <string>
#include <ostream>
#include "omp_node.h"

string OMPClause::getClause() const {
  string str;
  switch(type) {
    case OMPC_NUM_THREADS:
      str = "num_threads(" + expr + ")"; 
      break;
    case OMPC_IF:
      str = "if(" + expr + ")";
      break;
    default:
      str = "";
      break;
  }

  return str;
}

string OMPNode::getDirectiveName() const {
  switch(type) {
    case OMPN_PARALLEL:
      return "parallel";
      break;
    default:
      return "";
      break;
  }
}

bool OMPNode::isDirective() const {
  switch(type) {
    case OMPN_PARALLEL:
      return true;
    default:
      return false;
  }
}

void OMPNode::print(ostream &o) const {
      if (isDirective()) {
        // print directive
        o << "#pragma omp " << getDirectiveName();

        // print clauses
        list<OMPClause *>::const_iterator i;
        for (i = clauses.begin(); i != clauses.end(); ++i)
          o << " " << (*i)->getClause(); 

        o << " {" << "\n";

        // print child nodes
        list<OMPNode *>::const_iterator j;
        for (j = children.begin(); j != children.end(); ++j)
          (*j)->print(o);
        o << "}\n"; 

      } else {
        o << stmt << "\n";  
      }
}

