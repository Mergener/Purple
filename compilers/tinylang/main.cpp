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

		if (results.clean) {
			std::cout << "Compilation completed succesfully.\n";
		} else {
			std::cout << "Compilation aborted; errors occurred.\n";
		}

	} catch (const CompilationError& compErr) {
		std::cerr << compErr.what() << std::endl;
	} catch (const std::bad_alloc&) {
		std::cerr << "[FATAL ERROR] The compilation proccess was aborted because the proccess ran out of memory." << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "[FATAL ERROR] An unknown error has occurred. Error message: " << e.what() << std::endl;
	}
	return 0;
}