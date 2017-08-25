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

%type <std::shared_ptr<compound_definition_t>> compound_definition

%type <std::shared_ptr<expression_t>> primary_expression
%type <std::shared_ptr<expression_t>> unary_expr
%type <std::shared_ptr<expression_t>> multiplicative_expr
%type <std::shared_ptr<expression_t>> expression

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
| "if" "(" expression ")" compound_definition {
    auto condition = $3;
    auto _then =  $5;
    $$ = std::make_shared<if_t>(if_t{condition, _then, nullptr});
}
| "if" "(" expression ")" compound_definition "else" compound_definition {
    auto condition = $3;
    auto _then =  $5;
    auto _else =  $7;
    $$ = std::make_shared<if_t>(if_t{condition, _then, _else});
}
| "for" "(" IDENTIFIER "=" expression ";" expression ";" expression ")" compound_definition {
    auto variable = variable_t{$3};
    auto initializer = $5;
    auto condition = $7;
    auto modifier = $9;
    auto body = $11;
    $$ = std::make_shared<for_t>(for_t{variable, initializer, condition, modifier, body});
}
| "for" "(" ";" expression ";" expression ")" compound_definition {
    auto condition = $4;
    auto modifier = $6;
    auto body = $8;
    $$ = std::make_shared<for_t>(for_t{std::nullopt, nullptr, condition, modifier, body});
}
| "for" "(" ";" expression ";" ")" compound_definition {
    auto condition = $4;
    auto body = $7;
    $$ = std::make_shared<for_t>(for_t{std::nullopt, nullptr, condition, nullptr, body});
}
| "for" "(" ";" ";" ")" compound_definition {
    auto body = $6;
    $$ = std::make_shared<for_t>(for_t{std::nullopt, nullptr, nullptr, nullptr, body});
}
;

primary_expression
: IDENTIFIER {
    $$ = std::make_shared<expression_t>(expression_t{$1});
}
| INTEGER {
    $$ = std::make_shared<expression_t>(expression_t{$1});
}
| BITSTRING {
    $$ = std::make_shared<expression_t>(expression_t{$1});
}
;

unary_expr
: primary_expression{
    $$ = $1;
}
| "-" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::negate<expression_t>(), $2}});
}
| "!" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::logical_not<expression_t>(), $2}});
}
| "~" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::bit_not<expression_t>(), $2}});
}

multiplicative_expr
: unary_expr {
    $$ = $1;
}
| multiplicative_expr "*" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::multiplies<expression_t>(), $3}});
}
| multiplicative_expr "/" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::divides<expression_t>(), $3}});
}
| multiplicative_expr "%" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::modulus<expression_t>(), $3}});
}
| multiplicative_expr "&&" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::logical_and<expression_t>(), $3}});
}
| multiplicative_expr "==" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::equal_to<expression_t>(), $3}});
}
| multiplicative_expr "!=" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::not_equal_to<expression_t>(), $3}});
}
| multiplicative_expr "<" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::less<expression_t>(), $3}});
}
| multiplicative_expr ">" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::greater<expression_t>(), $3}});
}
;

expression
: multiplicative_expr{
    $$ = $1;
}
| expression "+" multiplicative_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::plus<expression_t>(), $3}});
}
| expression "-" multiplicative_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::minus<expression_t>(), $3}});
}
| expression "||" multiplicative_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::logical_or<expression_t>(), $3}});
}
| expression "++" {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::plus<expression_t>(), std::make_shared<expression_t>(expression_t{integer_t{1}})}});
}
| expression "--" {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::minus<expression_t>(), std::make_shared<expression_t>(expression_t{integer_t{1}})}});
}
;

compound_definition
: "{" entries "}" {
    $$ = std::make_shared<compound_definition_t>(compound_definition_t{$2});
}
| "{" "}" {
    $$ = std::make_shared<compound_definition_t>();
}
;

entries
: entry {
    $$.push_back($1);
}
| entries entry{
    $1.push_back($2); $$ = $1;
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
| IDENTIFIER "=" expression {
    doc.push_back( std::make_shared<variable_t>(variable_t{$1}));
    symbols[$1] = $3;
}
| bitstream IDENTIFIER "=" expression {
    doc.push_back( std::make_shared<variable_t>(variable_t{$2}));
    symbols[$2] = $4;
}
| END
;

%%
