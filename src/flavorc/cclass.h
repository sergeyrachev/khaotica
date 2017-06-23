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

#ifndef _cclass_h_
#define _cclass_h_


/* check if a class is pure abstract (has no non-abstract derived with an ID) */
int c_is_pure_abstract(symbol_t *sp);

/* generate the get part of variable declarations of type class */
void c_var_decl_get_class(symbol_t *sp, node_t *parse, node_t *param, int dims, node_t **dim, int usenext);

/* generate the put part of variable declarations of type class */
void c_var_decl_put_class(symbol_t *sp, node_t *param, node_t *parse, int dims, node_t **dim, int usenext);

/* generate the putxml part of variable declarations of type class */
void c_var_decl_putxml_class(symbol_t *sp, node_t *parse, node_t *param, int dims, node_t **dim, int usenext);

/* generate the corresponding XML schema for variable declarations of type class */
void c_var_decl_xsd_class(symbol_t *sp, node_t *parse, node_t *param, int dims);


#endif /* ! _cclass_h_ */
