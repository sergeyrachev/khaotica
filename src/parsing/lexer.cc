#include "lexer.h"
#include "bison_parser.hpp"


Lexer::Lexer(std::istream *in)
    : yyFlexLexer(*in, std::cerr), yylval(nullptr) {}

int
Lexer::yylex(bison::Parser::semantic_type *l_val) {
    yylval = l_val;
    return yylex();
}
