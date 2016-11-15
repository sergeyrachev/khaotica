
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CHAR = 258,
     BIT = 259,
     INT = 260,
     FLOAT = 261,
     DOUBLE = 262,
     CLASS = 263,
     IF = 264,
     ELSE = 265,
     DO = 266,
     WHILE = 267,
     FOR = 268,
     GOTO = 269,
     BREAK = 270,
     CONTINUE = 271,
     REPEAT = 272,
     MAP = 273,
     EXTENDS = 274,
     SWITCH = 275,
     CASE = 276,
     DEFAULT = 277,
     BAC = 278,
     PRAGMA = 279,
     PRAGMA_ARRAY = 280,
     PRAGMA_GET = 281,
     PRAGMA_NOGET = 282,
     PRAGMA_PUT = 283,
     PRAGMA_NOPUT = 284,
     PRAGMA_PUTXML = 285,
     PRAGMA_NOPUTXML = 286,
     PRAGMA_TRACE = 287,
     PRAGMA_NOTRACE = 288,
     PRAGMA_LINE = 289,
     PRAGMA_NOLINE = 290,
     PRAGMA_INCLUDES = 291,
     PRAGMA_NULLSTRINGS = 292,
     PRAGMA_NONULLSTRINGS = 293,
     PRAGMA_NOINCLUDES = 294,
     PRAGMA_BITSTREAM = 295,
     PRAGMA_PREFIX = 296,
     PRAGMA_ERROR_FUNC = 297,
     PRAGMA_TRACE_FUNC = 298,
     BAC_PREC = 299,
     BAC_OOC = 300,
     BAC_SOC = 301,
     BAC_TE = 302,
     BAC_RENORM = 303,
     BAC_BS = 304,
     BAC_INIT = 305,
     BAC_END = 306,
     BAC_RTABLE = 307,
     BAC_NEXTI = 308,
     BAC_TRANS = 309,
     BAC_EXCH = 310,
     INCLUDE = 311,
     IMPORT = 312,
     UNSIGNED = 313,
     SIGNED = 314,
     SHORT = 315,
     LONG = 316,
     CONST = 317,
     ALIGNED = 318,
     STATIC = 319,
     ABSTRACT = 320,
     LITTLE = 321,
     BIG = 322,
     INT_LITERAL = 323,
     DOUBLE_LITERAL = 324,
     BIT_LITERAL = 325,
     STRING_LITERAL = 326,
     VARIABLE = 327,
     CLASS_TYPE = 328,
     MAP_TYPE = 329,
     BAC_TYPE = 330,
     LABEL = 331,
     LS = 332,
     RS = 333,
     GE = 334,
     LE = 335,
     NE = 336,
     EQ = 337,
     OR = 338,
     AND = 339,
     INC = 340,
     DEC = 341,
     RANGE = 342,
     CLCL = 343,
     MUL_ASSIGN = 344,
     DIV_ASSIGN = 345,
     MOD_ASSIGN = 346,
     ADD_ASSIGN = 347,
     SUB_ASSIGN = 348,
     LS_ASSIGN = 349,
     RS_ASSIGN = 350,
     AND_ASSIGN = 351,
     XOR_ASSIGN = 352,
     OR_ASSIGN = 353,
     DBL_BRACE_L = 354,
     DBL_BRACE_R = 355,
     DECL = 356,
     EXPR = 357,
     VERBATIM = 358,
     VERBATIM_GET = 359,
     VERBATIM_PUT = 360,
     VERBATIM_BOTH = 361,
     VERBATIM_C = 362,
     VERBATIM_C_CPP = 363,
     VERBATIM_GET_C = 364,
     VERBATIM_PUT_C = 365,
     VERBATIM_BOTH_C = 366,
     VERBATIM_J = 367,
     VERBATIM_GET_J = 368,
     VERBATIM_PUT_J = 369,
     VERBATIM_BOTH_J = 370,
     LENGTHOF = 371,
     ISIDOF = 372,
     SKIPBITS = 373,
     NEXTBITS = 374,
     SNEXTBITS = 375,
     LITTLE_NEXTBITS = 376,
     LITTLE_SNEXTBITS = 377,
     NEXTCODE = 378,
     NUMBITS = 379,
     ESC_FTYPE = 380,
     ESC_MAP = 381,
     ARRAY = 382,
     LOWER_THAN_ELSE = 383
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 56 "parser.y"

    char            *string;
    int             ival;
    double          dval;
    unsigned int    uival;
    bit_t           *bit;
    list_t          *lp;
    symbol_t        *sp;
    node_t          *np;
    verbatim_t	    *vp;



/* Line 1676 of yacc.c  */
#line 194 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


