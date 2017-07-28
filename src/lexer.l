%{ 

/*
 * Copyright (c) 1997-2004 Alexandros Eleftheriadis, Danny Hong and 
 * Yuntai Kyong.
 *
 * This file is part of Flavor, developed at Columbia University
 * (http://flavor.sourceforge.net).
 *
 * Flavor is free software; you can redistribute it and/or modify
 * it under the terms of the Flavor Artistic License as described in
 * the file COPYING.txt. 
 *
 */

/*
 * Authors:
 * Alexandros Eleftheriadis <eleft@ee.columbia.edu>
 * Danny Hong <danny@ee.columbia.edu>
 * Yuntai Kyong <yuntaikyong@ieee.org>
 *
 */

/*
 * Lexical analyzer for the Flavor translator
 */

#include "scanner.h"
#include "parser.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

%}

%option yyclass="Scanner"
%option yylineno
%option debug
%option nodefault
%option noyywrap
%option c++
%option nounistd

DIGIT           [0-9]
LETTER          [a-zA-Z]
ALPHANUM        {LETTER}|{DIGIT}
ALL             {ALPHANUM}|[\.\[\]\\,;:/=+!@#$%\^&*'~_]
LETTERS         {LETTER}+
EXP             [DEde][+-]?{DIGIT}+
SPACES          [ \t\v\f]
STRING          \"({ALL})*\"

%%

"/*"   {}
"//"   {}

"%{"    {}

"%g{"	{}

"%p{"	{}

"%*{"	{}

"%.c{" {}

"%.c.cpp{" {}

"%p.c{" {}

"%g.c{" {}

"%*.c{" {}

"%.j{"  {}

"%p.j{" {}

"%g.j{" {}

"%*.j{" {}

"%pragma"  {}

"%include" {}

"%import"  {}

(_|{LETTER})({ALPHANUM}|_)* {
   
}

"++"      {}
"--"      {}
"<<"      {}
">>"      {}
"<="      {}
">="      {}
"=="      {}
"!="      {}
"&&"      {}
"||"      {}
"::"      {}
".."      {}

"->"|".*"|"->*" {}

"*="      {}
"/="      {}
"%="      {}
"+="      {}
"-="      {}
"<<="     {}
">>="     {}
"&="      {}
"^="      {}
"|="      {}

{DIGIT}+"."{DIGIT}*({EXP})? |
{DIGIT}*"."{DIGIT}+({EXP})? |
{DIGIT}+{EXP} {}

0(b|B)((0|1)(0|1)(0|1)(0|1)\.)*(0|1)+ {}

0(x|X)[0-9a-fA-F]+ {}

0{DIGIT}({DIGIT})* {}

{DIGIT}+ {}

[-+*/<>|&^%~\[\]!=,();?:\.] {}

\{   {}

\}   {}

{STRING} {}

'.'	    {}
'\\a'	{}
'\\b'	{}
'\\f'	{}
'\\n'	{}
'\\r'	{}
'\\t'	{}
'\\v'	{}
'\\?'	{}
'\\''	{}
'\\\"'	{}
'\\\\'	{}

'\\[0-7]+'	{}

'\\x[0-9a-fA-F]+'	{}

'\\.' {}

\n     {}

{SPACES} ;

. { };

%%
