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

#ifndef _ptree_h_
#define _ptree_h_


#include "symbol.h"


/*
 * Parsing tree structures for expressions
 */


/* node entry (same as yylval) */
typedef union {
    char            *string;
    int             ival;
    double          dval;
    unsigned int    uival;
    bit_t           *bit;
    symbol_t        *sp;
    struct _node_t  *np;
    struct _type_t  *tp;
    verbatim_t      *vp;
} item_t;

/* The actual parsing tree node.  All expressions are parsed into node_t trees, and 
 * attached to statement list_t structures (see below).
 */
struct _node_t {
    int op;                    /* operator to be applied */
    int type;                  /* track the type of subexpressions */
    symbol_t *ptype;           /* pointer to type symbol table entry */
    short int leaf;            /* set if this is a leaf node (so we don't have to check operator) */
	short paren;			   /* set if we need to maintain the parenthesis */
    struct _node_t *eval;      /* when doing eval substitution on constants, we save 
                                  here the original expr (this is used by lengthof()) */
    item_t left, right, third; /* 3 entries (to cover ?: ) */
};

/* Linked list of statements and declarations.
 *
 * Statements and declarations are stored sequentially;
 * if, while, etc. also have sub-statements (the block of statements that
 * they refer to; the 'e' entries are used to store the expressions for the
 * entries required (e.g., for requires 3 such entries).
 *
 * This is also used in a special way to pass type info in a 
 * list of declarations (see 'declaration_list' in parser.y);
 * 'type' gets the modifiers, 'e1' the init expression, 'e2' the parse
 * expression, 'sp' the type pointer, and 'e3' the argument list
 * (for parameter types).
 */
struct _list_t {
    /* identify a statement (expression, for, while, etc.), declaration or verbatim section */
    int type;

    /* expression pointers for the statement's components */
    node_t *e1, *e2, *e3;

    /* if a declaration, pointer to the symbol table entry */
    symbol_t *sp;

    /* if alignment argument is specified, it goes here */
    node_t *align;

    /* If a declaration of a parsable variable, the following flag signals that the next*() 
     * function version should be used (e.g., nextbits() instead of getbits()).
     */
    int usenext;

    /* if an array declaration, save number of dims and dimension expressions */
    int dims;
    node_t **dim;

    /* if a verbatim section, pointer to the verbatim string */
    verbatim_t *vp;

    /* next in the list */
    struct _list_t *next;

    /* sub-statements, e.g., 'if (a) stmt' will put 'a' in e1 and 'stmt' in sub1 */
    struct _list_t *sub1;
    struct _list_t *sub2; /* only used for the 'else' part of if statements */

    /* line number where the entry was inserted */
    int lineno;

    /* set if the list entry is from included file (so no code generation will happen) */
    int included;
};


/*
 * List management functions
 */

/* allocate new list entry */
#define lalloc() ((list_t *)getmem(sizeof(list_t)))

/* deallocate list entry (shallow) */
#define lfree(a) (free(a))

/* define new list entries (0-3 expr's, with/without subexpression) */
#define lnew0(t)                 lnew(t, NULL, NULL, NULL, NULL, NULL)
#define lnew1(t, e1)             lnew(t, e1,   NULL, NULL, NULL, NULL)
#define lnew2(t, e1, e2)         lnew(t, e1,   e2,   NULL, NULL, NULL)
#define lnew3(t, e1, e2 ,e3)     lnew(t, e1,   e2,   e3,   NULL, NULL)
#define lnews0(t, s)             lnew(t, NULL, NULL, NULL, s,    NULL)
#define lnews1(t, e1, s)         lnew(t, e1,   NULL, NULL, s,    NULL)
#define lnews2(t, e1, e2, s)     lnew(t, e1,   e2,   NULL, s,    NULL)
#define lnews3(t, e1, e2, e3, s) lnew(t, e1,   e2,   e3,   s,    NULL)

/* define new list entry (statement) */
list_t *lnew(int type, node_t *e1, node_t *e2, node_t *e3, list_t *sub1, list_t *sub2);

/* duplicate a list entry */
list_t *ldup(list_t *lp);

/* define new list entry with symbol pointer */
list_t *lnew_sp(int type, symbol_t *sp);

/* define new list entry with verbatim code pointer */
list_t *lnew_vp(int type, verbatim_t *vp);

/* define new list entry with symbol pointer and sub-statement (for class definitions) */
list_t *lnews_sp(int type, symbol_t *sp, list_t *sub);

/* define new list entry for declaration_list type info */
list_t *lnew_type(int mods, symbol_t *type, node_t *parse, int usenext);

/* define new if-else entry */
#define lnew_ifelse(e1, sub1, sub2) lnew(flavor::Parser::token::IF, e1, NULL, NULL, sub1, sub2)
 
/* define new array entry */
#define lnew_array(sub1) lnew(ARRAY, NULL, NULL, NULL, sub1, NULL)

/* append an entry at the end of a list */
list_t *app(list_t *list, list_t *item);

/* insert a map entry - order according to length */
list_t *app_map(list_t *list, list_t *item);

/* prepend an entry at the top of a list */
list_t *prep(list_t *list, list_t *item);

/* completely destroy a list */
void destroy(list_t *list);


/*
 * Parsing tree management functions
 */

/* allocate new node */
#define nalloc() ((node_t *)getmem(sizeof(node_t)))

/* define new node (2 args only) (NOT leaf nodes) */
#define node(op, l, r) node3(op, l, r, NULL)

/* define new node */
node_t *node3(int op, node_t *left, node_t *right, node_t *third);

/* delete a node (recursively) */
void node_del(node_t *np);

/* create a copy of a node (non-recursive; used for literals) */
node_t *node_dup(node_t *n);

/* define leaf node with symbol */
node_t *leafs(int type, symbol_t *sp);

/* define leaf node with int */
node_t *leafi(int type, int i);

/* define leaf node with double */
node_t *leafd(int type, double d);

/* define leaf node with bit */
node_t *leafb(int type, bit_t *b);

/* define leaf node with string */
node_t *leafstr(int type, char *str);

/* define empty leaf node */
node_t *leaf(int type);

/* set the lengthof variable of a symbol */
symbol_t *set_lengthof(symbol_t *var);

/* Set the type, modifiers, parse size, and initial value of a variable.
 * Also check parameters and alignment, if specified, and dimensions and dimension expressions as well.
 * The last parameter is used to identify regular declarations from formal parameter declarations.
 * Returns variable symbol (may be different from one supplied if rescoping has been done)
 */
symbol_t *set_var(symbol_t *var, symbol_t *type, unsigned int mod, node_t *parse, node_t *init, node_t *param, node_t *align, int dims, node_t **dim, int formal, int range);

node_t *leafbs(int type, symbol_t *sp);

/* id range */
node_t *idrange(node_t *node1, node_t *node2);


#endif /* ! _ptree_h_ */
