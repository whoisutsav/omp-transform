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

  for(int i=0; i<iterations.size(); i++) {
    tuple <int, int, Operator, int> params = generateLoopParameters(iterations[i]);  

    string emi_var_name = var_base + to_string(input_set.size() + 1); // TODO - fix
    int emi_var_val = get<0>(params);

    input_set.insert({emi_var_name, emi_var_val});

    // initialize loop counter node (using EMI) 
    string loop_counter_name = "i" + to_string(i);
    
    ASTNode * loop_counter_var = new ASTNode(); 
    loop_counter_var->type = VAR; 
    loop_counter_var->sval = loop_counter_name;

    ASTNode * loop_counter_vdecl = new ASTNode(); 
    loop_counter_vdecl->type = VDECL; 
    loop_counter_vdecl->ctype = INT;
    loop_counter_vdecl->children.push_back(loop_counter_var);

    ASTNode * emi_var_node = new ASTNode(); 
    emi_var_node->type = VAR; 
    emi_var_node->sval = emi_var_name;

    ASTNode * loop_counter_initialization = new ASTNode();
    loop_counter_initialization->type = ASSIGN_EXP;
    loop_counter_initialization->children.push_back(loop_counter_vdecl);
    loop_counter_initialization->children.push_back(emi_var_node);

    // initialize loop predicate node 
    ASTNode * predicate_var = new ASTNode();
    predicate_var->type = VAR;
    predicate_var->sval = loop_counter_name;

    ASTNode * predicate_constant = new ASTNode();
    predicate_constant->type = CONSTANT;
    predicate_constant->ival = get<3>(params); 

    ASTNode * pred_expr = new ASTNode(); 
    pred_expr->type = BINOP;
    pred_expr->op = get<2>(params);
    pred_expr->children.push_back(predicate_var);
    pred_expr->children.push_back(predicate_constant);

    // initialize loop step statement 
    ASTNode * step_constant = new ASTNode();
    step_constant->type = CONSTANT; 
    step_constant->ival = get<1>(params); 

    ASTNode * step_var = new ASTNode();
    step_var->type = VAR;
    step_var->sval = loop_counter_name;

    ASTNode * step_expr = new ASTNode();
    step_expr->type = BINOP;
    step_expr->op = ADD;
    step_expr->children.push_back(step_constant);
    step_expr->children.push_back(step_var);

    ASTNode * step_assignment = new ASTNode();
    step_assignment->type = ASSIGN_EXP;
    step_assignment->children.push_back(step_var);
    step_assignment->children.push_back(step_expr);

    ASTNode * for_loop = new ASTNode();
    for_loop->type = FOR_STMT;
    for_loop->children.push_back(loop_counter_initialization);
    for_loop->children.push_back(pred_expr);  
    for_loop->children.push_back(step_assignment);
    for_loop->children.push_back(current); 

    current = for_loop;
  }

  return current;
}

ASTNode * EMI_Transformer::generateIncrementLoop(vector<int> iterations) {

  // Generate loop body
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

  ASTNode * counter_assignment_exp = new ASTNode();
  counter_assignment_exp->type = ASSIGN_EXP;
  counter_assignment_exp->children.push_back(counter_var);
  counter_assignment_exp->children.push_back(counter_expr);

  ASTNode * body = new ASTNode();
  body->type = EXP_STMT;
  body->children.push_back(counter_assignment_exp);

  // Generate loop nest
  ASTNode * loop_nest = generateLoopNest(iterations, body);

  return loop_nest;
}

// TODO need some way to keep track of where in the tree the counter initialization is 
// Maybe merge into initialize counter routine
// Also where to position the increment loop 
void EMI_Transformer::insertIncrementLoop(ASTNode* blk_stmt, int n) {

  // Generate random loop structure
  // For now, all outer loops have single iteration 
  // Inner loop has `total` iterations
  vector<int> iterations;
  iterations.push_back(n);
  for(int i=1; i < (rand() % 4) + 3; i++) {
    iterations.push_back(1);
  } 

  ASTNode * loop_nest = generateIncrementLoop(iterations); 

  ASTNode * omp_for = new ASTNode();
  omp_for->type = OMPN_PARALLEL_FOR;
  omp_for->children.push_back(loop_nest);

  switch(rand() % 2) {
    case 0:
      // collapse the loops 
      // TODO - pick random value for collapse (instead of collapsing all) 
      omp_for->clauses.insert({OMPC_COLLAPSE, Fuzzer::getConstExpr(iterations.size())});
      break;
    case 1:
    default:
      // reduce the loop counter
      omp_for->clauses.insert({OMPC_REDUCTION, Fuzzer::getConstExpr("+:" + counter_name)});
      break;
  }

  ASTNode * omp_target = new ASTNode();
  omp_target->type = OMPN_TARGET;
  omp_target->children.push_back(omp_for);
  
  blk_stmt->children.insert(blk_stmt->children.begin(), omp_target);
  expected_counter_output += n;
}

