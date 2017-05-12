%code top{

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

#include "parser.hpp"
#include "scanner.h"

#include <stdio.h>
#include <string.h>

/* defined in the lexical analyzer */
//void yyerror(char *fmt, ...);
//int yywrap();
//int include(char *file, int import);

/* Disable 'unreferenced label' warning from Microsoft Visual C++.
 * MKS Yacc generates various labels that are not used.
 */
#if defined(_WIN32_) || defined(WIN32)
#pragma warning(disable : 4102)
#endif

static int yylex(flavor::Parser::semantic_type* yylval, flavor::Scanner &scanner) {
    return scanner.next_token(*yylval);
}

}

%code requires{
    namespace flavor{
        class Scanner;
    }

    class bit_t{
    };
    class list_t{};
    class symbol_t{};
    class verbatim_t{};
    class node_t{};
}

%skeleton "lalr1.cc"
%require "3.0"

%no-lines

%verbose
%define parse.trace
%define parse.error verbose
%define parser_class_name {Parser}
%define api.namespace {flavor}

%lex-param {flavor::Scanner& scanner}
%parse-param {flavor::Scanner& scanner}

%union{
    char            *string;
    int             ival;
    double          dval;
    unsigned int    uival;
    bit_t           *bit;
    list_t          *lp;
    symbol_t        *sp;
    node_t          *np;
    verbatim_t	    *vp;
}        

/* built-in types -- the order is significant (for type promotion) */
%token <sp> CHAR BIT INT FLOAT DOUBLE

/* keywords */
%token <ival> CLASS IF ELSE DO WHILE FOR GOTO BREAK
%token <ival> CONTINUE REPEAT MAP EXTENDS SWITCH CASE DEFAULT BAC

/* pragma keywords */
%token <ival> PRAGMA PRAGMA_ARRAY PRAGMA_GET PRAGMA_NOGET 
%token <ival> PRAGMA_PUT PRAGMA_NOPUT PRAGMA_PUTXML PRAGMA_NOPUTXML
%token <ival> PRAGMA_TRACE PRAGMA_NOTRACE PRAGMA_LINE PRAGMA_NOLINE PRAGMA_INCLUDES 
%token <ival> PRAGMA_NULLSTRINGS PRAGMA_NONULLSTRINGS PRAGMA_NOINCLUDES 
%token <ival> PRAGMA_BITSTREAM PRAGMA_PREFIX PRAGMA_ERROR_FUNC PRAGMA_TRACE_FUNC

/* bac keywords */
%token <ival> BAC_PREC BAC_OOC BAC_SOC BAC_TE BAC_RENORM BAC_BS BAC_INIT BAC_END 
%token <ival> BAC_RTABLE BAC_NEXTI BAC_TRANS BAC_EXCH

/* include/import keywords and expressions */
%token <ival> INCLUDE IMPORT
%type <lp> include_file import_file

/* modifiers */
%token <ival> UNSIGNED SIGNED SHORT LONG CONST ALIGNED STATIC ABSTRACT LITTLE BIG

/* literals */
%token <ival>   INT_LITERAL
%token <dval>   DOUBLE_LITERAL
%token <bit>    BIT_LITERAL
%token <string> STRING_LITERAL

/* user-defined variables, class, map and bac types */
%token <sp> VARIABLE CLASS_TYPE MAP_TYPE BAC_TYPE

/* labels (goto's and such) */
%token <sp> LABEL

/* single-character tokens */
%token <ival> ';' ',' '?' ':' '(' ')' '{' '}'

/* single-letter operators */
%token <ival> '+' '-' '*' '/' '<' '>' '|' '&' '^' '%' '[' ']' '.' '~' '!'

/* two-letter operators */
%token <ival> LS RS GE LE NE EQ OR AND INC DEC RANGE CLCL

/* assignment operators */
%token <ival> '=' MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN
%token <ival> LS_ASSIGN RS_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN

/* double braces */
%token <ival> DBL_BRACE_L DBL_BRACE_R

/* tokens used internally to separate statements from declarations and expressions in lists */
%token DECL EXPR

/* tokens indicating verbatim code to be copied over to class, put, or get methods */
%token <vp> VERBATIM VERBATIM_GET VERBATIM_PUT VERBATIM_BOTH
%token <vp> VERBATIM_C VERBATIM_C_CPP VERBATIM_GET_C VERBATIM_PUT_C VERBATIM_BOTH_C
%token <vp> VERBATIM_J VERBATIM_GET_J VERBATIM_PUT_J VERBATIM_BOTH_J

/* built-in functions */
%token <ival> LENGTHOF ISIDOF SKIPBITS NEXTBITS SNEXTBITS LITTLE_NEXTBITS LITTLE_SNEXTBITS NEXTCODE NUMBITS

/* tokens for map escape codes */
%token ESC_FTYPE ESC_MAP

/* "static" array expressions: { something } */
%token ARRAY

/* take care of shift/reduce conflict for 'else' */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


/* Non-terminals used for building expressions.
 * The important ones are:
 *  - conditional_expr:     This is an expression that can be used as an assignment value.
 *                          In Java, relational expressions cannot be assigned to a non-boolean lvalue. 
 *  - assignment_expr:      This is an assignment expression.
 *  - assignment_expr_list: This is a comma-separated list of assignment_exprs.
 *  - array_expr:           This is a comma-separated list of expressions within curly braces.
 */
%type <np> primary_expr postfix_expr rescoped_expr unary_expr builtin_function nextbits_function
%type <ival> unary_operator
%type <np> multiplicative_expr additive_expr shift_expr
%type <np> relational_expr logical_and_expr logical_or_expr conditional_expr 
%type <np> assignment_expr assignment_expr_list argument_expr_list
%type <ival> assignment_operator
%type <np> array array_expr array_expr_list 

/* non-terminals for building declarations */
%type <lp> declaration declaration_list type_info plain_type_info parsable_type_info
%type <ival> declaration_modifiers modifier modifier_list
%type <np> alignment_modifier
%type <sp> type_specifier user_type_specifier builtin_type_specifier
%type <lp> init_declarator
%type <lp> declarator declarator_with_params declarator_with_range

/* non-terminals for building statements */
%type <lp> statement labeled_statement compound_statement
%type <lp> statement_or_declaration_list  statement_or_declaration
%type <lp> expression_statement selection_statement
%type <lp> iteration_statement jump_statement bac_statement

/* class definitions */
%type <lp> class_definition 
%type <sp> complex_class_declarator derived_class_declarator 
%type <sp> param_class_declarator aligned_class_declarator class_declarator id_declarator id_range_declarator
%type <lp> class_body parameter_list parameter_declaration

