%skeleton "lalr1.cc"
%require "3.0"

%verbose

%defines

%define parser_class_name {Parser}

%define api.namespace {flavor}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires{
    #include "ast.h"

    namespace flavor{
        class Scanner;
        class Interpreter;
    }
}

%code top {
    #include "scanner.h"
    #include "parser.hpp"
    #include "interpreter.h"

    static flavor::Parser::symbol_type yylex(flavor::Scanner &scanner) {
        return scanner.next_token();
    }
}

%lex-param {flavor::Scanner& scanner}

%parse-param {flavor::Interpreter &driver}
%parse-param {flavor::Scanner& scanner}

%locations

%destructor {
    if ($$)  { delete ($$); ($$) = nullptr; }
} <ASTNode *> <PrototypeNode *> <FunctionNode *> <std::vector<std::string> *>

%token END 0
%token DEF "def"
%token EXTERN "extern"
%token <std::string> IDENTIFIER
%token <double> NUMBER

%token <char> ASSIGNMENT "="

%token <char> MULTIPLY "*"
%token <char> DIVIDE "/"
%token <char> ADD "+"
%token <char> SUBTRACT "-"
%token <char> GREATER_THAN ">"
%token <char> LESS_THAN "<"

%token IF "if"
%token THEN "then"
%token ELSE "else"
%token FOR "for"
%token IN "in"
%token VAR "var"

%token OPEN_PAREN "("
%token CLOSE_PAREN ")"
%token STATEMENT_END ";"
%token COMMA ","

%type <ASTNode *> expr number_literal binary_op call variable
%type <std::vector<ASTNode*> *> call_args
%type <ASTNode *> if_then for_loop var_declare
%type <PrototypeNode *> prototype extern
%type <std::vector<std::string> *> arg_names
%type <FunctionNode *> definition
%type <std::vector<std::pair<std::string, ASTNode*> > *> declarations
%type <std::pair<std::string, ASTNode*> *> declaration

%%
%start top;

top :
  definition END { driver.ast($1); }
| extern END { driver.ast($1); }
| expr END {
    PrototypeNode *proto = new PrototypeNode("anon", std::vector<std::string>());
    driver.ast(new FunctionNode(proto, $1));
}

expr :
  binary_op
| call
| if_then
| for_loop
| var_declare
| variable
| number_literal
| "(" expr ")" { $$ = $2; }

variable:
  IDENTIFIER { $$ = new VariableNode($1); }

number_literal :
  NUMBER { $$ = new NumberNode($1); }

%left "=";
%left ">" "<";
%left "+" "-";
%left "*" "/";

binary_op :
  expr "=" expr { $$ = new BinaryNode($2, $1, $3); }
| expr "+" expr { $$ = new BinaryNode($2, $1, $3); }
| expr "-" expr { $$ = new BinaryNode($2, $1, $3); }
| expr "*" expr { $$ = new BinaryNode($2, $1, $3); }
| expr "/" expr { $$ = new BinaryNode($2, $1, $3); }
| expr "<" expr { $$ = new BinaryNode($2, $1, $3); }
| expr ">" expr { $$ = new BinaryNode($2, $1, $3); }

call :
IDENTIFIER "(" call_args ")" {
  $$ = new CallNode($1, *$3);
}

call_args :
  { $$ = new std::vector<ASTNode*>(); }
| call_args "," expr {
    $$ = $1;
    $$->push_back($3);
  }
| expr {
    $$ = new std::vector<ASTNode*>();
    $$->push_back($1);
  }

extern :
"extern" prototype { $$ = $2; }

definition :
"def" prototype expr {
    $$ = new FunctionNode($2, $3);
}

prototype :
IDENTIFIER "(" arg_names ")" {
    $$ = new PrototypeNode($1, *$3);
}

arg_names:
  { $$ = new std::vector<std::string>(); }
| arg_names "," IDENTIFIER {
    $$ = $1;
    $$->push_back($3);
  }
| IDENTIFIER {
    $$ = new std::vector<std::string>();
    $$->push_back($1);
  }

%left "else" "then";
%right "," "in";

if_then :
  "if" expr "then" expr "else" expr {
    $$ = new IfNode($2, $4, $6);
  }

for_loop :
  "for" IDENTIFIER "=" expr "," expr "in" expr {
    $$ = new ForNode($2, $4, $6, 0, $8);
  }
| "for" IDENTIFIER "=" expr "," expr "," expr "in" expr {
    $$ = new ForNode($2, $4, $6, $8, $10);
  }


var_declare :
  "var" declarations "in" expr {
    $$ = new VarNode(*$2, $4);
  }

declarations :
  {
    $$ = new std::vector<std::pair<std::string, ASTNode*> >();
  }
| declarations "," declaration {
    $$ = $1;
    $$->push_back(*$3);
  }
| declaration {
    $$ = new std::vector<std::pair<std::string, ASTNode*> >();
    $$->push_back(*$1);
  }


declaration :
  IDENTIFIER "=" expr {
    $$ = new std::pair<std::string, ASTNode*>($1, $3);
  }
| IDENTIFIER {
    $$ = new std::pair<std::string, ASTNode*>($1, 0);
  }

%%
#include "logging.h"
void flavor::Parser::error( const location &loc, const std::string &err_message )
{
    //logging::debug() << "Parsing error: '" << err_message << "'\n";
}
