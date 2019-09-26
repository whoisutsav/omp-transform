#include <iostream>
#include "ast.h"
#include "emi_program.h"
#include "code_generator.h"

int main(int argc, char* argv[])
{
  EmiProgram* prog = EmiProgram::generateEmptyProgram(); 
  std::string output = CodeGenerator::generate(prog->getMain());
  std::cout << output;

  return 0;
}
