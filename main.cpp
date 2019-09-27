#include <iostream>
#include "ast.h"
#include "emi_program.h"
#include "code_generator.h"
#include "loop_transformer.h"

int main(int argc, char* argv[])
{
  EmiProgram* prog = EmiProgram::generateEmptyProgram(); 
  LoopTransformer::emiLoopTransform(prog, "myCounter", 15);
  std::string output = CodeGenerator::generate(0, prog->getMain());
  std::cout << output;

  return 0;
}
