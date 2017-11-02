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
%parse-param {scope_t* scope}

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

%type <std::list<std::shared_ptr<node_t>>> block
%type <std::list<std::shared_ptr<node_t>>> entries

%type <std::shared_ptr<node_t>> entry
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
    auto entry = std::make_shared<node_t>(node_t{bslbf_t{$1, $2}});
    $$ = entry;
    scope->definitions[$1] = entry;
}| IDENTIFIER INTEGER "uimsbf"  {
    auto entry = std::make_shared<node_t>(node_t{uimsbf_t{$1, $2}});
    $$ = entry;
    scope->definitions[$1] = entry;
}| IDENTIFIER "(" ")" {
    auto it = document.definitions.find($1);
    if( it == document.definitions.end()){
        document.definitions[$1] = nullptr;
    }
    auto entry = std::make_shared<node_t>(node_t{reference_t{$1}});
    $$ = entry;
}| if_block "(" expression ")" block {
    auto condition = $expression;
    auto _then =  $block;
    $$ = std::make_shared<node_t>(node_t{if_t{condition, _then, {}}});
    scope = scope->parent;
}| if_block "(" expression ")" block[then] "else" block[else] {
    auto condition = $expression;
    auto _then =  $then;
    auto _else =  $else;
    $$ = std::make_shared<node_t>(node_t{if_t{condition, _then, _else}});
    scope = scope->parent;
}| for_block "(" expression[initializer] ";" expression[condition] ";" expression[modifier] ")" block[body] {
    auto initializer = $initializer;
    auto condition = $condition;
    auto modifier = $modifier;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
    scope = scope->parent;
}| for_block "(" ";" expression[condition] ";" expression[modifier] ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = $condition;
    auto modifier = $modifier;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
    scope = scope->parent;
}| for_block "(" ";" expression[condition] ";" ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = $condition;
    auto modifier = std::nullopt;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
    scope = scope->parent;
}| for_block "(" ";" ";" ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = std::nullopt;
    auto modifier = std::nullopt;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
    scope = scope->parent;
}

if_block: "if" {
    auto p = std::make_shared<scope_t>(scope_t{scope});
    scope->childs.push_back(p);
    scope = p.get();
}

for_block: "for" {
    auto p = std::make_shared<scope_t>(scope_t{scope});
    scope->childs.push_back(p);
    scope = p.get();
}

block
: "{" entries "}" {
    $$ = $2;
}| "{" "}" {
    $$ = {};
}

entries
: entry entries{
    $$ = $2;
    $$.push_front($1);
} | entry {
    $$.push_back($1);
}

primary_expression
: IDENTIFIER {
    $$ = std::make_shared<node_t>(node_t{identifier_t{$1}});
}| INTEGER {
    $$ = std::make_shared<node_t>(node_t{integer_t{$1}});
}| BITSTRING {
    $$ = std::make_shared<node_t>(node_t{bitstring_t{$1}});
}| "(" expression ")" {
    $$ = $2;
}| postfix_expression{
    $$ = $1;
}| prefix_expression{
    $$ = $1;
}

postfix_expression
: IDENTIFIER "++" {
    auto expr = std::make_shared<node_t>(node_t{postincrement_t{$1, "+"}});
    $$ = expr;

}| IDENTIFIER "--" {
    auto expr = std::make_shared<node_t>(node_t{postincrement_t{$1, "-"}});
    $$ = expr;
}

prefix_expression
: "++" IDENTIFIER {
    auto expr = std::make_shared<node_t>(node_t{preincrement_t{$2, "+"}});
    $$ = expr;
}| "--" IDENTIFIER {
    auto expr = std::make_shared<node_t>(node_t{preincrement_t{$2, "-"}});
    $$ = expr;
}

unary_expr
: "-" primary_expression {
    $$ = std::make_shared<node_t>(node_t{unary_expression_t{$2, "-"}});
}| "!" primary_expression {
    $$ = std::make_shared<node_t>(node_t{unary_expression_t{$2, "!"}});
}| "~" primary_expression {
    $$ = std::make_shared<node_t>(node_t{unary_expression_t{$2, "~"}});
}| primary_expression{
    $$ = $1;
}

multiplicative_expr
: multiplicative_expr "*" unary_expr {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "*", $3}});
}| multiplicative_expr "/" unary_expr {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "/", $3}});
}| multiplicative_expr "%" unary_expr {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "%", $3}});
}| unary_expr {
    $$ = $1;
}

additive_expression
: additive_expression "+" multiplicative_expr {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "+", $3}});
}| additive_expression "-" multiplicative_expr{
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "-", $3}});
}| multiplicative_expr {
    $$ = $1;
}

relational_expression
: additive_expression "<" additive_expression  {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "<", $3}});
}| additive_expression ">" additive_expression  {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, ">", $3}});
}| additive_expression "<=" additive_expression  {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "<=", $3}});
}| additive_expression ">=" additive_expression  {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, ">=", $3}});
}| additive_expression {
    $$ = $1;
}

comparison_expression
: relational_expression "==" relational_expression {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "==", $3}});
}| relational_expression "!=" relational_expression {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "!=", $3}});
}| relational_expression {
    $$ = $1;
}

logical_and
: logical_and "&&" comparison_expression {
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "&&", $3}});
}| comparison_expression{
    $$ = $1;
}

logical_or
: logical_or "||" logical_and{
    $$ = std::make_shared<node_t>(node_t{binary_expression_t{$1, "||", $3}});
}| logical_and{
    $$ = $1;
}

expression
: logical_or {
    $$ = $1;
}| IDENTIFIER "=" expression {
    auto entry = std::make_shared<node_t>(node_t{assignment_t{$1, $3}});
    $$ = entry;
    scope->definitions[$1] = entry;
}

bitstream
: IDENTIFIER "(" ")" { auto p = std::make_shared<scope_t>(scope_t{scope}); scope->childs.push_back(p); scope = p.get(); } block[body] {     scope = scope->parent; } bitstream {
    auto entry = std::make_shared<node_t>(node_t{compound_t{$IDENTIFIER, $body}});
    auto it = document.definitions.find($IDENTIFIER);
    if(it == document.definitions.end()){
        document.structure.push_back(entry);
    }
    document.definitions[$IDENTIFIER] = entry;
}| IDENTIFIER "=" expression bitstream {
    auto entry = std::make_shared<node_t>(node_t{assignment_t{$1, $3}});
    scope->definitions[$1] = entry;
}| END
;

%%
