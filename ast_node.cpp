#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "ast_node.h"


string getDirectiveName(ASTNodeType type) {
  switch(type) {
    case OMPN_PARALLEL:
      return "parallel";
    case OMPN_CRITICAL:
      return "critical";
    case OMPN_MASTER:
      return "master";
    case OMPN_SINGLE:
      return "single";
    case OMPN_PARALLEL_FOR:
      return "parallel for";
    case OMPN_TARGET:
      return "target";
    default:
      return "";
  }
}

bool ASTNode::isDirective() const {
  switch(type) {
    case OMPN_PARALLEL:
    case OMPN_CRITICAL:
    case OMPN_MASTER:
    case OMPN_SINGLE:
    case OMPN_PARALLEL_FOR:
    case OMPN_TARGET:
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
    case LT:
      return "<";
    case LTE:
      return "<=";
    case GT:
      return ">";
    case GTE:
      return ">=";
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
    case OMPC_COLLAPSE:
      o << "collapse(";
      expr->print(o, 0);
      o << ")";
      break;
    case OMPC_REDUCTION:
      o << "reduction(";
      expr->print(o, 0);
      o << ")";
      break;
    default:
      break;
  }
}

void ASTNode::print(ostream &o, int tabLevel) const {
      if (isDirective()) {
        // print directive
        o << string(tabLevel, '\t') << "#pragma omp " << getDirectiveName(type);

        // print clauses
        unordered_map<int, ASTNode*>::const_iterator i;
        for (i = clauses.begin(); i != clauses.end(); ++i) {
          o << " ";
          printClause(o, i->first, i->second); 
        }
        o << "\n";

        // print child nodes
        for (ASTNode *j: children)
          j->print(o, tabLevel);

      } else switch (type) {
        case BINOP:
          (children[0])->print(o, tabLevel);
          o << operatorToStr(op);
          (children[1])->print(o, tabLevel);
          break;
        case VAR:
          o << sval;
          break;
        case CONSTANT:
          o << ival;
          break;
        case STR_CONSTANT:
          o << sval;
          break;
        case BLK_STMT:
          o << string(tabLevel, '\t') << "{\n";
          for (ASTNode* n: children) {
            n->print(o, tabLevel+1);
            o << "\n";
          }
          o << string(tabLevel, '\t') << "}\n";
          break;
        case IF_STMT:
          o << string(tabLevel, '\t') << "if ("; 
          (children[0])->print(o, tabLevel);
          o << ")\n";
          (children[1])->print(o, tabLevel);
          break;
        case FOR_STMT:
          o << string(tabLevel, '\t') << "for ("; 
          (children[0])->print(o, 0);
          o << ";";
          (children[1])->print(o, 0);
          o << ";";
          (children[2])->print(o, 0);
          o << ")\n";
          (children[3])->print(o, tabLevel+1);
          break;
        case VDECL:
          o << ctypeToStr(ctype) << " ";
          (children[0])->print(o, 0);
          break;
        case ASSIGN_EXP:
          (children[0])->print(o, 0);
          o << " = ";
          (children[1])->print(o, 0);
          break;
        case EXP_STMT:
          o << string(tabLevel, '\t');
          (children[0])->print(o, 0);
          o << ";"; 
          break;
        case RET_STMT:
          o << string(tabLevel, '\t') << "return ";
          (children[0])->print(o, 0);
          o << ";";
          break;
        // TODO remove
        case CUSTOM:
          o << string(tabLevel, '\t') << sval;
          break;
        default:
          break;
      }
}

