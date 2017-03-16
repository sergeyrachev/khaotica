#include "lexer.h"
#include "grammar.hpp"


Lexer::Lexer(std::istream *in)
    : yyFlexLexer(in), yylval(nullptr) {}

int
Lexer::yylex(bison::Parser::semantic_type *l_val) {
    yylval = l_val;
    return yylex();
}
