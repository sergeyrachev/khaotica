
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
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

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H



#include <string>
#include <iostream>
#include "stack.hh"


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 35 of lalr1.cc  */
#line 54 "parser.tab.hh"
  class position;
  class location;

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 35 of lalr1.cc  */
#line 64 "parser.tab.hh"

#include "location.hh"

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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
do {							\
  if (N)						\
    {							\
      (Current).begin = (Rhs)[1].begin;			\
      (Current).end   = (Rhs)[N].end;			\
    }							\
  else							\
    {							\
      (Current).begin = (Current).end = (Rhs)[0].end;	\
    }							\
} while (false)
#endif


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 35 of lalr1.cc  */
#line 112 "parser.tab.hh"

  /// A Bison parser.
  class parser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
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



/* Line 35 of lalr1.cc  */
#line 139 "parser.tab.hh"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
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

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    parser ();
    virtual ~parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default rule to reduce.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned short int yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const unsigned short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

  };

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 35 of lalr1.cc  */
#line 451 "parser.tab.hh"



#endif /* ! defined PARSER_HEADER_H */
