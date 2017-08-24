%skeleton "lalr1.cc"
%require "3.0"

//%no-lines

%verbose
%define parse.trace
%define parse.error verbose

%define parser_class_name {parser_t}

%define api.namespace {flavor}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

%code requires{
    //Header file
    #include "syntax.h"

    #include <list>

    namespace flavor{
        class lexer_t;
    }
}

%code top {
    //Implementation file
    #include "parser.hpp"
    #include "logging.h"
    #include "lexer.h"

    namespace {
        using namespace flavor;
        parser_t::symbol_type yylex(lexer_t &lexer) {
            return lexer.next_token();
        }
    }
}
%lex-param {lexer_t& lexer}

%parse-param {lexer_t& lexer}
%parse-param {symbols_t& symbols}
%parse-param {document_t& doc}

%locations

%destructor {

} <>

%token END 0
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
%token DECREMENT "--"

%token MINUS "-"
%token PLUS "+"
%token MULTIPLY "*"
%token DIVIDE "/"

%token OR "||"
%token AND "&&"
%token EQUAL "=="

%token LESSTHAN "<"
%token GREATERTHAN ">"

%token <std::string> IDENTIFIER
%token <bitstring_t> BITSTRING
%token <integer_t> INTEGER

%type <std::shared_ptr<bslbf_t>> bslbf
%type <std::shared_ptr<uimsbf_t>> uimsbf
%type <std::shared_ptr<tcimsbf_t>> tcimsbf

%type <entry_t> entry

%type <std::shared_ptr<variable_definition_t>> variable_definition
%type <std::shared_ptr<compound_t>> compound
%type <std::shared_ptr<compound_definition_t>> compound_definition

%%
%start bitstream;

entry : bslbf | uimsbf | tcimsbf | compound {
    $$ = $1;
}
;

bslbf
: IDENTIFIER INTEGER "bslbf"  {
        $$ = std::make_unique($1, $2);
        symbols[$1] = $$;
    }
;

uimsbf
: IDENTIFIER INTEGER "uimsbf"  {
        $$ = std::make_unique($1, $2);
        symbols[$1] = $$;
    }
;

tcimsbf
: IDENTIFIER INTEGER "tcimsbf" {
        $$ = std::make_unique($1, $2);
        symbols[$1] = $$;
    }
;

compound
: IDENTIFIER "(" ")" {
        $$ = {$1};
    }
;

compound_definition
:  "{" compound_definition entry "}" {
        $$ = std::make_unique();
        $2->entries.push_back($3);
    }
|
;

variable_definition
: INTEGER {
        $$ = $1;
    }
;

bitstream
: compound compound_definition{
        doc.push_back($1);
        symbols[$1.name] = $2;
    }
| bitstream compound compound_definition {
        doc.push_back($2);
        symbols[$2.name] = $3;
    }
| IDENTIFIER "=" variable_definition {
        doc.push_back( variable_t($1));
        symbols[$1] = $3;
    }
| bitstream IDENTIFIER "=" variable_definition {
        doc.push_back( variable_t($2));
        symbols[$2] = $4;
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



