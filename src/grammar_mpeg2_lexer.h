#pragma once

#include <iostream>

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#include "grammar_mpeg2_parser.hpp"
#include "location.hh"

#undef YY_DECL
#define YY_DECL parser_t::symbol_type lexer_t::next_token()

namespace khaotica::syntax::mpeg2{
    class lexer_t : public yyFlexLexer {
    public:
        explicit lexer_t(std::istream& in, std::ostream& out):yyFlexLexer(in, out){};

        parser_t::symbol_type next_token();

    private:
        location _location;
    };
}
