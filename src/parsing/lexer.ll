%{
#include "parser.hpp"
#include "scanner.h"

typedef bison::Parser::token token;

#define yyterminate() return( token::END )
%}

%s COMMENT

%option debug

%option c++
%option yyclass="Lexer"
%option noyywrap
%option nounistd

blank      [ \t]
identifier [a-zA-Z_][a-zA-Z_0-9]*
numeric    [0-9]+(\.[0-9][0-9]?)?

%%

{blank}+  /* skip whitespace */

"def"    return bison::Parser::make_DEF();
"extern" return bison::Parser::make_EXTERN();
"var"    return bison::Parser::make_VAR();
"if"     return bison::Parser::make_IF();
"then"   return bison::Parser::make_THEN();
"else"   return bison::Parser::make_ELSE();
"for"    return bison::Parser::make_FOR();
"in"     return bison::Parser::make_IN();

"(" return bison::Parser::make_OPEN_PAREN();
")" return bison::Parser::make_CLOSE_PAREN();
";" return bison::Parser::make_STATEMENT_END();
"," return bison::Parser::make_COMMA();

"=" return bison::Parser::make_ASSIGNMENT(*yytext);

"*" { return bison::Parser::make_MULTIPLY(*yytext); }
"/" { return bison::Parser::make_DIVIDE(*yytext); }
"+" { return bison::Parser::make_ADD(*yytext); }
"-" { return bison::Parser::make_SUBTRACT(*yytext); }
">" { return bison::Parser::make_GREATER_THAN(*yytext); }
"<" { return bison::Parser::make_LESS_THAN(*yytext); }


{identifier} { const std::string* p = new std::string(yytext); return bison::Parser::make_IDENTIFIER(p); }

{numeric} {
             uint64_t number = strtoull(yytext, 0, 10);
             return bison::Parser::make_NUMBER(number);
         }

"#" BEGIN(COMMENT);

<COMMENT>{
    [^\n]+  // eat eat eat
    \n   { yylineno++; BEGIN(INITIAL); }
}

.	printf("bad input character '%s' at line %d\n", yytext, yylineno);
