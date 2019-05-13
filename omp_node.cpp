#include <string>
#include <list>
#include <unordered_map>
#include <ostream>
#include "omp_node.h"

string formatClause(int t, string expr) {
  string result;
  switch(t) {
    case OMPC_NUM_THREADS:
      result = "num_threads(" + expr + ")"; 
      break;
    case OMPC_IF:
      result = "if(" + expr + ")";
      break;
    default:
      result = "";
      break;
  }

  return result;
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
        unordered_map<int, string>::const_iterator i;
        for (i = clauses.begin(); i != clauses.end(); ++i)
          o << " " << formatClause(i->first, i->second); 

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

