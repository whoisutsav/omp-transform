#ifndef _OMP_NODE_H
#define _OMP_NODE_H

#include <list>
#include <string>
#include <ostream>
using namespace std;

enum ClauseType { OMPC_NUM_THREADS, OMPC_IF};

class OMPClause {
  public:
    ClauseType type;
    string expr;

    string getClause() const;
};

enum NodeType { OMPN_PARALLEL, OMPN_STMT };

class OMPNode {
  public:
    NodeType type;

    list <OMPNode *> children;
    list <OMPClause *> clauses;

    string stmt;

    bool isDirective() const;
    string getDirectiveName() const;
    void print(ostream &o) const;
};


#endif /* _OMP_NODE_H */
