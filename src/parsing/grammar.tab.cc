// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "grammar.tab.cc" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "grammar.tab.hh"

// User implementation prologue.

#line 51 "grammar.tab.cc" // lalr1.cc:406
// Unqualified %code blocks.
#line 21 "grammar.yy" // lalr1.cc:407

static int yylex(bison::Parser::semantic_type *yylval,
                 Lexer &lexer);

#line 58 "grammar.tab.cc" // lalr1.cc:407


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 8 "grammar.yy" // lalr1.cc:473
namespace bison {
#line 125 "grammar.tab.cc" // lalr1.cc:473

  /// Build a parser object.
  Parser::Parser (Lexer &lexer_yyarg, STree &tree_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      tree (tree_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value (v)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
     : type (empty)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 5: // IDENTIFIER

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.str))  { delete ((yysym.value.str)); ((yysym.value.str)) = nullptr; }
}
#line 303 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 26: // expr

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 312 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 27: // variable

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 321 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 28: // number_literal

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 330 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 29: // binary_op

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 339 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 30: // call

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 348 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 32: // extern

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.proto))  { delete ((yysym.value.proto)); ((yysym.value.proto)) = nullptr; }
}
#line 357 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 33: // definition

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.func))  { delete ((yysym.value.func)); ((yysym.value.func)) = nullptr; }
}
#line 366 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 34: // prototype

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.proto))  { delete ((yysym.value.proto)); ((yysym.value.proto)) = nullptr; }
}
#line 375 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 35: // arg_names

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.strs))  { delete ((yysym.value.strs)); ((yysym.value.strs)) = nullptr; }
}
#line 384 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 36: // if_then

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 393 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 37: // for_loop

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 402 "grammar.tab.cc" // lalr1.cc:598
        break;

      case 38: // var_declare

#line 39 "grammar.yy" // lalr1.cc:598
        {
    if ((yysym.value.node))  { delete ((yysym.value.node)); ((yysym.value.node)) = nullptr; }
}
#line 411 "grammar.tab.cc" // lalr1.cc:598
        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, lexer));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* If YYLEN is nonzero, implement the default value of the action:
       '$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYLHS.VALUE to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yylhs.value = yystack_[yylen - 1].value;
    else
      yylhs.value = yystack_[0].value;


    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 2:
#line 85 "grammar.yy" // lalr1.cc:846
    { tree.set_root((yystack_[1].value.func)); }
#line 644 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 3:
#line 86 "grammar.yy" // lalr1.cc:846
    { tree.set_root((yystack_[1].value.proto)); }
#line 650 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 4:
#line 87 "grammar.yy" // lalr1.cc:846
    {
    PrototypeNode *proto = new PrototypeNode("", std::vector<std::string>());
    tree.set_root(new FunctionNode(proto, (yystack_[1].value.node)));
}
#line 659 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 12:
#line 100 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = (yystack_[1].value.node); }
#line 665 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 13:
#line 103 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new VariableNode(*(yystack_[0].value.str)); }
#line 671 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 14:
#line 106 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new NumberNode((yystack_[0].value.num)); }
#line 677 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 15:
#line 114 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 683 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 16:
#line 115 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 689 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 17:
#line 116 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 695 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 18:
#line 117 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 701 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 19:
#line 118 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 707 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 20:
#line 119 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 713 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 21:
#line 120 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.node) = new BinaryNode((yystack_[1].value.chr), (yystack_[2].value.node), (yystack_[0].value.node)); }
#line 719 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 22:
#line 123 "grammar.yy" // lalr1.cc:846
    {
  (yylhs.value.node) = new CallNode(*(yystack_[3].value.str), *(yystack_[1].value.nodes));
}
#line 727 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 23:
#line 128 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.nodes) = new std::vector<ASTNode*>(); }
#line 733 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 24:
#line 129 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.nodes) = (yystack_[2].value.nodes);
    (yylhs.value.nodes)->push_back((yystack_[0].value.node));
  }
#line 742 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 25:
#line 133 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.nodes) = new std::vector<ASTNode*>();
    (yylhs.value.nodes)->push_back((yystack_[0].value.node));
  }
#line 751 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 26:
#line 139 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.proto) = (yystack_[0].value.proto); }
#line 757 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 27:
#line 142 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.func) = new FunctionNode((yystack_[1].value.proto), (yystack_[0].value.node));
}
#line 765 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 28:
#line 147 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.proto) = new PrototypeNode(*(yystack_[3].value.str), *(yystack_[1].value.strs));
}
#line 773 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 29:
#line 152 "grammar.yy" // lalr1.cc:846
    { (yylhs.value.strs) = new std::vector<std::string>(); }