/* identifier, i.e., variables that may or may not have been declared yet */
%type <sp> identifier

/* starting non-terminal and what can go in global scope */
%type <lp> program external_definition

/* map definition */
%type <lp> map_definition
%type <sp> map_declarator
%type <lp> map_body map_entry_list map_entry map_entry_item_list map_entry_item map_code map_entry_item_escape map_entry_item_escape_list

/* bac definition */
%type <lp> bac_definition
%type <sp> bac_declarator
%type <lp> bac_body bac_entry_list bac_entry bac_entry_item_list bac_entry_item bac_keyword 

/* verbatim code */
%type <lp> verbatim

/* pragma code */
%type <lp> pragma pragma_list pragma_item

%start program

%%

primary_expr
    : identifier                { /*$$ = eval_ident($1);*/ }
    | INT_LITERAL               { /*$$ = leafi(flavor::Parser::token::INT_LITERAL, $1);*/ }
    | DOUBLE_LITERAL            { /*$$ = leafd(flavor::Parser::token::DOUBLE_LITERAL, $1);*/ }
    | BIT_LITERAL               { /*$$ = leafi(flavor::Parser::token::INT_LITERAL, $1->val); free($1);*/ }
    | '(' assignment_expr ')'   { /*$$ = $2; $$->paren = 1;*/ }
    /* in C++, we can have assignment_expr_list inside the parenthesis, but not in Java */
    ;

identifier
    : VARIABLE
    ;

/* note: array errors are caught in unary_expr */
postfix_expr
    : primary_expr
    | postfix_expr '[' assignment_expr ']'  { /* $$ = eval_subarray($1, $3); */ }
    /* in C++, we can have assignment_expr_list inside the parenthesis, but not in Java */

    | rescoped_expr identifier { 
      //  $$ = eval_scoped($1, $2);
//
      //  /* remove two temporary contexts */
      //  s_remcontext();
      //  s_remcontext();
    }
    | postfix_expr INC { /*$$ = eval_incdec($2, $1); */}
    | postfix_expr DEC { /*$$ = eval_incdec($2, $1); */}
    ;
	 
rescoped_expr
    : postfix_expr '.' {
        /* activate the context of the class; if not available, eval_scoped above will catch it */
        //if ($1->ptype!=NULL && $1->ptype->cxt!=NULL) s_inscontext($1->ptype->cxt);
        ///* add another context to protect the class members */
        //s_newcontext(0);
        //$$ = $1;
    }
    ;

unary_expr
    : builtin_function
    | postfix_expr              { /* $$ = eval_array($1); */ }
    | unary_operator unary_expr { /* $$ = eval_unary($1, $2); */ }
    ;

builtin_function
    : LENGTHOF '(' postfix_expr ')'                 { /* $$ = eval_lengthof(eval_array($3)); */ }
    | ISIDOF '(' CLASS_TYPE ',' assignment_expr ')' { /* $$ = eval_isidof($3, $5); */ }
    | SKIPBITS '(' assignment_expr ')'              { /* $$ = eval_skipbits($3); */ }
    | nextbits_function
    | NEXTCODE '(' BIT_LITERAL ')' { 
       // $$ = node(NEXTCODE, leafb(BIT_LITERAL, $3), NULL);
       // $$->type=INT; $$->ptype = s_lookup("int");
    }
    | NEXTCODE '(' alignment_modifier ',' BIT_LITERAL ')' { 
       // $$=node(NEXTCODE, leafb(BIT_LITERAL, $5), $3);
       // $$->type = INT; $$->ptype = s_lookup("int");
    }
    | NUMBITS '(' ')' { /* $$ = leaf(NUMBITS); $$->type = INT; $$->ptype = s_lookup("int"); */ }
    ;

nextbits_function
    : NEXTBITS '(' assignment_expr ')'                        { /* $$ = eval_nextbits($3, 0, NULL); */ }
    | NEXTBITS '(' modifier_list ',' assignment_expr ')'      { /* $$ = eval_nextbits($5, $3, NULL); */ }
    | NEXTBITS '(' alignment_modifier ',' assignment_expr ')' { /* $$ = eval_nextbits($5, 0, $3); */ }
    | NEXTBITS '(' alignment_modifier ',' modifier_list ',' assignment_expr ')' { /*$$ = eval_nextbits($7, $5, $3);*/ }
    ;

modifier_list
    : modifier                   { /* $$ = s_tok2mod($1); */ }
    | modifier_list ',' modifier { /* $$ = check_mod($1, s_tok2mod($3)); */ }
    ;

unary_operator
    : INC
    | DEC
    | '+'
    | '-'
    | '~'
    | '!'
    ;

multiplicative_expr
    : unary_expr
    | multiplicative_expr '*' unary_expr { /* $$ = eval_mult($2, $1, $3); */ }
    | multiplicative_expr '/' unary_expr { /* $$ = eval_mult($2, $1, $3); */ }
    | multiplicative_expr '%' unary_expr { /* $$ = eval_mult($2, $1, $3); */ }
    ;

additive_expr
    : multiplicative_expr
    | additive_expr '+' multiplicative_expr { /* $$ = eval_add($2, $1, $3); */}
    | additive_expr '-' multiplicative_expr { /* $$ = eval_add($2, $1, $3); */}
    ;

shift_expr
    : additive_expr
    | shift_expr LS additive_expr  { /* $$ = eval_shift($2, $1, $3); */}
    | shift_expr RS additive_expr  { /* $$ = eval_shift($2, $1, $3); */}
    | shift_expr '&' additive_expr { /* $$ = eval_shift($2, $1, $3); */}
    | shift_expr '^' additive_expr { /* $$ = eval_shift($2, $1, $3); */}
    | shift_expr '|' additive_expr { /* $$ = eval_shift($2, $1, $3); */}
    ;

relational_expr
    : shift_expr
    | relational_expr '<' shift_expr { /* $$ = eval_rel($2, $1, $3); */}
    | relational_expr '>' shift_expr { /* $$ = eval_rel($2, $1, $3); */}
    | relational_expr LE shift_expr  { /* $$ = eval_rel($2, $1, $3); */}
    | relational_expr GE shift_expr  { /* $$ = eval_rel($2, $1, $3); */}
    | relational_expr EQ shift_expr  { /* $$ = eval_rel($2, $1, $3); */}
    | relational_expr NE shift_expr  { /* $$ = eval_rel($2, $1, $3); */}
    ;

logical_and_expr
    : relational_expr
    | logical_and_expr AND relational_expr { /* $$ = eval_logic($2, $1, $3); */}
    ;

