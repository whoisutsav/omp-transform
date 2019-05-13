#ifndef _AST_NODE_H
#define _AST_NODE_H

#include <list>
#include <string>
using namespace std;

enum NodeType { OMP_PARALLEL, STMT };

class AstNode {
  public:
    NodeType type;
    virtual void print() const = 0;
};

class OmpNode : public AstNode {
  public:
    list <AstNode*> children;
    bool num_threads_clause;
    int num_threads_value;
    void print() const;
};

class StmtNode : public AstNode {
  public:
    string stmt;
    void print() const;
};

#endif /* _AST_NODE_H */
