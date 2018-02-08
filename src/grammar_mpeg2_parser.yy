%skeleton "lalr1.cc"
%require "3.0"

//%no-lines

%verbose
%define parse.trace
%define parse.error verbose

%define parser_class_name {parser_t}

%define api.namespace {khaotica::core::mpeg2}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

%code requires{
    //Header file
    #include "grammar_mpeg2_types.h"

    namespace khaotica::core::mpeg2{
        class lexer_t;
    }
}

%code top {
    //Implementation file
    #include "grammar_mpeg2_parser.hpp"
    #include "grammar_mpeg2_lexer.h"
    #include "logging.h"

    #include <sstream>
    #include <iostream>

    using namespace khaotica;
    using namespace khaotica::core::mpeg2;
    parser_t::symbol_type yylex(lexer_t &lexer) {
        return lexer.next_token();
    }

    void parser_t::error( const location &loc, const std::string &err_message )
    {
        std::ostringstream ss;
        ss << loc;
        logging::debug() << "Parsing error: '" << err_message << "' " << ss.str();
    }
}

%lex-param {lexer_t& lexer}

%parse-param {lexer_t& lexer}
%parse-param {std::list<std::shared_ptr<node_t>>& structure} {std::shared_ptr<scope_t> scope} {std::shared_ptr<scope_t> global}

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
%token FUNCTION_POSITION "__position"

//0b1000000 -> 0 0 0 0 0 0 0 1
%token BSLBF "bslbf"
//0b1000000 -> 1 0 0 0 0 0 0 0
%token UIMSBF "uimsbf"

%token TCIMSBF "tcimsbf"

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

%token <std::string> TEXT
%token <std::string> IDENTIFIER
%token <std::string> BITSTRING
%token <int64_t> INTEGER
%token <uint64_t> UINTEGER

%type <sequence_t> bitstream

%type <std::list<std::shared_ptr<node_t>>> parameters
%type <std::list<std::string>> arguments

%type <sequence_t> block
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

%type <std::shared_ptr<node_t>> internal_function

%type <std::nullptr_t> bslbf_mnemonic
%type <entry_length_t> entry_length

%type <std::shared_ptr<node_t>> entry_bslbf
%type <std::shared_ptr<node_t>> entry_uimsbf
%type <std::shared_ptr<node_t>> entry_collection
%type <std::shared_ptr<node_t>> entry_sparsed
%type <std::shared_ptr<node_t>> entry_reference
%type <std::shared_ptr<node_t>> entry_if
%type <std::shared_ptr<node_t>> entry_for
%type <std::shared_ptr<node_t>> entry_do
%type <std::shared_ptr<node_t>> entry_while

%type <std::shared_ptr<node_t>> closed_if
%type <std::shared_ptr<node_t>> opened_if
%type <std::shared_ptr<node_t>> condition

%%
%start bitstream;

arguments:
arguments "," IDENTIFIER[name] {
    $$.push_back($name);
}|
%empty{

}

parameters:
parameters "," expression {
    $$.push_front($expression);
}|
%empty{
    $$ = $$;
}

bslbf_mnemonic:
"bslbf" {

}|
TEXT{

}|
%empty{

}

entry_length:
UINTEGER[length] {
    $$ = fixed_length_t{$length};
}|
UINTEGER[from] "-" UINTEGER[to]{
    $$ = variable_length_t{$from, $to};
}

entry_bslbf:
IDENTIFIER[name] UINTEGER[length] bslbf_mnemonic {
    bslbf_t payload{$name, $length};

    auto entry = std::make_shared<node_t>(node_t{payload});
    scope->definitions[$name] = entry;

    $$ = entry;
}

entry_uimsbf:
IDENTIFIER[name] entry_length[length] "uimsbf" {
    uimsbf_t payload{$name, $length};

    auto entry = std::make_shared<node_t>(node_t{payload});
    scope->definitions[$name] = entry;

    $$ = entry;
}

entry_collection:
IDENTIFIER[name] "[" UINTEGER[capacity] "]" UINTEGER[length] "*" UINTEGER[times] "uimsbf" {
    entry_length_t length{fixed_length_t{$length}};
    uimsbf_t field{$name, length};
    collection_t payload{field, $times};

    auto entry = std::make_shared<node_t>(node_t{payload});
    scope->definitions[$name] = entry;

    $$ = entry;
}|
IDENTIFIER[name] "[" UINTEGER[capacity] "]" UINTEGER[length] "*" UINTEGER[times] "bslbf" {
    fixed_length_t length{$length};
    bslbf_t field{$name, length};
    collection_t payload{field, $times};

    auto entry = std::make_shared<node_t>(node_t{payload});
    scope->definitions[$name] = entry;

    $$ = entry;
}

