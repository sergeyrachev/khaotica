%skeleton "lalr1.cc"
%require "3.0"

%no-lines

%verbose
%define parse.trace
%define parse.error verbose

%define parser_class_name {Parser}

%define api.namespace {flavor}
%define api.token.constructor
%define api.value.type variant
%define api.token.prefix {TOKEN_}

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
    if ($$)  { ($$).reset(); ($$) = nullptr; }
} <std::shared_ptr<ASTNode>> <std::shared_ptr<PrototypeNode>> <std::shared_ptr<FunctionNode>>

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

%type <std::shared_ptr<ASTNode>> expr number_literal binary_op call variable
%type <std::vector<std::shared_ptr<ASTNode>>> call_args
%type <std::shared_ptr<ASTNode>> if_then for_loop var_declare
%type <std::shared_ptr<PrototypeNode>> prototype extern
%type <std::vector<std::string>> arg_names
%type <std::shared_ptr<FunctionNode>> definition
%type <std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> > > declarations
%type <std::pair<std::string, std::shared_ptr<ASTNode>> > declaration

%%
%start top;

top :
  definition STATEMENT_END top { driver.ast($1); }
| extern STATEMENT_END top { driver.ast($1); }
| expr END {
    std::shared_ptr<PrototypeNode> proto = std::make_shared<PrototypeNode>("anon", std::vector<std::string>());
    driver.ast(std::make_shared<FunctionNode>(proto, $1));
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
  IDENTIFIER { $$ = std::make_shared<VariableNode>($1); }

number_literal :
  NUMBER { $$ = std::make_shared<NumberNode>($1); }

%left "=";
%left ">" "<";
%left "+" "-";
%left "*" "/";

binary_op :
  expr "=" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr "+" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr "-" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr "*" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr "/" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr "<" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }
| expr ">" expr { $$ = std::make_shared<BinaryNode>($2, $1, $3); }

call :
IDENTIFIER "(" call_args ")" {
  $$ = std::make_shared<CallNode>($1, $3);
}

call_args :
  { $$ = std::vector<std::shared_ptr<ASTNode>>(); }
| call_args "," expr {
    $$ = $1;
    $$.push_back($3);
  }
| expr {
    $$ = std::vector<std::shared_ptr<ASTNode>>();
    $$.push_back($1);
  }

extern :
"extern" prototype { $$ = $2; }

definition :
"def" prototype expr {
    $$ = std::make_shared<FunctionNode>($2, $3);
}

prototype :
IDENTIFIER "(" arg_names ")" {
    $$ = std::make_shared<PrototypeNode>($1, $3);
}

arg_names:
  { $$ = std::vector<std::string>(); }
| arg_names "," IDENTIFIER {
    $$ = $1;
    $$.push_back($3);
  }
| IDENTIFIER {
    $$ = std::vector<std::string>();
    $$.push_back($1);
  }

%left "else" "then";
%right "," "in";

if_then :
  "if" expr "then" expr "else" expr {
    $$ = std::make_shared<IfNode>($2, $4, $6);
  }

for_loop :
  "for" IDENTIFIER "=" expr "," expr "in" expr {
    $$ = std::make_shared<ForNode>($2, $4, $6, nullptr, $8);
  }
| "for" IDENTIFIER "=" expr "," expr "," expr "in" expr {
    $$ = std::make_shared<ForNode>($2, $4, $6, $8, $10);
  }


var_declare :
  "var" declarations "in" expr {
    $$ = std::make_shared<VarNode>($2, $4);
  }

declarations :
  {
    $$ = std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> >();
  }
| declarations "," declaration {
    $$ = $1;
    $$.push_back($3);
  }
| declaration {
    $$ = std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> >();
    $$.push_back($1);
  }


declaration :
  IDENTIFIER "=" expr {
    $$ = std::pair<std::string, std::shared_ptr<ASTNode>>($1, $3);
  }
| IDENTIFIER {
    $$ = std::pair<std::string, std::shared_ptr<ASTNode>>($1, 0);
  }

%%
#include "logging.h"
#include <sstream>
#include <iostream>
void flavor::Parser::error( const location &loc, const std::string &err_message )
{
    std::ostringstream ss;
    ss << loc;
    logging::debug() << "Parsing error: " << err_message << ss.str();
}
