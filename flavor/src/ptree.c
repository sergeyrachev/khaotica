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

/* ptree.c */

#include <stdio.h>
#include <string.h>

#include "port.h"
#include "util.h"
#include "symbol.h"
#include "ptree.h"
#include "check.h"
#include "error.h"
#include "globals.h"

#include "y.tab.h"

/*
 * List management functions
 */

/* define new list entry with symbol pointer */
list_t *lnew_sp(int type, symbol_t *sp)
{
    list_t *n = lnew(type, NULL, NULL, NULL, NULL, NULL);
    n->sp = sp;
    return n;
}

/* define new list entry with symbol pointer and sub-statement (for class definitions) */
list_t *lnews_sp(int type, symbol_t *sp, list_t *sub)
{
    list_t *n = lnew_sp(type, sp);
    n->sub1 = sub;
    return n;
}

/* define new list entry for declaration_list type info */
list_t *lnew_type(int mods, symbol_t *type, node_t *parse, int usenext)
{
    list_t *n = lnew(mods, parse, NULL, NULL, NULL, NULL);
    n->sp = type;
    n->usenext = usenext;
    return n;
}

/* define new list entry with verbatim code pointer */
list_t *lnew_vp(int type, verbatim_t *vp)
{
    list_t *n = lnew(type, NULL, NULL, NULL, NULL, NULL);
    n->vp = vp;
    return n;
}

/* duplicate a list entry */
list_t *ldup(list_t *lp)
{
    list_t *n = lalloc();
    *n = *lp;
    n->next = NULL;
    return n;
}

/* define new list entry (statement) */
list_t *lnew(int type, node_t *e1, node_t *e2, node_t *e3, list_t *sub1, list_t *sub2)
{
    list_t *n = lalloc();
    
    n->type = type;
    n->e1 = e1;
    n->e2 = e2;
    n->e3 = e3;
    n->sub1 = sub1;
    n->sub2 = sub2;
    n->sp = NULL;
    n->align = NULL;
    n->usenext = 0;
    n->vp = NULL;
    n->dims = 0;
    n->dim = NULL;
    n->next = NULL;
    n->lineno = lineno;
    n->included = including();
    return n;
}

/* append an entry at the end of a list */
list_t *app(list_t *list, list_t *item)
{
    list_t *lp = list;
    
    if (lp == NULL) return item;
    while (lp->next != NULL) lp = lp->next;
    lp->next = item;
    
    /* return top of list */
    return list;
}

/* insert a map entry - order according to length */
list_t *app_map(list_t *list, list_t *item)
{
    list_t *lp, *prevlp;
    
    /* sanity check */
    if (item == NULL || item->e1 == NULL || item->e1->op != BIT_LITERAL || item->e1->left.bit == NULL)
        fatal("Internal error P%d: Expected code entry with bit literal", __LINE__);
    /* if list is empty, just return the item */
    if (list == NULL) return item;

    lp = list;
    prevlp = NULL;
    while (lp != NULL) {
        /* sanity check */
        if (lp->e1 == NULL || lp->e1->op != BIT_LITERAL || lp->e1->left.bit == NULL)
            fatal("Internal error P%d: Expected code entry with bit literal", __LINE__);
        /* compare length */
        if (item->e1->left.bit->len < lp->e1->left.bit->len) {
            if (prevlp == NULL) {
                /* we are at the top */
                item->next = list;
                return item;
            }
            else {
                prevlp->next = item;
                item->next = lp;
                return list;
            }
        }
        prevlp = lp;
        lp = lp->next;
    }
    /* end of the list; just append */
    prevlp->next = item;
    
    /* return top of list */
    return list;
}

/* prepend an entry at the top of a list */
list_t *prep(list_t *list, list_t *item)
{
    item->next = list;
    return item;
}

/* completely destroy a list */
void destroy(list_t *list)
{
    list_t *cur, *next;
    
    cur = list;
    while (cur != NULL) {
        next = cur->next;
        if (cur->type == DECL || cur->type == LABEL) continue;
        if (cur->e1 != NULL) node_del(cur->e1);
        if (cur->e2 != NULL) node_del(cur->e2);
        if (cur->e3 != NULL) node_del(cur->e3);
        if (cur->sub1 != NULL) destroy(cur->sub1);
        if (cur->sub2 != NULL) destroy(cur->sub2);
        free(cur);
        cur = next;
    }
}

/*
 * Parsing tree management functions
 */

/* define new node (NOT leaf nodes) */
node_t *node3(int op, node_t *left, node_t *right, node_t *third)
{
    node_t *np = nalloc();
    
    np->op = op;
    np->leaf = 0;
    np->paren = 0;
    np->eval = NULL;
    np->left.np = left;
    np->right.np = right;
    np->third.np = third;
    return np;
}

