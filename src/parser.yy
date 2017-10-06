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

%token LESSTHAN "<"
%token GREATERTHAN ">"
%token LESSTHAN_EQUAL "<="
%token GREATERTHAN_EQUAL ">="


%token <std::string> IDENTIFIER
%token <bitstring_t> BITSTRING
%token <integer_t> INTEGER
%type <entry_t> entry
%type <entries_t> entries
%type <compound_t> compound_signature

%type <std::shared_ptr<compound_definition_t>> compound_definition

%type <std::shared_ptr<expression_t>> expression
%type <std::shared_ptr<expression_t>> primary_expression
%type <std::shared_ptr<expression_t>> unary_expr
%type <std::shared_ptr<expression_t>> multiplicative_expr
%type <std::shared_ptr<expression_t>> additive_expression
%type <std::shared_ptr<expression_t>> logical_and
%type <std::shared_ptr<expression_t>> logical_or
%type <std::shared_ptr<expression_t>> postfix_expression
%type <std::shared_ptr<expression_t>> prefix_expression
%type <std::shared_ptr<expression_t>> relational_expression
%type <std::shared_ptr<expression_t>> comparison_expression

%%
%start bitstream;

entry
: IDENTIFIER INTEGER "bslbf"  {
    bslbf_t entry{$1, $2.value};
    $$ = entry;
    symbols[$1] = entry;
}| IDENTIFIER INTEGER "uimsbf"  {
    uimsbf_t entry{$1, $2.value};
    $$ = entry;
    symbols[$1] = entry;
}| IDENTIFIER INTEGER "tcimsbf" {
    tcimsbf_t entry{$1, $2.value};
    $$ = entry;
    symbols[$1] = entry;
}| compound_signature {
    $$ = $1;
    symbols[$1.name] = compound_definition_t{};

}| "if" "(" expression ")" compound_definition {
    auto condition = $3;
    auto _then =  $5;
    $$ = if_t{condition, _then, std::nullopt};
}| "if" "(" expression ")" compound_definition "else" compound_definition {
    auto condition = $3;
    auto _then =  $5;
    auto _else =  $7;
    $$ = if_t{condition, _then, _else};
}| "for" "(" expression ";" expression ";" expression ")" compound_definition {
    auto initializer = $3;
    auto condition = $5;
    auto modifier = $7;
    auto body = $9;
    $$ = for_t{initializer, condition, modifier, body};
}| "for" "(" ";" expression ";" expression ")" compound_definition {
    auto condition = $4;
    auto modifier = $6;
    auto body = $8;
    $$ = for_t{ nullptr, condition, modifier, body};
}| "for" "(" ";" expression ";" ")" compound_definition {
    auto condition = $4;
    auto body = $7;
    $$ = for_t{nullptr, condition, nullptr, body};
}| "for" "(" ";" ";" ")" compound_definition {
    auto body = $6;
    $$ = for_t{nullptr, nullptr, nullptr, body};
}
;

compound_signature
: IDENTIFIER "(" ")" {
    $$ = compound_t{$1};
}

primary_expression
: IDENTIFIER {
    $$ = std::make_shared<expression_t>(expression_t{variable_t{$1}});
}| INTEGER {
    $$ = std::make_shared<expression_t>(expression_t{$1});
}| BITSTRING {
    $$ = std::make_shared<expression_t>(expression_t{$1});
}| "(" expression ")" {
    $$ = $2;
}| postfix_expression{
    $$ = $1;
}| prefix_expression{
    $$ = $1;
}

postfix_expression
: IDENTIFIER "++" {
    auto expr = std::make_shared<expression_t>(expression_t{postincrement_t{std::plus<>()}});
    assignment_t assignment{variable_t{$1}, expr};
    $$ = std::make_shared<expression_t>(expression_t{assignment});

}| IDENTIFIER "--" {
    auto expr = std::make_shared<expression_t>(expression_t{postincrement_t{std::minus<>()}});
    assignment_t assignment{variable_t{$1}, expr};
    $$ = std::make_shared<expression_t>(expression_t{assignment});
}

prefix_expression
: "++" IDENTIFIER {
    auto expr = std::make_shared<expression_t>(expression_t{preincrement_t{std::plus<>()}});
    assignment_t assignment{variable_t{$2}, expr};
    $$ = std::make_shared<expression_t>(expression_t{assignment});
}| "--" IDENTIFIER {
    auto expr = std::make_shared<expression_t>(expression_t{preincrement_t{std::minus<>()}});
    assignment_t assignment{variable_t{$2}, expr};
    $$ = std::make_shared<expression_t>(expression_t{assignment});
}

unary_expr
: "-" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::negate<>(), $2}});
}| "!" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::logical_not<>(), $2}});
}| "~" primary_expression {
    $$ = std::make_shared<expression_t>(expression_t{unary_expression_t{std::bit_not<>(), $2}});
}| primary_expression{
    $$ = $1;
}

multiplicative_expr
: multiplicative_expr "*" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::multiplies<>(), $3}});
}| multiplicative_expr "/" unary_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::divides<>(), $3}});
}| multiplicative_expr "%" unary_expr {
     $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::modulus<>(), $3}});
}| unary_expr {
    $$ = $1;
}

additive_expression
: additive_expression "+" multiplicative_expr {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::plus<>(), $3}});
}| additive_expression "-" multiplicative_expr{
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::minus<>(), $3}});
}| multiplicative_expr {
       $$ = $1;
}
;

relational_expression
: additive_expression "<" additive_expression  {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::less<>(), $3}});
}| additive_expression ">" additive_expression  {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::greater<>(), $3}});
}| additive_expression "<=" additive_expression  {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::less_equal<>(), $3}});
}| additive_expression ">=" additive_expression  {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::greater_equal<>(), $3}});
}| additive_expression {
    $$ = $1;
}

comparison_expression
: relational_expression "==" relational_expression {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::equal_to<>(), $3}});
}| relational_expression "!=" relational_expression {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::not_equal_to<>(), $3}});
}| relational_expression {
    $$ = $1;
}

logical_and
: logical_and "&&" comparison_expression {
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::logical_and<>(), $3}});
}| comparison_expression{
    $$ = $1;
}

logical_or
: logical_or "||" logical_and{
    $$ = std::make_shared<expression_t>(expression_t{binary_expression_t{$1, std::logical_or<>(), $3}});
}| logical_and{
    $$ = $1;
}

expression
: logical_or {
    $$ = $1;
}| IDENTIFIER "=" logical_or {
    assignment_t assignment{ variable_t{$1}, $3};
    $$ = std::make_shared<expression_t>(expression_t{assignment});
    symbols[$1] = assignment;
}

compound_definition
: "{" entries "}" {
    $$ = std::make_shared<compound_definition_t>(compound_definition_t{$2});
}| "{" "}" {
    $$ = std::make_shared<compound_definition_t>();
}
;

entries
: entry {
    $$.push_back($1);
}| entries entry{
    $1.push_back($2);
    $$ = $1;
}
;

bitstream
: compound_signature compound_definition bitstream{
    auto it = symbols.find($1.name);
    if( it != symbols.end() ){
        it->second = {*$2};
    } else {
        doc.push_front($1);
    }

}| IDENTIFIER "=" expression bitstream {
    variable_t variable{$1};
    assignment_t assignment{variable, $3};
    doc.push_front(variable);
    symbols[$1] = assignment;
}| END
;

%%
