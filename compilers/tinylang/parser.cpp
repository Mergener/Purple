#include "parser.h"

#include "lex.h"
#include "error.h"

namespace tinylang {

/* 
	Adds 1 to tokenIdx and checks if it has reached the bounds of lr.
	If it has, returns false and pushes an unexpected EOF error.
	If it didn't, returns true without pushing any error.
*/
static void advanceTokenOrThrow(const LexResults& lr, int& tokenIdx) {
	const Token& t = lr.tokens[tokenIdx];
	tokenIdx++;
	if (lr.tokens.size() == tokenIdx) {
		throw UnexpectedEofError(lr.fileName, t.line, t.column);
	}
}

static StatementNode* genStatementNode() {
	
}

/*
	Generates a function declaration node. Expects the current token type to be 'TokenType::FunctionKeyword'.
	Right after this function call, tokenIdx must be either:
		- The index of a valid statement begin token in lr if the function was not marked as native or extern.
		- The index of a valid base node begin token in lr if the function was marked as native or extern.
*/
static FuncDeclNode* genFuncDeclNode(const LexResults& lr, int& tokenIdx, bool funcMarkedNative) {
	FuncDeclNode* ret;

	int declLine = lr.tokens[tokenIdx].line;
	int declColumn = lr.tokens[tokenIdx].column;

	// If this is a function, we expect the next token to be the function's name.
	advanceTokenOrThrow(lr, tokenIdx);

	std::string tokenText = lr.tokens[tokenIdx].getTextCopy();
	std::string fnName = tokenText;

	if (lr.tokens[tokenIdx].tokenType != TokenType::SymbolName) {
		throw UnexpectedTokenError(tokenText, lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
	}

	// If we reached this point, the token is a valid function name, and it is stored at 'tokenText'.
	// From now on, we expect a parenthesis start and a list (empty or not) of parameters.
	advanceTokenOrThrow(lr, tokenIdx);

	if (lr.tokens[tokenIdx].tokenType != TokenType::OpeningParenthesis) {
		throw UnexpectedTokenError(tokenText, lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
		return nullptr;
	}

	std::vector<std::string> paramTypeNames;
	std::vector<std::string> paramNames;

	advanceTokenOrThrow(lr, tokenIdx);
	if (lr.tokens[tokenIdx].tokenType != TokenType::ClosingParenthesis) {
		// Read all parameters
		while (true) {	// Breaks when closing parenthesis is reached	
			// Read parameter type name

			if (lr.tokens[tokenIdx].tokenType != TokenType::SymbolName) { // Expects type name
				throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
			}
			paramTypeNames.push_back(lr.tokens[tokenIdx].getTextCopy()); 
			
			// Read parameter name
			advanceTokenOrThrow(lr, tokenIdx);

			if (lr.tokens[tokenIdx].tokenType != TokenType::SymbolName) { // Expects parameter name
				throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
			}
			paramNames.push_back(lr.tokens[tokenIdx].getTextCopy());

			// Read comma or closing parenthesis
			advanceTokenOrThrow(lr, tokenIdx);

			if (lr.tokens[tokenIdx].tokenType == TokenType::ClosingParenthesis) {
				break; // Parameter list has ended
			} else if (lr.tokens[tokenIdx].tokenType == TokenType::Comma) {
				advanceTokenOrThrow(lr, tokenIdx);
				continue; // Keep reading parameters
			} else {
				throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
			}
		}
	}

	std::string returnTypeName;

	// Read return type
	tokenIdx++;
	if (lr.tokens.size() == tokenIdx && funcMarkedNative) {
		// Native function of type 'native func foo()' and end of file
		returnTypeName = "void";
	} else if (lr.tokens.size() > tokenIdx) {				
		if (lr.tokens[tokenIdx].tokenType == TokenType::Colon) {
			// Token type is colon, it must be of type '[native] func foo(): T'
			// Considering that, the next token must be a symbol name that identifies
			// the type T.
			advanceTokenOrThrow(lr, tokenIdx);

			if (lr.tokens[tokenIdx].tokenType == TokenType::SymbolName) {
				returnTypeName = lr.tokens[tokenIdx].getTextCopy();
			} else {
				throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
			}
		} else if (lr.tokens[tokenIdx].tokenType == TokenType::ScopeStart) {
			returnTypeName = "void";
		} else if (lr.tokens[tokenIdx].tokenType == TokenType::StatementEnd) {
			returnTypeName = "void";
		} else {
			throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
		}
	} else {
		throw UnexpectedEofError(lr.fileName, lr.tokens[tokenIdx - 1].line, lr.tokens[tokenIdx - 1].column);
	}

	if (!funcMarkedNative) {
		// If the function wasn't marked native, it means that the last token that was read was the
		// return type name. In this case, the current token is a ScopeStart, which means we need to 
		// move the current token index to one token after in order to fulfill this function's return
		// promise.
		advanceTokenOrThrow(lr, tokenIdx);
	}

	ret = new FuncDeclNode(fnName, returnTypeName, paramTypeNames, funcMarkedNative, declLine, declColumn);

	return ret;
}

/* 
	Generates a base node. The type of the base node varies according to the lex results current context.
	Returns nullptr if no base node was generated (not an error. In fact, this behaviour is to be expected when reaching EOF)
*/
static BaseNode* genBaseNode(const LexResults& lr, int& tokenIdx) {
	if (tokenIdx >= lr.tokens.size()) {
		return nullptr;
	}

	bool currFuncIsNative = false;

	BaseNode* ret;

	switch (lr.tokens[tokenIdx].tokenType) {
		case TokenType::NativeKeyword: {
			currFuncIsNative = true;

			advanceTokenOrThrow(lr, tokenIdx);

			if (lr.tokens[tokenIdx].tokenType != TokenType::FunctionKeyword) { // Next token must be the function declaration keyword
				throw UnexpectedTokenError(lr.tokens[tokenIdx].getTextCopy(), lr.fileName, lr.tokens[tokenIdx].line, lr.tokens[tokenIdx].column);
			}

			// fall into function keyword label on purpose...
		}
		case TokenType::FunctionKeyword: {
			ret = genFuncDeclNode(lr, tokenIdx, currFuncIsNative);
			if (!currFuncIsNative) {

			}
			currFuncIsNative = false;
			break;
		}
		default: {
			break;
		}
	}

	return ret;
}

void constructAST(Ast& ast, const LexResults& lr, const std::string& fileName) {
	int tokenIdx = 0;

	ast.fileName = fileName;
	ast.root = genBaseNode(lr, tokenIdx);

	for (BaseNode* n = ast.root; n != nullptr; n = genBaseNode(lr, tokenIdx)) {

	}
}

} // tinylang