/* delete a node (recursively) */
void node_del(node_t *np)
{
    if (np == NULL) return;
    if (np->op == BIT_LITERAL) {
        free(np->left.bit);
        np->left.bit = NULL;
    }
    if (np->op == STRING_LITERAL) {
        free(np->left.string);
        np->left.string = NULL;
    }
    /* here we must be careful not to delete something useful or meaningless (symbol table entry etc.) */
    if (!np->leaf) node_del(np->left.np);
    node_del(np->right.np);
    node_del(np->third.np);
    free(np);
}

/* create a copy of a node (non-recursive; used for literals) */
node_t *node_dup(node_t *n)
{
    node_t *np = nalloc();
    *np = *n;
    return np;
}

/* define leaf node with builtin symbol; for map escape code */
node_t *leafbs(int type, symbol_t *sp) 
{
	node_t *np = nalloc();
    
    np->op = type;
    np->type = sp->ident;
    np->ptype = sp;
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.sp = sp;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define leaf node with symbol */
node_t *leafs(int type, symbol_t *sp)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->leaf = 1;
    np->paren = 0;

    /* can be called from var declarations or expressions, so ptype may or may not be set yet */
    if (sp->ptype != NULL) {
        np->type = sp->ptype->ident;
        np->ptype = sp->ptype;
    }
    else {
        /* if type not set, we have an undeclared variable which is already
         * reported; set the type to 'int' so that we can continue
         */
        np->ptype = s_lookup("int");
        np->type = np->ptype->ident;
    }
    np->eval = NULL;
    np->left.sp = sp;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define leaf node with int */
node_t *leafi(int type, int i)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->type = INT;
    np->ptype = s_lookup("int");
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.ival = i;
    np->left.uival = (unsigned)i;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define leaf node with double */
node_t *leafd(int type, double d)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->type = DOUBLE;
    np->ptype = s_lookup("double");
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.dval = d;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define empty leaf node */
node_t *leaf(int type)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->type = 0;
    np->ptype = NULL;
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.np = NULL;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define leaf node with bit */
node_t *leafb(int type, bit_t *b)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->type = BIT;
    np->ptype = s_lookup("bit");
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.bit = b;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* define leaf node with string */
node_t *leafstr(int type, char *str)
{
    node_t *np = nalloc();
    
    np->op = type;
    np->type = 0;	/* no type info for strings (only used in pragma and include statements) */
    np->ptype = NULL;
    np->leaf = 1;
    np->paren = 0;
    np->eval = NULL;
    np->left.string = str;
    np->right.np = NULL;
    np->third.np = NULL;
    return np;
}

/* set the lengthof variable of a symbol */
symbol_t *set_lengthof(symbol_t *var)
{
    symbol_t *sp;
    char *str;

    /* set name (including '%s' that will insert current prefix) */
    if (var->ptype->ident == CLASS_TYPE)
        sp = s_create("%slengthof", VARIABLE);    /* a class */
    else {
        /* not a class */
        /* allocate memory for '%s<name>_lengthof\0' */
        str = (char *)getmem(strlen(var->name)+12);
        sprintf(str, "%%s%s_lengthof", var->name);
        sp = s_create(str, VARIABLE);
        /* dims are the same as base */
        sp->dims = var->dims;
        free(str);
    }
    /* type is always 'int' */
    sp->ptype = s_lookup("int");
    /* variable is always assigned, internal, and not exported */
    sp->assigned = 1;
    sp->internal = 1; /* these are used to avoid error message reporting for this variable */
    sp->exported = 0;

    /* save in base variable */
    var->plengthof = sp;

    return sp;
}

/* Set the type, modifiers, parse size, and initial value of a variable. Also check the 
 * parameters and alignment, if specified, and dimensions and dimension expressions as well; 
 * the last parameter is used to identify regular declarations from formal parameter declarations.
 *
 * Returns:
 * The variable symbol (may be different from the one supplied if rescoping has been done)
 */
symbol_t* set_var(symbol_t*     var, 
                  symbol_t*     type, 
                  unsigned int  mod, 
                  node_t*       parse, 
                  node_t*       init, 
                  node_t*       param, 
                  node_t*       align, 
                  int           dims, 
                  node_t**      dim, 
                  int           formal,
                  int           range)
{
    if (var == NULL) return var;

    var->range = range;
    
    /* See if it is already declared.
     * If it is, and it's non-parsable, this is an error (unless we are in a new scope).
     * If it is, and it's parsable, types/mods must match and shouldn't be a constant.
     */
    if (var->ptype != NULL) {

        /* if already declared in a parent class or global scope, create a local copy */
        if (var->owner != s_get_cur_class())
            return set_var(s_add(var->name, VARIABLE), type, mod, parse, init, param, align, dims, dim, formal, range);
       
        /* see if we have a conflict in a higher scope */
        if (var->cxt_level != s_get_cur_cxt_level()) {
            /* watch out for class vars which end up in context level 1 */
            if (var->cxt_level == 1) {
                /* collision with class variable */
                if (!var->parsable && s_get_cur_cxt_level() > 3) {
                    /* create new instance in current context */
                    return set_var(s_add(var->name, VARIABLE), type, mod, parse, init, param, align, dims, dim, formal, range);
                }
            }
        }
       
        if (!var->parsable) {
            yyerror("Redeclaration of non-parsable variable '%s'", var->name);
            return var;
        }
        
        /* from here on, it's guaranteed to be parsable */

        if (parse == NULL && !type->parsable) {
            yyerror("Redeclaration of parsable variable '%s' as non-parsable not allowed", var->name);
            return var;
        }
        if (var->modifiers & M_CONST) {
            yyerror("Redeclaration of constant parsable variable '%s' not allowed", var->name);
            return var;
        }
        if (var->ptype != type) {
            yyerror("Redeclaration of parsable variable '%s' has different type", var->name);
            return var;
        }
        if (var->modifiers != mod) {
            yyerror("Redeclaration of parsable variable '%s' has different type modifiers", var->name);
            return var;
        }
        /* dimensions must match */
        if (dims != var->dims)
            yyerror("Number of dimensions (%d) for '%s' must match original declaration (%d)", dims, var->name, var->dims);
        
        /* you can't declare a value for a parsable without specifying a parse size */
        if (init != NULL && parse == NULL) {
            yyerror("Cannot declare value of parsable variable '%s' without specifying parse size", var->name);
            return var;
        }
        /* classes cannot be initialized */
        if (type->ident == CLASS_TYPE && init != NULL) {
            yyerror("Class '%s' cannot be initialized (use parameter types)", var->name);
            return var;
        }
        /* check if params are needed/match */
        if (type->ident == CLASS_TYPE) {
            if (!formal) {
                if (param != NULL && type->param == NULL)
                    yyerror("Class '%s' does not require parameters", var->name);
                else if (param == NULL && type->param != NULL)
                    yyerror("Class '%s' requires parameters", var->name);
                else if (param != NULL && type->param != NULL) {
                    /* check that they match */
                    check_actual_params(var, var->ptype->param, param);
                }
            }
        }
        else if (param != NULL && parse->op != BAC_TYPE)
            yyerror("Parameters are only allowed in appropriately defined classes");
        
        /* check that assigned value satisfies the type (if init has no type, then it involves an undeclared var; don't check) */
        if (init != NULL && init->ptype != NULL && !check_type_compat(var->ptype, init->ptype)) {
            yyerror("Initial value for '%s' not compatible with type declaration", var->name);
            return var;
        }
        /* if it is an array, check that the dimensions match (if not, init will be set to 0) */
        if (init != NULL && init->op == ARRAY) {
            if (!check_array_init(var, dim, dims, init)) {
                /* rewrite 'init' as a single 0 */
                init->op = INT_LITERAL;
                init->left.ival = 0;
            }
        }
       
#ifdef NOT  
        /* if float/double, check that parse size is correct */
        if (var->ptype->ident == FLOAT || var->ptype->ident == DOUBLE)
            check_double_parse(var, parse);
#endif
        /* if both parse and init are literals, we can check that the init fits within the parse size (also checks double/float constraints) */
        if (init != NULL && is_literal(parse->op))
            if (is_literal(init->op))
                check_fit(var, init, parse);
            else if (init->op == ARRAY)
                check_fit_array(var, init, parse);
        
        return var;
    }

    /* not declared yet */
    var->ptype = type;
    var->modifiers = mod;
    
    /* if the type is a forward-declared class F for which we haven't yet seen the actual definition, mark the owning class in F so that we can detect cyclic references */
    if (type->ident == CLASS_TYPE && type->forward == 1)
        type->forwardref = (symbol_t **)buildarray((void **)type->forwardref, (void *)var->owner);

    /* only const non-parsables allowed in global scope */
    if (var->cxt_level == 0) {
        if (var->ptype->ident == CLASS_TYPE) {
            yyerror("Variables of class type not allowed in global scope");
            return var;
        }
        if (!(mod & M_CONST) || parse != NULL) {
            yyerror("Only constant non-parsable variables allowed in global scope");
            return var;
        }
        /* note: arrays now allowed in global scope */
#ifdef NOT
        if (dims > 0) {
            yyerror("Arrays not allowed in global scope (they cannot be initialized)");
            return var;
        }
#endif
    }
    
    /* parsable variable */
    if (parse != NULL || type->parsable) {
        var->parsable = 1;
        
        /* if parsable, it is also assigned */
        var->assigned = 1;
        
        /* if a constant, save the parse info (needed for ID vars) */
        if ((mod & M_CONST) || var->range) var->parse = parse;

#ifdef NOT 
        /* if float/double, check that parse size is correct */
        if (var->ptype->ident == FLOAT || var->ptype->ident == DOUBLE) check_double_parse(var, parse);
#endif
        /* all parsable variables are moved to the class context */
        s_transfer_class(var);
        
        /* mark the currently defined class as parsable */
        s_mark_class_parsable();
    }
    
    /* note that a class with parse expr is not allowed by the grammar, i.e., 'A(3) a;' is forbidden) */
    if (type->ident == CLASS_TYPE) {
        /* if type is a parsable class, so is the variable */
        if (type->parsable) var->parsable = 1;

        /* classes are always considered assigned (no way to statically figure out if all vars are set) */
        var->assigned = 1;
        
        /* classes cannot have init value */
        if (init != NULL) {
            yyerror("Class '%s' cannot be initialized (use parameter types)", var->name);
            return var;
        }
        
        /* check if params are needed/match */
        if (!formal) {
            if (param != NULL && type->param == NULL)
                yyerror("Class '%s' does not require parameters", var->name);
            else if (param == NULL && type->param != NULL)
                yyerror("Class '%s' requires parameters", var->name);
            else if (param != NULL && type->param != NULL) 
                /* check that they match */
                check_actual_params(var, var->ptype->param, param);
        }
    }
    else if (param != NULL && parse->op != BAC_TYPE)
        yyerror("Parameters are only allowed in appropriately defined classes");
    
    /* if constant and not a class, there must be an init value */
    if (type->ident != CLASS_TYPE && (mod & M_CONST) && init == NULL) 
        yyerror("Declaring constant '%s' without an initial value", var->name);
    
    /* alignment is meaningful only for parsable variables */
    if (align != NULL) {
        if (!var->parsable) yyerror("Alignment specification for non-parsable variables is meaningless");
        else if ((mod & M_CONST) || var->range) var->align = align;
    }
    
    /* initialized variable */
    if (init != NULL) {
        /* check that assigned value satisfies the type (if init has no type, then it involves an undeclared var; don't check) */
        if (init->ptype != NULL && !check_type_compat(var->ptype, init->ptype))
            yyerror("Initial value for '%s' not compatible with type declaration", var->name);
        /* if it is an array, check that the dimensions match (if not, init will be set to 0) */
        if (init->op == ARRAY)
            if (!check_array_init(var, dim, dims, init)) {
                /* rewrite 'init' as a single 0 */
                init->op = INT_LITERAL;
                init->left.ival = 0;
            }

        /* if both parse and init are literals, we can check that the init fits within the parse size. */
        if (parse != NULL && is_literal(parse->op))
            if ((is_literal(init->op) || var->range)) check_fit(var, init, parse);
            else if (init->op == ARRAY) check_fit_array(var, init, parse);

        /* set the variable's initial value *only* if it is a constant */
        if ((mod & M_CONST) || var->range) var->init = init;

        var->assigned = 1;
    }
    
    /* dimensions are set regardless if it is a constant or not */
    var->dims = dims;
    
    /* if constant parsable, or non-parsable, set dimension expressions */
    if (dims != 0 && ((mod & M_CONST) || !var->parsable)) var->dim = dim;

    /* if non-parsable, cannot have double brace */
    if (!var->parsable) 
        if (!check_no_double_brace(dim))
            yyerror("Double brace not allowed in array declaration of non-parsable variable '%s'", var->name);

    return var;    
}


/* id range */
node_t *idrange(node_t *node1, node_t *node2)
{
    /* need to check that node1 < node2 */
    if ( (node1->op == INT_LITERAL && node1->left.ival > node2->left.ival) ||
         (node1->op == DOUBLE_LITERAL && node1->left.dval > node2->left.dval) ||
         (node1->op == BIT_LITERAL && node1->left.bit->val > node2->left.bit->val) ) {
        yyerror("The first id in the id range must be smaller than or equal to the second id");
    }
    
    node1->right.np = NULL;
    node1->third.np = node2;

    return node1;
}
