#include <iostream>

#include "error.h"
#include "lex.h"
#include "ast.h"
#include "parser.h"

#include <fstream>

using namespace tinylang;

int main(int argc, char** argv) {
	try {
	if (argc < 2)
		return -1;

	std::ifstream stream(argv[1]);

	std::string fileContents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	
	LexResults results = lex(fileContents, argv[1]);

	int i = 0;
	genBaseNode(results, i);

	if (results.clean) {
		std::cout << "Compilation completed succesfully.\n";
	} else {
		std::cout << "Compilation aborted; errors occurred.\n";
	}

	} catch (const CompilationError& compErr) {
		
		std::cerr << compErr.what() << std::endl;

	}
	return 0;
}