#include "lex.h"

#include <unordered_map>
#include <istream>
#include <iostream>

#include "error.h"

namespace tinylang {

/*
	Maps languages keywords into their respective token types.
*/
static std::unordered_map<std::string, TokenType> s_ReservedWords = {
	{ "func", TokenType::FunctionKeyword },
	{ "native", TokenType::NativeKeyword },
	{ "return", TokenType::ReturnKeyword }
};

/*
	Returns the token type of a keyword.
*/
static TokenType getWordTokenType(const std::string_view& v) {
	std::string s(v.data(), v.length());

	auto it = s_ReservedWords.find(s);
	if (it == s_ReservedWords.end()) {
		return TokenType::SymbolName;
	} else {
		return it->second;
	}
}

/*
	Creates a token with the specified arguments and adds it to the LexResults passed by reference.
*/
static void addTokenToResults(LexResults& results, int start, int end, TokenType type, int line, int column) {
	Token token;
	
	std::string_view v = std::string_view(results.fullStr);

	token.tokenStr = v.substr(start, end - start + 1);
	token.tokenType = type;
	token.line = line;
	token.column = column;

	results.tokens.push_back(token);
}

LexResults lex(const std::string& s, const std::string& filename, int line, int column) {
	LexResults results;
	results.clean = true;
	results.fileName = filename;
	results.fullStr = s;

	if (s.length() == 0) {
		return results;
	}

	enum {		

		ScanningNothing,
		ScanningComment,
		ScanningWord,
		ScanningStringLiteral,
		ScanningCharLiteral,
		ScanningNumber

	} currentScanState = ScanningNothing;

	int stateStart = 0;
	int stateEnd;
	bool shouldBreak = false;
	bool hasScannedFloatDot = false;
	char prevChar = '\0';
	char nextChar = s[1];

	column--;

	for (int i = 0; i < s.length(); ++i) {	
		try {
			stateEnd = i - 1;

			char c = s[i];

			if (c == '\n') {
				line++;
				column = 1;
			} else {
				column++;
			}

			switch (currentScanState) {

				case ScanningComment: {
					if (c == '\n') {
						currentScanState = ScanningNothing;
					}
					break;
				}

				case ScanningNumber: {
					if (hasScannedFloatDot) {
						if (!isdigit(c)) {
							addTokenToResults(results, stateStart, stateEnd, TokenType::FloatLiteral, line, column);
							currentScanState = ScanningNothing;
						}
					} else {
						if (c == '.') {
							hasScannedFloatDot = true;
						} else if (!isdigit(c)) {
							addTokenToResults(results, stateStart, stateEnd, TokenType::IntegerLiteral, line, column);
							currentScanState = ScanningNothing;
						}
					}

					break;
				}

				case ScanningWord: {
					if (!isalpha(c) && !isdigit(c)) {
						addTokenToResults(results, stateStart, stateEnd, TokenType::SymbolName, line, column);
						Token& t = results.tokens[results.tokens.size() - 1];
						t.tokenType = getWordTokenType(t.tokenStr);

						currentScanState = ScanningNothing;
					}
					break;
				}

				case ScanningStringLiteral: {
					if (c == '\"' && prevChar != '\\') {
						addTokenToResults(results, stateStart, stateEnd, TokenType::StringLiteral, line, column);
						currentScanState = ScanningNothing;
					}

					break;
				}

				case ScanningCharLiteral: {
					if (c != '\'' && prevChar != '\\') {
						addTokenToResults(results, stateStart, stateEnd, TokenType::CharLiteral, line, column);
						currentScanState = ScanningNothing;
					}
				}

				default: // ScanningNothing state should be treated downwards
					break;
			}
			
			if (currentScanState == ScanningNothing) {
				stateStart = i;

				switch (c) {
					case '\'':
						currentScanState = ScanningCharLiteral;
						break;

					case '\"':
						currentScanState = ScanningStringLiteral;
						break;

					case '(':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OpeningParenthesis, line, column);
						break;

					case ')':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::ClosingParenthesis, line, column);
						break;

					case ',':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::Comma, line, column);
						break;

					case '{':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::ScopeStart, line, column);
						break;

					case '}':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::ScopeEnd, line, column);
						break;

					case '+':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorAddition, line, column);
						break;

					case '-':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorSubtraction, line, column);
						break;

					case '*':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorMultiplication, line, column);
						break;

					case '=':
						if (prevChar == '=') {
							addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorEqualityComparison, line, column);
						} else if (nextChar != '=') {
							addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorAssignment, line, column);
						}
						break;

					case ':':
						addTokenToResults(results, stateStart, stateEnd + 1, TokenType::Colon, line, column);
						break;

					case '/':
						if (prevChar == '/') {
							currentScanState = ScanningComment;
						} else if (nextChar != '/') {
							addTokenToResults(results, stateStart, stateEnd + 1, TokenType::OperatorDivision, line, column);
						}
						break;

					case '\t':
					case '\n':
					case ' ':
						break;

					default:
						if (isalpha(c) || c == '_') {
							currentScanState = ScanningWord;
						} else if (isdigit(c)) {
							currentScanState = ScanningNumber;
						} else if (c == '.') {
							currentScanState = ScanningNumber;
							hasScannedFloatDot = true;
						} else {
							throw UnexpectedCharacterError(c, filename, line, column);
						}
						break;
				}		
			}	

			prevChar = c;
			nextChar = s[i+1];

		} catch (const CompilationError& compErr) {
			results.clean = false;
			std::cerr << compErr.what() << std::endl;
		}
	}

	return results;
}

const char* getTokenTypeName(TokenType t) {
	switch (t) {
		case TokenType::CharLiteral: return "Char Literal";
		case TokenType::ClosingParenthesis: return "Closing Parenthesis";
		case TokenType::Comma: return "Comma";
		case TokenType::FloatLiteral: return "Float Literal";
		case TokenType::FunctionKeyword: return "Function Keyword";
		case TokenType::IntegerLiteral: return "Integer Literal";
		case TokenType::NativeKeyword: return "Native Keyword";
		case TokenType::OpeningParenthesis: return "Opening Parenthesis";
		case TokenType::OperatorAddition: return "Operator - Addition";
		case TokenType::OperatorAssignment: return "Operator - Assignment";
		case TokenType::OperatorDivision: return "Operator - Division";
		case TokenType::OperatorMultiplication: return "Operator - multiplication";
		case TokenType::OperatorSubtraction: return "Operator - subtraction";
		case TokenType::ReturnKeyword: return "Return Keyword";
		case TokenType::ScopeEnd: return "Scope End";
		case TokenType::ScopeStart: return "Scope Start";
		case TokenType::StringLiteral: return "String Literal";
		case TokenType::SymbolName: return "Symbol Name";
		default: return "Unknown Token Type";
	}
}

void dumpLexResults(const LexResults& lr, std::ostream& stream) {
	for (const auto& t : lr.tokens) {
		stream << getTokenTypeName(t.tokenType) << ": |" << t.tokenStr << "|\n";
	}
}

} // tinylang