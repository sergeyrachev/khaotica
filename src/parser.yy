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
    #include "grammar.h"

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
%parse-param {document_t& document}

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
%token ELSE "else"
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
%token NOTEQUAL "!="

%token LOGICAL_NOT "!"
%token BITWISE_NOT "~"

%token LESSTHAN "<"
%token GREATERTHAN ">"
%token LESSTHAN_EQUAL "<="
%token GREATERTHAN_EQUAL ">="


%token <std::string> IDENTIFIER
%token <std::string> BITSTRING
%token <int64_t> INTEGER
%type <std::shared_ptr<node_t>> entry
%type <std::shared_ptr<block_t>> entries
%type <std::shared_ptr<block_t>> block
%type <std::shared_ptr<node_t>> expression
%type <std::shared_ptr<node_t>> primary_expression
%type <std::shared_ptr<node_t>> unary_expr
%type <std::shared_ptr<node_t>> multiplicative_expr
%type <std::shared_ptr<node_t>> additive_expression
%type <std::shared_ptr<node_t>> logical_and
%type <std::shared_ptr<node_t>> logical_or
%type <std::shared_ptr<node_t>> postfix_expression
%type <std::shared_ptr<node_t>> prefix_expression
%type <std::shared_ptr<node_t>> relational_expression
%type <std::shared_ptr<node_t>> comparison_expression

%%
%start bitstream;

entry
: IDENTIFIER INTEGER "bslbf"  {
    auto entry = std::make_shared<bslbf_t>($1, $2);
    $$ = entry;
}| IDENTIFIER INTEGER "uimsbf"  {
    auto entry = std::make_shared<uimsbf_t>($1, $2);
    $$ = entry;
}| IDENTIFIER "(" ")" {
    if(auto it = document.definitions.find($1); it != document.definitions.end() ){
        $$ = it->second;
    } else {
        auto entry = std::make_shared<compound_t>($1);
        document.definitions[$1] = entry;
        $$ = entry;
    }
}| "if" "(" expression ")" block {
    auto condition = $3;
    auto _then =  $5;
    auto _else = std::nullopt;
    $$ = std::make_shared<if_t>(condition, _then, _else);
}| "if" "(" expression ")" block "else" block {
    auto condition = $3;
    auto _then =  $5;
    auto _else =  $7;
    $$ = std::make_shared<if_t>(condition, _then, _else);
}| "for" "(" expression ";" expression ";" expression ")" block {
    auto initializer = $3;
    auto condition = $5;
    auto modifier = $7;
    auto body = $9;
    $$ = std::make_shared<for_t>(initializer, condition, modifier, body);
}| "for" "(" ";" expression ";" expression ")" block {
    auto initializer = std::nullopt;
    auto condition = $4;
    auto modifier = $6;
    auto body = $8;
    $$ = std::make_shared<for_t>(initializer, condition, modifier, body);
}| "for" "(" ";" expression ";" ")" block {
    auto initializer = std::nullopt;
    auto condition = $4;
    auto modifier = std::nullopt;
    auto body = $7;
    $$ = std::make_shared<for_t>(initializer, condition, modifier, body);
}| "for" "(" ";" ";" ")" block {
    auto initializer = std::nullopt;
    auto condition = std::nullopt;
    auto modifier = std::nullopt;
    auto body = $6;
    $$ = std::make_shared<for_t>(initializer, condition, modifier, body);
}

primary_expression
: IDENTIFIER {
    $$ = std::make_shared<identifier_t>($1);
}| INTEGER {
    $$ = std::make_shared<integer_t>($1);
}| BITSTRING {
    $$ = std::make_shared<bitstring_t>($1);
}| "(" expression ")" {
    $$ = $2;
}| postfix_expression{
    $$ = $1;
}| prefix_expression{
    $$ = $1;
}

