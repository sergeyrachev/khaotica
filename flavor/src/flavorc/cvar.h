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

#ifndef _cvar_h_
#define _cvar_h_


/* generate variable declarations/definitions */
void c_var_decl(symbol_t *sp, int dims, node_t **dim);

/* generate external variable declarations for C++ */
void c_var_decl_ext(symbol_t *sp, int dims, node_t **dim);

/* generate variable declarations for those vars that have class or deeper scope */
void c_var_decl_inclass(symbol_t *sp, node_t *init, node_t *parse, int dims, node_t **dim);

/* generate variable declarations/definitions (get() part) */
void c_var_decl_get(symbol_t *sp, node_t *init, node_t *parse, node_t *param, node_t *align, int dims, node_t **dim, int usenext);

/* generate variable declarations/definitions (put() part) */
void c_var_decl_put(symbol_t *sp, node_t *init, node_t *parse, node_t *param, node_t *align, int dims, node_t **dim, int usenext);

/* generate variable declarations/definitions (putxml() part) */
void c_var_decl_putxml(symbol_t *sp, node_t *init, node_t *parse, node_t *param, node_t *align, int dims, node_t **dim, int usenext);

/* generate variable declarations/definitions (xsd() part) */
void c_var_decl_xsd(symbol_t *sp, node_t *init, node_t *parse, node_t *param, node_t *align, int dims, node_t **dim, int usenext);


#endif /* ! _cvar_h_ */
