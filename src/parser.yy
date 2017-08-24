%skeleton "lalr1.cc"
%require "3.0"

//%no-lines

%verbose
%define parse.trace
%define parse.error verbose

%define parser_class_name {Parser}

%define api.namespace {flavor}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

%code requires{
    //Header file
    #include "grammar.h"

    #include <list>

    namespace flavor{
        class Scanner;
    }
}

%code top {
    //Implementation file
    #include "logging.h"
    #include "scanner.h"
    #include "parser.hpp"

    namespace {
        using namespace flavor;
        Parser::symbol_type yylex(Scanner &scanner) {
            return scanner.next_token();
        }
    }
}
%lex-param {Scanner& scanner}

%parse-param {Scanner& scanner}
%parse-param {symbols_t& symbols}
%parse-param {symbols_table_t& table}

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


%token <bitstring_t> BITSTRING
%token <numeric_t> INTEGER


%type <bslbf_t> bslbf
%type <uimsbf_t> uimsbf
%type <tcimsbf_t> tcimsbf
%type <symbol_t> field_definition

%type <symbols_t> entries
%type <compound_t> compound
%type <std::string> signature
%type <symbols_t> scope

%type <variable_t> variable_definition
%type <expression_t> expression
%type <entity_t> primary_expression
%type <expression_t> unary_expr
%type <expression_t> multiplicative_expr
%type <expression_t> additive_expr

%%
%start bitstream;

field_definition
: bslbf { $$ = $1;}
| uimsbf { $$ = $1;}
| tcimsbf {}
;

bslbf
: IDENTIFIER INTEGER "bslbf"  {
        $$ = {$1, $2};
        table[$1] = $$;
    }
;

uimsbf
: IDENTIFIER INTEGER "uimsbf"  {
        $$ = {$1, $2};
        table[$1] = $$;
    }
;

tcimsbf
: IDENTIFIER INTEGER "tcimsbf" {
        $$ = {$1, $2};
        table[$1] = $$;
    }
;

entries
: field_definition {
        $$.push_back($1);
    }
| entries field_definition {
        $1.push_back($2);
        $$ = $1;
    }
;

scope
: "{" entries "}" {
        $$ = $2;
    }
| "{" "}" {
        $$ = {};
    }
;

signature
: IDENTIFIER "(" ")" {
        $$ = $1;
    }
;

compound
: signature scope {
        $$ = {$1, $2};
        table[$1] = $$;
    }
;

variable_definition
: IDENTIFIER "=" expression {
        $$ = {$1, $3};
        table[$1] = $$;
    }
;

primary_expression
: IDENTIFIER {
        $$ = table[$1];
    }
| INTEGER {
        $$ = $1;
    }
| BITSTRING {
        $$ = $1;
    }
;

unary_expr
: "!" primary_expression {
        $$ = {std::logical_not(), $2};
    }
| "-" primary_expression {
        $$ = {std::negate(), $2};
    }
;

multiplicative_expr
: unary_expr {
        $$ = $1;
    }
| multiplicative_expr '*' unary_expr {
        $$ = {$1, std::multiply(), $3};
    }
| multiplicative_expr '/' unary_expr {

    }
| multiplicative_expr '%' unary_expr {

    }
;

additive_expr
: multiplicative_expr
| additive_expr '+' multiplicative_expr {

    }
| additive_expr '-' multiplicative_expr {

    }
;

expression
: multiplicative_expr {

    }
| additive_expr {

    }
;

bitstream
: field_definition {
        symbols.push_back($1);
    }
| bitstream field_definition {
        symbols.push_back($2);
    }
| compound {
        symbols.push_back($1);
    }
| bitstream compound {
        symbols.push_back($2);
    }
| variable_definition {
        symbols.push_back($1);
    }
| END
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



