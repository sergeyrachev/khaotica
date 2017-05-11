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

    #include "logging.h"

    namespace flavor{
        class Scanner;
    }
}

%code top {
    #include "scanner.h"
    #include "parser.hpp"

    static flavor::Parser::symbol_type yylex(flavor::Scanner &scanner) {
        return scanner.next_token();
    }
}

%lex-param {flavor::Scanner& scanner}

%parse-param {flavor::Scanner& scanner}
%parse-param {std::vector<ASTNode*>& asts}

%locations

%destructor {

} <>

%token END 0
%token DEF "def"
%token <std::string> IDENTIFIER
%token <double> NUMBER

%token TYPE "uint"
%token OPEN_CURLY "{"
%token CLOSE_CURLY "}"
%token OPEN_PAREN "("
%token CLOSE_PAREN ")"
%token STATEMENT_END ";"

%type <ASTNode*> field expr
%type <std::vector<ASTNode*>> fields definition

%%
%start top;

top :
  definition STATEMENT_END top { 
    std::copy (($1).begin(), ($1).end(), std::back_inserter(asts)); 
    logging::debug() << "append def";
  }
| expr END {
    asts.push_back($1);
    logging::debug() << "add expr";
}

expr :
IDENTIFIER { $$ = new CallNode($1); }

definition :
"def" IDENTIFIER OPEN_CURLY fields CLOSE_CURLY{
    std::copy (($4).begin(), ($4).end(), std::back_inserter($$)); 
    logging::debug() << "def is fields";
}

fields:
field fields { 
    ($2).push_back($1);
    $$ = $2;
    logging::debug() << "Push field into def " << ($$).size();
}
|

field:
TYPE OPEN_PAREN NUMBER CLOSE_PAREN IDENTIFIER STATEMENT_END {
    $$ = new VariableNode($5, $3);
    logging::debug() << "Create field";
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
