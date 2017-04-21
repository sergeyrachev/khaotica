%{
#include "logging.h"
#include "parser.hpp"
#include "scanner.h"

#define YY_USER_ACTION { _location.step(); _location.columns(yyleng); }
#define yyterminate() flavor::Parser::make_END(_location);

%}

%option yylineno
%option debug
%option nodefault
%option noyywrap
%option c++
%option nounistd
%option yyclass="Scanner"

blank      [ \t]
identifier [a-zA-Z_][a-zA-Z_0-9]*
numeric    [0-9]+(\.[0-9][0-9]?)?

%s COMMENT

%%

{blank}+  /* skip whitespace */

"def"    return flavor::Parser::make_DEF(_location);
"extern" return flavor::Parser::make_EXTERN(_location);
"var"    return flavor::Parser::make_VAR(_location);
"if"     return flavor::Parser::make_IF(_location);
"then"   return flavor::Parser::make_THEN(_location);
"else"   return flavor::Parser::make_ELSE(_location);
"for"    return flavor::Parser::make_FOR(_location);
"in"     return flavor::Parser::make_IN(_location);

"(" return flavor::Parser::make_OPEN_PAREN(_location);
")" return flavor::Parser::make_CLOSE_PAREN(_location);
";" return flavor::Parser::make_STATEMENT_END(_location);
"," return flavor::Parser::make_COMMA(_location);

"=" return flavor::Parser::make_ASSIGNMENT(*yytext, _location);

"*" { return flavor::Parser::make_MULTIPLY(*yytext, _location); }
"/" { return flavor::Parser::make_DIVIDE(*yytext, _location); }
"+" { return flavor::Parser::make_ADD(*yytext, _location); }
"-" { return flavor::Parser::make_SUBTRACT(*yytext, _location); }
">" { return flavor::Parser::make_GREATER_THAN(*yytext, _location); }
"<" { return flavor::Parser::make_LESS_THAN(*yytext, _location); }

{identifier} { return flavor::Parser::make_IDENTIFIER(yytext, _location); }

{numeric} {
    uint64_t number = strtoull(yytext, 0, 10);
    return flavor::Parser::make_NUMBER(number, _location);
}

"#" BEGIN(COMMENT);

<COMMENT>{
    [^\n]+
    \n   { yylineno++; BEGIN(INITIAL); }
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

"\n" { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>>     { return yyterminate(); }

%%