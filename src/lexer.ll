%{
#include "logging.h"
#include "parser.hpp"
#include "lexer.h"

#define YY_USER_ACTION { _location.step(); _location.columns(yyleng); }
#define yyterminate() khaotica::parser_t::make_END(_location);

using namespace khaotica;

%}

%option yylineno
%option debug
%option nodefault
%option noyywrap
%option c++
%option nounistd
%option yyclass="lexer_t"

newline    \r?\n
blank      [ \t]
identifier [a-zA-Z_][a-zA-Z_0-9]*
integer_dec    [0-9]+[0-9]*
integer_hex    0x[0-9a-fA-F]+[0-9a-fA-F]*
integer_oct    0[0-7]+[0-7]*
integer_bin    0b[01]+[01]*
bits       [01][01[ \t]*]*
alphanum [a-zA-Z_ \t0-9.,]+

%x quoted
%x double_quoted
%%

"//".*{newline} { }

"/*".*"*/" {}

\" {
    BEGIN(double_quoted);
}

<double_quoted>{alphanum} {
    return parser_t::make_IDENTIFIER( {yytext}, _location);
}

<double_quoted>\" {
    BEGIN(INITIAL);
}

{blank}+  /* skip whitespace */

"[" return parser_t::make_BRACKET_OPEN(_location);
"]" return parser_t::make_BRACKET_CLOSE(_location);
"{" return parser_t::make_BRACE_OPEN(_location);
"}" return parser_t::make_BRACE_CLOSE(_location);
"(" return parser_t::make_PARENTHESIS_OPEN(_location);
")" return parser_t::make_PARENTHESIS_CLOSE(_location);
"do" return parser_t::make_DO(_location);
"while" return parser_t::make_WHILE(_location);
"for" return parser_t::make_FOR(_location);
"if" return parser_t::make_IF(_location);
"else" return parser_t::make_ELSE(_location);
"nextbits" return parser_t::make_FUNCTION_NEXTBITS(_location);
"lengthof" return parser_t::make_FUNCTION_LENGTHOF(_location);
"__position" return parser_t::make_FUNCTION_POSITION(_location);
"==" return parser_t::make_EQUAL(_location);
"!=" return parser_t::make_NOTEQUAL(_location);
"bslbf" return parser_t::make_MNEMONIC_BSLBF(_location);
"uimsbf" return parser_t::make_MNEMONIC_UIMSBF(_location);
"tcimsbf" return parser_t::make_MNEMONIC_TCIMSBF(_location);
".." return parser_t::make_RANGE(_location);
"=" return parser_t::make_ASSIGN(_location);
";" return parser_t::make_SEMICOLON(_location);
"++" return parser_t::make_INCREMENT(_location);
"&&" return parser_t::make_AND(_location);
"||" return parser_t::make_OR(_location);
"--" return parser_t::make_DECREMENT(_location);
"-" return parser_t::make_MINUS(_location);
"+" return parser_t::make_PLUS(_location);

"!" return parser_t::make_LOGICAL_NOT(_location);
"~" return parser_t::make_BITWISE_NOT(_location);

"*" return parser_t::make_MULTIPLY(_location);
"/" return parser_t::make_DIVIDE(_location);
"<" return parser_t::make_LESSTHAN(_location);
">" return parser_t::make_GREATERTHAN(_location);
"<=" return parser_t::make_LESSTHAN_EQUAL(_location);
">=" return parser_t::make_GREATERTHAN_EQUAL(_location);

\' {
    BEGIN(quoted);
}

<quoted>\' {
    BEGIN(INITIAL);
}

<quoted>{bits} {
    return parser_t::make_BITSTRING( {yytext}, _location);
}

<quoted>.|{newline} { printf("Bad quoted character '%s' at line %d\n", yytext, yylineno); return yyterminate();}

{identifier} {
    return parser_t::make_IDENTIFIER( {yytext}, _location);
}

{integer_dec}|{integer_hex}|{integer_oct} {
    auto number = std::stoll(yytext, 0, 0);
    return khaotica::parser_t::make_INTEGER( {number}, _location);
}

{integer_bin} {
    auto number = std::stoll(yytext, 0, 2);
    return khaotica::parser_t::make_INTEGER( {number}, _location);
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

{newline} { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>>     { return yyterminate(); }

%%
