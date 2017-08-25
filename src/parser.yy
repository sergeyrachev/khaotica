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
    #include "logging.h"
    #include "parser.hpp"
    #include "lexer.h"

    #include <sstream>
    #include <iostream>

    flavor::parser_t::symbol_type yylex(flavor::lexer_t &lexer) {
        return lexer.next_token();
    }

    void flavor::parser_t::error( const location &loc, const std::string &err_message )
    {
        std::ostringstream ss;
        ss << loc;
        logging::debug() << "Parsing error: '" << err_message << "' " << ss.str();
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

%type <entry_t> entry
%type <entries_t> entries

%type <std::shared_ptr<variable_definition_t>> variable_definition
%type <std::shared_ptr<compound_definition_t>> compound_definition

%%
%start bitstream;

entry
: IDENTIFIER INTEGER "bslbf"  {
        auto entry = std::make_shared<bslbf_t>(bslbf_t{$1, $2.value});
        $$ = entry;
        symbols[$1] = entry;
    }
| IDENTIFIER INTEGER "uimsbf"  {
        auto entry = std::make_shared<uimsbf_t>(uimsbf_t{$1, $2.value});
        $$ = entry;
        symbols[$1] = entry;
    }
| IDENTIFIER INTEGER "tcimsbf" {
        auto entry = std::make_shared<tcimsbf_t>(tcimsbf_t{$1, $2.value});
        $$ = entry;
        symbols[$1] = entry;
    }
| IDENTIFIER "(" ")" {
        $$ = std::make_shared<compound_t>(compound_t{$1});
    }
;

compound_definition
: "{" entries "}" {
        $$ = std::make_shared<compound_definition_t>(compound_definition_t{$2});
    }
;

entries
: entry {
        $$.push_back($1);
    }
| entries entry{
        $1.push_back($2); $$ = $1;
    }
|
;
variable_definition
: INTEGER {
        $$ = std::make_shared<variable_definition_t>(variable_definition_t{std::make_shared<expression_t>(expression_t{$1})});
    }
;

bitstream
: IDENTIFIER "(" ")" compound_definition{
        doc.push_back(std::make_shared<compound_t>(compound_t{$1}));
        symbols[$1] = $4;
    }
| bitstream IDENTIFIER "(" ")" compound_definition {
        doc.push_back(std::make_shared<compound_t>(compound_t{$2}));
        symbols[$2] = $5;
    }
| IDENTIFIER "=" variable_definition {
        doc.push_back( std::make_shared<variable_t>(variable_t{$1}));
        symbols[$1] = $3;
    }
| bitstream IDENTIFIER "=" variable_definition {
        doc.push_back( std::make_shared<variable_t>(variable_t{$2}));
        symbols[$2] = $4;
    }
| END
;

%%
