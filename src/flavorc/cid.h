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

#ifndef _cid_h_
#define _cid_h_


/* check if a class or a base class has an ID; returns the class where an ID was first found */
symbol_t *c_find_id(symbol_t *sp);

/* same as above, but abstract classes are ignored */
symbol_t *c_find_id_notabstract(symbol_t *sp);

/* get the ID of a class */
void c_class_id_get(symbol_t *sp);

/* put the ID of a class */
void c_class_id_put(symbol_t *sp);

/* get the ID of a class and put it in XML form */
void c_class_id_putxml(symbol_t *sp);

/* declare temp variable to hold probed id value and then probe the id value calling the nextbits function */
void c_var_decl_get_class_probe_id_simple(symbol_t *sp);

/* declare temp variable to hold probed id value and then probe the id value calling the nextvlc function */
void c_var_decl_get_class_probe_id_map(symbol_t *sp, int dims);

/* Generate 'if' statements for ID parsing of a variable 'orig' of type class based on its class type 
 *'sp' (used by c_variable_decl_get_class_boot); cnt is the number of comparisons already produced.
 * Returns cnt + number of comparisons produced in this run.
 */
int c_var_decl_get_class_id(symbol_t *orig, int dims, symbol_t *sp, int cnt, int xml);

/* Generate 'if' statements for ID comparison of a variable of type 'sp' (used by c_class_isidof()).
 * Cnt is the number of comparisons already produced.
 * Returns cnt + number of comparisons it produced in this run.
 */
int c_class_isidof_check_id(symbol_t *sp, int cnt);


#endif /* ! _carray_h_ */
