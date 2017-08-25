%{
#include "logging.h"
#include "parser.hpp"
#include "lexer.h"

#define YY_USER_ACTION { _location.step(); _location.columns(yyleng); }
#define yyterminate() flavor::parser_t::make_END(_location);

using namespace flavor;

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
integer    [0-9]+[0-9]*
bits       [01]+

%s commented_line
%s quoted
%%

"//".*{newline} { }

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
"nextbits" return parser_t::make_FUNCTION_NEXTBITS(_location);
"lengthof" return parser_t::make_FUNCTION_LENGTHOF(_location);
"==" return parser_t::make_EQUAL(_location);
"bslbf" return parser_t::make_MNEMONIC_BSLBF(_location);
"uimsbf" return parser_t::make_MNEMONIC_UIMSBF(_location);
"tcimsbf" return parser_t::make_MNEMONIC_TCIMSBF(_location);
".." return parser_t::make_RANGE(_location);
"=" return parser_t::make_ASSIGN(_location);
";" return parser_t::make_SEMICOLON(_location);
"++" return parser_t::make_INCREMENT(_location);
"-" return parser_t::make_MINUS(_location);
"+" return parser_t::make_PLUS(_location);

"*" return parser_t::make_MULTIPLY(_location);
"||" return parser_t::make_OR(_location);
"<" return parser_t::make_LESSTHAN(_location);
">" return parser_t::make_GREATERTHAN(_location);

\' {
    BEGIN(quoted);
}

<quoted>{bits} {
    return parser_t::make_BITSTRING( {yytext}, _location);
}

<quoted>\' {
    BEGIN(INITIAL);
}

{identifier} {
    return parser_t::make_IDENTIFIER(yytext, _location);
}

{integer} {
    int64_t number = strtoll(yytext, 0, 10);
    return flavor::parser_t::make_INTEGER(number, _location);
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

{newline} { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>>     { return yyterminate(); }

%%