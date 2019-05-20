#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "ast_node.h"


string getDirectiveName(ASTNodeType node_type) {
  switch(node_type) {
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

bool ASTNode::isDirective() const {
  switch(node_type) {
    case OMPN_PARALLEL:
    case OMPN_CRITICAL:
    case OMPN_MASTER:
      return true;
    default:
      return false;
  }
}

string operatorToStr(Operator op) {
  switch(op) {
    case ADD:
      return "+";
    case MINUS:
      return "-";
    case MULT:
      return "*";
    case DIV:
      return "\\";
    default:
      return "";
  }
}

string ctypeToStr(CType ctype) {
  switch(ctype) {
    case CHAR:
      return "char";
    case SHORT:
      return "short";
    case INT:
      return "int";
    case LONG:
      return "long";
    default:
      return "";
  }
}

void printClause(ostream &o, int t, ASTNode * expr) {
  switch(t) {
    case OMPC_NUM_THREADS:
      o << "num_threads("; 
      expr->print(o, 0);
      o << ")";
      break;
    case OMPC_IF:
      o << "if(";
      expr->print(o, 0);
      o << ")";
      break;
    default:
      break;
  }
}

// TODO split string generation/stream printing
void ASTNode::print(ostream &o, int tabLevel) const {
      if (isDirective()) {
        // print directive
        o << string(tabLevel, '\t') << "#pragma omp " << getDirectiveName(node_type);

        // print clauses
        unordered_map<int, ASTNode*>::const_iterator i;
        for (i = clauses.begin(); i != clauses.end(); ++i) {
          o << " ";
          printClause(o, i->first, i->second); 
        }
        o << "\n";

        // TODO can there be more than one child?
        // print child nodes
        for (ASTNode *j: children)
          j->print(o, tabLevel);

      } else switch (node_type) {
        case UNOP:
          // TODO 
          break;
        case BINOP:
          (children[0])->print(o, tabLevel);
          o << operatorToStr(op);
          (children[0])->print(o, tabLevel);
          break;
        case VAR:
          o << sval;
          break;
        case CONSTANT:
          o << ival;
          break;
        case BLK_STMT:
          o << string(tabLevel, '\t') << "{\n";
          for (ASTNode* n: children)
            n->print(o, tabLevel+1);
          o << string(tabLevel, '\t') << "}\n";
          break;
        case IF_STMT:
          o << string(tabLevel, '\t') << "if ("; 
          (children[0])->print(o, tabLevel);
          o << ")\n";
          (children[1])->print(o, tabLevel);
          break;
        case VDECL:
          o << ctypeToStr(ctype) << " ";
          (children[0])->print(o, 0);
          break;
        case ASSIGN_STMT:
          o << string(tabLevel, '\t');
          (children[0])->print(o, 0);
          o << " = ";
          (children[1])->print(o, 0);
          o << ";\n" ;
          break;
        case EXP_STMT:
          o << string(tabLevel, '\t');
          (children[0])->print(o, 0);
          o << ";\n"; 
          break;
        case RET_STMT:
          o << string(tabLevel, '\t') << "return ";
          (children[0])->print(o, 0);
          o << ";\n";
          break;
        default:
          break;
      }
}

