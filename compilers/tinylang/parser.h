#ifndef PARSER_H
#define PARSER_H

#include "lex.h"
#include "ast.h"

namespace tinylang {

void constructAST(Ast& ast, const LexResults& lr, const std::string& fileName);

} // tinylang

#endif // PARSER_H