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
#define YY_DECL flavor::Parser::symbol_type flavor::Scanner::next_token()

#include "parser.hpp"
#include "location.hh"
#include "position.hh"

namespace flavor{
    class Interpreter;

    class Scanner : public yyFlexLexer {
    public:
        explicit Scanner(std::istream& in, std::ostream& out):yyFlexLexer(in, out){

        };

        flavor::Parser::symbol_type next_token();

    private:
        flavor::location _location;
    };
}

#endif //KHAOTICA_SCANNER_H
