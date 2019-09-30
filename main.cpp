#include <iostream>
#include <ctime>
#include <chrono>
#include "ast.h"
#include "program_wrapper.h"
#include "codegen.h"
#include "loop_injector.h"

bool optExists(char** begin, char** end, const std::string& option) {
  return std::find(begin, end, option) != end;
}

std::string expectedInputs(ProgramWrapper* prog) {
	std::string output = "// ";
	for(int input : prog->getExpectedInputs()) {
		output += (" " + std::to_string(input));
	}
	
	return output + "\n";
}

std::string headers() {
	return "#include <omp.h>\n#include <assert.h>\n#include <stdlib.h>\n"; 
}

int main(int argc, char* argv[])
{
  bool useTarget = optExists(argv, argv+argc, "-target");

  ProgramWrapper* prog = ProgramWrapper::create(); 

	int seed = (int) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  LoopInjector::injectOmpLoopAndCounterAssertion(prog, useTarget, seed);

	std::string output = expectedInputs(prog) + headers() + Codegen::generate(prog->getMain()); 
  std::cout << output;

  return 0;
}
