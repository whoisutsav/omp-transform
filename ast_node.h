#ifndef _AST_NODE_H
#define _AST_NODE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

enum OMPClauseType { OMPC_NUM_THREADS, OMPC_IF, OMPC_COLLAPSE, OMPC_REDUCTION };
enum Operator { ADD, MINUS, MULT, DIV, LT, GT, LTE, GTE };
enum CType { CHAR, SHORT, INT, LONG }; 

enum ASTNodeType { UNOP, BINOP, VAR, CONSTANT, STR_CONSTANT, IF_STMT, FOR_STMT, ASSIGN_EXP, EXP_STMT, BLK_STMT, RET_STMT, VDECL, CUSTOM, OMPN_PARALLEL, OMPN_SINGLE, OMPN_CRITICAL, OMPN_MASTER, OMPN_PARALLEL_FOR, OMPN_TARGET};

class ASTNode {
  public:
    ASTNodeType type;
    Operator op;
    CType ctype;

    // CONSTANT value
    int ival;

    // VAR name
    // STR_CONSTANT value
    // CUSTOM statement
    string sval;

    // BINOP:  [0] = op1, [1] = op2
    // UNOP:  [0] = op1
    // IF: [0] = pred-exp, [1] = body
    // VDECL: [0] = var
    // ASSIGN_EXP: [0] = var/vdecl, [1] = exp
    // EXP_STMT: [0] = exp
    // BLK_STMT: [0],[1],... = stmt
    // RET_STMT: [0] = expr
    // FOR_STMT: [0] = assign_exp, [1] = pred, [2] = assign_exp, [3] = blk_stmt 
    vector <ASTNode *> children;

    unordered_map<int, ASTNode*> clauses; // Indexed by OMPClauseType (int)

    bool isDirective() const;
    void print(ostream &o, int tabLevel) const;
};


#endif /* _AST_NODE_H */
