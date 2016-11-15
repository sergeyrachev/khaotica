
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser.y"


/*
 * Copyright (c) 1997-2005 Alexandros Eleftheriadis, Danny Hong and 
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
 *  Yacc parser for Flavor translator
 */

#include <stdio.h>
#include <string.h>

#include "port.h"
#include "error.h"
#include "globals.h"
#include "symbol.h"
#include "ptree.h"
#include "check.h"
#include "util.h"
#include "eval.h"
#include "compile.h"
#include "carray.h"

/* defined in the lexical analyzer */
void yyerror(char *fmt, ...);
int yywrap();
int include(char *file, int import);

/* Disable 'unreferenced label' warning from Microsoft Visual C++.
 * MKS Yacc generates various labels that are not used.
 */
#if defined(_WIN32_) || defined(WIN32)
#pragma warning(disable : 4102)
#endif



/* Line 189 of yacc.c  */
#line 129 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 307 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 319 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  96
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2153

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  153
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  86
/* YYNRULES -- Number of rules.  */
#define YYNRULES  292
/* YYNRULES -- Number of states.  */
#define YYNSTATES  547

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   383

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    99,     2,     2,     2,    94,    92,     2,
      81,    82,    87,    85,    78,    86,    97,    88,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    80,    77,
      89,   112,    90,    79,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    95,     2,    96,    93,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,    91,    84,    98,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    17,    19,
      24,    27,    30,    33,    36,    38,    40,    43,    48,    55,
      60,    62,    67,    74,    78,    83,    90,    97,   106,   108,
     112,   114,   116,   118,   120,   122,   124,   126,   130,   134,
     138,   140,   144,   148,   150,   154,   158,   162,   166,   170,
     172,   176,   180,   184,   188,   192,   196,   198,   202,   204,
     208,   210,   216,   218,   222,   224,   226,   228,   230,   232,
     234,   236,   238,   240,   242,   244,   246,   250,   252,   256,
     259,   262,   266,   269,   272,   274,   276,   279,   284,   289,
     294,   300,   306,   312,   317,   322,   328,   334,   340,   346,
     353,   360,   365,   367,   370,   372,   374,   376,   378,   380,
     382,   384,   385,   387,   392,   394,   396,   398,   400,   402,
     404,   406,   408,   410,   414,   418,   420,   422,   426,   428,
     430,   434,   436,   438,   440,   445,   452,   457,   463,   465,
     467,   469,   471,   473,   475,   477,   481,   486,   490,   493,
     497,   499,   502,   504,   506,   508,   510,   512,   514,   516,
     519,   525,   533,   539,   545,   553,   560,   568,   576,   585,
     593,   602,   611,   621,   625,   628,   631,   633,   636,   638,
     640,   642,   644,   646,   648,   650,   652,   655,   658,   660,
     664,   668,   670,   674,   678,   680,   685,   688,   691,   695,
     698,   702,   711,   721,   730,   739,   749,   758,   769,   781,
     792,   803,   815,   826,   830,   835,   838,   842,   844,   848,
     851,   853,   855,   857,   859,   861,   863,   865,   867,   869,
     871,   873,   875,   877,   879,   882,   884,   888,   892,   894,
     896,   898,   900,   902,   904,   906,   908,   910,   912,   914,
     916,   918,   920,   924,   928,   932,   936,   939,   942,   945,
     951,   957,   964,   968,   970,   974,   978,   980,   982,   986,
     991,   997,   999,  1003,  1005,  1009,  1011,  1015,  1017,  1020,
    1023,  1027,  1029,  1033,  1037,  1039,  1041,  1043,  1047,  1049,
    1051,  1055,  1059
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     201,     0,    -1,   155,    -1,    68,    -1,    69,    -1,    70,
      -1,    81,   170,    82,    -1,    72,    -1,   154,    -1,   156,
      95,   170,    96,    -1,   157,   155,    -1,   156,   108,    -1,
     156,   109,    -1,   156,    97,    -1,   159,    -1,   156,    -1,
     162,   158,    -1,   140,    81,   156,    82,    -1,   141,    81,
      73,    78,   170,    82,    -1,   142,    81,   170,    82,    -1,
     160,    -1,   147,    81,    70,    82,    -1,   147,    81,   181,
      78,    70,    82,    -1,   148,    81,    82,    -1,   143,    81,
     170,    82,    -1,   143,    81,   161,    78,   170,    82,    -1,
     143,    81,   181,    78,   170,    82,    -1,   143,    81,   181,
      78,   161,    78,   170,    82,    -1,   180,    -1,   161,    78,
     180,    -1,   108,    -1,   109,    -1,    85,    -1,    86,    -1,
      98,    -1,    99,    -1,   158,    -1,   163,    87,   158,    -1,
     163,    88,   158,    -1,   163,    94,   158,    -1,   163,    -1,
     164,    85,   163,    -1,   164,    86,   163,    -1,   164,    -1,
     165,   100,   164,    -1,   165,   101,   164,    -1,   165,    92,
     164,    -1,   165,    93,   164,    -1,   165,    91,   164,    -1,
     165,    -1,   166,    89,   165,    -1,   166,    90,   165,    -1,
     166,   103,   165,    -1,   166,   102,   165,    -1,   166,   105,
     165,    -1,   166,   104,   165,    -1,   166,    -1,   167,   107,
     166,    -1,   167,    -1,   168,   106,   167,    -1,   168,    -1,
     168,    79,   169,    80,   169,    -1,   169,    -1,   158,   171,
     170,    -1,   112,    -1,   113,    -1,   114,    -1,   115,    -1,
     116,    -1,   117,    -1,   118,    -1,   119,    -1,   120,    -1,
     121,    -1,   122,    -1,   170,    -1,   172,    78,   170,    -1,
     170,    -1,   170,    78,   173,    -1,   175,    77,    -1,   176,
     185,    -1,   175,    78,   185,    -1,   181,   177,    -1,   181,
     178,    -1,   182,    -1,   179,    -1,   179,   182,    -1,   183,
      81,   169,    82,    -1,   183,    81,    74,    82,    -1,   184,
      81,    74,    82,    -1,   183,    81,   169,    82,    87,    -1,
     183,    81,    74,    82,    87,    -1,   184,    81,    74,    82,
      87,    -1,   179,    81,   169,    82,    -1,   179,    81,    74,
      82,    -1,   179,    81,   169,    82,    87,    -1,   179,    81,
      74,    82,    87,    -1,   179,   183,    81,   169,    82,    -1,
     179,   183,    81,    74,    82,    -1,   179,   183,    81,   169,
      82,    87,    -1,   179,   183,    81,    74,    82,    87,    -1,
     183,    81,    75,    82,    -1,   180,    -1,   179,   180,    -1,
      58,    -1,    59,    -1,    60,    -1,    61,    -1,    62,    -1,
      66,    -1,    67,    -1,    -1,    63,    -1,    63,    81,   169,
      82,    -1,   183,    -1,   184,    -1,     4,    -1,     3,    -1,
       5,    -1,     6,    -1,     7,    -1,    73,    -1,   189,    -1,
     189,   112,   170,    -1,   189,   112,   186,    -1,   190,    -1,
     191,    -1,    83,   187,    84,    -1,    71,    -1,   188,    -1,
     187,    78,   188,    -1,   170,    -1,   186,    -1,   155,    -1,
     189,    95,   170,    96,    -1,   189,    95,    95,   170,    96,
      96,    -1,   189,    81,   173,    82,    -1,   189,   112,   169,
     110,   169,    -1,   193,    -1,   194,    -1,   197,    -1,   198,
      -1,   199,    -1,   200,    -1,   238,    -1,   155,    80,   196,
      -1,    21,   169,    80,   196,    -1,    22,    80,   196,    -1,
      83,    84,    -1,    83,   195,    84,    -1,   196,    -1,   195,
     196,    -1,   192,    -1,   174,    -1,   214,    -1,   215,    -1,
     219,    -1,   218,    -1,    77,    -1,   170,    77,    -1,     9,
      81,   168,    82,   196,    -1,     9,    81,   168,    82,   196,
      10,   196,    -1,    20,    81,   170,    82,   196,    -1,    12,
      81,   168,    82,   196,    -1,    11,   196,    12,    81,   168,
      82,    77,    -1,    13,    81,    77,    77,    82,   196,    -1,
      13,    81,    77,    77,   172,    82,   196,    -1,    13,    81,
      77,   168,    77,    82,   196,    -1,    13,    81,    77,   168,
      77,   172,    82,   196,    -1,    13,    81,   172,    77,    77,
      82,   196,    -1,    13,    81,   172,    77,    77,   172,    82,
     196,    -1,    13,    81,   172,    77,   168,    77,    82,   196,
      -1,    13,    81,   172,    77,   168,    77,   172,    82,   196,
      -1,    14,    76,    77,    -1,    16,    77,    -1,    15,    77,
      -1,   202,    -1,   201,   202,    -1,   174,    -1,   203,    -1,
     220,    -1,   214,    -1,   215,    -1,   219,    -1,   218,    -1,
     230,    -1,   204,   211,    -1,   204,    77,    -1,   205,    -1,
     205,    80,   209,    -1,   205,    80,   210,    -1,   206,    -1,
     206,    19,    73,    -1,   206,    19,   155,    -1,   207,    -1,
     207,    81,   212,    82,    -1,   181,   208,    -1,     8,   155,
      -1,    65,     8,   155,    -1,     8,    73,    -1,    65,     8,
      73,    -1,   181,   183,    81,   169,    82,   155,   112,   169,
      -1,   181,   179,   183,    81,   169,    82,   155,   112,   169,
      -1,   181,   179,    81,   169,    82,   155,   112,   169,    -1,
     181,   183,    81,    74,    82,   155,   112,   169,    -1,   181,
     179,   183,    81,    74,    82,   155,   112,   169,    -1,   181,
     179,    81,    74,    82,   155,   112,   169,    -1,   181,   183,
      81,   169,    82,   155,   112,   169,   110,   169,    -1,   181,
     179,   183,    81,   169,    82,   155,   112,   169,   110,   169,
      -1,   181,   179,    81,   169,    82,   155,   112,   169,   110,
     169,    -1,   181,   183,    81,    74,    82,   155,   112,   169,
     110,   169,    -1,   181,   179,   183,    81,    74,    82,   155,
     112,   169,   110,   169,    -1,   181,   179,    81,    74,    82,
     155,   112,   169,   110,   169,    -1,    83,   195,    84,    -1,
      83,   195,    84,    77,    -1,    83,    84,    -1,    83,    84,
      77,    -1,   213,    -1,   212,    78,   213,    -1,   177,   189,
      -1,   127,    -1,   128,    -1,   129,    -1,   130,    -1,   131,
      -1,   133,    -1,   134,    -1,   135,    -1,   132,    -1,   136,
      -1,   137,    -1,   138,    -1,   139,    -1,    24,    -1,    24,
     216,    -1,   217,    -1,   216,    78,   217,    -1,    25,   112,
      68,    -1,    26,    -1,    27,    -1,    28,    -1,    29,    -1,
      30,    -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,
      35,    -1,    37,    -1,    38,    -1,    36,    -1,    39,    -1,
      40,   112,    71,    -1,    41,   112,    71,    -1,    42,   112,
      71,    -1,    43,   112,    71,    -1,    57,    71,    -1,    56,
      71,    -1,   221,   222,    -1,    18,   155,    81,   182,    82,
      -1,    18,   155,    81,   179,    82,    -1,    18,   155,    81,
     179,   183,    82,    -1,    83,   223,    84,    -1,   224,    -1,
     223,    78,   224,    -1,   225,    78,   229,    -1,    70,    -1,
     169,    -1,   226,    78,   169,    -1,   183,    81,   169,    82,
      -1,   179,   183,    81,   169,    82,    -1,   227,    -1,   228,
      78,   227,    -1,   169,    -1,    83,   226,    84,    -1,   227,
      -1,    83,   228,    84,    -1,    71,    -1,   231,   232,    -1,
      23,   155,    -1,    83,   233,    84,    -1,   234,    -1,   233,
      78,   234,    -1,   235,    78,   236,    -1,    71,    -1,   169,
      -1,    71,    -1,    83,   237,    84,    -1,   169,    -1,    71,
      -1,   237,    78,   169,    -1,   237,    78,    71,    -1,    75,
      81,    68,    82,    77,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   206,   206,   207,   208,   209,   210,   215,   220,   221,
     224,   231,   232,   236,   246,   247,   248,   252,   253,   254,
     255,   256,   260,   264,   268,   269,   270,   271,   275,   276,
     280,   281,   282,   283,   284,   285,   289,   290,   291,   292,
     296,   297,   298,   302,   303,   304,   305,   306,   307,   311,
     312,   313,   314,   315,   316,   317,   321,   322,   326,   327,
     331,   332,   336,   337,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   355,   356,   360,   361,   369,
     373,   401,   419,   420,   424,   425,   429,   435,   436,   437,
     438,   439,   440,   441,   445,   449,   453,   457,   460,   463,
     466,   469,   475,   476,   480,   481,   482,   483,   484,   485,
     486,   490,   491,   492,   496,   497,   501,   502,   503,   504,
     505,   509,   517,   518,   523,   528,   529,   533,   538,   546,
     547,   556,   557,   561,   562,   569,   578,   586,   596,   597,
     598,   599,   600,   601,   602,   606,   612,   618,   622,   623,
     627,   628,   632,   633,   634,   635,   636,   637,   641,   642,
     646,   647,   648,   657,   658,   659,   660,   661,   662,   663,
     664,   665,   666,   670,   671,   672,   676,   681,   685,   686,
     687,   688,   689,   690,   691,   692,   700,   715,   748,   756,
     763,   773,   774,   799,   807,   816,   826,   834,   845,   857,
     868,   883,   888,   893,   899,   904,   909,   919,   924,   929,
     935,   940,   945,   956,   957,   958,   959,   963,   964,   968,
     984,   985,   989,   993,   997,   998,  1002,  1006,  1010,  1014,
    1015,  1019,  1023,  1034,  1035,  1039,  1040,  1044,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1065,  1066,  1075,  1087,  1102,  1109,
    1116,  1122,  1131,  1135,  1136,  1144,  1149,  1158,  1159,  1163,
    1166,  1185,  1186,  1190,  1191,  1192,  1201,  1210,  1222,  1230,
    1239,  1243,  1244,  1248,  1252,  1256,  1257,  1258,  1262,  1263,
    1264,  1265,  1269
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "BIT", "INT", "FLOAT", "DOUBLE",
  "CLASS", "IF", "ELSE", "DO", "WHILE", "FOR", "GOTO", "BREAK", "CONTINUE",
  "REPEAT", "MAP", "EXTENDS", "SWITCH", "CASE", "DEFAULT", "BAC", "PRAGMA",
  "PRAGMA_ARRAY", "PRAGMA_GET", "PRAGMA_NOGET", "PRAGMA_PUT",
  "PRAGMA_NOPUT", "PRAGMA_PUTXML", "PRAGMA_NOPUTXML", "PRAGMA_TRACE",
  "PRAGMA_NOTRACE", "PRAGMA_LINE", "PRAGMA_NOLINE", "PRAGMA_INCLUDES",
  "PRAGMA_NULLSTRINGS", "PRAGMA_NONULLSTRINGS", "PRAGMA_NOINCLUDES",
  "PRAGMA_BITSTREAM", "PRAGMA_PREFIX", "PRAGMA_ERROR_FUNC",
  "PRAGMA_TRACE_FUNC", "BAC_PREC", "BAC_OOC", "BAC_SOC", "BAC_TE",
  "BAC_RENORM", "BAC_BS", "BAC_INIT", "BAC_END", "BAC_RTABLE", "BAC_NEXTI",
  "BAC_TRANS", "BAC_EXCH", "INCLUDE", "IMPORT", "UNSIGNED", "SIGNED",
  "SHORT", "LONG", "CONST", "ALIGNED", "STATIC", "ABSTRACT", "LITTLE",
  "BIG", "INT_LITERAL", "DOUBLE_LITERAL", "BIT_LITERAL", "STRING_LITERAL",
  "VARIABLE", "CLASS_TYPE", "MAP_TYPE", "BAC_TYPE", "LABEL", "';'", "','",
  "'?'", "':'", "'('", "')'", "'{'", "'}'", "'+'", "'-'", "'*'", "'/'",
  "'<'", "'>'", "'|'", "'&'", "'^'", "'%'", "'['", "']'", "'.'", "'~'",
  "'!'", "LS", "RS", "GE", "LE", "NE", "EQ", "OR", "AND", "INC", "DEC",
  "RANGE", "CLCL", "'='", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "LS_ASSIGN", "RS_ASSIGN", "AND_ASSIGN",
  "XOR_ASSIGN", "OR_ASSIGN", "DBL_BRACE_L", "DBL_BRACE_R", "DECL", "EXPR",
  "VERBATIM", "VERBATIM_GET", "VERBATIM_PUT", "VERBATIM_BOTH",
  "VERBATIM_C", "VERBATIM_C_CPP", "VERBATIM_GET_C", "VERBATIM_PUT_C",
  "VERBATIM_BOTH_C", "VERBATIM_J", "VERBATIM_GET_J", "VERBATIM_PUT_J",
  "VERBATIM_BOTH_J", "LENGTHOF", "ISIDOF", "SKIPBITS", "NEXTBITS",
  "SNEXTBITS", "LITTLE_NEXTBITS", "LITTLE_SNEXTBITS", "NEXTCODE",
  "NUMBITS", "ESC_FTYPE", "ESC_MAP", "ARRAY", "LOWER_THAN_ELSE", "$accept",
  "primary_expr", "identifier", "postfix_expr", "rescoped_expr",
  "unary_expr", "builtin_function", "nextbits_function", "modifier_list",
  "unary_operator", "multiplicative_expr", "additive_expr", "shift_expr",
  "relational_expr", "logical_and_expr", "logical_or_expr",
  "conditional_expr", "assignment_expr", "assignment_operator",
  "assignment_expr_list", "argument_expr_list", "declaration",
  "declaration_list", "type_info", "plain_type_info", "parsable_type_info",
  "declaration_modifiers", "modifier", "alignment_modifier",
  "type_specifier", "builtin_type_specifier", "user_type_specifier",
  "init_declarator", "array", "array_expr_list", "array_expr",
  "declarator", "declarator_with_params", "declarator_with_range",
  "statement", "labeled_statement", "compound_statement",
  "statement_or_declaration_list", "statement_or_declaration",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "program", "external_definition", "class_definition",
  "complex_class_declarator", "derived_class_declarator",
  "param_class_declarator", "aligned_class_declarator", "class_declarator",
  "id_declarator", "id_range_declarator", "class_body", "parameter_list",
  "parameter_declaration", "verbatim", "pragma", "pragma_list",
  "pragma_item", "import_file", "include_file", "map_definition",
  "map_declarator", "map_body", "map_entry_list", "map_entry", "map_code",
  "map_entry_item_list", "map_entry_item_escape",
  "map_entry_item_escape_list", "map_entry_item", "bac_definition",
  "bac_declarator", "bac_body", "bac_entry_list", "bac_entry",
  "bac_keyword", "bac_entry_item", "bac_entry_item_list", "bac_statement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,    59,    44,    63,
      58,    40,    41,   123,   125,    43,    45,    42,    47,    60,
      62,   124,    38,    94,    37,    91,    93,    46,   126,    33,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,    61,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   153,   154,   154,   154,   154,   154,   155,   156,   156,
     156,   156,   156,   157,   158,   158,   158,   159,   159,   159,
     159,   159,   159,   159,   160,   160,   160,   160,   161,   161,
     162,   162,   162,   162,   162,   162,   163,   163,   163,   163,
     164,   164,   164,   165,   165,   165,   165,   165,   165,   166,
     166,   166,   166,   166,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   170,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   172,   172,   173,   173,   174,
     175,   175,   176,   176,   177,   177,   177,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   180,
     180,   181,   181,   181,   182,   182,   183,   183,   183,   183,
     183,   184,   185,   185,   185,   185,   185,   186,   186,   187,
     187,   188,   188,   189,   189,   189,   190,   191,   192,   192,
     192,   192,   192,   192,   192,   193,   193,   193,   194,   194,
     195,   195,   196,   196,   196,   196,   196,   196,   197,   197,
     198,   198,   198,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   200,   200,   200,   201,   201,   202,   202,
     202,   202,   202,   202,   202,   202,   203,   203,   204,   204,
     204,   205,   205,   205,   206,   206,   207,   208,   208,   208,
     208,   209,   209,   209,   209,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   211,   212,   212,   213,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   215,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   219,   220,   221,
     221,   221,   222,   223,   223,   224,   225,   226,   226,   227,
     227,   228,   228,   229,   229,   229,   229,   229,   230,   231,
     232,   233,   233,   234,   235,   236,   236,   236,   237,   237,
     237,   237,   238
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     1,     1,     4,
       2,     2,     2,     2,     1,     1,     2,     4,     6,     4,
       1,     4,     6,     3,     4,     6,     6,     8,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     2,
       2,     3,     2,     2,     1,     1,     2,     4,     4,     4,
       5,     5,     5,     4,     4,     5,     5,     5,     5,     6,
       6,     4,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     1,     3,     1,     1,
       3,     1,     1,     1,     4,     6,     4,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     2,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       5,     7,     5,     5,     7,     6,     7,     7,     8,     7,
       8,     8,     9,     3,     2,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     3,
       3,     1,     3,     3,     1,     4,     2,     2,     3,     2,
       3,     8,     9,     8,     8,     9,     8,    10,    11,    10,
      10,    11,    10,     3,     4,     2,     3,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     2,     2,     2,     5,
       5,     6,     3,     1,     3,     3,     1,     1,     3,     4,
       5,     1,     3,     1,     3,     1,     3,     1,     2,     2,
       3,     1,     3,     3,     1,     1,     1,     3,     1,     1,
       3,     3,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     111,     0,     0,   233,     0,     0,   112,   220,   221,   222,
     223,   224,   228,   225,   226,   227,   229,   230,   231,   232,
     178,     0,     0,     0,   111,   176,   179,     0,   188,   191,
     194,   181,   182,   184,   183,   180,     0,   185,     0,     7,
       0,   279,     0,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   250,   248,   249,   251,     0,     0,     0,
       0,   234,   235,   257,   256,     0,    79,     0,   133,    80,
     122,   125,   126,   117,   116,   118,   119,   120,     0,   104,
     105,   106,   107,   108,     0,   109,   110,   121,    82,    83,
      85,   102,    84,   114,   115,   196,     1,   177,   187,   111,
     186,   111,     0,     0,     0,   258,     0,   278,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     5,     0,    32,
      33,    34,    35,    30,    31,     0,     0,     0,     0,     0,
       0,     8,     2,    15,     0,    36,    14,    20,     0,    40,
      43,    49,    56,    58,    60,     0,    81,     0,     0,     0,
     199,   197,     0,     0,   103,    86,   114,   115,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   111,   215,     2,    36,    62,     0,   153,     0,
     152,   138,   139,   111,   150,   140,   141,   142,   143,   154,
     155,   157,   156,   144,     0,   189,   190,   192,   193,     0,
      85,   114,     0,   217,   266,     0,   263,     0,   284,     0,
     281,     0,     0,     0,   237,   252,   253,   254,   255,   236,
       0,     0,     0,     0,   111,   111,     0,     0,    13,    11,
      12,    10,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    77,     0,     0,     0,   128,     0,    62,
     123,   124,   200,   198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,   174,     0,     0,
     111,     0,   148,   111,   216,   111,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,     0,   159,   213,
     151,     0,     0,   219,     0,   195,     0,   262,     0,     0,
     280,     0,   260,     0,   259,     6,     0,     0,     0,     0,
       0,    28,     0,     0,     0,    23,     0,    37,    38,    39,
      41,    42,    48,    46,    47,    44,    45,    50,    51,    53,
      52,    55,    54,    57,     0,    59,     0,   136,     0,   134,
     131,   132,     0,   129,     0,    94,    93,     0,     0,    88,
     101,    87,    89,     0,     0,     0,     0,    75,     0,   173,
       0,   111,   147,     0,   149,   145,    63,   214,     0,     0,
       0,   218,   264,   277,     0,   273,     0,     0,   275,   265,
     282,   286,     0,   285,   283,   261,    17,     0,    19,     0,
      24,     0,    21,     0,     9,     0,    78,     0,     0,   127,
     137,    96,    95,    98,    97,    91,    90,    92,   111,     0,
     111,     0,     0,     0,     0,   111,   146,     0,     0,     0,
       0,     0,     0,   267,     0,   271,     0,     0,     0,   289,
     288,     0,     0,     0,    29,     0,     0,     0,    61,   135,
     130,   100,    99,   160,     0,   163,   111,     0,     0,     0,
       0,    76,   162,   292,     0,     0,     0,     0,     0,     0,
       0,   274,     0,   276,     0,     0,     0,   287,    18,    25,
       0,    26,    22,   111,     0,   165,   111,   111,     0,   111,
       0,     0,     0,     0,     0,     0,     0,     0,   268,   272,
       0,   269,   291,   290,     0,   161,   164,   166,   167,   111,
     169,   111,   111,     0,     0,     0,     0,     0,     0,     0,
     270,    27,   168,   170,   171,   111,   206,   203,     0,     0,
     204,   201,   172,     0,     0,   205,   202,     0,     0,   212,
     209,     0,     0,   210,   207,   211,   208
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   131,   132,   133,   134,   135,   136,   137,   319,   138,
     139,   140,   141,   142,   143,   144,   176,   177,   297,   368,
     254,   178,    21,    22,    88,    89,   386,    91,   179,    92,
     201,   157,    69,   351,   352,   353,    70,    71,    72,   180,
     181,   182,   183,   184,   185,   186,   187,   188,    24,    25,
      26,    27,    28,    29,    30,    95,   195,   196,   100,   202,
     203,   189,   190,    61,    62,   191,   192,    35,    36,   105,
     205,   206,   207,   434,   388,   436,   389,    37,    38,   107,
     209,   210,   211,   394,   441,   193
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -408
static const yytype_int16 yypact[] =
{
     656,   -47,   -47,  2110,   -52,   -35,   -43,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,   -54,   -47,  2038,   130,  -408,  -408,   -13,    19,    46,
     -31,  -408,  -408,  -408,  -408,  -408,    -7,  -408,    -5,  -408,
     -19,  -408,     2,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,    15,    17,    29,
      35,    66,  -408,  -408,  -408,  1986,  -408,   -47,  -408,  -408,
     -69,  -408,  -408,  -408,  -408,  -408,  -408,  -408,    14,  -408,
    -408,  -408,  -408,  -408,   100,  -408,  -408,  -408,  -408,  -408,
      76,  -408,  -408,    71,    81,  -408,  -408,  -408,  -408,   747,
    -408,   107,    16,   682,   108,  -408,   114,  -408,   682,   122,
     125,   126,   128,   131,  2110,  -408,  -408,  -408,  1986,  -408,
    -408,  -408,  -408,  -408,  -408,   120,   123,   134,   135,   137,
     139,  -408,  -408,    10,   -47,  -408,  -408,  -408,  1986,   -48,
      36,    74,   -30,   115,   -59,   124,  -408,  1986,  1405,   448,
    -408,  -408,    83,  1451,  -408,  -408,   140,  -408,  1340,   133,
     143,  1151,   144,   145,   153,   154,   155,   149,  1986,   159,
     160,  -408,   848,   163,   164,   423,  -408,   166,  -408,   682,
    -408,  -408,  -408,   949,  -408,  -408,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  1206,  -408,  -408,  -408,  -408,   -47,
     682,  -408,   -11,  -408,  -408,    18,  -408,   167,  -408,    25,
    -408,   168,   333,   165,  -408,  -408,  -408,  -408,  -408,  -408,
     170,    43,   175,  1986,  1242,    -2,   172,  1986,  -408,  -408,
    -408,  -408,  -408,  1986,  1986,  1986,  1986,  1986,  1986,  1986,
    1986,  1986,  1986,  1986,  1986,  1986,  1986,  1986,  1986,  1986,
    1986,  1986,  -408,   171,   174,  1986,   176,  -408,   448,   161,
    -408,  -408,  -408,  -408,   191,   192,  1470,   193,   194,   195,
     196,  1986,   269,  1986,  1551,   206,  -408,  -408,  1986,   205,
    1151,   220,  -408,  1050,  -408,  1151,  -408,  -408,  -408,  -408,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  1986,  -408,   212,
    -408,  1196,   211,   198,   682,  -408,   108,  -408,   559,   114,
    -408,  1386,  -408,   213,  -408,  -408,   -60,   216,   215,   225,
     222,  -408,   227,   224,   229,  -408,   202,  -408,  -408,  -408,
     -48,   -48,    36,    36,    36,    36,    36,    74,    74,    74,
      74,    74,    74,   -30,   228,   115,  1986,  -408,   214,  -408,
    -408,  -408,    39,  -408,  1986,   231,   232,   230,   234,   236,
    -408,   238,   240,   -65,   233,   -64,  1570,  -408,   111,  -408,
     239,  1151,  -408,   246,  -408,  -408,  -408,  -408,  1616,   248,
    1635,  -408,  -408,  -408,   666,  -408,  1206,   249,  -408,  -408,
    -408,  -408,  1681,  -408,  -408,  -408,  -408,  1986,  -408,  1294,
    -408,  1294,  -408,   241,  -408,  1986,  -408,   235,   448,  -408,
    -408,  -408,  -408,   245,   254,  -408,  -408,  -408,  1151,  1986,
    1151,  1727,   -62,  1773,  1986,  1151,  -408,   256,   261,   263,
    1792,   266,   267,  -408,    47,  -408,    48,   270,  1986,  -408,
    -408,    61,   268,   272,  -408,   278,   276,   277,  -408,  -408,
    -408,  -408,  -408,   299,   -53,  -408,  1151,    68,  1856,  1875,
     -61,  -408,  -408,  -408,   -47,   -47,   279,   280,   -47,   -47,
    1986,  -408,  1206,  -408,  1986,   282,  1921,  -408,  -408,  -408,
    1294,  -408,  -408,  1151,   283,  -408,  1151,  1151,    99,  1151,
     101,  1967,   255,   259,   -47,   -47,   262,   265,  -408,  -408,
     286,  -408,  -408,  -408,   291,  -408,  -408,  -408,  -408,  1151,
    -408,  1151,  1151,   102,  1986,  1986,   271,   285,  1986,  1986,
    -408,  -408,  -408,  -408,  -408,  1151,   274,   288,  1986,  1986,
     292,   295,  -408,  1986,  1986,   296,   298,  1986,  1986,  -408,
    -408,  1986,  1986,  -408,  -408,  -408,  -408
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -408,  -408,    -1,   157,  -408,   152,  -408,  -408,   -20,  -408,
     -42,    -4,   -34,   141,   138,  -260,   -58,   -55,  -408,  -407,
      42,    30,  -408,  -408,   -99,  -408,   -18,   -84,     3,   -80,
     -21,   -15,   315,   252,  -408,     1,   221,  -408,  -408,  -408,
    -408,  -408,   242,    72,  -408,  -408,  -408,  -408,  -408,   387,
    -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,  -408,
     109,    31,    32,  -408,   307,    33,    34,  -408,  -408,  -408,
    -408,   116,  -408,  -408,  -375,  -408,  -408,  -408,  -408,  -408,
    -408,   118,  -408,  -408,  -408,  -408
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      40,    41,    93,    23,   199,    90,   154,   145,    94,   435,
     155,   363,   147,   365,   457,   458,   491,   418,   420,    63,
     250,    68,   396,    66,    67,    39,   148,    23,   213,   484,
      20,    31,    32,    33,    34,   227,    64,   228,    65,   233,
     234,   251,   251,   149,   251,   251,   235,   251,   229,   230,
     103,   488,   490,   251,    20,    31,    32,    33,    34,   243,
     244,     6,   108,   220,    98,   102,    68,   304,   323,   156,
      99,   305,   245,   246,   247,   248,   104,   151,   106,    73,
      74,    75,    76,    77,   513,   200,    39,   150,    39,   197,
     212,   259,   253,   256,   260,   265,   306,   499,   174,   101,
     269,   198,   307,   309,   194,   227,   422,   228,   152,   310,
     279,   115,   116,   117,   109,    39,   154,   408,   229,   230,
     155,   236,   237,   409,   118,   470,   472,   110,   154,   111,
      96,   471,   473,   231,    79,    80,    81,    82,    83,   476,
     321,   112,    85,    86,   114,   477,   424,   113,     1,    87,
     486,   263,   158,     2,     3,    39,   262,   153,    93,   454,
     174,    90,   159,   460,    94,   238,   239,   240,   318,   320,
       6,   174,   326,   302,   241,   242,   301,   424,   204,   424,
     424,   509,   174,   511,   525,   208,     4,     5,   423,   424,
     214,   313,   344,     6,   330,   331,   215,   216,    68,   217,
     348,   221,   218,   350,   222,   199,   252,   270,   358,   337,
     338,   339,   340,   341,   342,   223,   224,   154,   225,   367,
     226,   266,   249,   370,   271,   273,   274,   322,   324,   275,
     278,   276,   277,   272,   332,   333,   334,   335,   336,   280,
     284,   281,   376,   298,   285,   308,   311,   314,   317,   346,
     385,   175,   315,   393,   325,   300,   347,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     175,   354,   349,   355,   356,   359,   360,   361,   362,   174,
     379,   364,   174,   369,   174,   371,   200,   387,   373,   377,
     232,   253,   380,   148,   397,   395,   410,   398,   404,   175,
     175,   175,   154,   399,   400,   401,   402,   403,   405,   483,
     407,   447,   413,   175,   419,   444,   414,   321,   411,   412,
     429,   425,   432,   415,   175,   416,   433,   417,   427,   430,
     438,   449,   451,   463,   440,   175,    73,    74,    75,    76,
      77,   452,   442,   464,   443,   465,   446,   448,   468,   469,
     478,   474,   372,   350,   479,   300,   480,   375,   481,   482,
     506,   494,   495,   387,   501,   437,   367,   514,   520,   461,
     174,   515,   467,   521,   518,   175,   175,   519,   316,   175,
     475,   445,   146,   528,   533,   327,   328,   329,   406,   345,
     343,    79,    80,    81,    82,    83,   444,   529,   534,    85,
      86,   261,   537,   367,   367,   538,   541,   175,   542,   450,
     175,    97,   498,   381,   283,   312,   500,   174,   503,   174,
     303,   219,   382,     0,   174,   504,   175,   390,     0,     0,
     175,     0,   175,     0,     0,   175,   367,   175,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,   175,
       0,   387,     0,     0,     0,   174,   526,   527,     0,     0,
     530,   531,     0,   492,   493,     0,     0,   496,   497,     0,
     535,   536,     0,     0,     0,   539,   540,     0,     0,   543,
     544,     0,   174,   545,   546,   174,   174,     0,   174,     0,
     453,     0,   455,   516,   517,     0,     0,   462,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   174,     0,
     174,   174,     0,     0,     0,     0,   115,   116,   117,   257,
      39,     0,     0,   175,   174,     0,     0,     0,   485,   118,
       0,   258,     0,   119,   120,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   121,   122,     0,   175,
       0,   175,     0,   175,     0,   505,   123,   124,   507,   508,
     175,   510,    73,    74,    75,    76,    77,     0,     0,     0,
     175,     0,   175,   175,     0,     0,   175,   175,     0,     0,
       0,   522,     0,   523,   524,     0,     0,     0,   125,   126,
     127,   128,     0,     0,     0,   129,   130,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
     175,   175,     0,     0,     0,     0,     0,    79,    80,    81,
      82,    83,     0,     0,     0,    85,    86,   115,   116,   117,
     383,    39,   175,     0,     0,   175,     0,     0,   175,   175,
     118,   175,   384,   175,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   175,     0,   175,   175,     0,     0,   123,   124,    73,
      74,    75,    76,    77,     1,     0,     0,   175,     0,     2,
       3,     0,     0,     0,     0,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   125,
     126,   127,   128,     0,     0,     0,   129,   130,     0,     0,
       0,     0,     4,     5,     0,     0,     0,     0,     0,     6,
       0,     0,     0,     0,    79,    80,    81,    82,    83,     0,
       0,     0,    85,    86,   115,   116,   117,     0,    39,     0,
      79,    80,    81,    82,    83,     0,     0,   118,    85,    86,
       0,   119,   120,     0,     0,    87,   160,     0,   161,   162,
     163,   164,   165,   166,   121,   122,     0,   167,   168,   169,
       0,     3,     0,     0,   123,   124,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     0,   125,   126,   127,   128,
       6,     0,     0,   129,   130,   115,   116,   117,     0,    39,
       0,     0,   170,     0,   171,     0,     0,     0,   118,     0,
     172,   173,   119,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,     0,
       0,     0,     0,     0,     0,   123,   124,   160,     0,   161,
     162,   163,   164,   165,   166,     0,     0,     0,   167,   168,
     169,     0,     3,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,   125,   126,   127,
     128,     0,     0,     0,   129,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     0,     0,     0,     0,
       0,     6,     0,     0,     0,     0,   115,   116,   117,     0,
      39,     0,     0,   170,     0,   171,     0,     0,     0,   118,
       0,   172,   282,   119,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
       0,     0,     0,     0,     0,     0,   123,   124,   160,     0,
     161,   162,   163,   164,   165,   166,     0,     0,     0,   167,
     168,   169,     0,     3,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,   125,   126,
     127,   128,     0,     0,     0,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     0,     0,     0,
       0,     0,     6,     0,     0,     0,     0,   115,   116,   117,
       0,    39,     0,     0,   170,     0,   171,     0,     0,     0,
     118,     0,   172,   299,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,   123,   124,   160,
       0,   161,   162,   163,   164,   165,   166,     0,     0,     0,
     167,   168,   169,     0,     3,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   125,
     126,   127,   128,     0,     0,     0,   129,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     0,     0,
       0,     0,     0,     6,     0,     0,     0,     0,   115,   116,
     117,     0,    39,     0,     0,   170,     0,   171,     0,     0,
       0,   118,     0,   172,   374,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,     0,     0,     0,     0,   123,   124,
     160,     0,   161,   162,   163,   164,   165,   166,     0,     0,
       0,   167,   168,   169,     0,     3,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     125,   126,   127,   128,     0,     0,     0,   129,   130,    73,
      74,    75,    76,    77,     0,     0,     0,     4,     5,    73,
      74,    75,    76,    77,     6,     0,     0,     0,     0,   115,
     116,   117,     0,    39,     0,     0,   170,     0,   171,     0,
       0,     0,   118,     0,   172,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,     0,    79,    80,    81,    82,    83,   123,
     124,     0,    85,    86,    79,    80,    81,    82,    83,     0,
       0,     0,    85,    86,     0,     0,     0,   378,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,   125,   126,   127,   128,     0,     0,     0,   129,   130,
      79,    80,    81,    82,    83,     6,     0,     0,    85,    86,
     115,   116,   117,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   124,    79,    80,    81,    82,    83,     0,     0,     0,
      85,    86,   115,   116,   117,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,   118,     0,     0,     0,   119,
     120,     0,   125,   126,   127,   128,     0,     0,     0,   129,
     130,     0,   121,   122,     0,     0,     0,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,     0,   115,   116,
     117,     0,    39,     0,   267,   268,     0,     0,     0,     0,
       0,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   121,   122,
       0,   129,   130,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,     0,   115,   116,   117,   391,    39,     0,
       0,     0,     0,     0,     0,     0,     0,   118,     0,   392,
       0,   119,   120,   115,   116,   117,     0,    39,     0,     0,
     125,   126,   127,   128,   121,   122,   118,   129,   130,     0,
     119,   120,     0,     0,   123,   124,     0,     0,     0,     0,
     255,     0,     0,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,   115,
     116,   117,     0,    39,     0,   264,   125,   126,   127,   128,
       0,     0,   118,   129,   130,     0,   119,   120,   115,   116,
     117,     0,    39,     0,   357,   125,   126,   127,   128,   121,
     122,   118,   129,   130,     0,   119,   120,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   125,   126,   127,   128,     0,     0,     0,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,   126,   127,   128,     0,     0,     0,   129,   130,   115,
     116,   117,     0,    39,     0,     0,     0,     0,   366,     0,
       0,     0,   118,     0,     0,     0,   119,   120,   115,   116,
     117,     0,    39,     0,     0,     0,     0,   421,     0,   121,
     122,   118,     0,     0,     0,   119,   120,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,     0,   115,   116,   117,     0,    39,     0,
     428,   125,   126,   127,   128,     0,     0,   118,   129,   130,
       0,   119,   120,   115,   116,   117,     0,    39,     0,   431,
     125,   126,   127,   128,   121,   122,   118,   129,   130,     0,
     119,   120,     0,     0,   123,   124,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,   115,
     116,   117,   439,    39,     0,     0,   125,   126,   127,   128,
       0,     0,   118,   129,   130,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   121,
     122,     0,   129,   130,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,   115,   116,   117,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,   118,   456,
       0,     0,   119,   120,     0,     0,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   121,   122,     0,   129,   130,
       0,     0,     0,     0,     0,   123,   124,     0,     0,     0,
       0,   115,   116,   117,     0,    39,     0,     0,     0,     0,
     459,     0,     0,     0,   118,     0,     0,     0,   119,   120,
     115,   116,   117,     0,    39,     0,   466,   125,   126,   127,
     128,   121,   122,   118,   129,   130,     0,   119,   120,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,   126,   127,   128,     0,     0,     0,
     129,   130,     0,     0,   115,   116,   117,     0,    39,     0,
       0,     0,   125,   126,   127,   128,     0,   118,   487,   129,
     130,   119,   120,   115,   116,   117,     0,    39,     0,     0,
       0,     0,     0,     0,   121,   122,   118,   489,     0,     0,
     119,   120,     0,     0,   123,   124,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,   115,
     116,   117,   502,    39,     0,     0,   125,   126,   127,   128,
       0,     0,   118,   129,   130,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   121,
     122,     0,   129,   130,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,   115,   116,   117,     0,    39,
       0,    73,    74,    75,    76,    77,    78,     0,   118,   512,
       0,     0,   119,   120,   115,   116,   117,     0,    39,     0,
       0,   125,   126,   127,   128,   121,   122,   118,   129,   130,
       0,   119,   120,     0,     0,   123,   124,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
       0,     0,     0,     0,   123,   124,    79,    80,    81,    82,
      83,     0,     0,    84,    85,    86,     0,   125,   126,   127,
     128,    87,     0,     0,   129,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,   126,   127,   128,
       0,     0,     0,   129,   130,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60
};