logical_or_expr
    : logical_and_expr                     { /* $$ = check_unassigned($1, $1); */}
    | logical_or_expr OR logical_and_expr  { /* $$ = eval_logic($2, $1, $3); */}
    ;

conditional_expr
    : logical_or_expr
    | logical_or_expr '?' conditional_expr ':' conditional_expr { /*$$ = eval_cond($2, $1, $3, $5); */}
    ;

assignment_expr
    : conditional_expr
    | unary_expr assignment_operator assignment_expr { /*$$ = check_assign($2, $1, $3); */}
    ;

assignment_operator
    : '='
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LS_ASSIGN
    | RS_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;

assignment_expr_list
    : assignment_expr
    | assignment_expr_list ',' assignment_expr { /*$$ = node($2, $1, $3); */}
    ;

argument_expr_list
    : assignment_expr
    | assignment_expr ',' argument_expr_list { /*$$ = node($2, $1, $3); */}
    ;

/************************/
/* variable declaration */
/************************/

declaration
    : declaration_list ';'
    ;

declaration_list
    : type_info init_declarator {
       // $$ = $2;

       // /* copy parse, align, and usenext */
       // $$->e2 = check_parse($1->sp, $1->type, $1->e1);
       //
       // /* also check for lookahead parsing of escaped maps */
       // if ($1->usenext && $1->e1->op == MAP_TYPE && $1->e1->left.sp->escape)
       //     yyerror("Map with escape (%s) cannot be used in lookahead parsing", $1->e1->left.sp->name);

       // /* check that for a bac, the declarator must be accompanied with param types */
       // if ($1->e1 && $1->e1->op == BAC_TYPE)
       //     if ($2 == NULL || $2->e3 == NULL)
       //         yyerror("BAC (%s) must be accompanied with corresponding parameters", $1->e1->left.sp->name);
       //
       // $$->align = $1->align;
       // $$->usenext = $1->usenext;
       //
       // /* if BIT type, treat as unsigned */
       // if ($1->sp->ident == BIT) $1->type |= M_UNSIGNED;
       //
       // /* set type, mods, parse, init, check param, check alignment, dims and dim values */
       // $$->sp = set_var($$->sp, $1->sp, $1->type, $2->e2, $2->e1, $2->e3, $1->align, $2->dims, $2->dim, 0, $$->sp->range);
       //
       // /* if we have a static array declaration of a non-global variable, unroll it */
       // if ($2->e1 != NULL && $2->e1->op == ARRAY && $2->sp->cxt_level > 0)
       //     $$ = c_array_unroll($$);
    }
    | declaration_list ',' init_declarator {
        /* copy parse, align, and usenext */
//        $3->e2 = $1->e2;
//        $3->align = $1->align;
//        $3->usenext = $1->usenext;
//
//        /* set type, mods, parse, init, check param, check alignment, dims and dim values */
//        $3->sp = set_var($3->sp, $1->sp->ptype, $1->sp->modifiers, $3->e2, $3->e1, $3->e3, $1->align, $3->dims, $3->dim, 0, $3->sp->range);
//
//        /* if we have a static array declaration of a non-global variable, unroll it */
//        if ($3->e1 != NULL && $3->e1->op == ARRAY && $3->sp->cxt_level > 0)
//            $$ = app($1, c_array_unroll($3));
//        else
//            $$ = app($1, $3);
    }
    ;

type_info
    : alignment_modifier plain_type_info    { /* $2->align = $1; $$ = $2; */}
    | alignment_modifier parsable_type_info { /* $2->align = $1; $$ = $2; */}
    ;

plain_type_info
    : type_specifier { /* $$ = lnew_type(M_NONE, $1, NULL, 0); */ }
    | declaration_modifiers {
       // if (!($1 & (M_SHORT | M_LONG))) yyerror("No storage class specified");
       // $$ = lnew_type(check_modtype($1, s_lookup("int")), s_lookup("int"), NULL, 0);
    }
    | declaration_modifiers type_specifier {
       // $$ = lnew_type(check_modtype($1, $2), $2, NULL, 0);
    }
    ;

parsable_type_info
    : builtin_type_specifier '(' conditional_expr ')'     { /* $$ = lnew_type(M_NONE, $1, $3, 0);  */ }
    | builtin_type_specifier '(' MAP_TYPE ')'             { /* $$ = lnew_type(M_NONE, $1, leafs(MAP_TYPE, $3), 0);  */ }
    | user_type_specifier '(' MAP_TYPE ')'                { /* $$ = lnew_type(M_NONE, $1, leafs(MAP_TYPE, $3), 0);  */ }
    | builtin_type_specifier '(' conditional_expr ')' '*' { /* $$ = lnew_type(M_NONE, $1, $3, 1);  */ }
    | builtin_type_specifier '(' MAP_TYPE ')' '*'         { /* $$ = lnew_type(M_NONE, $1, leafs(MAP_TYPE, $3), 1);  */ }
    | user_type_specifier '(' MAP_TYPE ')' '*'            { /* $$ = lnew_type(M_NONE, $1, leafs(MAP_TYPE, $3), 1);  */ }
    | declaration_modifiers '(' conditional_expr ')' {
       // if (!($1 & (M_SHORT | M_LONG))) yyerror("No storage class specified");
       // $$ = lnew_type(check_modtype($1, s_lookup("int")), s_lookup("int"), $3, 0);
    }
    | declaration_modifiers '(' MAP_TYPE ')' {
       // if (!($1 & (M_SHORT | M_LONG))) yyerror("No storage class specified");
       // $$ = lnew_type(check_modtype($1, s_lookup("int")), s_lookup("int"), leafs(MAP_TYPE, $3), 0);
    }
    | declaration_modifiers '(' conditional_expr ')' '*' {
       // if (!($1 & (M_SHORT | M_LONG))) yyerror("No storage class specified");
       // $$ = lnew_type(check_modtype($1, s_lookup("int")), s_lookup("int"), $3, 1);
    }
    | declaration_modifiers '(' MAP_TYPE ')' '*' {
       // if (!($1 & (M_SHORT | M_LONG))) yyerror("No storage class specified");
       // $$ = lnew_type(check_modtype($1, s_lookup("int")), s_lookup("int"), leafs(MAP_TYPE, $3), 1);
    }
    | declaration_modifiers builtin_type_specifier '(' conditional_expr ')' {
       // $$ = lnew_type(check_modtype($1, $2), $2, $4, 0);
    }
    | declaration_modifiers builtin_type_specifier '(' MAP_TYPE ')' {
       // $$ = lnew_type(check_modtype($1, $2), $2, leafs(MAP_TYPE, $4), 0);
    }
    | declaration_modifiers builtin_type_specifier '(' conditional_expr ')' '*' {
       // $$ = lnew_type(check_modtype($1, $2), $2, $4, 1);
    }
    | declaration_modifiers builtin_type_specifier '(' MAP_TYPE ')' '*' {
       // $$ = lnew_type(check_modtype($1, $2), $2, leafs(MAP_TYPE, $4), 1);
    }
    | builtin_type_specifier '(' BAC_TYPE ')' {
       // $$ = lnew_type(M_NONE, $1, leafs(BAC_TYPE, $3), 0);
    }
    ;

