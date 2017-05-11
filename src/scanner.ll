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

";" return flavor::Parser::make_STATEMENT_END(_location);
"{" return flavor::Parser::make_OPEN_CURLY(_location);
"}" return flavor::Parser::make_CLOSE_CURLY(_location);
"(" return flavor::Parser::make_OPEN_PAREN(_location);
")" return flavor::Parser::make_CLOSE_PAREN(_location);
"uint" return flavor::Parser::make_TYPE(_location);

{identifier} { return flavor::Parser::make_IDENTIFIER(yytext, _location); }

{numeric} {
    uint64_t number = strtoull(yytext, 0, 10);
    return flavor::Parser::make_NUMBER(number, _location);
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

"\n" { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>>     { return yyterminate(); }

%%