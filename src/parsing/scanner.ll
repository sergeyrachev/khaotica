%{
#include "logging.h"
#include "parser.hpp"
#include "scanner.h"

#define YY_USER_ACTION {}
#define yyterminate() flavor::Parser::make_END(flavor::location());

%}

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

"def"    return flavor::Parser::make_DEF(flavor::location());
"extern" return flavor::Parser::make_EXTERN(flavor::location());
"var"    return flavor::Parser::make_VAR(flavor::location());
"if"     return flavor::Parser::make_IF(flavor::location());
"then"   return flavor::Parser::make_THEN(flavor::location());
"else"   return flavor::Parser::make_ELSE(flavor::location());
"for"    return flavor::Parser::make_FOR(flavor::location());
"in"     return flavor::Parser::make_IN(flavor::location());

"(" return flavor::Parser::make_OPEN_PAREN(flavor::location());
")" return flavor::Parser::make_CLOSE_PAREN(flavor::location());
";" return flavor::Parser::make_STATEMENT_END(flavor::location());
"," return flavor::Parser::make_COMMA(flavor::location());

"=" return flavor::Parser::make_ASSIGNMENT(*yytext, flavor::location());

"*" { return flavor::Parser::make_MULTIPLY(*yytext, flavor::location()); }
"/" { return flavor::Parser::make_DIVIDE(*yytext, flavor::location()); }
"+" { return flavor::Parser::make_ADD(*yytext, flavor::location()); }
"-" { return flavor::Parser::make_SUBTRACT(*yytext, flavor::location()); }
">" { return flavor::Parser::make_GREATER_THAN(*yytext, flavor::location()); }
"<" { return flavor::Parser::make_LESS_THAN(*yytext, flavor::location()); }

{identifier} { return flavor::Parser::make_IDENTIFIER(yytext, flavor::location()); }

{numeric} {
    uint64_t number = strtoull(yytext, 0, 10);
    return flavor::Parser::make_NUMBER(number, flavor::location());
}

"#" BEGIN(COMMENT);

<COMMENT>{
    [^\n]+
    \n   { yylineno++; BEGIN(INITIAL); }
}

.	printf("Unknown character '%s' at line %d\n", yytext, yylineno);

%%
