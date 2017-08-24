%{
#include "logging.h"
#include "parser.hpp"
#include "scanner.h"

#define YY_USER_ACTION { _location.step(); _location.columns(yyleng); }
#define yyterminate() flavor::Parser::make_END(_location);

using namespace flavor;

%}

%option yylineno
%option debug
%option nodefault
%option noyywrap
%option c++
%option nounistd
%option yyclass="Scanner"

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

"[" return Parser::make_BRACKET_OPEN(_location);
"]" return Parser::make_BRACKET_CLOSE(_location);
"{" return Parser::make_BRACE_OPEN(_location);
"}" return Parser::make_BRACE_CLOSE(_location);
"(" return Parser::make_PARENTHESIS_OPEN(_location);
")" return Parser::make_PARENTHESIS_CLOSE(_location);
"do" return Parser::make_DO(_location);
"while" return Parser::make_WHILE(_location);
"for" return Parser::make_FOR(_location);
"if" return Parser::make_IF(_location);
"nextbits" return Parser::make_FUNCTION_NEXTBITS(_location);
"lengthof" return Parser::make_FUNCTION_LENGTHOF(_location);
"==" return Parser::make_EQUAL(_location);
"bslbf" return Parser::make_MNEMONIC_BSLBF(_location);
"uimsbf" return Parser::make_MNEMONIC_UIMSBF(_location);
"tcimsbf" return Parser::make_MNEMONIC_TCIMSBF(_location);
".." return Parser::make_RANGE(_location);
"=" return Parser::make_ASSIGN(_location);
";" return Parser::make_SEMICOLON(_location);
"++" return Parser::make_INCREMENT(_location);
"-" return Parser::make_MINUS(_location);
"+" return Parser::make_PLUS(_location);

"*" return Parser::make_MULTIPLY(_location);
"||" return Parser::make_OR(_location);
"<" return Parser::make_LESSTHAN(_location);
">" return Parser::make_GREATERTHAN(_location);

"'" {
    BEGIN(quoted);
}

<quoted>{bits} {
    return Parser::make_BITSTRING(yytext, _location);
}

<quoted>"'" {
    BEGIN(INITIAL);
}

{identifier} {
    return Parser::make_IDENTIFIER(yytext, _location);
}

{integer} {
    int64_t number = strtoll(yytext, 0, 10);
    return flavor::Parser::make_INTEGER(number, _location);
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

{newline} { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>>     { return yyterminate(); }

%%