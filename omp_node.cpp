#include <string>
#include <list>
#include <unordered_map>
#include <iostream>
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
    case OMPN_CRITICAL:
      return "critical";
    case OMPN_MASTER:
      return "master";
    case OMPN_SINGLE:
      return "single";
    default:
      return "";
  }
}

bool OMPNode::isDirective() const {
  switch(type) {
    case OMPN_PARALLEL:
    case OMPN_CRITICAL:
    case OMPN_MASTER:
      return true;
    default:
      return false;
  }
}

void OMPNode::print(ostream &o, int tabLevel) const {
      if (isDirective()) {
        // print directive
        o << string(tabLevel, '\t') << "#pragma omp " << getDirectiveName();

        // print clauses
        unordered_map<int, string>::const_iterator i;
        for (i = clauses.begin(); i != clauses.end(); ++i)
          o << " " << formatClause(i->first, i->second); 

        o << "\n" << string(tabLevel, '\t') << " {" << "\n";

        // print child nodes
        list<OMPNode *>::const_iterator j;
        for (j = children.begin(); j != children.end(); ++j)
          (*j)->print(o, tabLevel+1);
        o << string(tabLevel, '\t') << "}\n"; 

      } else {
        o << string(tabLevel, '\t') << stmt << "\n";  
      }
}

