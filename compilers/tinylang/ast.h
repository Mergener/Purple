#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

#include "type.h"
#include "lex.h"

namespace tinylang {

struct BaseNode;

struct Ast {
	std::string fileName;
	BaseNode* root;
};

struct Node {
	int line;
	int column;

	Node(int line, int column) : line(line), column(column) { }
};

struct BaseNode : Node { 
	BaseNode* next = nullptr;

protected:
	inline BaseNode(int line, int column) : Node(line, column) { }
};

struct StatementNode : Node {
	StatementNode* nextStmt = nullptr; // Null if last statement of a function

protected:
	inline StatementNode(int line, int column) : Node(line, column) { }
};

struct FuncDeclNode : BaseNode {
	std::string funcName;
	std::string returnTypeName;
	std::vector<std::string> paramTypeNames;
	std::vector<std::string> paramNames;

	bool isNative = false;

	StatementNode* firstStmt = nullptr; // Null if forward decl or empty function

	FuncDeclNode(const std::string& funcName, const std::string& returnTypeNames, const std::vector<std::string>& paramTypeNames, bool isNative, int line, int column)
		: BaseNode(line, column), funcName(funcName), paramTypeNames(paramTypeNames), isNative(isNative) { }
};

struct RValue {
	virtual bool isConst() = 0;
};

} // tinylang

#endif // AST_H