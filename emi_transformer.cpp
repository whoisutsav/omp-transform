#include <ctime>
#include <cstdlib>
#include <climits>
#include "emi_transformer.h"
#include "fuzzer.h"
#include "ast_node.h"

// TODO allow more flexibility when num_iterations = 0
tuple <int, int, Operator, int> decFromIntMax(int num_iterations) {
  int initial_value = INT_MAX;
  int step = -1;
  Operator op = GT;
  int pred_rhs = initial_value - num_iterations;

  return make_tuple(initial_value, step, op, pred_rhs); 
}

tuple <int, int, Operator, int> incFromIntMin(int num_iterations) {
  int initial_value = INT_MIN;
  int step = 1;
  Operator op = LT;
  int pred_rhs = initial_value + num_iterations;

  return make_tuple(initial_value, step, op, pred_rhs); 
}

tuple <int, int, Operator, int> endAtIntMax(int num_iterations) {
  int initial_value = INT_MAX - num_iterations;
  int step = 1;
  Operator op = LT;
  int pred_rhs = INT_MAX;

  return make_tuple(initial_value, step, op, pred_rhs); 
}

tuple <int, int, Operator, int> endAtIntMin(int num_iterations) {
  int initial_value = INT_MIN + num_iterations;
  int step = -1;
  Operator op = GT;
  int pred_rhs = INT_MIN;

  return make_tuple(initial_value, step, op, pred_rhs); 
}

tuple <int, int, Operator, int> standardParams(int num_iterations) {
  int initial_value = 0;
  int step = 1;
  Operator op = LT;
  int pred_rhs = num_iterations;

  return make_tuple(initial_value, step, op, pred_rhs); 
}

// initial value, step, predicate operator, predicate rhs 
tuple <int, int, Operator, int> generateLoopParameters(int num_iterations) {
  switch(rand() % 5) {
    case 0:
      return decFromIntMax(num_iterations);
    case 1:
      return incFromIntMin(num_iterations);
    case 2:
      return endAtIntMax(num_iterations);
    case 3:
      return endAtIntMin(num_iterations);
    case 4:
    default:
      return standardParams(num_iterations);
  }
}

// iterations[0] = iterations of innermost loop, iterations[1] = level higher, etc.
ASTNode * EMI_Transformer::generateLoopNest(vector<int> iterations, ASTNode* body) {
  ASTNode * current = body;

  for(int n: iterations) {
    tuple <int, int, Operator, int> params = generateLoopParameters(n);  

    string emiVarName = var_base + to_string(input_set.size() + 1); // TODO - fix
    int emiVarVal = get<0>(params);

    input_set.insert({emiVarName, emiVarVal});

    // initialize loop counter node (using EMI) 
    ASTNode * loop_counter_var = new ASTNode(); 
    loop_counter_var->type = VAR; 
    loop_counter_var->sval = emiVarName;

    ASTNode * loop_counter_initialization = new ASTNode();
    loop_counter_initialization->type = EXP_STMT;
    loop_counter_initialization->children.push_back(loop_counter_var);

    // initialize loop predicate node 
    ASTNode * predicate_var = new ASTNode();
    predicate_var->type = VAR;
    predicate_var->sval = emiVarName;

    ASTNode * predicate_constant = new ASTNode();
    predicate_constant->type = CONSTANT;
    predicate_constant->ival = get<3>(params); 

    ASTNode * pred_expr = new ASTNode(); 
    pred_expr->type = BINOP;
    pred_expr->op = get<2>(params);
    pred_expr->children.push_back(predicate_var);
    pred_expr->children.push_back(predicate_constant);

    ASTNode * pred_stmt = new ASTNode(); 
    pred_stmt->type = EXP_STMT;
    pred_stmt->children.push_back(pred_expr);

    // initialize loop step statement 
    ASTNode * step_constant = new ASTNode();
    step_constant->type = CONSTANT; 
    step_constant->ival = get<1>(params); 

    ASTNode * step_var = new ASTNode();
    step_var->type = VAR;
    step_var->sval = emiVarName;

    ASTNode * step_expr = new ASTNode();
    step_expr->type = BINOP;
    step_expr->op = ADD;
    step_expr->children.push_back(step_constant);
    step_expr->children.push_back(step_var);

    ASTNode * step_assignment = new ASTNode();
    step_assignment->type = ASSIGN_STMT;
    step_assignment->children.push_back(step_var);
    step_assignment->children.push_back(step_expr);

    ASTNode * for_loop = new ASTNode();
    for_loop->type = FOR_STMT;
    for_loop->children.push_back(loop_counter_initialization);
    for_loop->children.push_back(pred_stmt);  
    for_loop->children.push_back(step_assignment);
    for_loop->children.push_back(current); 

    current = for_loop;
  }

  return current;
}