postfix_expression
: IDENTIFIER "++" {
    auto expr = std::make_shared<postincrement_t<std::plus<>>>($1);
    $$ = expr;

}| IDENTIFIER "--" {
    auto expr = std::make_shared<postincrement_t<std::minus<>>>($1);
    $$ = expr;
}

prefix_expression
: "++" IDENTIFIER {
    auto expr = std::make_shared<preincrement_t<std::plus<>>>($2);
    $$ = expr;
}| "--" IDENTIFIER {
    auto expr = std::make_shared<preincrement_t<std::minus<>>>($2);
    $$ = expr;
}

unary_expr
: "-" primary_expression {
    $$ = std::make_shared<unary_expression_t<std::minus<>>>($2);
}| "!" primary_expression {
    $$ = std::make_shared<unary_expression_t<std::logical_not<>>>($2);
}| "~" primary_expression {
    $$ = std::make_shared<unary_expression_t<std::bit_not<>>>($2);
}| primary_expression{
    $$ = $1;
}

multiplicative_expr
: multiplicative_expr "*" unary_expr {
    $$ = std::make_shared<binary_expression_t<std::multiplies<>>>($1, $3);
}| multiplicative_expr "/" unary_expr {
    $$ = std::make_shared<binary_expression_t<std::divides<>>>($1, $3);
}| multiplicative_expr "%" unary_expr {
    $$ = std::make_shared<binary_expression_t<std::modulus<>>>($1, $3);
}| unary_expr {
    $$ = $1;
}

additive_expression
: additive_expression "+" multiplicative_expr {
    $$ = std::make_shared<binary_expression_t<std::plus<>>>($1, $3);
}| additive_expression "-" multiplicative_expr{
    $$ = std::make_shared<binary_expression_t<std::minus<>>>($1, $3);
}| multiplicative_expr {
    $$ = $1;
}

relational_expression
: additive_expression "<" additive_expression  {
    $$ = std::make_shared<binary_expression_t<std::less<>>>($1, $3);
}| additive_expression ">" additive_expression  {
    $$ = std::make_shared<binary_expression_t<std::greater<>>>($1, $3);
}| additive_expression "<=" additive_expression  {
    $$ = std::make_shared<binary_expression_t<std::less_equal<>>>($1, $3);
}| additive_expression ">=" additive_expression  {
    $$ = std::make_shared<binary_expression_t<std::greater_equal<>>>($1, $3);
}| additive_expression {
    $$ = $1;
}

comparison_expression
: relational_expression "==" relational_expression {
    $$ = std::make_shared<binary_expression_t<std::equal_to<>>>($1, $3);
}| relational_expression "!=" relational_expression {
    $$ = std::make_shared<binary_expression_t<std::not_equal_to<>>>($1, $3);
}| relational_expression {
    $$ = $1;
}

logical_and
: logical_and "&&" comparison_expression {
    $$ = std::make_shared<binary_expression_t<std::logical_and<>>>($1, $3);
}| comparison_expression{
    $$ = $1;
}

logical_or
: logical_or "||" logical_and{
    $$ = std::make_shared<binary_expression_t<std::logical_or<>>>($1, $3);
}| logical_and{
    $$ = $1;
}

expression
: logical_or {
    $$ = $1;
}| IDENTIFIER "=" expression {
    $$ = std::make_shared<assignment_t>($1, $3);
}

block
: "{" entries "}" {
    $$ = $2;
}| "{" "}" {
    $$ = std::make_shared<block_t>();
}

entries
: entry {
    auto p = std::make_shared<block_t>();
    p->entries.push_back($1);
    $$ = p;
}| entries entry{
    $1->entries.push_back($2);
    $$ = $1;
}

bitstream
: IDENTIFIER "(" ")" block bitstream{
    auto it = document.definitions.find($1);
    if( it == document.definitions.end() ){
        auto compound = std::make_shared<compound_t>($1, $4);
        document.hierarchy.push_back(compound);
    }
    document.definitions[$1] = $4;
}| IDENTIFIER "=" expression bitstream {
    document.definitions[$1] = $3;
}| END
;

%%
