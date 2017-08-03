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

    #include <list>

    namespace flavor{
        class Scanner;
    }
}

%code top {
    #include "logging.h"
    #include "scanner.h"
    #include "parser.hpp"

    static flavor::Parser::symbol_type yylex(flavor::Scanner &scanner) {
        return scanner.next_token();
    }
}

%lex-param {flavor::Scanner& scanner}

%parse-param {flavor::Scanner& scanner}
%parse-param {std::list<symbol_t>& symbols}

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


%type <bitstring_t> bitstring
%type <uint_t> uint
%type <symbol_t> field_definition

%%
%start bitstream;

field_definition
: bitstring { $$ = {$1};}
| uint { $$ = {$1};}
| tcint {}
;

bitstring
: IDENTIFIER INTEGER_LITERAL MNEMONIC_BSLBF  { $$ = {$1, $2}; }
;

uint
: IDENTIFIER INTEGER_LITERAL MNEMONIC_UIMSBF  {  $$ = {$1, $2}; }
;

tcint
: IDENTIFIER INTEGER_LITERAL MNEMONIC_TCIMSBF  {  }
;


bitstream
: field_definition { symbols.push_back($1); }
| bitstream field_definition { symbols.push_back($2); }
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