declaration_modifiers
    : modifier { /* $$ = s_tok2mod($1); */ }
    | declaration_modifiers modifier { /* $$ = check_mod($1, s_tok2mod($2)); */ }
    ;

modifier
    : UNSIGNED
    | SIGNED
    | SHORT
    | LONG
    | CONST
    | LITTLE
    | BIG
    ;

alignment_modifier	
    : /* empty */ { /*$$ = NULL; */}
    | ALIGNED { /*$$ = leafi(INT_LITERAL, 8);*/ }
    | ALIGNED '(' conditional_expr ')' { /*$$ = check_align_arg($3); */}
    ;

type_specifier
    : builtin_type_specifier
    | user_type_specifier
    ;

builtin_type_specifier
    : BIT
    | CHAR
    | INT
    | FLOAT
    | DOUBLE
    ;

user_type_specifier
    : CLASS_TYPE {
      //  if (w_cyclic && s_get_cur_class() == $1)
      //      yyerror("Declaration creates cyclic reference in class '%s'", $1->name);
      //  $$ = $1;
    }
    ;

init_declarator
    : declarator
    | declarator '=' assignment_expr {   
       // $1->sp->assigned = 1;
       // $1->e1 = $3;
       // $$ = $1;
    }
    | declarator '=' array {
       // $1->sp->assigned = 1;
       // $1->e1 = $3;
       // $$ = $1;
    }
    | declarator_with_params
    | declarator_with_range
    ;

array
    : '{' array_expr_list '}' { 
      //  $$ = node(ARRAY, $2, NULL);
      //  $$->ptype = $2->ptype;
      //  $$->type = $2->type;
    }
    | STRING_LITERAL {
      //  $$ = node(ARRAY, eval_array_string($1), NULL);
      //  $$->ptype = s_lookup("char");
      //  $$->type = CHAR;
    }
    ;

array_expr_list
    : array_expr
    | array_expr_list ',' array_expr 
    { 
      //  $$ = node($2, $1, $3);
      //  $$->ptype = check_type_promote($1->ptype, $3->ptype);
      //  $$->type = $$->ptype->ident;
    }
    ;

array_expr
    : assignment_expr
    | array
    ;

declarator
    : identifier { /*$$ = lnew_sp(DECL, $1); */}
    | declarator '[' assignment_expr ']' {
        /* TBD: If the array is not parsable and not initialized, then the dimx_end variable is useless */
      //  s_cur_class_dimx_nonpartial($1->dims, 1);
      //  $1->dims++;
      //  $1->dim = (node_t **)buildarray((void **)$1->dim, node($2, check_array_size($3), NULL));
      //  $$ = $1;
    }
    | declarator '[' '[' assignment_expr ']' ']' {
       // s_cur_class_dimx_nonpartial($1->dims, 0);
       // $1->dims++;
       // $1->dim = (node_t **)buildarray((void **)$1->dim, node(DBL_BRACE_L, check_array_index($4), NULL));
       // $$ = $1;
    }
    ;

declarator_with_params
    : declarator '(' argument_expr_list ')' {
        /* add argument expression list to declaration */
       // $1->e3 = $3;
       // $$ = $1;
    }   
    ;

declarator_with_range
    : declarator '=' conditional_expr RANGE conditional_expr
    {
      //  $1->sp->range = 1;
      //  $1->sp->assigned = 1;
      //  $1->e1 = idrange($3, $5);
      //  $$ = $1;
    }
    ;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    | bac_statement
    ;

labeled_statement
    : identifier ':' statement_or_declaration
    { 
    //    if ($1->ptype != NULL) yyerror("Use of variable '%s' as a label is illegal", $1->name);
    //    $1->ident = LABEL;
    //    $$ = prep($3, lnew_sp(LABEL, $1));
    }
    | CASE conditional_expr ':' statement_or_declaration
    {
     //   if ($2->type > INT) yyerror("The 'case' expression must be of integral type");
     //   if (!is_literal($2->op) || ($2->op == VARIABLE && !($2->left.sp->modifiers & M_CONST))) yyerror("The 'case' expression must be a constant");
     //   $$ = lnews1($1, $2, $4);
    }
    | DEFAULT ':' statement_or_declaration { /*$$ = lnews0($1, $3); */}
    ;

compound_statement
    : '{' '}' { /*$$ = lnew0($1); */}
    | '{' statement_or_declaration_list '}' { /*$$ = lnews0($1, $2); */}
    ;

statement_or_declaration_list
    : statement_or_declaration
    | statement_or_declaration_list statement_or_declaration   { /*$$ = app($1, $2); */}
    ;

statement_or_declaration
    : statement
    | declaration
    | verbatim
    | pragma
    | include_file
    | import_file
    ;

expression_statement
    : ';'                   { /* $$ = lnew0(EXPR); */}
    | assignment_expr ';'   { /* $$ = lnew1(EXPR, $1); */}
    ;

selection_statement
    : IF '(' logical_or_expr ')' statement_or_declaration %prec LOWER_THAN_ELSE { /*$$ = lnew_ifelse($3, $5, NULL); */}
    | IF '(' logical_or_expr ')' statement_or_declaration ELSE statement_or_declaration { /*$$ = lnew_ifelse($3, $5, $7);*/ }
    | SWITCH '(' assignment_expr ')' statement_or_declaration 
    /* in C++, we can have assignment_expr_list inside the brackets, but not in Java */
    {
        //if ($3->type>INT) yyerror("'switch' expression must be of integral type");
        //$$ = lnews1($1, $3, $5);
    }
    ;

