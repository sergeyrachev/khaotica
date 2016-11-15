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

#ifndef _carray_h_
#define _carray_h_


/* generate a tmp variable name for array dimensions (uses static memory) */
const char *c_gentmp_dim(int i);

/* generate array size declaration */
void c_array_decl(symbol_t *sp, int dims, node_t **dim);

/* generate array size declaration using actual sizes */
void c_array_decl_actual(symbol_t *sp, int dims, node_t **dim);

/* generate temp variable declarations for arrays */
void c_array_tmp_decl(int dims, int *dimx_end_var);

/* array prologue ('for' loops) */
void c_array_prologue(symbol_t *sp, int dims, node_t **dim);

/* array proper (the indices with the tmp variables) */
void c_array_proper(symbol_t *sp, int dims);

/* array epilogue ('for' loops) */
void c_array_epilogue(symbol_t *sp, int dims, node_t **dim);

/* unrolls declaration of array initializers - this way no special code is needed in the code generator */
list_t *c_array_unroll(list_t *lp);

/* get a particular element from an array initializer */
node_t *c_array_get_init(symbol_t *sp, int *dim_values, int *dim_idx, node_t *ip);


#endif /* ! _carray_h_ */
