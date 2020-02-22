#ifndef PARSER_H
#define PARSER_H

#include "lex.h"
#include "ast.h"

namespace tinylang {

void constructAST(Ast& ast, const LexResults& lr, const std::string& fileName);

/*
	Parses the lex results and returns a valid base node.
*/
BaseNode* genBaseNode(const LexResults& lr, int& tokenIdx);

} // tinylang

#endif // PARSER_H