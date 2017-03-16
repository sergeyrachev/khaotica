#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int Lexer::yylex()

#include "grammar.hpp"


class Lexer : public yyFlexLexer {
    int yylex();
    bison::Parser::semantic_type *yylval;

public:
    Lexer(std::istream *in);

    int yylex(bison::Parser::semantic_type *l_val);
};