#line 779 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 30:
#line 153 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.strs) = (yystack_[2].value.strs);
    (yylhs.value.strs)->push_back(*(yystack_[0].value.str));
  }
#line 788 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 31:
#line 157 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.strs) = new std::vector<std::string>();
    (yylhs.value.strs)->push_back(*(yystack_[0].value.str));
  }
#line 797 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 32:
#line 166 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.node) = new IfNode((yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
  }
#line 805 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 33:
#line 171 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.node) = new ForNode(*(yystack_[6].value.str), (yystack_[4].value.node), (yystack_[2].value.node), 0, (yystack_[0].value.node));
  }
#line 813 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 34:
#line 174 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.node) = new ForNode(*(yystack_[8].value.str), (yystack_[6].value.node), (yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
  }
#line 821 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 35:
#line 180 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.node) = new VarNode(*(yystack_[2].value.declrs), (yystack_[0].value.node));
  }
#line 829 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 36:
#line 185 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.declrs) = new std::vector<std::pair<std::string, ASTNode*> >();
  }
#line 837 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 37:
#line 188 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.declrs) = (yystack_[2].value.declrs);
    (yylhs.value.declrs)->push_back(*(yystack_[0].value.declr));
  }
#line 846 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 38:
#line 192 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.declrs) = new std::vector<std::pair<std::string, ASTNode*> >();
    (yylhs.value.declrs)->push_back(*(yystack_[0].value.declr));
  }
#line 855 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 39:
#line 199 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.declr) = new std::pair<std::string, ASTNode*>(*(yystack_[2].value.str), (yystack_[0].value.node));
  }
#line 863 "grammar.tab.cc" // lalr1.cc:846
    break;

  case 40:
#line 202 "grammar.yy" // lalr1.cc:846
    {
    (yylhs.value.declr) = new std::pair<std::string, ASTNode*>(*(yystack_[0].value.str), 0);
  }
#line 871 "grammar.tab.cc" // lalr1.cc:846
    break;


