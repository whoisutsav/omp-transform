#ifndef _EMI_TRANSFORMER_H
#define _EMI_TRANSFORMER_H

#include <ctime>
#include <cstdlib>
#include "ast_node.h"
#include "fuzzer.h"
using namespace std;

class EMI_Transformer {
  public:
    void transform(ASTNode* root);
    void initializeCounter(ASTNode* root);
    void insertIncrementLoopCollapse(ASTNode* blk_stmt, int n);
    void insertEmptyIncrementLoop(ASTNode * blk_stmt);
    void processEMI(ASTNode* root);
    EMI_Transformer(int seed)
            : fuzzer(seed) 
            {srand(seed);}; 
  private:
    Fuzzer fuzzer;  
    unordered_map<string, int> input_set;
    void dead_code_transform(ASTNode * node);
    ASTNode * create_dead_fragment_modulo_input();
    void expression_transform(ASTNode * node);
    ASTNode * generateIncrementLoopCollapse(int total); 
    ASTNode * generateLoopNest(vector<int> iterations, ASTNode* body); 

    const string var_base = "emi_var";
    const string counter_name = "my_counter";
};

#endif /* _EMI_TRANSFORMER_H */
