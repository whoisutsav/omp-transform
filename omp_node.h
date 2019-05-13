#ifndef _OMP_NODE_H
#define _OMP_NODE_H

#include <list>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

enum OMPClauseType { OMPC_NUM_THREADS, OMPC_IF };

enum NodeType { OMPN_PARALLEL, OMPN_CRITICAL, OMPN_STMT };

class OMPNode {
  public:
    NodeType type;

    list <OMPNode *> children;

    // Indexed by OMPClauseType (int)
    unordered_map<int, string> clauses;

    string stmt;

    bool isDirective() const;
    string getDirectiveName() const;
    void print(ostream &o, int tabLevel) const;
};


#endif /* _OMP_NODE_H */