iteration_statement
    : WHILE '(' logical_or_expr ')' statement_or_declaration                                                 { /* $$ = lnews1($1, $3, $5); */}
    | DO statement_or_declaration WHILE '(' logical_or_expr ')' ';'                                          { /* $$ = lnews1($1, $5, $2); */}
    | FOR '(' ';' ';' ')' statement_or_declaration                                                           { /* $$ = lnews3($1, NULL, NULL, NULL, $6); */}
    | FOR '(' ';' ';' assignment_expr_list ')' statement_or_declaration                                      { /* $$ = lnews3($1, NULL, NULL, $5,   $7); */}
    | FOR '(' ';' logical_or_expr ';' ')' statement_or_declaration                                           { /* $$ = lnews3($1, NULL, $4,   NULL, $7); */}
    | FOR '(' ';' logical_or_expr ';' assignment_expr_list ')' statement_or_declaration                      { /* $$ = lnews3($1, NULL, $4,   $6,   $8); */}
    | FOR '(' assignment_expr_list ';' ';' ')' statement_or_declaration                                      { /* $$ = lnews3($1, $3,   NULL, NULL, $7); */}
    | FOR '(' assignment_expr_list ';' ';' assignment_expr_list ')' statement_or_declaration                 { /* $$ = lnews3($1, $3,   NULL, $6,   $8); */}
    | FOR '(' assignment_expr_list ';' logical_or_expr ';' ')' statement_or_declaration                      { /* $$ = lnews3($1, $3,   $5,   NULL, $8); */}
    | FOR '(' assignment_expr_list ';' logical_or_expr ';' assignment_expr_list ')' statement_or_declaration { /* $$ = lnews3($1, $3,   $5,   $7,   $9); */}
    ;

jump_statement
    : GOTO LABEL ';' { /*$$ = lnew_sp($1, $2); */}
    | CONTINUE ';'   { /*$$ = lnew0($1); */}
    | BREAK ';'      { /*$$ = lnew0($1); */}
    ;

program
    : external_definition         
    { 
        /* 'program' may be called multiple times (when we do includes) */
        //$$ = parsed_code=app(parsed_code, $1);
    }
    | program external_definition { /*$$ = app($1, $2); */}
    ;

external_definition
    : declaration
    | class_definition
    | map_definition
    | verbatim
    | pragma
    | include_file
    | import_file
    | bac_definition
    ;

/********************/
/* class definition */
/********************/

class_definition
    : complex_class_declarator class_body
    {
     //   if ($1->forward == 1) {
     //       $1->forward = 2;
     //       /* check for cyclic references in contained members, if requested */
     //       if (w_cyclic) check_cyclic_decl_contained($1, $1->forwardref);
     //   }
     //   /* if it has align spec, then it is considered parsable */
     //   if ($1->align != NULL) $1->parsable = 1;

     //   s_remcontext();         /* exit param type scope */
     //   s_remcontext();         /* exit class scope (will not be deleted) */
     //   s_set_cur_class(NULL);  /* exiting class decl */
     //   $$ = lnews_sp(CLASS, $1, $2);
    }
    | complex_class_declarator ';'  /* forward declaration */
    {
     //   $1->forward = 1;        /* mark as a forward declaration */
     //   $1->parsable = 1;       /* all forwards are parsable */
     //   $1->accessed--;         /* don't count forward decl as access */
     //   /* forward declarations shouldn't use 'extends' */
     //   if ($1->ptype != NULL) {
     //       yyerror("Forward declaration of a class cannot use 'extends'");
     //       fatal("Translation aborted");
     //   }
     //   /* they should also not use IDs */
     //   if ($1->id != NULL) {
     //       yyerror("Forward declaration of a class cannot use an ID declaration");
     //       fatal("Translation aborted");
     //   }
     //   /* they cannot use the abstract keyword (although it wouldn't do harm) */
     //   if ($1->abstract == 1) {
     //       yyerror("Forward declaration of a class should not use 'abstract'");
     //       $1->abstract = 0;
     //   }
     //   /* finally, they cannot use align() */
     //   if ($1->align != NULL) {
     //       yyerror("Forward declaration of a class should not use 'align'");
     //       $1->align = NULL;
     //   }
     //   s_remcontext();         /* exit param type scope */
     //   s_remcontext();         /* exit class scope (will not be deleted) */
     //   s_set_cur_class(NULL);  /* exiting class decl */
     //   $$ = lnews_sp(CLASS, $1, NULL);
    }
    ;

complex_class_declarator
    : derived_class_declarator
    {
      //  if ($1->abstract && $1->id==NULL) {
      //      yyerror("Abstract class '%s' requires an ID", $1->name);
      //      $1->abstract=0;
      //  }
      //  $$ = $1;
    }
    | derived_class_declarator ':' id_declarator
    { 
     //   $1->id = $3;
     //   s_transfer($3, $1->cxt);
     //   if (!$1->abstract) check_id($1);
     //   $$ = $1;
    }
    | derived_class_declarator ':' id_range_declarator
    { 
      //  $1->id = $3;
      //  s_transfer($3, $1->cxt);
      //  if (!$1->abstract) check_id($1);
      //  $$ = $1;
    }
    ;

derived_class_declarator
    : param_class_declarator
    | param_class_declarator EXTENDS CLASS_TYPE
    { 
      //  /* forward declarations *cannot* be used as base classes */
      //  if ($3->forward == 1) {
      //      yyerror("Forward-declared class '%s' cannot be used as a base class", $3->name);
      //      $3->accessed--; /* don't count this as an access */
      //  }
      //  else {
      //      /* check for cyclic refs, if requested */
      //      if (w_cyclic) check_cyclic_decl_derived($1, $3, $1->forwardref);
      //      /* derived class, mark base type */
      //      $1->ptype = $3;
      //      /* mark derivation on base class as well */
      //      $3->derived = (symbol_t **)buildarray((void **)$3->derived, $1);
      //      /* if base is parsable, so is derived */
      //      $1->parsable = $3->parsable;
      //      /* if no alignment info, copy from base */
      //      if ($1->align == NULL) $1->align = $3->align;
      //      /* copy symbol table to derived class (derived table is *empty* at this point) */
      //      s_copy_st($3->cxt, $1->cxt);
      //      /* check that parameter types match exactly */
      //      check_formal_params($1, $1->param, $3->param);
      //  }
      //  $$ = $1;
    }
    | param_class_declarator EXTENDS identifier
    {
      //  yyerror("Incorrect base class '%s'", $3->name);
      //  $$ = $1;
    }
    ;

param_class_declarator
    : aligned_class_declarator
    {
      //  /* if we had a forward declaration, it should also not have params */
      //  if ($1->forward == 1 && $1->param != NULL) {
      //      yyerror("Class '%s' definition/declaration has no parameters but previous declaration did", $1->name);
      //      $1->param = NULL;
      //  }
      //  $$=$1;
    }
    | aligned_class_declarator '(' parameter_list ')' 
    {
     //   /* if we had a forward declaration, params must match */
     //   if ($1->forward == 1) check_forward_params($1, $3, $1->param);
     //   $1->param = $3; /* we always trust the last declaration or the definition */
     //   $$ = $1;
    }
    ;