entry_sparsed:
IDENTIFIER[name] "[" UINTEGER[from] ".." UINTEGER[to] "]" UINTEGER[length] "bslbf" {
    fixed_length_t length{$length};
    bslbf_t field{$name, length};
    sparsed_t payload{field, $from, $to};

    auto entry = std::make_shared<node_t>(node_t{payload});
    scope->definitions[$name] = entry;

    $$ = entry;
}

entry_reference:
IDENTIFIER[name] "(" parameters ")" {
    reference_t payload{$name, $parameters};

    auto entry = std::make_shared<node_t>(node_t{payload});
    $$ = entry;

    auto it = global->definitions.find($name);
    if( it == global->definitions.end()){
        global->definitions[$name] = nullptr;
    }
}

entry:
entry_bslbf
|
entry_uimsbf
|
entry_collection
|
entry_sparsed
|
entry_reference
|
entry_for
|
entry_do
|
entry_while
|
entry_if

entry_if:
opened_if
|
closed_if

opened_if:
condition entry[then] {

    sequence_t _then{{$then}, scope};
    sequence_t _else{{}, scope};

    if_t payload{$condition, _then, _else};
    $$ = std::make_shared<node_t>(node_t{payload});

    scope = scope->close();
}|
condition closed_if[then] "else" opened_if[else]{

    sequence_t _then{{$then}, scope};
    sequence_t _else{{$else}, scope};

    if_t payload{$condition, _then, _else};
    $$ = std::make_shared<node_t>(node_t{payload});

    scope = scope->close();
}

closed_if:
condition closed_if[then] "else" closed_if [else]{
    sequence_t _then{{$then}, scope};
    sequence_t _else{{$else}, scope};

    if_t payload{$condition, _then, _else};
    $$ = std::make_shared<node_t>(node_t{payload});

    scope = scope->close();
}|
condition block[then] {
    sequence_t _then{$then};
    sequence_t _else{{}, scope};

    if_t payload{$condition, _then, _else};
    $$ = std::make_shared<node_t>(node_t{payload});

    scope = scope->close();
}

condition:
"if" {scope = scope_t::open(scope);} "(" expression ")" {
    $$ = $expression;
}

entry_for:
for_scope "(" expression[initializer] ";" expression[condition] ";" expression[modifier] ")" block[body] {
    auto initializer = $initializer;
    auto condition = $condition;
    auto modifier = $modifier;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
    scope = scope->close();
}|
for_scope "(" ";" expression[condition] ";" expression[modifier] ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = $condition;
    auto modifier = $modifier;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
}|
for_scope "(" ";" expression[condition] ";" ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = $condition;
    auto modifier = std::nullopt;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
}|
for_scope "(" ";" ";" ")" block[body] {
    auto initializer = std::nullopt;
    auto condition = std::nullopt;
    auto modifier = std::nullopt;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{for_t{initializer, condition, modifier, body}});
}

for_scope:
"for" {
    scope = scope_t::open(scope);
}


entry_do:
"do" block[body] "while" "(" expression[condition] ")" {
    auto condition = $condition;
    auto body = $body;
    $$ = std::make_shared<node_t>(node_t{do_t{condition, body}});
    scope = scope->close();
}

entry_while:
"while" "(" expression[condition] ")" block[body] {
     auto condition = $condition;
     auto body = $body;
     $$ = std::make_shared<node_t>(node_t{while_t{condition, body}});
     scope = scope->close();
}

block
: "{" {scope = scope_t::open(scope);} entries "}" {
    $$ = {$entries, scope};
    scope = scope->close();
}

entries
: entries entry {
    $$ = $1;
    $$.push_back($entry);
}|
%empty {
    $$ = {};
}

internal_function
: FUNCTION_POSITION "(" IDENTIFIER ")" {
    $$ = std::make_shared<node_t>(node_t{position_t{$IDENTIFIER}});
}| FUNCTION_POSITION "(" ")" {
    $$ = std::make_shared<node_t>(node_t{position_t{}});
}| FUNCTION_NEXTBITS "(" ")" {
    $$ = std::make_shared<node_t>(node_t{nextbits_t{}});
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
}| internal_function {
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

bitstream:
bitstream IDENTIFIER[name] "(" arguments ")" { scope = scope_t::open(scope); } block[body]  {
    compound_t payload{$name, $arguments, $body};
    auto entry = std::make_shared<node_t>(node_t{payload});

    auto it = global->definitions.find($name);
    if(it == global->definitions.end()){
        structure.push_back(entry);
    }
    global->definitions[$name] = entry;
}|
bitstream IDENTIFIER[name] "=" expression {
    assignment_t payload{$name, $expression};
    auto entry = std::make_shared<node_t>(node_t{payload});

    global->definitions[$name] = entry;
}|
%empty {
    $$ = {};
}


%%
