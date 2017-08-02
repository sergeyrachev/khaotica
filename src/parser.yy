%skeleton "lalr1.cc"
%require "3.0"

%no-lines

%verbose
%define parse.trace
%define parse.error verbose

%define parser_class_name {Parser}

%define api.namespace {flavor}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

%code requires{
    #include "grammar.h"
    #include "logging.h"

    namespace flavor{
        class Scanner;
    }
}

%code top {
    #include "scanner.h"
    #include "parser.hpp"

    static flavor::Parser::symbol_type yylex(flavor::Scanner &scanner) {
        return scanner.next_token();
    }
}

%lex-param {flavor::Scanner& scanner}

%parse-param {flavor::Scanner& scanner}

%locations

%destructor {

} <>

%token END 0
%token
%token DEF "def"
%token <std::string> IDENTIFIER
%token <double> NUMBER

%token TYPE "uint"
%token OPEN_CURLY "{"
%token CLOSE_CURLY "}"
%token OPEN_PAREN "("
%token CLOSE_PAREN ")"
%token STATEMENT_END ";"


%%
%start top;

top :
END {
    logging::debug() << "add expr";
}

%%
#include "logging.h"
#include <sstream>
#include <iostream>
void flavor::Parser::error( const location &loc, const std::string &err_message )
{
    std::ostringstream ss;
    ss << loc;
    logging::debug() << "Parsing error: " << err_message << ss.str();
}