aligned_class_declarator
    : alignment_modifier class_declarator 
    {
     //   $2->align = $1;
     //   $$ = $2;
    }
    ;

class_declarator
    : CLASS identifier
    {
      //  /* set current class; used for tracking recursive decls */
      //  s_set_cur_class($2);
      //  /* new context for class members */
      //  $2->cxt = s_newcontext(CLASS);
      //  /* yet another context for parameter list, if any */
      //  s_newcontext(0);
      //  $2->ident = CLASS_TYPE;
      //  $$ = $2;
    }
    | ABSTRACT CLASS identifier
    {
      //  /* set current class; used for tracking recursive decls */
      //  s_set_cur_class($3);
      //  /* new context for class members */
      //  $3->cxt = s_newcontext(CLASS);
      //  /* yet another context for parameter list, if any */
      //  s_newcontext(0);
      //  $3->ident = CLASS_TYPE;
      //  $3->abstract = 1;
      //  $$ = $3;
    }
    | CLASS CLASS_TYPE
    {
      //  if ($2->forward != 1) yyerror("Duplicate definition of class '%s'", $2->name);
      //  /* set current class; used for tracking recursive decls */
      //  s_set_cur_class($2);
      //  /* new context for class members */
      //  $2->cxt = s_newcontext(CLASS);
      //  /* yet another context for parameter list, if any */
      //  s_newcontext(0);
      //  $$ = $2;
    }
    | ABSTRACT CLASS CLASS_TYPE
    {
      //  if ($3->forward != 1) yyerror("Duplicate definition of class '%s'", $3->name);
      //  /* set current class; used for tracking recursive decls */
      //  s_set_cur_class($3);
      //  /* new context for class members */
      //  $3->cxt = s_newcontext(CLASS);
      //  /* yet another context for parameter list, if any */
      //  s_newcontext(0);
      //  $3->abstract = 1;
      //  $$ = $3;
    }
    ;