ASTNode * EMI_Transformer::create_dead_fragment_modulo_input() {
  string emi_var_name = var_base + to_string(input_set.size() + 1);
  int emi_var_val = 0;

  input_set.insert({emi_var_name, emi_var_val});

  ASTNode* if_node =  new ASTNode();
  if_node->type = IF_STMT;

  ASTNode * expr = new ASTNode();
  expr->type = VAR;
  expr->sval = emi_var_name;

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
ASTNode* EMI_Transformer::expression_transform_add_emi(ASTNode * exp_node) {

  string emi_var_name = var_base + to_string(input_set.size() + 1);
  int emi_var_val = (rand() % 20) + 1;
  input_set.insert({emi_var_name, emi_var_val});
  
  ASTNode * const_lhs = new ASTNode();
  const_lhs->type = VAR;
  const_lhs->sval = emi_var_name;

  ASTNode * const_rhs = new ASTNode();
  const_rhs->type = CONSTANT;
  const_rhs->ival = emi_var_val;

  ASTNode * identical_binop_exp = new ASTNode();
  identical_binop_exp->type = BINOP; 
  identical_binop_exp->op = MINUS;
  identical_binop_exp->children.push_back(const_lhs);
  identical_binop_exp->children.push_back(const_rhs);

  ASTNode * modified_exp_node = new ASTNode();
  modified_exp_node->type = BINOP;
  modified_exp_node->op = ADD;
  modified_exp_node->children.push_back(exp_node);
  modified_exp_node->children.push_back(identical_binop_exp);

  return modified_exp_node;
}

ASTNode * generate_emi_stmt_node(string varName, int input_index) {
  ASTNode * var_node = new ASTNode();
  var_node->type = VAR;
  var_node->sval = varName;

  ASTNode * vdecl_node = new ASTNode();
  vdecl_node->type = VDECL;
  vdecl_node->ctype = INT;
  vdecl_node->children.push_back(var_node);

  ASTNode * const_node = new ASTNode();
  const_node->type = CUSTOM;
  //const_node->ctype = INT;
  const_node->sval = "atoi(argv[" + to_string(input_index) + "]);";

  ASTNode * assign_exp_node = new ASTNode();
  assign_exp_node->type = ASSIGN_EXP;
  assign_exp_node->children.push_back(vdecl_node);
  assign_exp_node->children.push_back(const_node);

  ASTNode * exp_stmt_node = new ASTNode();
  exp_stmt_node->type = EXP_STMT;
  exp_stmt_node->children.push_back(assign_exp_node);

  return exp_stmt_node;
}

vector<int> EMI_Transformer::get_inputs() {
  vector<int> inputs;
  unordered_map<string, int>::const_iterator i;
  for (i = input_set.begin(); i != input_set.end(); ++i) {
    inputs.push_back(i->second);
  }

  return inputs;
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

  ASTNode * assign_exp = new ASTNode();
  assign_exp->type = ASSIGN_EXP;
  assign_exp->children.push_back(vdecl_node);
  assign_exp->children.push_back(const_node);

  ASTNode * exp_stmt = new ASTNode();
  exp_stmt->type = EXP_STMT;
  exp_stmt->children.push_back(assign_exp);

  root->children.insert(root->children.begin(), exp_stmt);

  // add EMI vars
  // TODO not efficient 
  unordered_map<string, int>::const_iterator i;
  int input_index=1;
  for (i = input_set.begin(); i != input_set.end(); ++i) {
    root->children.insert(root->children.begin(), generate_emi_stmt_node(i->first, input_index)); 
    input_index++;
  }
}

