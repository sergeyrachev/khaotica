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

#ifndef _check_h_
#define _check_h_


#include "symbol.h"
#include "ptree.h"

/*
 * Checks for errors that cannot be caught by the grammar
 */

/* checks that lengthof() operator gets meaningful argument */
int check_lengthof(node_t *args);

/* checks that isisof() operator gets meaningful argument */
int check_isidof(symbol_t *c, node_t *args);

/* Return the dominant type between two types (used when constructing arrays defs, to figure out
 * a type spec for the entire array; e.g. {1, 2.0, 3.141592654 } - type should be double.
 */
symbol_t *check_type_promote(symbol_t *type1, symbol_t *type2);

/* check if type2 has type 1 as a base */
int check_type_compat_class(symbol_t *type1, symbol_t *type2);

/* check that type2 can be assigned to type1 */
int check_type_compat(symbol_t *type1, symbol_t *type2);

/* check that a new and current modifier spec make sense (returns sanitized mod) */
unsigned int check_mod(unsigned int mod, unsigned int newmod);

/* check that an expression tree contains only non-parsables */
int check_nonparse(node_t *np);

/* check that a combination of modifier and type makes sense returns sanitized modifiers */
unsigned int check_modtype(unsigned int mod, symbol_t *type);

/* check an assignment operation */
node_t *check_assign(int op, node_t *left, node_t *right);

/* check an expression for unassigned/undeclared variables (second arg is the root of the tree) */
node_t *check_unassigned(node_t *exp, node_t *root);

/* compare parameter lists of a class (declared vs. provided) */
void check_actual_params(symbol_t *c, list_t *def, node_t *given);

/* compare formal parameter lists of two classes (for derived classes) 'c' is the derived class */
void check_formal_params(symbol_t *c, list_t *derived, list_t *base);

/* compare formal parameter lists of forward declaration and definition of a classes */
void check_forward_params(symbol_t *c, list_t *def, list_t *decl);

/* check if an expression is suitable as an operand for INC or DEC operation */
int check_unary(node_t *expr);

/* check that array init fits within parse size */
int check_fit_array(symbol_t *var, node_t *init, node_t *parse);

/* check that init fits within parse size (assumes that type checking on init and parse has been done) */
int check_fit(symbol_t *var, node_t *init, node_t *parse);

/* check that alignment expression is multiple of 8 */
node_t *check_align_arg(node_t *expr);

/* check array index (increase max_array_size if necessary) */
node_t *check_array_index(node_t *expr);

/* check array size (increase max_array_size if necessary) */
node_t *check_array_size(node_t *expr);

/* check that init value for array matches dimensions (the dims *must* be literals) */
int check_array_init(symbol_t *sp, node_t **dim, int dims, node_t *init);

/* check that a list of dimension expr contain no double braces */
int check_no_double_brace(node_t **dim);

/* Check an expression for proper array formation.
 * Places symbol table entry in spp, sets literal if all indices are literals.
 */
int check_array(node_t *expr, symbol_t **spp, int *literal);

/* check a parse expression for type and value */
node_t *check_parse(symbol_t *type, unsigned int mods, node_t *parse);

/* check that float/double/long double gets appropriate fixed parse size */
node_t *check_double_parse(symbol_t *sp, node_t *parse);

/* Check that class ID has a constant value, and that it matches the type
 * and name of that of the base class, if any.
 */
symbol_t *check_id(symbol_t *var);

/* if ID declaration catches the declaration of the ID of the base class, rescope the ID */
symbol_t *check_rescope_id(symbol_t *id);

/*
 * Check cyclic declarations:
 * These can only occur with forward declarations, and can trigger infinite loops
 * of get() or put() calls. To detect them, the following algorithm is used:
 * 1) If a class X has as a member a forward-declared class F, it is added in a list
 *    in F.
 * 2) When we process the definition of a forward-declared class, we check that
 *    a. F does not extend X or a class derived from X.
 *    b. F does not contain a variable of type X, or of a class derived from X.
 */

/* 
 * This is part (a)
 */
int check_cyclic_decl_derived(symbol_t *sp, symbol_t *type, symbol_t **derived);

/*
 * This is part (b)
 */
void check_cyclic_decl_contained(symbol_t *sp, symbol_t **derived);

/* check that a map code and value is unique (not a prefix of another code) if not, returns 0 */
int check_map(symbol_t *map, list_t *codes, list_t *code);

/* check that a class is appropriate for use within a map if not, return 0 */
int check_map_class(symbol_t *sp);

list_t* check_map_simple_value(symbol_t *map, list_t *val);

void check_map_esc_simple_value(symbol_t *map, list_t *val);

list_t* check_map_class_value(symbol_t *map, list_t *val);

void check_map_esc_class_value(symbol_t *map, list_t *val);

/* check BAC param */
int check_bac_param(char *param);

/* check BAC param's value */
void check_bac_val(list_t *lp);


#endif /* ! _check_h_ */