id_declarator
    : alignment_modifier builtin_type_specifier '(' conditional_expr ')' identifier '=' conditional_expr
    {
        // $$ = set_var(check_rescope_id($6), $2, M_CONST, check_parse($2, M_CONST, $4), $8, NULL, $1, 0, NULL, 0, 0);
        // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers builtin_type_specifier '(' conditional_expr ')' identifier '=' conditional_expr
    {
        // $$ = set_var(check_rescope_id($7), $3, check_modtype(($2|M_CONST), $3), check_parse($3, ($2|M_CONST), $5), $9, NULL, $1, 0, NULL, 0, 0);
        // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers '(' conditional_expr ')' identifier '=' conditional_expr
    {
       // if (!($2 & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", $6->name);
       // $$ = set_var(check_rescope_id($6), s_lookup("int"), check_modtype(($2|M_CONST), s_lookup("int")), check_parse(s_lookup("int"), ($2|M_CONST), $4), $8, NULL, $1, 0, NULL, 0, 0);
       // $$->align = $1;
    }
    | alignment_modifier builtin_type_specifier '(' MAP_TYPE ')' identifier '=' conditional_expr
    {
       // $$ = set_var(check_rescope_id($6), $2, M_CONST, check_parse($2, M_CONST, leafs(MAP_TYPE, $4)), $8, NULL, $1, 0, NULL, 0, 0);
       // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers builtin_type_specifier '(' MAP_TYPE ')' identifier '=' conditional_expr
    {
      // $$ = set_var(check_rescope_id($7), $3, check_modtype(($2|M_CONST), $3), check_parse($3, ($2|M_CONST), leafs(MAP_TYPE, $5)), $9, NULL, $1, 0, NULL, 0, 0);
      // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers '(' MAP_TYPE ')' identifier '=' conditional_expr
    {
       // if (!($2 & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", $6->name);
       // $$ = set_var(check_rescope_id($6), s_lookup("int"), check_modtype(($2|M_CONST), s_lookup("int")), check_parse(s_lookup("int"), ($2|M_CONST), leafs(MAP_TYPE, $4)), $8, NULL, $1, 0, NULL, 0, 0);
       // $$->align = $1;
    }
    ;

/* take care of the ID ranges */
id_range_declarator
    : alignment_modifier builtin_type_specifier '(' conditional_expr ')' identifier '=' conditional_expr RANGE conditional_expr
    {
       // $$ = set_var(check_rescope_id($6), $2, M_NONE, check_parse($2, M_CONST, $4), idrange($8, $10), NULL, $1, 0, NULL, 0, 1);
       // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers builtin_type_specifier '(' conditional_expr ')' identifier '=' conditional_expr RANGE conditional_expr
    {
       // $$ = set_var(check_rescope_id($7), $3, check_modtype($2, $3), check_parse($3, ($2|M_CONST), $5), idrange($9, $11), NULL, $1, 0, NULL, 0, 1);
       // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers '(' conditional_expr ')' identifier '=' conditional_expr RANGE conditional_expr
    {
       // if (!($2 & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", $6->name);
       // $$ = set_var(check_rescope_id($6), s_lookup("int"), check_modtype($2, s_lookup("int")), check_parse(s_lookup("int"), ($2|M_CONST), $4), idrange($8, $10), NULL, $1, 0, NULL, 0, 1);
       // $$->align = $1;
    } 
    | alignment_modifier builtin_type_specifier '(' MAP_TYPE ')' identifier '=' conditional_expr RANGE conditional_expr
    {
       // $$ = set_var(check_rescope_id($6), $2, M_CONST, check_parse($2, M_NONE, leafs(MAP_TYPE, $4)), idrange($8, $10), NULL, $1, 0, NULL, 0, 1);
       // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers builtin_type_specifier '(' MAP_TYPE ')' identifier '=' conditional_expr RANGE conditional_expr
    {
       // $$ = set_var(check_rescope_id($7), $3, check_modtype($2, $3), check_parse($3, ($2|M_CONST), leafs(MAP_TYPE, $5)), idrange($9, $11), NULL, $1, 0, NULL, 0, 1);
       // $$->align = $1;
    }
    | alignment_modifier declaration_modifiers '(' MAP_TYPE ')' identifier '=' conditional_expr RANGE conditional_expr
    {
      //  if (!($2 & (M_SHORT | M_LONG))) yyerror("No storage class specified for ID variable '%s'", $6->name);
      //  $$ = set_var(check_rescope_id($6), s_lookup("int"), check_modtype($2, s_lookup("int")), check_parse(s_lookup("int"), ($2|M_CONST), leafs(MAP_TYPE, $4)), idrange($8, $10), NULL, $1, 0, NULL, 0, 1);
      //  $$->align = $1;
    }
    ;


/* allow both C++ and Java style class declarations */
class_body
    : '{' statement_or_declaration_list '}'     { /* $$ = $2; */ }
    | '{' statement_or_declaration_list '}' ';' { /* $$ = $2; */ }
    | '{' '}'                                   { /* $$ = NULL; */ }
    | '{' '}' ';'                               { /* $$ = NULL; */ }
    ;

parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration { /* $$ = app($1, $3); */}
    ;

parameter_declaration
    : plain_type_info declarator {
       // /* set type and mods; no parse, init, params, or alignment here */
       // set_var($2->sp, $1->sp, $1->type, NULL, NULL, NULL, NULL, $2->dims, $2->dim, 1, $2->sp->range);

       // /* formal parameters are considered assigned */
       // $2->sp->assigned = 1;
       // $2->sp->isparam =1 ;
       // $$ = $2;
    }
    ;

/*****************/
/* verbatim code */
/*****************/

verbatim
    : VERBATIM { /*$$ = lnew_vp(VERBATIM, $1); */}
    | VERBATIM_GET { 
      //  if (!s_in_class()) yyerror("Verbatim get() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_GET, $1);
    }
    | VERBATIM_PUT {
      //  if (!s_in_class()) yyerror("Verbatim put() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_PUT, $1);
    }
    | VERBATIM_BOTH {
      //  if (!s_in_class()) yyerror("Verbatim get()/put() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_BOTH, $1);
    }
    | VERBATIM_C { /*$$ = lnew_vp(VERBATIM_C, $1); */}
    | VERBATIM_GET_C {
      //  if (!s_in_class()) yyerror("Verbatim C++ get() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_GET_C, $1);
    }
    | VERBATIM_PUT_C {
      //  if (!s_in_class()) yyerror("Verbatim C++ put() code can only be included within a class");
       // $$ = lnew_vp(VERBATIM_PUT_C, $1);
    }
    | VERBATIM_BOTH_C {
      //  if (!s_in_class()) yyerror("Verbatim C++ get()/put() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_BOTH_C, $1);
    }
    | VERBATIM_C_CPP {
      //  if (s_in_class()) yyerror("Verbatim C++ code for .cpp file can not be included within a class");
      //  $$ = lnew_vp(VERBATIM_C_CPP, $1);
    }
    | VERBATIM_J { /*$$ = lnew_vp(VERBATIM_J, $1); */}
    | VERBATIM_GET_J {
      //  if (!s_in_class()) yyerror("Verbatim Java get() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_GET_J, $1);
    }
    | VERBATIM_PUT_J {
      //  if (!s_in_class()) yyerror("Verbatim Java put() code can only be included within a class");
      //  $$ = lnew_vp(VERBATIM_PUT_J, $1);
    }
    | VERBATIM_BOTH_J {
      //  if (!s_in_class()) yyerror("Verbatim Java get()/put() code can only be included within a class");
       // $$ = lnew_vp(VERBATIM_BOTH_J, $1);
    }
    ;

/***************/
/* pragma code */
/***************/

pragma
    : PRAGMA                { /* $$ = lnew0(PRAGMA); */ }
    | PRAGMA pragma_list    { /* $$ = lnews0(PRAGMA, $2); */ }
    ;

pragma_list
    : pragma_item
    | pragma_list ',' pragma_item { /* $$ = app($1, $3); */}
    ;

pragma_item
    : PRAGMA_ARRAY '=' INT_LITERAL { 
      //  if ($3 <= 0) yyerror("Negative or zero size for 'array' pragma: %d", $3);
      //  else max_array_size = $3;
      //  $$ = lnew1($1, leafi(INT_LITERAL, max_array_size));
    }
    | PRAGMA_GET		        { /* $$ = lnew0($1); */ }
    | PRAGMA_NOGET		        { /* $$ = lnew0($1); */ }
    | PRAGMA_PUT		        { /* $$ = lnew0($1); */ }
    | PRAGMA_NOPUT	            { /* $$ = lnew0($1); */ }
    | PRAGMA_PUTXML		        { /* $$ = lnew0($1); */ }
    | PRAGMA_NOPUTXML           { /* $$ = lnew0($1); */ }
    | PRAGMA_TRACE		        { /* $$ = lnew0($1); if (!$$->included) decl_trace = 1; */ }
    | PRAGMA_NOTRACE	        { /* $$ = lnew0($1); */ }
    | PRAGMA_LINE               { /* $$ = lnew0($1); */ }
    | PRAGMA_NOLINE             { /* $$ = lnew0($1); */ }
    | PRAGMA_NULLSTRINGS        { /* null_strings = 1; $$ = lnew0($1); */ }
    | PRAGMA_NONULLSTRINGS      { /* null_strings = 0; $$ = lnew0($1); */ }
    | PRAGMA_INCLUDES           { /* $$ = lnew0($1); if (!$$->included) gen_includes = 1; */ }
    | PRAGMA_NOINCLUDES         { /* $$ = lnew0($1); if (!$$->included) gen_includes = 0; */ }
    | PRAGMA_BITSTREAM '=' STRING_LITERAL   { /* $$ = lnew1($1, leafstr(STRING_LITERAL, $3)); */ }
    | PRAGMA_PREFIX '=' STRING_LITERAL      { /* $$ = lnew1($1, leafstr(STRING_LITERAL, $3)); */ }
    | PRAGMA_ERROR_FUNC '=' STRING_LITERAL  { /* $$ = lnew1($1, leafstr(STRING_LITERAL, $3)); */ }
    | PRAGMA_TRACE_FUNC '=' STRING_LITERAL  { /* $$ = lnew1($1, leafstr(STRING_LITERAL, $3)); */ }
    ;

/**************/
/* directives */
/**************/

/* imports */
import_file
    : IMPORT STRING_LITERAL {
       // $$ = lnew1(IMPORT, leafstr(STRING_LITERAL, $2));
       // if (!include($2, 1)) {
       //     /* loop would occur; don't issue include statement */
       //     if (w_includes) yyerror("Importing file '%s' would create an infinite loop", $2);
       //     $$ = NULL;
       // }
    }
    ;

/* includes */    
include_file
    : INCLUDE STRING_LITERAL {
      //  $$ = lnew1($1, leafstr(STRING_LITERAL, gethname($2)));
      //  if (!include($2, 0)) {
      //      /* loop would occur; don't issue include statement */
      //      if (w_includes) yyerror("Including file '%s' would create an infinite loop", $2);
      //      $$ = NULL;
      //  }
    }
    ;

/******************/
/* map definition */
/******************/

map_definition
    : map_declarator map_body {
       // s_set_cur_class(NULL);  /* no current class or map */
       // $$ = lnews_sp(MAP, $1, $2);
    }
    ;

map_declarator
    : MAP identifier '(' type_specifier ')' {
       // $2->ident = MAP_TYPE;
       // $2->modifiers = M_NONE;
       // $2->ptype = $4;
       // if ($4->ident == CLASS_TYPE) check_map_class($4);
       // $$ = s_set_cur_class($2); /* mark current map */
    }
    | MAP identifier '(' declaration_modifiers ')' {
      //  $2->ident = MAP_TYPE;
      //  $2->modifiers = $4;
      //  $2->ptype = s_lookup("int");
      //  $$ = s_set_cur_class($2);
    }
    | MAP identifier '(' declaration_modifiers builtin_type_specifier ')' {
      // $2->ident = MAP_TYPE;
      // $2->modifiers = $4;
      // $2->ptype = $5;
      // $$ = s_set_cur_class($2);
    }
    ;

map_body
    : '{' map_entry_list '}' { /* $$ = $2; */ }
	;

map_entry_list
    : map_entry
    | map_entry_list ',' map_entry { 
       // /* check for unique decodability and unique value */
       // if (check_map(s_get_cur_class(), $1, $3)) $$ = app_map($1, $3);
       // else $$ = $1;
    }
    ;

map_entry
    : map_code ',' map_entry_item { /* $$ = $1; $$->sub1 = $3; */}
    ;

/* 'map_code' is needed to save lineno info for the code */
map_code
    : BIT_LITERAL { /*$$ = lnew1(BIT_LITERAL, leafb(BIT_LITERAL, $1)); */}
    ;

/* Allow only single brace, not nested.  Also it's easy to check class type map value, 
 * which is checked below when '{' map_entry_item_list '}' is reduced to map_entry_item 
 * with check_map_class_value() function.
 */

map_entry_item_list
    : conditional_expr                         { /* $$ = lnew1(EXPR, $1); */ }
    | map_entry_item_list ',' conditional_expr { /* $$ = app($1, lnew1(EXPR, $3)); */ }
    ;

map_entry_item_escape
    : builtin_type_specifier '(' conditional_expr ')' { 
        //$$ = lnew3(ESC_FTYPE, leafi(EXPR, M_NONE), leafbs(EXPR, $1), $3);
    }
    | declaration_modifiers builtin_type_specifier '(' conditional_expr ')' { 
        //$$ = lnew3(ESC_FTYPE, leafi(EXPR, check_modtype($1, $2)), leafbs(EXPR, $2), $4);
    }

    /* The following elements will be used if and when we allow map cascading.

    | CLASS_TYPE '(' MAP_TYPE ')' { 
        //$$ = lnew3(ESC_MAP, leafi(EXPR, M_NONE), leafs(CLASS_TYPE, $1), leafs(MAP_TYPE, $3));
    }
    | builtin_type_specifier '(' MAP_TYPE ')' {
        //$$ = lnew3(ESC_MAP, leafi(EXPR, M_NONE), leafs(EXPR, $1), leafs(MAP_TYPE, $3));
    }
    | declaration_modifiers builtin_type_specifier '(' MAP_TYPE ')' {
        //$$ = lnew3(ESC_MAP, leafi(EXPR, check_modtype($1, $2)), leafs(EXPR, $2), leafs(MAP_TYPE, $4));
    }
    */
    ;

map_entry_item_escape_list
    : map_entry_item_escape                                { /* $$ = $1; */ }
    | map_entry_item_escape_list ',' map_entry_item_escape { /* $$ = app($1, $3); */ }
    ;

map_entry_item
    : conditional_expr { /* $$ = check_map_simple_value(s_get_cur_class(), lnew1(EXPR, $1)); */}
    | '{' map_entry_item_list '}' { /*$$ = check_map_class_value(s_get_cur_class(), lnews0($1, $2)); */}
	| map_entry_item_escape {
      //  check_map_esc_simple_value(s_get_cur_class(), $1);
      //  if (s_get_cur_class()->escape == 1) {
      //      yyerror("Escape code is allowed only once in a map");
      //      exit(1);
      //  }
      //  s_get_cur_class()->escape = 1;
      //  $$ = $1;
    }
    | '{' map_entry_item_escape_list '}' { 
       // check_map_esc_class_value(s_get_cur_class(), $2);
       // if (s_get_cur_class()->escape == 1) {
       //     yyerror("Escape code is allowed only once in a map");
       //     exit(1);
       // }
       // s_get_cur_class()->escape = 1;
       // $$ = $2;
    } 
    | STRING_LITERAL {
        /* must expand string to a list_t (*not* a node_t as with regular arrays...) */
       // $$ = NULL;
    }
    ;


/******************/
/* bac definition */
/******************/

bac_definition
    : bac_declarator bac_body 
    {
       // s_set_cur_class(NULL);
       // $$ = lnews_sp(BAC, $1, $2);
    }
    ;

bac_declarator
    : BAC identifier  
    {
        //$2->ident = BAC_TYPE;
        //$2->modifiers = M_NONE;
        //$$ = s_set_cur_class($2);
    }
    ;

bac_body
    : '{' bac_entry_list '}' { /*$$ = $2; */}
	;

bac_entry_list
    : bac_entry
    | bac_entry_list ',' bac_entry      { /*$$ = app($1, $3); */}
    ;

bac_entry
    : bac_keyword ',' bac_entry_item    { /*$$=$1; $$->sub1=$3; check_bac_val($$); */}
    ;

bac_keyword
    : STRING_LITERAL { /*$$ = lnew1(INT_LITERAL, leafi(INT_LITERAL, check_bac_param($1))); */}
    ;

bac_entry_item
    : conditional_expr              { /* $$ = lnew1(EXPR, $1); */ }
    | STRING_LITERAL                { /* $$ = lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, $1)); */ }
    | '{' bac_entry_item_list '}'   { /* $$ = lnews0($1, $2); */ }
    ;

bac_entry_item_list
    : conditional_expr                          { /* $$ = lnew1(EXPR, $1); */ }
    | STRING_LITERAL                            { /* $$ = lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, $1)); */ }
    | bac_entry_item_list ',' conditional_expr  { /* $$ = app($1, lnew1(EXPR, $3)); */ }
    | bac_entry_item_list ',' STRING_LITERAL    { /* $$ = app($1, lnew1(STRING_LITERAL, leafstr(STRING_LITERAL, $3))); */ }
    ;

bac_statement
    : BAC_TYPE '(' INT_LITERAL ')' ';' { /* $$ = lnew2(BAC_TYPE, leafs(BAC_TYPE, $1), leafi(INT_LITERAL, $3)); */}
    ;

%%

void
flavor::Parser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}