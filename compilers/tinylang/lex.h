#ifndef LEX_H
#define LEX_H

#include <string>
#include <vector>
#include <ostream>
#include <string_view>

namespace tinylang {

enum class TokenType {

	SymbolName,

	FunctionKeyword,
	ReturnKeyword,
	NativeKeyword,

	ScopeStart,
	ScopeEnd,
	
	OpeningParenthesis,
	ClosingParenthesis,

	Comma,
	Colon,

	StatementEnd,
	
	StringLiteral,
	CharLiteral,
	IntegerLiteral,
	FloatLiteral,

	OperatorAssignment,
	OperatorAddition,
	OperatorSubtraction,
	OperatorMultiplication,
	OperatorDivision,
	OperatorEqualityComparison
};

struct Token {
	std::string_view tokenStr;
	TokenType tokenType;
	int line;
	int column;

	/*
		Returns a std::string copy of 'tokenStr'.
	*/
	inline std::string getTextCopy() const {
		return std::string(tokenStr.data(), tokenStr.size());
	}
};

struct LexResults {
	std::string fileName;
	std::string fullStr;
	std::vector<Token> tokens;
	bool clean; // True if no errors occurred during tokenization.
};

/*
	Analyses the specified string lexically, identifying its tokens and returning the lexical analysis
	results.
*/
LexResults lex(const std::string& s, const std::string& filename, int startingLine = 1, int startingColumn = 1);

void dumpLexResults(const LexResults& lr, std::ostream& stream);

const char* getTokenTypeName(TokenType t);

} // tinylang

#endif // LEX_H