ASTNode * EMI_Transformer::generateIncrementLoopCollapse(int total) {

  // Generate random loop structure
  // For now, all outer loops have single iteration 
  // Inner loop has `total` iterations
  vector<int> iterations;
  iterations.push_back(total);
  for(int i=1; i < (rand() % 5) + 1; i++) {
    iterations.push_back(1);
  } 

  ASTNode * counter_constant = new ASTNode();
  counter_constant->type = CONSTANT; 
  counter_constant->ival = 1; 

  ASTNode * counter_var = new ASTNode();
  counter_var->type = VAR;
  counter_var->sval = counter_name;


  ASTNode * counter_expr = new ASTNode();
  counter_expr->type = BINOP;
  counter_expr->op = ADD;
  counter_expr->children.push_back(counter_var);
  counter_expr->children.push_back(counter_constant);

  ASTNode * counter_assignment = new ASTNode();
  counter_assignment->type = ASSIGN_STMT;
  counter_assignment->children.push_back(counter_var);
  counter_assignment->children.push_back(counter_expr);

  ASTNode * loop_nest = generateLoopNest(iterations, counter_assignment);

  // collapse the loops with OpenMP
  // TODO - pick random value for collapse (instead of collapsing all) 
  int collapse_num = iterations.size(); 
  ASTNode * omp_for = new ASTNode();
  omp_for->type = OMPN_FOR;
  omp_for->children.push_back(loop_nest);
  omp_for->clauses.insert({OMPC_COLLAPSE, Fuzzer::getConstExpr(collapse_num)});

  return omp_for;
}

// TODO need some way to keep track of where in the tree the counter initialization is 
// Maybe merge into initialize counter routine
// Also where to position the increment loop 
void EMI_Transformer::insertIncrementLoopCollapse(ASTNode* blk_stmt, int n) {
        // parameters: number of loops, starting variable, step (increment amount), predicate, whether predicate contains EMI, etc.

      ASTNode * loop_nest = generateIncrementLoopCollapse(n); 
      blk_stmt->children.insert(blk_stmt->children.begin(), loop_nest);
}

ASTNode * EMI_Transformer::create_dead_fragment_modulo_input() {
  string emiVarName = var_base + to_string(input_set.size() + 1);
  int emiVarVal = 0;

  input_set.insert({emiVarName, emiVarVal});

  ASTNode* if_node =  new ASTNode();
  if_node->type = IF_STMT;

  ASTNode * expr = new ASTNode();
  expr->type = VAR;
  expr->sval = emiVarName;

  ASTNode * blk_stmt_node = new ASTNode();
  blk_stmt_node->type = BLK_STMT;
  blk_stmt_node->children.push_back(fuzzer.generateFragment());

  if_node->children.push_back(expr);
  if_node->children.push_back(blk_stmt_node);

  return if_node;
}


// inject code that is "dead-by-construction" (modulo input) 
void EMI_Transformer::dead_code_transform(ASTNode * node) {
  switch(node->type) {
    case(BLK_STMT):
      node->children.insert(node->children.begin(), create_dead_fragment_modulo_input()); 
      break;
    default:
      break;
  } 

  //for(ASTNode* i  : node->children)
  //  dead_code_transform(i); 

}

// convert expressions to new expressions that are equivalent (modulo input)
void EMI_Transformer::expression_transform(ASTNode * node) {
  // TODO
}

ASTNode * generate_assignment_stmt_node(string varName, int val) {
  ASTNode * var_node = new ASTNode();
  var_node->type = VAR;
  var_node->sval = varName;

  ASTNode * vdecl_node = new ASTNode();
  vdecl_node->type = VDECL;
  vdecl_node->ctype = INT;
  vdecl_node->children.push_back(var_node);

  ASTNode * const_node = new ASTNode();
  const_node->type = CONSTANT;
  const_node->ctype = INT;
  const_node->ival = val;

  ASTNode * assign_node = new ASTNode();
  assign_node->type = ASSIGN_STMT;
  assign_node->children.push_back(vdecl_node);
  assign_node->children.push_back(const_node);

  return assign_node;
}

void EMI_Transformer::processEMI(ASTNode * root) {
 
  if (root->type != BLK_STMT) {
    cerr << "EMI_Transformer::processEMI must be called with node of type BLK_STMT\n";
    return;
  }

  // initialize counter
  ASTNode * var_node = new ASTNode();
  var_node->type = VAR;
  var_node->sval = counter_name;

  ASTNode * vdecl_node = new ASTNode();
  vdecl_node->type = VDECL;
  vdecl_node->ctype = INT;
  vdecl_node->children.push_back(var_node);

  ASTNode * const_node = new ASTNode();
  const_node->type = CONSTANT;
  const_node->ctype = INT;
  const_node->ival = 0;

  ASTNode * assign_node = new ASTNode();
  assign_node->type = ASSIGN_STMT;
  assign_node->children.push_back(vdecl_node);
  assign_node->children.push_back(const_node);

  root->children.insert(root->children.begin(), assign_node);

  // add EMI vars
  // TODO not efficient 
  unordered_map<string, int>::const_iterator i;
  for (i = input_set.begin(); i != input_set.end(); ++i) {
    root->children.insert(root->children.begin(), generate_assignment_stmt_node(i->first, i->second)); 
  }
}

void EMI_Transformer::transform(ASTNode * root) {
  if (root->type != BLK_STMT) {
    cerr << "EMI_Transformer::transformer must be called with node of type BLK_STMT\n";
    return;
  }

  dead_code_transform(root);
  expression_transform(root);
}
