#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>

#include <string>

namespace tinylang {

class CompilationError : public std::runtime_error {
public:
	std::string fileName;
	int line;
	int column;

	inline CompilationError(const std::string& message, const std::string& fileName, int line, int column)
		: std::runtime_error("[ERROR] At line " + std::to_string(line) + ", column " + std::to_string(column) + ": " + message),
		fileName(fileName), line(line), column(column) { }
};

class UnexpectedTokenError : public CompilationError {
public:
	std::string token;

	inline UnexpectedTokenError(const std::string& token, const std::string& fileName, int line, int column)
		: CompilationError("Unexpected token '" + token + "'.", fileName, line, column), token(token) { }
};

inline std::string charToS(char c) {
	return std::string("") + c;
}

class UnexpectedCharacterError : public CompilationError {
public:
	char character;

	inline UnexpectedCharacterError(char c, const std::string& filename, int line, int column)
		: CompilationError("Unexpected character '" + charToS(c) + "'.", filename, line, column), character(c) { }
};

class UnexpectedEofError : public CompilationError {
public:
	char character;

	inline UnexpectedEofError(const std::string& fileName, int line, int column)
		: CompilationError("Unexpected end of file.", fileName, line, column) { }
};

} // tinylang

#endif // ERROR_H