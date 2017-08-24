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
#define YY_DECL flavor::grammar_t::symbol_type flavor::lexer_t::next_token()

#include "parser.hpp"
#include "location.hh"
#include "position.hh"

namespace flavor{
    class Interpreter;

    class lexer_t : public yyFlexLexer {
    public:
        explicit lexer_t(std::istream& in, std::ostream& out):yyFlexLexer(in, out){

        };

        flavor::parser_t::symbol_type next_token();

    private:
        flavor::location _location;
    };
}

#endif //KHAOTICA_SCANNER_H
