//
// Created by sergey.rachev on 4/21/17.
//

#ifndef KHAOTICA_SCANNER_H
#define KHAOTICA_SCANNER_H

#include <iostream>
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#undef  YY_DECL
#define YY_DECL khaotica::parser_t::symbol_type khaotica::lexer_t::next_token()

#include "parser.hpp"
#include "location.hh"

namespace khaotica{
    class interpreter_t;

    class lexer_t : public yyFlexLexer {
    public:
        explicit lexer_t(std::istream& in, std::ostream& out):yyFlexLexer(in, out){

        };

        khaotica::parser_t::symbol_type next_token();

    private:
        khaotica::location _location;
    };
}

#endif //KHAOTICA_SCANNER_H