#line 875 "grammar.tab.cc" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const signed char Parser::yypact_ninf_ = -20;

  const signed char Parser::yytable_ninf_ = -1;

  const short int
  Parser::yypact_[] =
  {
       3,    -4,    -4,   -15,   -20,    78,    16,    33,    78,    39,
     107,   -20,   -20,   -20,   -20,    52,    54,   -20,   -20,   -20,
      38,    78,   -20,    78,   136,    50,    55,     1,   -20,    92,
     -20,   -20,    78,    78,    78,    78,    78,    78,    78,   -20,
     -20,    56,   145,   145,   -19,    78,    78,    78,    78,    33,
     -20,     2,   -20,   -20,    17,    17,    26,    26,   -20,    30,
     -20,    78,   126,    68,   145,   -20,   -20,   -20,    58,   145,
      78,    78,   -20,   -20,    37,    78,    78,   -20,   114,    78,
     -20
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       0,     0,     0,    13,    14,     0,     0,    36,     0,     0,
       0,    10,    11,     5,     6,     0,     0,     7,     8,     9,
       0,     0,    26,    23,     0,     0,    40,     0,    38,     0,
       1,     4,     0,     0,     0,     0,     0,     0,     0,     3,
       2,    29,    27,    25,     0,     0,     0,     0,     0,     0,
      12,    15,    18,    19,    16,    17,    21,    20,    31,     0,
      22,     0,     0,     0,    39,    35,    37,    28,     0,    24,
       0,     0,    30,    32,     0,     0,     0,    33,     0,     0,
      34
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -20,   -20,    -5,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
      57,   -20,   -20,   -20,   -20,   -20,    15
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     9,    10,    11,    12,    13,    14,    44,    15,    16,
      21,    59,    17,    18,    19,    27,    28
  };

  const unsigned char
  Parser::yytable_[] =
  {
      24,    20,    60,    29,    61,    23,     1,     2,     3,     4,
      33,    34,    35,    36,    37,    38,    42,     5,    43,    48,
       6,    25,     7,     8,    49,    33,    34,    51,    52,    53,
      54,    55,    56,    57,    33,    34,    35,    36,    26,    30,
      62,    63,    64,    65,    32,    33,    34,    35,    36,    37,
      38,    67,    39,    68,    40,    75,    69,    46,    41,    22,
      76,    58,    47,    72,    66,    73,    74,     0,     0,     0,
      77,    78,     0,     0,    80,    32,    33,    34,    35,    36,
      37,    38,     0,     3,     4,     0,     0,     0,     0,     0,
       0,    71,     5,     0,     0,     6,     0,     7,     8,    32,
      33,    34,    35,    36,    37,    38,     0,    31,     0,     0,
       0,     0,     0,    50,    32,    33,    34,    35,    36,    37,
      38,    32,    33,    34,    35,    36,    37,    38,     0,     0,
       0,     0,    79,    32,    33,    34,    35,    36,    37,    38,
       0,     0,    70,    32,    33,    34,    35,    36,    37,    38,
       0,    45,    32,    33,    34,    35,    36,    37,    38
  };

  const signed char
  Parser::yycheck_[] =
  {
       5,     5,    21,     8,    23,    20,     3,     4,     5,     6,
       8,     9,    10,    11,    12,    13,    21,    14,    23,    18,
      17,     5,    19,    20,    23,     8,     9,    32,    33,    34,
      35,    36,    37,    38,     8,     9,    10,    11,     5,     0,
      45,    46,    47,    48,     7,     8,     9,    10,    11,    12,
      13,    21,     0,    23,     0,    18,    61,     7,    20,     2,
      23,     5,     7,     5,    49,    70,    71,    -1,    -1,    -1,
      75,    76,    -1,    -1,    79,     7,     8,     9,    10,    11,
      12,    13,    -1,     5,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    14,    -1,    -1,    17,    -1,    19,    20,     7,
       8,     9,    10,    11,    12,    13,    -1,     0,    -1,    -1,
      -1,    -1,    -1,    21,     7,     8,     9,    10,    11,    12,
      13,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    -1,    18,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    16,     7,     8,     9,    10,    11,    12,    13,
      -1,    15,     7,     8,     9,    10,    11,    12,    13
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,     3,     4,     5,     6,    14,    17,    19,    20,    25,
      26,    27,    28,    29,    30,    32,    33,    36,    37,    38,
       5,    34,    34,    20,    26,     5,     5,    39,    40,    26,
       0,     0,     7,     8,     9,    10,    11,    12,    13,     0,
       0,    20,    26,    26,    31,    15,     7,     7,    18,    23,
      21,    26,    26,    26,    26,    26,    26,    26,     5,    35,
      21,    23,    26,    26,    26,    26,    40,    21,    23,    26,
      16,    23,     5,    26,    26,    18,    23,    26,    26,    18,
      26
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    24,    25,    25,    25,    26,    26,    26,    26,    26,
      26,    26,    26,    27,    28,    29,    29,    29,    29,    29,
      29,    29,    30,    31,    31,    31,    32,    33,    34,    35,
      35,    35,    36,    37,    37,    38,    39,    39,    39,    40,
      40
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     4,     0,     3,     1,     2,     3,     4,     0,
       3,     1,     6,     8,    10,     4,     0,     3,     1,     3,
       1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "END", "error", "$undefined", "\"def\"", "\"extern\"", "IDENTIFIER",
  "NUMBER", "\"=\"", "\"*\"", "\"/\"", "\"+\"", "\"-\"", "\">\"", "\"<\"",
  "\"if\"", "\"then\"", "\"else\"", "\"for\"", "\"in\"", "\"var\"",
  "\"(\"", "\")\"", "\";\"", "\",\"", "$accept", "top", "expr", "variable",
  "number_literal", "binary_op", "call", "call_args", "extern",
  "definition", "prototype", "arg_names", "if_then", "for_loop",
  "var_declare", "declarations", "declaration", YY_NULL
  };


  const unsigned char
  Parser::yyrline_[] =
  {
       0,    85,    85,    86,    87,    93,    94,    95,    96,    97,
      98,    99,   100,   103,   106,   114,   115,   116,   117,   118,
     119,   120,   123,   128,   129,   133,   139,   142,   147,   152,
     153,   157,   166,   171,   174,   180,   185,   188,   192,   199,
     202
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23
    };
    const unsigned int user_token_number_max_ = 278;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 8 "grammar.yy" // lalr1.cc:1156
} // bison
#line 1265 "grammar.tab.cc" // lalr1.cc:1156
#line 206 "grammar.yy" // lalr1.cc:1157

#include "lexer.h"

static int yylex(bison::Parser::semantic_type *yylval,
                 Lexer &lexer) {
    return lexer.yylex(yylval);
}

void
bison::Parser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
