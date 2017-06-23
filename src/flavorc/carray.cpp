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

/* carray.c */

#include "cdef.h"


/* generate a tmp variable name for array dimensions (uses static memory) -- _F_dim0 for i = 0 */
const char *c_gentmp_dim(int i)
{
    static char *tmp = NULL;
    /* don't make assumptions about maximum size of identifiers */
    if (tmp == NULL) {
        tmp = (char *)getmem(1024);
        tmp[0] = '\0';
    }
    if (strlen(tmp) < strlen(prefix)+32) {
        tmp = (char *)regetmem(tmp, strlen(prefix)+32);
        tmp[0] = '\0';
    }
    sprintf(tmp, "%sdim%d", prefix, i);
    return tmp;
}

/* generate array size declaration */
void c_array_decl(symbol_t *sp, int dims, node_t **dim)
{
    int i;
    /* always generate arrays with max size */
    for (i=0; i<dims; i++) {
        if (cpp()) 
            c_out("[%d]", max_array_size);
		else if (java()) 
            c_out("[]");
    }
    if (java() && dims > 0) {
        if (sp->ptype->ident == flavor::Parser::token::BIT)
            c_out(" = new %s", (sp->modifiers & M_LONG ? "long" : "int"));
        else 
            c_out(" = new %s", (sp->modifiers & M_LONG ? "long" : sp->ptype->name));

        for (i=0; i<dims; i++) 
            c_out("[%d]", max_array_size);
    }
}

/* generate array size declaration using actual sizes */
void c_array_decl_actual(symbol_t *sp, int dims, node_t **dim)
{
    int i;
    /* always generate arrays with max size */
    for (i=0; i<dims; i++) {
        if (cpp()) {
            c_out("[");
            c_expression(dim[i]->left.np, 0);
            c_out("]");
        }
        else if (java()) 
            c_out("[]");
    }
}

/* generate temp variable declarations for arrays -- dims = # of dimensions */
void c_array_tmp_decl(int dims, int *dimx_end_var)
{
    int i;
    const char *pvar;
    if (dims <= 0) return;
    c_outi("int ");
    for (i=0; i<dims; i++) {
        pvar = c_gentmp_dim(i);
        if (dimx_end_var[i])
            c_out("%s, %s_end", pvar, pvar);
        else 
            c_out("%s", pvar);

        if (i != dims-1) 
            c_out(", ");
    }
    c_out(";\n");
}

/* array prologue ('for' loops) */
void c_array_prologue(symbol_t *sp, int dims, node_t **dim)
{
    int i;
    const char *tmp;
    node_t *np;
    /* compute array bounds */
    for (i=0; i<dims; i++) {
        tmp = c_gentmp_dim(i); /* get the tmp dimension variable -- e.g., _F_dim0 */
        np = dim[i];
        /* sanity check */
        if (np == NULL || np->left.np == NULL) 
            fatal("Internal error CA%d: Invalid dimension expression", __LINE__);
        if (np->op == flavor::Parser::token::DBL_BRACE_L)
            c_outi("%s = ", tmp);
        else 
            c_outi("%s_end = ", tmp);
        c_expression(np->left.np, 0);
        c_out(";\n");
    }
    /* generate 'for' loops */
    for (i=0; i<dims; i++) {
        tmp = c_gentmp_dim(i);
        np = dim[i];
        if (np->op == '[') {
            c_outi("for (%s = 0; %s < %s_end; %s++) {\n", tmp, tmp, tmp, tmp);
            c_identlevel++;
        }
    }
}

/* array proper (the indices with the tmp variables) */
void c_array_proper(symbol_t *sp, int dims)
{
    int i;
    const char* tmp;
    for (i=0; i<dims; i++) {
        tmp = c_gentmp_dim(i);
        c_out("[%s]", tmp);
    }
}

/* array epilogue ('for' loops) */
void c_array_epilogue(symbol_t *sp, int dims, node_t **dim)
{
    int i;
    node_t *n;
    /* close 'for' loops */
    for (i=0; i<dims; i++) {
        n = dim[i];
        if (n->op == '[') {
            c_identlevel--;
            c_outi("}\n");
        }
    }
}

