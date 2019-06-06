#ifndef _EMI_TRANSFORMER_H
#define _EMI_TRANSFORMER_H

#include <ctime>
#include <cstdlib>
#include "ast_node.h"
#include "fuzzer.h"
using namespace std;

class EMI_Transformer {
  public:
    void insertIncrementLoop(ASTNode* blk_stmt, int n);
    void processEMI(ASTNode* root);
    void dead_code_transform(ASTNode * node);
    EMI_Transformer(int seed)
            : fuzzer(seed) 
            {srand(seed);}; 
    int expected_counter_output = 0;
    vector<int> get_inputs();
  private:
    Fuzzer fuzzer;  
    unordered_map<string, int> input_set;
    ASTNode * create_dead_fragment_modulo_input();
    ASTNode * expression_transform_add_emi(ASTNode * node);
    ASTNode * generateIncrementLoop(vector<int> iterations); 
    ASTNode * generateLoopNest(vector<int> iterations, ASTNode* body); 

    const string var_base = "emi_var";
    const string counter_name = "my_counter";
};

#endif /* _EMI_TRANSFORMER_H */
