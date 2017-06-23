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

#ifndef _util_h_
#define _util_h_

#include "ptree.h"

/* 
 * Utility functions and macros 
 */

/* memory allocation with error reporting */
void *getmem(int size);

/* realloc with error control */
void *regetmem(void *buf, int size);

/* Incrementally build a string.
 * You start with a NULL str and the initial character, and you keep calling 
 * 'buildstr' with the previously returned value and the new character.
 */
char *buildstr(char *str, int c);

/* Incrementally build an array of non-NULL pointers.
 * You start with a NULL p and the initial pointer, and you keep calling 
 * 'buildarray' with the previously returned value and the new pointer.
 */
void **buildarray(void **p, void *n);

/* returns true if id is a literal */
int is_literal(int id);

/* convert a node_t from bitstring literal to int literal */
node_t *n_bit2int(node_t *n);

/* get the length of floats/doubles/long doubles (if long, 'long_set' is set) */
int get_double_size(int type, int long_set);

/* convert a bit value to a string (allocates new memory) */
char *bit2str(bit_t *b);

/* find the statements of a particular class */
list_t *find_class_stmts(symbol_t *sp);

/* check if a file name is absolute */
int isabsolute(char *path);

/* Get basename from file name.
 * We support both Unix-style and MS-style; note that people with shells on NT
 * can use Unix-style paths, so we can't have it platform dependent.
 */
char *basename(char *path);

/* Get directory from file name.
 * We support both Unix-style and MS-style; note that people with shells on NT
 * can use Unix-style paths, so we can't have it platform dependent.
 */
char *dirname(char *path);

/* create .h include file name from .fl include statement */
char *gethname(char *flname);

char *node2string(node_t* n);
int is_verbatim(int type);


#endif /* ! _util_h_ */