static const yytype_int16 yycheck[] =
{
       1,     2,    23,     0,   103,    23,    90,    65,    23,   384,
      90,   271,    81,   273,   421,    77,    77,    82,    82,    71,
      79,    22,    82,    77,    78,    72,    95,    24,   108,    82,
       0,     0,     0,     0,     0,    95,    71,    97,    81,    87,
      88,   106,   106,   112,   106,   106,    94,   106,   108,   109,
      81,   458,   459,   106,    24,    24,    24,    24,    24,    89,
      90,    63,    81,   118,    77,    19,    67,    78,    70,    90,
      83,    82,   102,   103,   104,   105,    83,    78,    83,     3,
       4,     5,     6,     7,   491,   103,    72,    73,    72,    73,
     108,   149,   147,   148,   149,   153,    78,   472,    99,    80,
     158,   102,    84,    78,   101,    95,   366,    97,     8,    84,
     168,    68,    69,    70,   112,    72,   200,    78,   108,   109,
     200,    85,    86,    84,    81,    78,    78,   112,   212,   112,
       0,    84,    84,   134,    58,    59,    60,    61,    62,    78,
     224,   112,    66,    67,    78,    84,    78,   112,    18,    73,
      82,   152,    81,    23,    24,    72,    73,    81,   179,   419,
     161,   179,    81,   423,   179,    91,    92,    93,   223,   224,
      63,   172,   227,   194,   100,   101,   194,    78,    70,    78,
      78,    82,   183,    82,    82,    71,    56,    57,    77,    78,
      68,   212,   250,    63,   236,   237,    71,    71,   199,    71,
     255,    81,    71,   258,    81,   304,    82,    74,   266,   243,
     244,   245,   246,   247,   248,    81,    81,   301,    81,   274,
      81,    81,   107,   278,    81,    81,    81,   224,   225,    76,
      81,    77,    77,   161,   238,   239,   240,   241,   242,    80,
      77,    81,   297,    77,    80,    78,    78,    82,    73,    78,
     308,    99,    82,   311,    82,   183,    82,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     118,   110,    96,    82,    82,    82,    82,    82,    82,   280,
     301,    12,   283,    77,   285,    80,   304,   308,    68,    77,
     138,   346,    81,    95,    78,    82,   354,    82,    96,   147,
     148,   149,   386,    78,    82,    78,    82,    78,    80,    10,
      96,    70,    82,   161,    81,   399,    82,   401,    87,    87,
     378,    82,   380,    87,   172,    87,   384,    87,    82,    81,
      81,    96,    87,    77,   392,   183,     3,     4,     5,     6,
       7,    87,   397,    82,   399,    82,   401,   405,    82,    82,
      82,    81,   280,   408,    82,   283,    78,   285,    82,    82,
      77,    82,    82,   384,    82,   386,   421,   112,    82,   424,
     371,   112,   430,    82,   112,   223,   224,   112,   221,   227,
     438,   401,    67,   112,   110,   233,   234,   235,   346,   251,
     249,    58,    59,    60,    61,    62,   480,   112,   110,    66,
      67,   149,   110,   458,   459,   110,   110,   255,   110,   408,
     258,    24,   470,   304,   172,    82,   474,   418,   476,   420,
     199,   114,   306,    -1,   425,   480,   274,   309,    -1,    -1,
     278,    -1,   280,    -1,    -1,   283,   491,   285,    -1,    -1,
      -1,    -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,   297,
      -1,   472,    -1,    -1,    -1,   456,   514,   515,    -1,    -1,
     518,   519,    -1,   464,   465,    -1,    -1,   468,   469,    -1,
     528,   529,    -1,    -1,    -1,   533,   534,    -1,    -1,   537,
     538,    -1,   483,   541,   542,   486,   487,    -1,   489,    -1,
     418,    -1,   420,   494,   495,    -1,    -1,   425,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   509,    -1,
     511,   512,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      72,    -1,    -1,   371,   525,    -1,    -1,    -1,   456,    81,
      -1,    83,    -1,    85,    86,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    98,    99,    -1,   397,
      -1,   399,    -1,   401,    -1,   483,   108,   109,   486,   487,
     408,   489,     3,     4,     5,     6,     7,    -1,    -1,    -1,
     418,    -1,   420,   421,    -1,    -1,   424,   425,    -1,    -1,
      -1,   509,    -1,   511,   512,    -1,    -1,    -1,   140,   141,
     142,   143,    -1,    -1,    -1,   147,   148,   525,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   456,    -1,
     458,   459,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,   480,    -1,    -1,   483,    -1,    -1,   486,   487,
      81,   489,    83,   491,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,   509,    -1,   511,   512,    -1,    -1,   108,   109,     3,
       4,     5,     6,     7,    18,    -1,    -1,   525,    -1,    23,
      24,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,    -1,    -1,    -1,   147,   148,    -1,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    -1,    72,    -1,
      58,    59,    60,    61,    62,    -1,    -1,    81,    66,    67,
      -1,    85,    86,    -1,    -1,    73,     9,    -1,    11,    12,
      13,    14,    15,    16,    98,    99,    -1,    20,    21,    22,
      -1,    24,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    57,    -1,   140,   141,   142,   143,
      63,    -1,    -1,   147,   148,    68,    69,    70,    -1,    72,
      -1,    -1,    75,    -1,    77,    -1,    -1,    -1,    81,    -1,
      83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   109,     9,    -1,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    20,    21,
      22,    -1,    24,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,    -1,    -1,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    -1,    77,    -1,    -1,    -1,    81,
      -1,    83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,     9,    -1,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    22,    -1,    24,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,    -1,    -1,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,    -1,
      81,    -1,    83,    84,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,     9,
      -1,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      20,    21,    22,    -1,    24,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    -1,    -1,    -1,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,
      -1,    81,    -1,    83,    84,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
       9,    -1,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    -1,    24,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,    -1,    -1,   147,   148,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    56,    57,     3,
       4,     5,     6,     7,    63,    -1,    -1,    -1,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    -1,    77,    -1,
      -1,    -1,    81,    -1,    83,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    58,    59,    60,    61,    62,   108,
     109,    -1,    66,    67,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    81,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    -1,    -1,    -1,   147,   148,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,    67,
      68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,
      86,    -1,   140,   141,   142,   143,    -1,    -1,    -1,   147,
     148,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    68,    69,
      70,    -1,    72,    -1,    74,    75,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    85,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,    98,    99,
      -1,   147,   148,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    83,
      -1,    85,    86,    68,    69,    70,    -1,    72,    -1,    -1,
     140,   141,   142,   143,    98,    99,    81,   147,   148,    -1,
      85,    86,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    68,
      69,    70,    -1,    72,    -1,    74,   140,   141,   142,   143,
      -1,    -1,    81,   147,   148,    -1,    85,    86,    68,    69,
      70,    -1,    72,    -1,    74,   140,   141,   142,   143,    98,
      99,    81,   147,   148,    -1,    85,    86,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,    -1,    -1,    -1,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,    -1,    -1,    -1,   147,   148,    68,
      69,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    85,    86,    68,    69,
      70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    98,
      99,    81,    -1,    -1,    -1,    85,    86,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,    -1,    -1,    68,    69,    70,    -1,    72,    -1,
      74,   140,   141,   142,   143,    -1,    -1,    81,   147,   148,
      -1,    85,    86,    68,    69,    70,    -1,    72,    -1,    74,
     140,   141,   142,   143,    98,    99,    81,   147,   148,    -1,
      85,    86,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    -1,    -1,   140,   141,   142,   143,
      -1,    -1,    81,   147,   148,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,    98,
      99,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,    98,    99,    -1,   147,   148,
      -1,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    81,    -1,    -1,    -1,    85,    86,
      68,    69,    70,    -1,    72,    -1,    74,   140,   141,   142,
     143,    98,    99,    81,   147,   148,    -1,    85,    86,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,    -1,    -1,    -1,
     147,   148,    -1,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    -1,   140,   141,   142,   143,    -1,    81,    82,   147,
     148,    85,    86,    68,    69,    70,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    81,    82,    -1,    -1,
      85,    86,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,   109,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    -1,    -1,   140,   141,   142,   143,
      -1,    -1,    81,   147,   148,    -1,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,    98,
      99,    -1,   147,   148,    -1,    -1,    -1,    -1,    -1,   108,
     109,    -1,    -1,    -1,    -1,    68,    69,    70,    -1,    72,
      -1,     3,     4,     5,     6,     7,     8,    -1,    81,    82,
      -1,    -1,    85,    86,    68,    69,    70,    -1,    72,    -1,
      -1,   140,   141,   142,   143,    98,    99,    81,   147,   148,
      -1,    85,    86,    -1,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,    58,    59,    60,    61,
      62,    -1,    -1,    65,    66,    67,    -1,   140,   141,   142,
     143,    73,    -1,    -1,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
      -1,    -1,    -1,   147,   148,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    18,    23,    24,    56,    57,    63,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     174,   175,   176,   181,   201,   202,   203,   204,   205,   206,
     207,   214,   215,   218,   219,   220,   221,   230,   231,    72,
     155,   155,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,   216,   217,    71,    71,    81,    77,    78,   155,   185,
     189,   190,   191,     3,     4,     5,     6,     7,     8,    58,
      59,    60,    61,    62,    65,    66,    67,    73,   177,   178,
     179,   180,   182,   183,   184,   208,     0,   202,    77,    83,
     211,    80,    19,    81,    83,   222,    83,   232,    81,   112,
     112,   112,   112,   112,    78,    68,    69,    70,    81,    85,
      86,    98,    99,   108,   109,   140,   141,   142,   143,   147,
     148,   154,   155,   156,   157,   158,   159,   160,   162,   163,
     164,   165,   166,   167,   168,   169,   185,    81,    95,   112,
      73,   155,     8,    81,   180,   182,   183,   184,    81,    81,
       9,    11,    12,    13,    14,    15,    16,    20,    21,    22,
      75,    77,    83,    84,   155,   158,   169,   170,   174,   181,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   214,
     215,   218,   219,   238,   181,   209,   210,    73,   155,   177,
     179,   183,   212,   213,    70,   223,   224,   225,    71,   233,
     234,   235,   179,   182,    68,    71,    71,    71,    71,   217,
     170,    81,    81,    81,    81,    81,    81,    95,    97,   108,
     109,   155,   158,    87,    88,    94,    85,    86,    91,    92,
      93,   100,   101,    89,    90,   102,   103,   104,   105,   107,
      79,   106,    82,   170,   173,    95,   170,    71,    83,   169,
     170,   186,    73,   155,    74,   169,    81,    74,    75,   169,
      74,    81,   196,    81,    81,    76,    77,    77,    81,   169,
      80,    81,    84,   195,    77,    80,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   171,    77,    84,
     196,   179,   183,   189,    78,    82,    78,    84,    78,    78,
      84,    78,    82,   183,    82,    82,   156,    73,   170,   161,
     170,   180,   181,    70,   181,    82,   170,   158,   158,   158,
     163,   163,   164,   164,   164,   164,   164,   165,   165,   165,
     165,   165,   165,   166,   169,   167,    78,    82,   170,    96,
     170,   186,   187,   188,   110,    82,    82,    74,   169,    82,
      82,    82,    82,   168,    12,   168,    77,   170,   172,    77,
     170,    80,   196,    68,    84,   196,   170,    77,    81,   183,
      81,   213,   224,    71,    83,   169,   179,   183,   227,   229,
     234,    71,    83,   169,   236,    82,    82,    78,    82,    78,
      82,    78,    82,    78,    96,    80,   173,    96,    78,    84,
     169,    87,    87,    82,    82,    87,    87,    87,    82,    81,
      82,    77,   168,    77,    78,    82,   196,    82,    74,   169,
      81,    74,   169,   169,   226,   227,   228,   183,    81,    71,
     169,   237,   170,   170,   180,   161,   170,    70,   169,    96,
     188,    87,    87,   196,   168,   196,    82,   172,    77,    77,
     168,   170,   196,    77,    82,    82,    74,   169,    82,    82,
      78,    84,    78,    84,    81,   169,    78,    84,    82,    82,
      78,    82,    82,    10,    82,   196,    82,    82,   172,    82,
     172,    77,   155,   155,    82,    82,   155,   155,   169,   227,
     169,    82,    71,   169,   170,   196,    77,   196,   196,    82,
     196,    82,    82,   172,   112,   112,   155,   155,   112,   112,
      82,    82,   196,   196,   196,    82,   169,   169,   112,   112,
     169,   169,   196,   110,   110,   169,   169,   110,   110,   169,
     169,   110,   110,   169,   169,   169,   169
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 206 "parser.y"
    { (yyval.np) = eval_ident((yyvsp[(1) - (1)].sp)); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 207 "parser.y"
    { (yyval.np) = leafi(INT_LITERAL, (yyvsp[(1) - (1)].ival)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 208 "parser.y"
    { (yyval.np) = leafd(DOUBLE_LITERAL, (yyvsp[(1) - (1)].dval)); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 209 "parser.y"
    { (yyval.np) = leafi(INT_LITERAL, (yyvsp[(1) - (1)].bit)->val); free((yyvsp[(1) - (1)].bit)); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 210 "parser.y"
    { (yyval.np) = (yyvsp[(2) - (3)].np); (yyval.np)->paren = 1; ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 221 "parser.y"
    { (yyval.np) = eval_subarray((yyvsp[(1) - (4)].np), (yyvsp[(3) - (4)].np)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 224 "parser.y"
    { 
        (yyval.np) = eval_scoped((yyvsp[(1) - (2)].np), (yyvsp[(2) - (2)].sp));

        /* remove two temporary contexts */
        s_remcontext();
        s_remcontext();
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 231 "parser.y"
    { (yyval.np) = eval_incdec((yyvsp[(2) - (2)].ival), (yyvsp[(1) - (2)].np)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 232 "parser.y"
    { (yyval.np) = eval_incdec((yyvsp[(2) - (2)].ival), (yyvsp[(1) - (2)].np)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 236 "parser.y"
    {
        /* activate the context of the class; if not available, eval_scoped above will catch it */
        if ((yyvsp[(1) - (2)].np)->ptype!=NULL && (yyvsp[(1) - (2)].np)->ptype->cxt!=NULL) s_inscontext((yyvsp[(1) - (2)].np)->ptype->cxt);
        /* add another context to protect the class members */
        s_newcontext(0);
        (yyval.np) = (yyvsp[(1) - (2)].np);
    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 247 "parser.y"
    { (yyval.np) = eval_array((yyvsp[(1) - (1)].np)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 248 "parser.y"
    { (yyval.np) = eval_unary((yyvsp[(1) - (2)].ival), (yyvsp[(2) - (2)].np)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 252 "parser.y"
    { (yyval.np) = eval_lengthof(eval_array((yyvsp[(3) - (4)].np))); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 253 "parser.y"
    { (yyval.np) = eval_isidof((yyvsp[(3) - (6)].sp), (yyvsp[(5) - (6)].np)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 254 "parser.y"
    { (yyval.np) = eval_skipbits((yyvsp[(3) - (4)].np)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 256 "parser.y"
    { 
        (yyval.np) = node(NEXTCODE, leafb(BIT_LITERAL, (yyvsp[(3) - (4)].bit)), NULL);
        (yyval.np)->type=INT; (yyval.np)->ptype = s_lookup("int"); 
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 260 "parser.y"
    { 
        (yyval.np)=node(NEXTCODE, leafb(BIT_LITERAL, (yyvsp[(5) - (6)].bit)), (yyvsp[(3) - (6)].np));
        (yyval.np)->type = INT; (yyval.np)->ptype = s_lookup("int"); 
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 264 "parser.y"
    { (yyval.np) = leaf(NUMBITS); (yyval.np)->type = INT; (yyval.np)->ptype = s_lookup("int"); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 268 "parser.y"
    { (yyval.np) = eval_nextbits((yyvsp[(3) - (4)].np), 0, NULL); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 269 "parser.y"
    { (yyval.np) = eval_nextbits((yyvsp[(5) - (6)].np), (yyvsp[(3) - (6)].ival), NULL); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 270 "parser.y"
    { (yyval.np) = eval_nextbits((yyvsp[(5) - (6)].np), 0, (yyvsp[(3) - (6)].np)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 271 "parser.y"
    { (yyval.np) = eval_nextbits((yyvsp[(7) - (8)].np), (yyvsp[(5) - (8)].ival), (yyvsp[(3) - (8)].np)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 275 "parser.y"
    { (yyval.ival) = s_tok2mod((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 276 "parser.y"
    { (yyval.ival) = check_mod((yyvsp[(1) - (3)].ival), s_tok2mod((yyvsp[(3) - (3)].ival))); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 290 "parser.y"
    { (yyval.np) = eval_mult((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 291 "parser.y"
    { (yyval.np) = eval_mult((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 292 "parser.y"
    { (yyval.np) = eval_mult((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 297 "parser.y"
    { (yyval.np) = eval_add((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 298 "parser.y"
    { (yyval.np) = eval_add((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 303 "parser.y"
    { (yyval.np) = eval_shift((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 304 "parser.y"
    { (yyval.np) = eval_shift((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 305 "parser.y"
    { (yyval.np) = eval_shift((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 306 "parser.y"
    { (yyval.np) = eval_shift((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 307 "parser.y"
    { (yyval.np) = eval_shift((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 312 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 313 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 314 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 315 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 316 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 317 "parser.y"
    { (yyval.np) = eval_rel((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 322 "parser.y"
    { (yyval.np) = eval_logic((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 326 "parser.y"
    { (yyval.np) = check_unassigned((yyvsp[(1) - (1)].np), (yyvsp[(1) - (1)].np)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 327 "parser.y"
    { (yyval.np) = eval_logic((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    { (yyval.np) = eval_cond((yyvsp[(2) - (5)].ival), (yyvsp[(1) - (5)].np), (yyvsp[(3) - (5)].np), (yyvsp[(5) - (5)].np)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 337 "parser.y"
    { (yyval.np) = check_assign((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 356 "parser.y"
    { (yyval.np) = node((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 361 "parser.y"
    { (yyval.np) = node((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 373 "parser.y"
    {
        (yyval.lp) = (yyvsp[(2) - (2)].lp);

        /* copy parse, align, and usenext */
        (yyval.lp)->e2 = check_parse((yyvsp[(1) - (2)].lp)->sp, (yyvsp[(1) - (2)].lp)->type, (yyvsp[(1) - (2)].lp)->e1);
        
        /* also check for lookahead parsing of escaped maps */
        if ((yyvsp[(1) - (2)].lp)->usenext && (yyvsp[(1) - (2)].lp)->e1->op == MAP_TYPE && (yyvsp[(1) - (2)].lp)->e1->left.sp->escape)
            yyerror("Map with escape (%s) cannot be used in lookahead parsing", (yyvsp[(1) - (2)].lp)->e1->left.sp->name);

        /* check that for a bac, the declarator must be accompanied with param types */
        if ((yyvsp[(1) - (2)].lp)->e1 && (yyvsp[(1) - (2)].lp)->e1->op == BAC_TYPE) 
            if ((yyvsp[(2) - (2)].lp) == NULL || (yyvsp[(2) - (2)].lp)->e3 == NULL)
                yyerror("BAC (%s) must be accompanied with corresponding parameters", (yyvsp[(1) - (2)].lp)->e1->left.sp->name);
        
        (yyval.lp)->align = (yyvsp[(1) - (2)].lp)->align;
        (yyval.lp)->usenext = (yyvsp[(1) - (2)].lp)->usenext;
        
        /* if BIT type, treat as unsigned */
        if ((yyvsp[(1) - (2)].lp)->sp->ident == BIT) (yyvsp[(1) - (2)].lp)->type |= M_UNSIGNED;
        
        /* set type, mods, parse, init, check param, check alignment, dims and dim values */
        (yyval.lp)->sp = set_var((yyval.lp)->sp, (yyvsp[(1) - (2)].lp)->sp, (yyvsp[(1) - (2)].lp)->type, (yyvsp[(2) - (2)].lp)->e2, (yyvsp[(2) - (2)].lp)->e1, (yyvsp[(2) - (2)].lp)->e3, (yyvsp[(1) - (2)].lp)->align, (yyvsp[(2) - (2)].lp)->dims, (yyvsp[(2) - (2)].lp)->dim, 0, (yyval.lp)->sp->range);
        
        /* if we have a static array declaration of a non-global variable, unroll it */
        if ((yyvsp[(2) - (2)].lp)->e1 != NULL && (yyvsp[(2) - (2)].lp)->e1->op == ARRAY && (yyvsp[(2) - (2)].lp)->sp->cxt_level > 0)
            (yyval.lp) = c_array_unroll((yyval.lp)); 
    ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 401 "parser.y"
    {
        /* copy parse, align, and usenext */
        (yyvsp[(3) - (3)].lp)->e2 = (yyvsp[(1) - (3)].lp)->e2;
        (yyvsp[(3) - (3)].lp)->align = (yyvsp[(1) - (3)].lp)->align;
        (yyvsp[(3) - (3)].lp)->usenext = (yyvsp[(1) - (3)].lp)->usenext;
 
        /* set type, mods, parse, init, check param, check alignment, dims and dim values */
        (yyvsp[(3) - (3)].lp)->sp = set_var((yyvsp[(3) - (3)].lp)->sp, (yyvsp[(1) - (3)].lp)->sp->ptype, (yyvsp[(1) - (3)].lp)->sp->modifiers, (yyvsp[(3) - (3)].lp)->e2, (yyvsp[(3) - (3)].lp)->e1, (yyvsp[(3) - (3)].lp)->e3, (yyvsp[(1) - (3)].lp)->align, (yyvsp[(3) - (3)].lp)->dims, (yyvsp[(3) - (3)].lp)->dim, 0, (yyvsp[(3) - (3)].lp)->sp->range);
 
        /* if we have a static array declaration of a non-global variable, unroll it */
        if ((yyvsp[(3) - (3)].lp)->e1 != NULL && (yyvsp[(3) - (3)].lp)->e1->op == ARRAY && (yyvsp[(3) - (3)].lp)->sp->cxt_level > 0)
            (yyval.lp) = app((yyvsp[(1) - (3)].lp), c_array_unroll((yyvsp[(3) - (3)].lp))); 
        else
            (yyval.lp) = app((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp));
    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 419 "parser.y"
    { (yyvsp[(2) - (2)].lp)->align = (yyvsp[(1) - (2)].np); (yyval.lp) = (yyvsp[(2) - (2)].lp); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 420 "parser.y"
    { (yyvsp[(2) - (2)].lp)->align = (yyvsp[(1) - (2)].np); (yyval.lp) = (yyvsp[(2) - (2)].lp); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 424 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (1)].sp), NULL, 0); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 425 "parser.y"
    {
        if (!((yyvsp[(1) - (1)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified");
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (1)].ival), s_lookup("int")), s_lookup("int"), NULL, 0);
    ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 429 "parser.y"
    {
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (2)].ival), (yyvsp[(2) - (2)].sp)), (yyvsp[(2) - (2)].sp), NULL, 0);
    ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 435 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (4)].sp), (yyvsp[(3) - (4)].np), 0); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 436 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (4)].sp), leafs(MAP_TYPE, (yyvsp[(3) - (4)].sp)), 0); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 437 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (4)].sp), leafs(MAP_TYPE, (yyvsp[(3) - (4)].sp)), 0); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 438 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (5)].sp), (yyvsp[(3) - (5)].np), 1); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 439 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (5)].sp), leafs(MAP_TYPE, (yyvsp[(3) - (5)].sp)), 1); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 440 "parser.y"
    { (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (5)].sp), leafs(MAP_TYPE, (yyvsp[(3) - (5)].sp)), 1); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 441 "parser.y"
    {
        if (!((yyvsp[(1) - (4)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified");
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (4)].ival), s_lookup("int")), s_lookup("int"), (yyvsp[(3) - (4)].np), 0);
    ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 445 "parser.y"
    {
        if (!((yyvsp[(1) - (4)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified");
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (4)].ival), s_lookup("int")), s_lookup("int"), leafs(MAP_TYPE, (yyvsp[(3) - (4)].sp)), 0);
    ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 449 "parser.y"
    {
        if (!((yyvsp[(1) - (5)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified");
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (5)].ival), s_lookup("int")), s_lookup("int"), (yyvsp[(3) - (5)].np), 1);
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 453 "parser.y"
    {
        if (!((yyvsp[(1) - (5)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified");
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (5)].ival), s_lookup("int")), s_lookup("int"), leafs(MAP_TYPE, (yyvsp[(3) - (5)].sp)), 1);
    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 457 "parser.y"
    {
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (5)].ival), (yyvsp[(2) - (5)].sp)), (yyvsp[(2) - (5)].sp), (yyvsp[(4) - (5)].np), 0);
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 460 "parser.y"
    {
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (5)].ival), (yyvsp[(2) - (5)].sp)), (yyvsp[(2) - (5)].sp), leafs(MAP_TYPE, (yyvsp[(4) - (5)].sp)), 0);
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 463 "parser.y"
    {
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (6)].ival), (yyvsp[(2) - (6)].sp)), (yyvsp[(2) - (6)].sp), (yyvsp[(4) - (6)].np), 1);
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 466 "parser.y"
    {
        (yyval.lp) = lnew_type(check_modtype((yyvsp[(1) - (6)].ival), (yyvsp[(2) - (6)].sp)), (yyvsp[(2) - (6)].sp), leafs(MAP_TYPE, (yyvsp[(4) - (6)].sp)), 1);
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 469 "parser.y"
    {
        (yyval.lp) = lnew_type(M_NONE, (yyvsp[(1) - (4)].sp), leafs(BAC_TYPE, (yyvsp[(3) - (4)].sp)), 0);
    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 475 "parser.y"
    { (yyval.ival) = s_tok2mod((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 476 "parser.y"
    { (yyval.ival) = check_mod((yyvsp[(1) - (2)].ival), s_tok2mod((yyvsp[(2) - (2)].ival))); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 490 "parser.y"
    { (yyval.np) = NULL; ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 491 "parser.y"
    { (yyval.np) = leafi(INT_LITERAL, 8); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 492 "parser.y"
    { (yyval.np) = check_align_arg((yyvsp[(3) - (4)].np)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 509 "parser.y"
    {
        if (w_cyclic && s_get_cur_class() == (yyvsp[(1) - (1)].sp)) 
            yyerror("Declaration creates cyclic reference in class '%s'", (yyvsp[(1) - (1)].sp)->name);
        (yyval.sp) = (yyvsp[(1) - (1)].sp);
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 518 "parser.y"
    {   
        (yyvsp[(1) - (3)].lp)->sp->assigned = 1;
        (yyvsp[(1) - (3)].lp)->e1 = (yyvsp[(3) - (3)].np);
        (yyval.lp) = (yyvsp[(1) - (3)].lp);
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 523 "parser.y"
    {
        (yyvsp[(1) - (3)].lp)->sp->assigned = 1;
        (yyvsp[(1) - (3)].lp)->e1 = (yyvsp[(3) - (3)].np);
        (yyval.lp) = (yyvsp[(1) - (3)].lp);
    ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 533 "parser.y"
    { 
        (yyval.np) = node(ARRAY, (yyvsp[(2) - (3)].np), NULL); 
        (yyval.np)->ptype = (yyvsp[(2) - (3)].np)->ptype;
        (yyval.np)->type = (yyvsp[(2) - (3)].np)->type;
    ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 538 "parser.y"
    {
        (yyval.np) = node(ARRAY, eval_array_string((yyvsp[(1) - (1)].string)), NULL);
        (yyval.np)->ptype = s_lookup("char");
        (yyval.np)->type = CHAR;
    ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 548 "parser.y"
    { 
        (yyval.np) = node((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].np), (yyvsp[(3) - (3)].np)); 
        (yyval.np)->ptype = check_type_promote((yyvsp[(1) - (3)].np)->ptype, (yyvsp[(3) - (3)].np)->ptype);
        (yyval.np)->type = (yyval.np)->ptype->ident;
    ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 561 "parser.y"
    { (yyval.lp) = lnew_sp(DECL, (yyvsp[(1) - (1)].sp)); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 562 "parser.y"
    {
        /* TBD: If the array is not parsable and not initialized, then the dimx_end variable is useless */
        s_cur_class_dimx_nonpartial((yyvsp[(1) - (4)].lp)->dims, 1);
        (yyvsp[(1) - (4)].lp)->dims++;
        (yyvsp[(1) - (4)].lp)->dim = (node_t **)buildarray((void **)(yyvsp[(1) - (4)].lp)->dim, node((yyvsp[(2) - (4)].ival), check_array_size((yyvsp[(3) - (4)].np)), NULL));
        (yyval.lp) = (yyvsp[(1) - (4)].lp);
    ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 569 "parser.y"
    {
        s_cur_class_dimx_nonpartial((yyvsp[(1) - (6)].lp)->dims, 0);
        (yyvsp[(1) - (6)].lp)->dims++;
        (yyvsp[(1) - (6)].lp)->dim = (node_t **)buildarray((void **)(yyvsp[(1) - (6)].lp)->dim, node(DBL_BRACE_L, check_array_index((yyvsp[(4) - (6)].np)), NULL));
        (yyval.lp) = (yyvsp[(1) - (6)].lp);
    ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 578 "parser.y"
    {
        /* add argument expression list to declaration */
        (yyvsp[(1) - (4)].lp)->e3 = (yyvsp[(3) - (4)].np);
        (yyval.lp) = (yyvsp[(1) - (4)].lp);
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 587 "parser.y"
    {
        (yyvsp[(1) - (5)].lp)->sp->range = 1;
        (yyvsp[(1) - (5)].lp)->sp->assigned = 1;
        (yyvsp[(1) - (5)].lp)->e1 = idrange((yyvsp[(3) - (5)].np), (yyvsp[(5) - (5)].np));
        (yyval.lp) = (yyvsp[(1) - (5)].lp);   
    ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 607 "parser.y"
    { 
        if ((yyvsp[(1) - (3)].sp)->ptype != NULL) yyerror("Use of variable '%s' as a label is illegal", (yyvsp[(1) - (3)].sp)->name);
        (yyvsp[(1) - (3)].sp)->ident = LABEL;
        (yyval.lp) = prep((yyvsp[(3) - (3)].lp), lnew_sp(LABEL, (yyvsp[(1) - (3)].sp)));
    ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 613 "parser.y"
    {
        if ((yyvsp[(2) - (4)].np)->type > INT) yyerror("The 'case' expression must be of integral type");
        if (!is_literal((yyvsp[(2) - (4)].np)->op) || ((yyvsp[(2) - (4)].np)->op == VARIABLE && !((yyvsp[(2) - (4)].np)->left.sp->modifiers & M_CONST))) yyerror("The 'case' expression must be a constant");
        (yyval.lp) = lnews1((yyvsp[(1) - (4)].ival), (yyvsp[(2) - (4)].np), (yyvsp[(4) - (4)].lp));
    ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 618 "parser.y"
    { (yyval.lp) = lnews0((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].lp)); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 622 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (2)].ival)); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 623 "parser.y"
    { (yyval.lp) = lnews0((yyvsp[(1) - (3)].ival), (yyvsp[(2) - (3)].lp)); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 628 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (2)].lp), (yyvsp[(2) - (2)].lp)); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 641 "parser.y"
    { (yyval.lp) = lnew0(EXPR); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 642 "parser.y"
    { (yyval.lp) = lnew1(EXPR, (yyvsp[(1) - (2)].np)); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 646 "parser.y"
    { (yyval.lp) = lnew_ifelse((yyvsp[(3) - (5)].np), (yyvsp[(5) - (5)].lp), NULL); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 647 "parser.y"
    { (yyval.lp) = lnew_ifelse((yyvsp[(3) - (7)].np), (yyvsp[(5) - (7)].lp), (yyvsp[(7) - (7)].lp)); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 650 "parser.y"
    {
        if ((yyvsp[(3) - (5)].np)->type>INT) yyerror("'switch' expression must be of integral type");
        (yyval.lp) = lnews1((yyvsp[(1) - (5)].ival), (yyvsp[(3) - (5)].np), (yyvsp[(5) - (5)].lp));
    ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 657 "parser.y"
    { (yyval.lp) = lnews1((yyvsp[(1) - (5)].ival), (yyvsp[(3) - (5)].np), (yyvsp[(5) - (5)].lp)); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 658 "parser.y"
    { (yyval.lp) = lnews1((yyvsp[(1) - (7)].ival), (yyvsp[(5) - (7)].np), (yyvsp[(2) - (7)].lp)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 659 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (6)].ival), NULL, NULL, NULL, (yyvsp[(6) - (6)].lp)); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 660 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (7)].ival), NULL, NULL, (yyvsp[(5) - (7)].np),   (yyvsp[(7) - (7)].lp)); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 661 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (7)].ival), NULL, (yyvsp[(4) - (7)].np),   NULL, (yyvsp[(7) - (7)].lp)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 662 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (8)].ival), NULL, (yyvsp[(4) - (8)].np),   (yyvsp[(6) - (8)].np),   (yyvsp[(8) - (8)].lp)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 663 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (7)].ival), (yyvsp[(3) - (7)].np),   NULL, NULL, (yyvsp[(7) - (7)].lp)); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 664 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (8)].ival), (yyvsp[(3) - (8)].np),   NULL, (yyvsp[(6) - (8)].np),   (yyvsp[(8) - (8)].lp)); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 665 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (8)].ival), (yyvsp[(3) - (8)].np),   (yyvsp[(5) - (8)].np),   NULL, (yyvsp[(8) - (8)].lp)); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 666 "parser.y"
    { (yyval.lp) = lnews3((yyvsp[(1) - (9)].ival), (yyvsp[(3) - (9)].np),   (yyvsp[(5) - (9)].np),   (yyvsp[(7) - (9)].np),   (yyvsp[(9) - (9)].lp)); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 670 "parser.y"
    { (yyval.lp) = lnew_sp((yyvsp[(1) - (3)].ival), (yyvsp[(2) - (3)].sp)); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 671 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (2)].ival)); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 672 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (2)].ival)); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 677 "parser.y"
    { 
        /* 'program' may be called multiple times (when we do includes) */
        (yyval.lp) = parsed_code=app(parsed_code, (yyvsp[(1) - (1)].lp));
    ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 681 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (2)].lp), (yyvsp[(2) - (2)].lp)); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 701 "parser.y"
    {
        if ((yyvsp[(1) - (2)].sp)->forward == 1) {
            (yyvsp[(1) - (2)].sp)->forward = 2; 
            /* check for cyclic references in contained members, if requested */
            if (w_cyclic) check_cyclic_decl_contained((yyvsp[(1) - (2)].sp), (yyvsp[(1) - (2)].sp)->forwardref);
        }
        /* if it has align spec, then it is considered parsable */
        if ((yyvsp[(1) - (2)].sp)->align != NULL) (yyvsp[(1) - (2)].sp)->parsable = 1;

        s_remcontext();         /* exit param type scope */
        s_remcontext();         /* exit class scope (will not be deleted) */
        s_set_cur_class(NULL);  /* exiting class decl */
        (yyval.lp) = lnews_sp(CLASS, (yyvsp[(1) - (2)].sp), (yyvsp[(2) - (2)].lp)); 
    ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 716 "parser.y"
    {
        (yyvsp[(1) - (2)].sp)->forward = 1;        /* mark as a forward declaration */
        (yyvsp[(1) - (2)].sp)->parsable = 1;       /* all forwards are parsable */
        (yyvsp[(1) - (2)].sp)->accessed--;         /* don't count forward decl as access */
        /* forward declarations shouldn't use 'extends' */
        if ((yyvsp[(1) - (2)].sp)->ptype != NULL) {
            yyerror("Forward declaration of a class cannot use 'extends'");
            fatal("Translation aborted");
        }
        /* they should also not use IDs */
        if ((yyvsp[(1) - (2)].sp)->id != NULL) {
            yyerror("Forward declaration of a class cannot use an ID declaration");
            fatal("Translation aborted");
        }
        /* they cannot use the abstract keyword (although it wouldn't do harm) */
        if ((yyvsp[(1) - (2)].sp)->abstract == 1) {
            yyerror("Forward declaration of a class should not use 'abstract'");
            (yyvsp[(1) - (2)].sp)->abstract = 0;
        }
        /* finally, they cannot use align() */
        if ((yyvsp[(1) - (2)].sp)->align != NULL) {
            yyerror("Forward declaration of a class should not use 'align'");
            (yyvsp[(1) - (2)].sp)->align = NULL;
        }
        s_remcontext();         /* exit param type scope */
        s_remcontext();         /* exit class scope (will not be deleted) */
        s_set_cur_class(NULL);  /* exiting class decl */
        (yyval.lp) = lnews_sp(CLASS, (yyvsp[(1) - (2)].sp), NULL); 
    ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 749 "parser.y"
    {
        if ((yyvsp[(1) - (1)].sp)->abstract && (yyvsp[(1) - (1)].sp)->id==NULL) {
            yyerror("Abstract class '%s' requires an ID", (yyvsp[(1) - (1)].sp)->name);
            (yyvsp[(1) - (1)].sp)->abstract=0;
        }
        (yyval.sp) = (yyvsp[(1) - (1)].sp);
    ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 757 "parser.y"
    { 
        (yyvsp[(1) - (3)].sp)->id = (yyvsp[(3) - (3)].sp);
        s_transfer((yyvsp[(3) - (3)].sp), (yyvsp[(1) - (3)].sp)->cxt);
        if (!(yyvsp[(1) - (3)].sp)->abstract) check_id((yyvsp[(1) - (3)].sp));
        (yyval.sp) = (yyvsp[(1) - (3)].sp);
    ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 764 "parser.y"
    { 
        (yyvsp[(1) - (3)].sp)->id = (yyvsp[(3) - (3)].sp);
        s_transfer((yyvsp[(3) - (3)].sp), (yyvsp[(1) - (3)].sp)->cxt);
        if (!(yyvsp[(1) - (3)].sp)->abstract) check_id((yyvsp[(1) - (3)].sp));
        (yyval.sp) = (yyvsp[(1) - (3)].sp);
    ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 775 "parser.y"
    { 
        /* forward declarations *cannot* be used as base classes */
        if ((yyvsp[(3) - (3)].sp)->forward == 1) {
            yyerror("Forward-declared class '%s' cannot be used as a base class", (yyvsp[(3) - (3)].sp)->name);
            (yyvsp[(3) - (3)].sp)->accessed--; /* don't count this as an access */
        }
        else {
            /* check for cyclic refs, if requested */
            if (w_cyclic) check_cyclic_decl_derived((yyvsp[(1) - (3)].sp), (yyvsp[(3) - (3)].sp), (yyvsp[(1) - (3)].sp)->forwardref);
            /* derived class, mark base type */
            (yyvsp[(1) - (3)].sp)->ptype = (yyvsp[(3) - (3)].sp);
            /* mark derivation on base class as well */
            (yyvsp[(3) - (3)].sp)->derived = (symbol_t **)buildarray((void **)(yyvsp[(3) - (3)].sp)->derived, (yyvsp[(1) - (3)].sp));
            /* if base is parsable, so is derived */
            (yyvsp[(1) - (3)].sp)->parsable = (yyvsp[(3) - (3)].sp)->parsable;
            /* if no alignment info, copy from base */
            if ((yyvsp[(1) - (3)].sp)->align == NULL) (yyvsp[(1) - (3)].sp)->align = (yyvsp[(3) - (3)].sp)->align;
            /* copy symbol table to derived class (derived table is *empty* at this point) */
            s_copy_st((yyvsp[(3) - (3)].sp)->cxt, (yyvsp[(1) - (3)].sp)->cxt);
            /* check that parameter types match exactly */
            check_formal_params((yyvsp[(1) - (3)].sp), (yyvsp[(1) - (3)].sp)->param, (yyvsp[(3) - (3)].sp)->param);
        }
        (yyval.sp) = (yyvsp[(1) - (3)].sp);
    ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 800 "parser.y"
    {
        yyerror("Incorrect base class '%s'", (yyvsp[(3) - (3)].sp)->name);
        (yyval.sp) = (yyvsp[(1) - (3)].sp);
    ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 808 "parser.y"
    {
        /* if we had a forward declaration, it should also not have params */
        if ((yyvsp[(1) - (1)].sp)->forward == 1 && (yyvsp[(1) - (1)].sp)->param != NULL) {
            yyerror("Class '%s' definition/declaration has no parameters but previous declaration did", (yyvsp[(1) - (1)].sp)->name);
            (yyvsp[(1) - (1)].sp)->param = NULL;
        }
        (yyval.sp)=(yyvsp[(1) - (1)].sp);
    ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 817 "parser.y"
    {
        /* if we had a forward declaration, params must match */
        if ((yyvsp[(1) - (4)].sp)->forward == 1) check_forward_params((yyvsp[(1) - (4)].sp), (yyvsp[(3) - (4)].lp), (yyvsp[(1) - (4)].sp)->param);
        (yyvsp[(1) - (4)].sp)->param = (yyvsp[(3) - (4)].lp); /* we always trust the last declaration or the definition */
        (yyval.sp) = (yyvsp[(1) - (4)].sp); 
    ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 827 "parser.y"
    {
        (yyvsp[(2) - (2)].sp)->align = (yyvsp[(1) - (2)].np);
        (yyval.sp) = (yyvsp[(2) - (2)].sp);
    ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 835 "parser.y"
    {
        /* set current class; used for tracking recursive decls */
        s_set_cur_class((yyvsp[(2) - (2)].sp));
        /* new context for class members */
        (yyvsp[(2) - (2)].sp)->cxt = s_newcontext(CLASS);
        /* yet another context for parameter list, if any */
        s_newcontext(0);
        (yyvsp[(2) - (2)].sp)->ident = CLASS_TYPE;
        (yyval.sp) = (yyvsp[(2) - (2)].sp);
    ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 846 "parser.y"
    {
        /* set current class; used for tracking recursive decls */
        s_set_cur_class((yyvsp[(3) - (3)].sp));
        /* new context for class members */
        (yyvsp[(3) - (3)].sp)->cxt = s_newcontext(CLASS);
        /* yet another context for parameter list, if any */
        s_newcontext(0);
        (yyvsp[(3) - (3)].sp)->ident = CLASS_TYPE;
        (yyvsp[(3) - (3)].sp)->abstract = 1;
        (yyval.sp) = (yyvsp[(3) - (3)].sp);
    ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 858 "parser.y"
    {
        if ((yyvsp[(2) - (2)].sp)->forward != 1) yyerror("Duplicate definition of class '%s'", (yyvsp[(2) - (2)].sp)->name);
        /* set current class; used for tracking recursive decls */
        s_set_cur_class((yyvsp[(2) - (2)].sp));
        /* new context for class members */
        (yyvsp[(2) - (2)].sp)->cxt = s_newcontext(CLASS);
        /* yet another context for parameter list, if any */
        s_newcontext(0);
        (yyval.sp) = (yyvsp[(2) - (2)].sp);
    ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 869 "parser.y"
    {
        if ((yyvsp[(3) - (3)].sp)->forward != 1) yyerror("Duplicate definition of class '%s'", (yyvsp[(3) - (3)].sp)->name);
        /* set current class; used for tracking recursive decls */
        s_set_cur_class((yyvsp[(3) - (3)].sp));
        /* new context for class members */
        (yyvsp[(3) - (3)].sp)->cxt = s_newcontext(CLASS);
        /* yet another context for parameter list, if any */
        s_newcontext(0);
        (yyvsp[(3) - (3)].sp)->abstract = 1;
        (yyval.sp) = (yyvsp[(3) - (3)].sp);
    ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 884 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (8)].sp)), (yyvsp[(2) - (8)].sp), M_CONST, check_parse((yyvsp[(2) - (8)].sp), M_CONST, (yyvsp[(4) - (8)].np)), (yyvsp[(8) - (8)].np), NULL, (yyvsp[(1) - (8)].np), 0, NULL, 0, 0);
        (yyval.sp)->align = (yyvsp[(1) - (8)].np);
    ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 889 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(7) - (9)].sp)), (yyvsp[(3) - (9)].sp), check_modtype(((yyvsp[(2) - (9)].ival)|M_CONST), (yyvsp[(3) - (9)].sp)), check_parse((yyvsp[(3) - (9)].sp), ((yyvsp[(2) - (9)].ival)|M_CONST), (yyvsp[(5) - (9)].np)), (yyvsp[(9) - (9)].np), NULL, (yyvsp[(1) - (9)].np), 0, NULL, 0, 0);	
        (yyval.sp)->align = (yyvsp[(1) - (9)].np);
    ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 894 "parser.y"
    {
        if (!((yyvsp[(2) - (8)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", (yyvsp[(6) - (8)].sp)->name);
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (8)].sp)), s_lookup("int"), check_modtype(((yyvsp[(2) - (8)].ival)|M_CONST), s_lookup("int")), check_parse(s_lookup("int"), ((yyvsp[(2) - (8)].ival)|M_CONST), (yyvsp[(4) - (8)].np)), (yyvsp[(8) - (8)].np), NULL, (yyvsp[(1) - (8)].np), 0, NULL, 0, 0);	
        (yyval.sp)->align = (yyvsp[(1) - (8)].np);
    ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 900 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (8)].sp)), (yyvsp[(2) - (8)].sp), M_CONST, check_parse((yyvsp[(2) - (8)].sp), M_CONST, leafs(MAP_TYPE, (yyvsp[(4) - (8)].sp))), (yyvsp[(8) - (8)].np), NULL, (yyvsp[(1) - (8)].np), 0, NULL, 0, 0);
        (yyval.sp)->align = (yyvsp[(1) - (8)].np);
    ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 905 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(7) - (9)].sp)), (yyvsp[(3) - (9)].sp), check_modtype(((yyvsp[(2) - (9)].ival)|M_CONST), (yyvsp[(3) - (9)].sp)), check_parse((yyvsp[(3) - (9)].sp), ((yyvsp[(2) - (9)].ival)|M_CONST), leafs(MAP_TYPE, (yyvsp[(5) - (9)].sp))), (yyvsp[(9) - (9)].np), NULL, (yyvsp[(1) - (9)].np), 0, NULL, 0, 0);	
        (yyval.sp)->align = (yyvsp[(1) - (9)].np);
    ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 910 "parser.y"
    {
        if (!((yyvsp[(2) - (8)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", (yyvsp[(6) - (8)].sp)->name);
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (8)].sp)), s_lookup("int"), check_modtype(((yyvsp[(2) - (8)].ival)|M_CONST), s_lookup("int")), check_parse(s_lookup("int"), ((yyvsp[(2) - (8)].ival)|M_CONST), leafs(MAP_TYPE, (yyvsp[(4) - (8)].sp))), (yyvsp[(8) - (8)].np), NULL, (yyvsp[(1) - (8)].np), 0, NULL, 0, 0);	
        (yyval.sp)->align = (yyvsp[(1) - (8)].np);
    ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 920 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (10)].sp)), (yyvsp[(2) - (10)].sp), M_NONE, check_parse((yyvsp[(2) - (10)].sp), M_CONST, (yyvsp[(4) - (10)].np)), idrange((yyvsp[(8) - (10)].np), (yyvsp[(10) - (10)].np)), NULL, (yyvsp[(1) - (10)].np), 0, NULL, 0, 1);
        (yyval.sp)->align = (yyvsp[(1) - (10)].np);
    ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 925 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(7) - (11)].sp)), (yyvsp[(3) - (11)].sp), check_modtype((yyvsp[(2) - (11)].ival), (yyvsp[(3) - (11)].sp)), check_parse((yyvsp[(3) - (11)].sp), ((yyvsp[(2) - (11)].ival)|M_CONST), (yyvsp[(5) - (11)].np)), idrange((yyvsp[(9) - (11)].np), (yyvsp[(11) - (11)].np)), NULL, (yyvsp[(1) - (11)].np), 0, NULL, 0, 1);	
        (yyval.sp)->align = (yyvsp[(1) - (11)].np);
    ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 930 "parser.y"
    {
        if (!((yyvsp[(2) - (10)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", (yyvsp[(6) - (10)].sp)->name);
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (10)].sp)), s_lookup("int"), check_modtype((yyvsp[(2) - (10)].ival), s_lookup("int")), check_parse(s_lookup("int"), ((yyvsp[(2) - (10)].ival)|M_CONST), (yyvsp[(4) - (10)].np)), idrange((yyvsp[(8) - (10)].np), (yyvsp[(10) - (10)].np)), NULL, (yyvsp[(1) - (10)].np), 0, NULL, 0, 1);	
        (yyval.sp)->align = (yyvsp[(1) - (10)].np);
    ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 936 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (10)].sp)), (yyvsp[(2) - (10)].sp), M_CONST, check_parse((yyvsp[(2) - (10)].sp), M_NONE, leafs(MAP_TYPE, (yyvsp[(4) - (10)].sp))), idrange((yyvsp[(8) - (10)].np), (yyvsp[(10) - (10)].np)), NULL, (yyvsp[(1) - (10)].np), 0, NULL, 0, 1);
        (yyval.sp)->align = (yyvsp[(1) - (10)].np);
    ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 941 "parser.y"
    {
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(7) - (11)].sp)), (yyvsp[(3) - (11)].sp), check_modtype((yyvsp[(2) - (11)].ival), (yyvsp[(3) - (11)].sp)), check_parse((yyvsp[(3) - (11)].sp), ((yyvsp[(2) - (11)].ival)|M_CONST), leafs(MAP_TYPE, (yyvsp[(5) - (11)].sp))), idrange((yyvsp[(9) - (11)].np), (yyvsp[(11) - (11)].np)), NULL, (yyvsp[(1) - (11)].np), 0, NULL, 0, 1);	
        (yyval.sp)->align = (yyvsp[(1) - (11)].np);
    ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 946 "parser.y"
    {
        if (!((yyvsp[(2) - (10)].ival) & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", (yyvsp[(6) - (10)].sp)->name);
        (yyval.sp) = set_var(check_rescope_id((yyvsp[(6) - (10)].sp)), s_lookup("int"), check_modtype((yyvsp[(2) - (10)].ival), s_lookup("int")), check_parse(s_lookup("int"), ((yyvsp[(2) - (10)].ival)|M_CONST), leafs(MAP_TYPE, (yyvsp[(4) - (10)].sp))), idrange((yyvsp[(8) - (10)].np), (yyvsp[(10) - (10)].np)), NULL, (yyvsp[(1) - (10)].np), 0, NULL, 0, 1);	
        (yyval.sp)->align = (yyvsp[(1) - (10)].np);
    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 956 "parser.y"
    { (yyval.lp) = (yyvsp[(2) - (3)].lp); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 957 "parser.y"
    { (yyval.lp) = (yyvsp[(2) - (4)].lp); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 958 "parser.y"
    { (yyval.lp) = NULL; ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 959 "parser.y"
    { (yyval.lp) = NULL; ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 964 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp)); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 968 "parser.y"
    {
        /* set type and mods; no parse, init, params, or alignment here */
        set_var((yyvsp[(2) - (2)].lp)->sp, (yyvsp[(1) - (2)].lp)->sp, (yyvsp[(1) - (2)].lp)->type, NULL, NULL, NULL, NULL, (yyvsp[(2) - (2)].lp)->dims, (yyvsp[(2) - (2)].lp)->dim, 1, (yyvsp[(2) - (2)].lp)->sp->range);
	
        /* formal parameters are considered assigned */
        (yyvsp[(2) - (2)].lp)->sp->assigned = 1;
        (yyvsp[(2) - (2)].lp)->sp->isparam =1 ;
        (yyval.lp) = (yyvsp[(2) - (2)].lp);
    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 984 "parser.y"
    { (yyval.lp) = lnew_vp(VERBATIM, (yyvsp[(1) - (1)].vp)); ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 985 "parser.y"
    { 
        if (!s_in_class()) yyerror("Verbatim get() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_GET, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 989 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_PUT, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 993 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim get()/put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_BOTH, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 997 "parser.y"
    { (yyval.lp) = lnew_vp(VERBATIM_C, (yyvsp[(1) - (1)].vp)); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 998 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim C++ get() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_GET_C, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1002 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim C++ put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_PUT_C, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1006 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim C++ get()/put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_BOTH_C, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1010 "parser.y"
    {
        if (s_in_class()) yyerror("Verbatim C++ code for .cpp file can not be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_C_CPP, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1014 "parser.y"
    { (yyval.lp) = lnew_vp(VERBATIM_J, (yyvsp[(1) - (1)].vp)); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1015 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim Java get() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_GET_J, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1019 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim Java put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_PUT_J, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1023 "parser.y"
    {
        if (!s_in_class()) yyerror("Verbatim Java get()/put() code can only be included within a class");
        (yyval.lp) = lnew_vp(VERBATIM_BOTH_J, (yyvsp[(1) - (1)].vp));
    ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1034 "parser.y"
    { (yyval.lp) = lnew0(PRAGMA); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1035 "parser.y"
    { (yyval.lp) = lnews0(PRAGMA, (yyvsp[(2) - (2)].lp)); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1040 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp)); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1044 "parser.y"
    { 
        if ((yyvsp[(3) - (3)].ival) <= 0) yyerror("Negative or zero size for 'array' pragma: %d", (yyvsp[(3) - (3)].ival));
        else max_array_size = (yyvsp[(3) - (3)].ival);
        (yyval.lp) = lnew1((yyvsp[(1) - (3)].ival), leafi(INT_LITERAL, max_array_size));
    ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1049 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1050 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1051 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1052 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1053 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1054 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1055 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); if (!(yyval.lp)->included) decl_trace = 1; ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1056 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1057 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1058 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1059 "parser.y"
    { null_strings = 1; (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1060 "parser.y"
    { null_strings = 0; (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1061 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); if (!(yyval.lp)->included) gen_includes = 1; ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1062 "parser.y"
    { (yyval.lp) = lnew0((yyvsp[(1) - (1)].ival)); if (!(yyval.lp)->included) gen_includes = 0; ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1063 "parser.y"
    { (yyval.lp) = lnew1((yyvsp[(1) - (3)].ival), leafstr(STRING_LITERAL, (yyvsp[(3) - (3)].string))); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1064 "parser.y"
    { (yyval.lp) = lnew1((yyvsp[(1) - (3)].ival), leafstr(STRING_LITERAL, (yyvsp[(3) - (3)].string))); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1065 "parser.y"
    { (yyval.lp) = lnew1((yyvsp[(1) - (3)].ival), leafstr(STRING_LITERAL, (yyvsp[(3) - (3)].string))); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1066 "parser.y"
    { (yyval.lp) = lnew1((yyvsp[(1) - (3)].ival), leafstr(STRING_LITERAL, (yyvsp[(3) - (3)].string))); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1075 "parser.y"
    {
        (yyval.lp) = lnew1(IMPORT, leafstr(STRING_LITERAL, (yyvsp[(2) - (2)].string)));
        if (!include((yyvsp[(2) - (2)].string), 1)) {
            /* loop would occur; don't issue include statement */
            if (w_includes) yyerror("Importing file '%s' would create an infinite loop", (yyvsp[(2) - (2)].string));
            (yyval.lp) = NULL;
        }
    ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1087 "parser.y"
    {
        (yyval.lp) = lnew1((yyvsp[(1) - (2)].ival), leafstr(STRING_LITERAL, gethname((yyvsp[(2) - (2)].string))));
        if (!include((yyvsp[(2) - (2)].string), 0)) {
            /* loop would occur; don't issue include statement */
            if (w_includes) yyerror("Including file '%s' would create an infinite loop", (yyvsp[(2) - (2)].string));
            (yyval.lp) = NULL;
        }
    ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1102 "parser.y"
    {
        s_set_cur_class(NULL);  /* no current class or map */
        (yyval.lp) = lnews_sp(MAP, (yyvsp[(1) - (2)].sp), (yyvsp[(2) - (2)].lp));
    ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1109 "parser.y"
    {
        (yyvsp[(2) - (5)].sp)->ident = MAP_TYPE;
        (yyvsp[(2) - (5)].sp)->modifiers = M_NONE;
        (yyvsp[(2) - (5)].sp)->ptype = (yyvsp[(4) - (5)].sp);
        if ((yyvsp[(4) - (5)].sp)->ident == CLASS_TYPE) check_map_class((yyvsp[(4) - (5)].sp));
        (yyval.sp) = s_set_cur_class((yyvsp[(2) - (5)].sp)); /* mark current map */
    ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1116 "parser.y"
    {
        (yyvsp[(2) - (5)].sp)->ident = MAP_TYPE;
        (yyvsp[(2) - (5)].sp)->modifiers = (yyvsp[(4) - (5)].ival);
        (yyvsp[(2) - (5)].sp)->ptype = s_lookup("int");
        (yyval.sp) = s_set_cur_class((yyvsp[(2) - (5)].sp));
    ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1122 "parser.y"
    {
        (yyvsp[(2) - (6)].sp)->ident = MAP_TYPE;
        (yyvsp[(2) - (6)].sp)->modifiers = (yyvsp[(4) - (6)].ival);
        (yyvsp[(2) - (6)].sp)->ptype = (yyvsp[(5) - (6)].sp);
        (yyval.sp) = s_set_cur_class((yyvsp[(2) - (6)].sp));
    ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1131 "parser.y"
    { (yyval.lp) = (yyvsp[(2) - (3)].lp); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1136 "parser.y"
    { 
        /* check for unique decodability and unique value */
        if (check_map(s_get_cur_class(), (yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp))) (yyval.lp) = app_map((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp)); 
        else (yyval.lp) = (yyvsp[(1) - (3)].lp);
    ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1144 "parser.y"
    { (yyval.lp) = (yyvsp[(1) - (3)].lp); (yyval.lp)->sub1 = (yyvsp[(3) - (3)].lp); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1149 "parser.y"
    { (yyval.lp) = lnew1(BIT_LITERAL, leafb(BIT_LITERAL, (yyvsp[(1) - (1)].bit))); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1158 "parser.y"
    { (yyval.lp) = lnew1(EXPR, (yyvsp[(1) - (1)].np)); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1159 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), lnew1(EXPR, (yyvsp[(3) - (3)].np))); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1163 "parser.y"
    { 
        (yyval.lp) = lnew3(ESC_FTYPE, leafi(EXPR, M_NONE), leafbs(EXPR, (yyvsp[(1) - (4)].sp)), (yyvsp[(3) - (4)].np)); 
    ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1166 "parser.y"
    { 
        (yyval.lp) = lnew3(ESC_FTYPE, leafi(EXPR, check_modtype((yyvsp[(1) - (5)].ival), (yyvsp[(2) - (5)].sp))), leafbs(EXPR, (yyvsp[(2) - (5)].sp)), (yyvsp[(4) - (5)].np));       
    ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1185 "parser.y"
    { (yyval.lp) = (yyvsp[(1) - (1)].lp); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1186 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp)); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1190 "parser.y"
    { (yyval.lp) = check_map_simple_value(s_get_cur_class(), lnew1(EXPR, (yyvsp[(1) - (1)].np))); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1191 "parser.y"
    { (yyval.lp) = check_map_class_value(s_get_cur_class(), lnews0((yyvsp[(1) - (3)].ival), (yyvsp[(2) - (3)].lp))); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1192 "parser.y"
    {
        check_map_esc_simple_value(s_get_cur_class(), (yyvsp[(1) - (1)].lp));
        if (s_get_cur_class()->escape == 1) {
            yyerror("Escape code is allowed only once in a map");
            exit(1);
        }
        s_get_cur_class()->escape = 1;
        (yyval.lp) = (yyvsp[(1) - (1)].lp);
    ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1201 "parser.y"
    { 
        check_map_esc_class_value(s_get_cur_class(), (yyvsp[(2) - (3)].lp)); 
        if (s_get_cur_class()->escape == 1) {
            yyerror("Escape code is allowed only once in a map");
            exit(1);
        }
        s_get_cur_class()->escape = 1;
        (yyval.lp) = (yyvsp[(2) - (3)].lp);
    ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1210 "parser.y"
    {
        /* must expand string to a list_t (*not* a node_t as with regular arrays...) */
        (yyval.lp) = NULL;
    ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1223 "parser.y"
    {
        s_set_cur_class(NULL);  
        (yyval.lp) = lnews_sp(BAC, (yyvsp[(1) - (2)].sp), (yyvsp[(2) - (2)].lp));
    ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1231 "parser.y"
    {
        (yyvsp[(2) - (2)].sp)->ident = BAC_TYPE;
        (yyvsp[(2) - (2)].sp)->modifiers = M_NONE;
        (yyval.sp) = s_set_cur_class((yyvsp[(2) - (2)].sp)); 
    ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1239 "parser.y"
    { (yyval.lp) = (yyvsp[(2) - (3)].lp); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1244 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), (yyvsp[(3) - (3)].lp)); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1248 "parser.y"
    { (yyval.lp)=(yyvsp[(1) - (3)].lp); (yyval.lp)->sub1=(yyvsp[(3) - (3)].lp); check_bac_val((yyval.lp)); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1252 "parser.y"
    { (yyval.lp) = lnew1(INT_LITERAL, leafi(INT_LITERAL, check_bac_param((yyvsp[(1) - (1)].string)))); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1256 "parser.y"
    { (yyval.lp) = lnew1(EXPR, (yyvsp[(1) - (1)].np)); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1257 "parser.y"
    { (yyval.lp) = lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, (yyvsp[(1) - (1)].string))); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1258 "parser.y"
    { (yyval.lp) = lnews0((yyvsp[(1) - (3)].ival), (yyvsp[(2) - (3)].lp)); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1262 "parser.y"
    { (yyval.lp) = lnew1(EXPR, (yyvsp[(1) - (1)].np)); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1263 "parser.y"
    { (yyval.lp) = lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, (yyvsp[(1) - (1)].string))); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1264 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), lnew1(EXPR, (yyvsp[(3) - (3)].np))); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1265 "parser.y"
    { (yyval.lp) = app((yyvsp[(1) - (3)].lp), lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, (yyvsp[(3) - (3)].string)))); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1269 "parser.y"
    { (yyval.lp) = lnew2(BAC_TYPE, leafs(BAC_TYPE, (yyvsp[(1) - (5)].sp)), leafi(INT_LITERAL, (yyvsp[(3) - (5)].ival))); ;}
    break;



/* Line 1455 of yacc.c  */
#line 4323 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1272 "parser.y"



