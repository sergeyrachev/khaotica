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
%token <std::string> IDENTIFIER
%token BRACE_OPEN "{"
%token BRACE_CLOSE "}"
%token PARENTHESIS_OPEN "("
%token PARENTHESIS_CLOSE ")"
%token DO "do"
%token WHILE "while"
%token FOR "for"
%token IF "if"
%token FUNCTION_NEXTBITS "nextbits"
%token FUNCTION_LENGTHOF "lengthof"
%token EQUAL "=="

%token <int64_t> INTEGER_LITERAL

//0b1000000 -> 0 0 0 0 0 0 0 1
%token MNEMONIC_BSLBF "bslbf"
//0b1000000 -> 1 0 0 0 0 0 0 0
%token MNEMONIC_UIMSBF "uimsbf"
%token MNEMONIC_TCIMSBF "tcimsbf"

%token BRACKET_OPEN "["
%token BRACKET_CLOSE "]"
%token RANGE ".."
%token ASSIGN "="
%token SEMICOLON ";"
%token INCREMENT "++"

%token MINUS "-"
%token PLUS "+"
%token MULTIPLY "*"
%token DIVIDE "/"

%token OR "||"

%token LESSTHAN "<"
%token GREATERTHAN ">"
%token QUOTE_SINGLE "'"
%token BITS

%%
%start bitstream;

bitstream
: symbol_definition {}
| bitstream symbol_definition {}
;

symbol_definition
: block_definition {}
| field_definition {}
| variable_definition {}
;

blocks
: variable_definition {}
| block_declaration {}
| block_definition {}
| blocks variable_definition {}
| blocks block_definition {}
;

block_declaration
: IDENTIFIER "(" ")" {}
;

block_definition
: field_definition
| IDENTIFIER "(" ")" "{" blocks "}" {}
| IF "(" condition ")" "{" blocks "}" {}
| FOR "(" variable_definition ";" condition ";" action ")" "{" blocks "}" {}
| DO "{" blocks "}" WHILE "(" condition ")"  {}
;

condition
: logical_expression {}
| logical_expression "||" logical_expression {}
;

logical_expression
: variable comparator expression
| expression comparator variable
;

comparator
: "<"
| "=="
| ">"
;

action
: variable "++" {}
;

field_definition
: variable INTEGER_LITERAL mnemonic  {}
| variable "[" INTEGER_LITERAL RANGE INTEGER_LITERAL "]" INTEGER_LITERAL mnemonic {}
;

mnemonic
: MNEMONIC_BSLBF
| MNEMONIC_UIMSBF
| MNEMONIC_TCIMSBF
;

variable_definition
: variable "=" expression {}
;

variable
: IDENTIFIER
;

expression
: expression "+" term
| expression "-" term
| "(" expression ")"
| term
;

term
: term "*" factor
| term "/" factor
| factor
;

factor
: "-" factor
| function_call
| INTEGER_LITERAL
| BITS
| variable
;

function_call
: internal_function "(" variable ")" {}
| internal_function "(" ")" {}
;

internal_function
: FUNCTION_LENGTHOF {}
| FUNCTION_NEXTBITS {}
;

%%
#include "logging.h"
#include <sstream>
#include <iostream>
void flavor::Parser::error( const location &loc, const std::string &err_message )
{
    std::ostringstream ss;
    ss << loc;
    logging::debug() << "Parsing error: '" << err_message << "' " << ss.str();
}


