#ifndef _AST_NODE_H
#define _AST_NODE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

enum OMPClauseType { OMPC_NUM_THREADS, OMPC_IF, OMPC_COLLAPSE };
enum Operator { ADD, MINUS, MULT, DIV, LT, GT, LTE, GTE };
enum CType { CHAR, SHORT, INT, LONG }; 

enum ASTNodeType { UNOP, BINOP, VAR, CONSTANT, IF_STMT, FOR_STMT, ASSIGN_STMT, EXP_STMT, BLK_STMT, RET_STMT, VDECL, OMPN_PARALLEL, OMPN_SINGLE, OMPN_CRITICAL, OMPN_MASTER, OMPN_FOR };

class ASTNode {
  public:
    ASTNodeType type;
    Operator op;
    CType ctype;

    // CONSTANT value
    int ival;

    // VAR name
    string sval;

    // BINOP:  [0] = op1, [1] = op2
    // UNOP:  [0] = op1
    // IF: [0] = pred-exp, [1] = body
    // VDECL: [0] = var
    // ASSIGN_STMT: [0] = var/vdecl, [1] = exp
    // EXP_STMT: [0] = exp
    // BLK_STMT: [0],[1],... = stmt
    // RET_STMT: [0] = expr
    // FOR_STMT: [0] = assign_stmt, [1] = pred, [2] = assign_stmt, [3] = blk_stmt 
    vector <ASTNode *> children;

    unordered_map<int, ASTNode*> clauses; // Indexed by OMPClauseType (int)

    bool isDirective() const;
    void print(ostream &o, int tabLevel) const;
};


#endif /* _AST_NODE_H */
