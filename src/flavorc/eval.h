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

#ifndef _eval_h_
#define _eval_h_


#include "symbol.h"
#include "ptree.h"

/* 
 * Expression evaluation and error check functions
 */

/* eval identifier (for constants) */
node_t *eval_ident(symbol_t *ident);

/* eval a++ and a-- expressions */
node_t *eval_incdec(int op, node_t *expr);

/* eval scoped variable: a '.' b */
node_t *eval_scoped(node_t *left, symbol_t *right);

/* eval unary operator expression (e.g., -expr) */
node_t *eval_unary(int op, node_t *expr);

/* eval multiplicative operator expression (*, /, %) */
node_t *eval_mult(int op, node_t *left, node_t *right);

/* eval additive operator expression (+, -) */
node_t *eval_add(int op, node_t *left, node_t *right);

/* eval shift operator expression (<<, >>) */
node_t *eval_shift(int op, node_t *left, node_t *right);

/* eval relational operator expression (<, >, LE, GE, EQ, NE) */
node_t *eval_rel(int op, node_t *left, node_t *right);

/* eval logical operator expression (&, ^, |, &&, ||) */
node_t *eval_logic(int op, node_t *left, node_t *right);

/* eval conditional operator expression (?:) */
node_t *eval_cond(int op, node_t *cond, node_t *left, node_t *right);

/* eval array operator */
node_t *eval_subarray(node_t *left, node_t *right);

/* eval a unary expression for completed arrays */
node_t *eval_array(node_t *expr);

/* eval lengthof operator */
node_t *eval_lengthof(node_t *args);

/* eval isidof operator */
node_t *eval_isidof(symbol_t *sp, node_t *args);

/* eval skipbits operator */
node_t *eval_skipbits(node_t *args);

/* eval nextbits operator */
node_t *eval_nextbits(node_t *n, unsigned int mod_val, node_t* align_expr);

/* expand a string into an array */
node_t *eval_array_string(char *str);


#endif /* ! _eval_h_ */