/* duplicate declaration for c_array_unroll_rec */
static list_t *c_array_unroll_decl(list_t *lp, int *dim_values, int *dim_cnt, int *dim_skip)
{
    node_t *ip, **dim;
    list_t *ret;
    int i, j;

    /* first, create the index expression */
    dim = (node_t **)getmem((lp->dims+1)*sizeof(node_t *));
    for (i=0; i<lp->dims; i++)
        dim[i] = node(flavor::Parser::token::DBL_BRACE_L, leafi(flavor::Parser::token::INT_LITERAL, dim_cnt[i]), NULL);
    dim[i] = NULL;

    /* find the init expression */
    if (lp->sp->modifiers & M_CONST)
        ip = lp->sp->init;
    else 
        ip = lp->e1;

    /* position after the topmost ARRAY node */
    ip = ip->left.np;
    for (i=0; i<lp->dims; i++) {
        if (dim_skip[i]) continue;
        for (j=0; j<dim_values[i]-dim_cnt[i]-1; j++) {
            if (ip->left.np == NULL)
                fatal("Internal error CA%d: Unexpected end of array expression", __LINE__);
            ip = ip->left.np;
        }
        /* if we hit a comma, go right */
        if (ip->op == ',') 
            ip = ip->right.np;
        /* if we are at an array go left */
        if (ip->op == flavor::Parser::token::ARRAY)
            ip = ip->left.np;
        /* check again for a leaf */
        if (ip->leaf)
            break;
    }
    if (!ip->leaf) 
        fatal("Internal error CA%d: Failed to locate entry in array initialization expression", __LINE__);

    /* now create a duplicate list entry */
    ret = ldup(lp);

    /* attach */
    ret->dim = dim;
    ret->e1 = ip;

    return ret;
}

/* recursive helper for c_array_unroll */
static list_t *c_array_unroll_rec(list_t *lp, int *dim_values, int *dim_cnt, int *dim_skip, int dim, list_t *prev)
{
    int i, begin, end;
    list_t *n, *ret;

    /* if we reached the end, create duplicate */
    if (dim == lp->dims) {
        n = c_array_unroll_decl(lp, dim_values, dim_cnt, dim_skip);
        if (prev == NULL)
            return n;
        else
            return app(prev, n);
    }
    if (dim_skip[dim]) {
        begin = dim_values[dim];
        end = begin+1;
    }
    else {
        begin = 0;
        end = dim_values[dim];
    }
    ret = prev;
    for (i=begin; i<end; i++) {
        dim_cnt[dim] = i;
        ret = c_array_unroll_rec(lp, dim_values, dim_cnt, dim_skip, dim+1, ret);
    }
    return ret;
}

/* unrolls declaration of array initializers - this way no special code is needed in the code generator */
list_t *c_array_unroll(list_t *lp)
{
    symbol_t *sp;
    node_t *ip, *p;
    int *dim_values, *dim_skip; /* dim_skip is used to flag '[[' */
    int *dim_cnt;
    int i;
    list_t *ret = lp;

    sp = lp->sp;
    if (sp->modifiers & M_CONST) 
        ip = sp->init;
    else 
        ip = lp->e1;

    if (ip->op != flavor::Parser::token::ARRAY)
        fatal("Internal error CA%d: Cannot unroll array", __LINE__);

    /* first, figure out the dimensions of the symbol */
    dim_values = (int *)getmem(lp->dims*sizeof(int));
    dim_skip = (int *)getmem(lp->dims*sizeof(int));

    for (i=0; i<lp->dims; i++) {
        p = lp->dim[i];
        if (p == NULL) 
            fatal("Internal error CA%d: Unexpected end of dimensioning array", __LINE__);

        /* skip double braces */
        if (p->op == flavor::Parser::token::DBL_BRACE_L)
            dim_skip[i]=1;
        else {
            dim_skip[i] = 0;
            if (p->op != '[') 
                fatal("Internal error CA%d: Expected array expresion", __LINE__);
        }
        if (p->left.np == NULL)
            fatal("Internal error CA%d: Array expression without value", __LINE__);
        /* invalid array size type has already been reported, as well as use of non-literals */
        if (p->left.np->op != flavor::Parser::token::INT_LITERAL)
            goto c_array_unroll_end;
        dim_values[i] = p->left.np->left.ival;
    }
    
    /* create our running counters array */
    dim_cnt = (int *)getmem(lp->dims*sizeof(int));

    /* finally, create list of declarations, one element at a time */
    ret = c_array_unroll_rec(lp, dim_values, dim_cnt, dim_skip, 0, NULL);

c_array_unroll_end:
    free(dim_values);
    free(dim_skip);
    free(dim_cnt);
    return ret;
}

/* get a particular element from an array initializer */
node_t *c_array_get_init(symbol_t *sp, int *dim_values, int *dim_idx, node_t *ip)
{
    int i, j;

    if (ip->op != flavor::Parser::token::ARRAY) return NULL;

    /* position after the topmost ARRAY node */
    ip = ip->left.np;
    for (i=0; i<sp->dims; i++) {
        for (j=0; j<dim_values[i]-dim_idx[i]-1; j++) {
            if (ip->left.np == NULL) 
                fatal("Internal error CA%d: Unexpected end of array expression", __LINE__);
            ip = ip->left.np;
        }
        /* if we hit a comma, go right */
        if (ip->op == ',')
            ip = ip->right.np;
        /* if we are at an array go left */
        if (ip->op == flavor::Parser::token::ARRAY)
            ip = ip->left.np;
        /* check again for a leaf */
        if (ip->leaf)
            break;
    }
    if (!ip->leaf) 
        fatal("Internal error CA%d: Failed to locate entry in array initialization expression", __LINE__);

    return ip;
}
