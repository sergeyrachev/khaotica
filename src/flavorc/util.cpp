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

/* util.c */

#include <stdio.h>
#include <string.h>

#include "port.h"
#include "util.h"
#include "error.h"
#include "symbol.h"
#include "ptree.h"
#include "globals.h"
#include "y.tab.h"

/* malloc with error control */
void *getmem(int size)
{
    void *x = (void *)malloc(size);
    if (x == NULL) fatal("Cannot allocate memory (%d bytes)", size);
    return x;
}

/* realloc with error control */
void *regetmem(void *buf, int size)
{
    void *x = (void *)realloc(buf, size);
    if (x == NULL) fatal("Cannot allocate memory (%d bytes)", size);
    return x;
}

/* Incrementally build a string.
 * Start with a NULL str and the initial character, and keep calling 
 * 'buildstr' with the previously returned value and the new character.
 */
#define BUILDSTR_SIZE (10)

char *buildstr(char *str, int c)
{
    int size;

    if (str == NULL) {
        /* initialize */
        str = (char *)getmem(BUILDSTR_SIZE);
        str[0] = c;
        str[1] = '\0';
        return str;
    }
    /* previously initialized */
    
    /* get size, including trailing null */
    size = strlen(str)+1;

    /* filled up, expand */
    if ((size % BUILDSTR_SIZE) == 0) str = (char*)regetmem(str, size+BUILDSTR_SIZE);
   
    str[size-1] = c;
    str[size] = '\0';
    return str;
}

/* Incrementally build an array of non-NULL pointers.
 * You start with a NULL p and the initial pointer, and you keep calling 
 * 'buildarray' with the previously returned value and the new pointer.
 */
#define BUILDARRAY_SIZE (10)

void **buildarray(void **p, void *n)
{
    int size;

    if (n == NULL) fatal("Internal error U%d: Invalid argument", __LINE__);

    if (p == NULL) {
        /* initialize */
        p = (void **)getmem(BUILDARRAY_SIZE*sizeof(void *));
        p[0] = n;
        p[1] = NULL;
        return p;
    }
    /* previously initialized */
    
    /* get size, including trailing null */
    size = 0;
    while (p[size++] != NULL);

    if ((size % BUILDARRAY_SIZE) == 0) {
        /* filled up, expand */
        p = (void**)regetmem(p, (size+BUILDARRAY_SIZE)*sizeof(void *));
    }
    p[size-1] = n;
    p[size] = NULL;
    return p;
}

/* returns true if id is a literal */
int is_literal(int id)
{
    if (id == flavor::Parser::token::BIT_LITERAL || id == flavor::Parser::token::INT_LITERAL ||  id == flavor::Parser::token::DOUBLE_LITERAL || id == flavor::Parser::token::STRING_LITERAL) return 1;
    else return 0;
}

/* convert a node_t from bitstring literal to int literal */
node_t *n_bit2int(node_t *n)
{
    bit_t *sp;

    if (n->op != flavor::Parser::token::BIT_LITERAL) fatal("Internal error U%d: argument is not a bitstring literal", __LINE__);
    sp = n->left.bit;
    n->op = flavor::Parser::token::INT_LITERAL;
    n->type = flavor::Parser::token::INT;
    n->ptype = s_lookup("int");
    n->left.ival = sp->val;
    
    /* bitstring no longer needed */
    free(sp);

    return n;
}

/* get the length of floats/doubles/long doubles (if long, 'long_set' is set) */
int get_double_size(int type, int long_set)
{
    switch (type) {
    case flavor::Parser::token::FLOAT:
        return sizeof(float)*8;

    case flavor::Parser::token::DOUBLE:
        if (long_set) return sizeof(long double)*8;
        else return sizeof(double)*8;

    default:
        fatal("Internal error U%d: unknown type", __LINE__);
        /* not reached */
        return 0;
    }
}

/* convert a bit value to a string (allocates new memory) */
char *bit2str(bit_t *b)
{
    char *ret;
    int i;

    if (b == NULL || b->len < 0 || b->len > 32) fatal("Internal error U%d: Invalid bit literal", __LINE__);
    ret = (char *)getmem(35); /* 32 bits + '0b' + nul */
    ret[0] = '0';
    ret[1] = 'b';
    for (i=0; i<b->len; i++) {
        if ((b->val >> i) % 2) ret[b->len-i+1] = '1';
        else ret[b->len-i+1] = '0';
    }
    ret[i+2] = '\0';
    return ret;
}

char *node2string(node_t* n) 
{
	char *ret = NULL;
	char temp[1024];
	switch(n->type) {
	case flavor::Parser::token::CHAR:
		temp[0] = (char)n->left.ival;
		temp[1] = '\0';
		break;
	case flavor::Parser::token::BIT:
		ret = bit2str(n->left.bit);
		break;
	case flavor::Parser::token::INT:
		sprintf(temp, "%d", n->left.ival);
		break;
	case flavor::Parser::token::FLOAT:
	case flavor::Parser::token::DOUBLE:
		sprintf(temp, "%f", n->left.dval);
		break;
	default:
		return NULL;
	}
	ret = (char*)getmem(strlen(temp)+1);
	return strcpy(ret, temp);
}

/* find the statements of a particular class */
list_t *find_class_stmts(symbol_t *sp)
{
    list_t *p = parsed_code;
    while (p != NULL) {
        if (p->sp == sp) return p->sub1;
        p = p->next;
    }
    return NULL;
}

/* check if a file name is absolute */
int isabsolute(char *path)
{
    if (path[0] == '/' || path[0] == '\\' || (strlen(path)>0 && path[1] == ':'))
        return 1;
    else
        return 0;
}

/* Get basename from file name.
 * We support both Unix-style and MS-style; note that people with shells on NT
 * can use Unix-style paths, so we can't have it platform dependent.
 */
char *basename(char *path) throw ()
{
    char *u1, *u2;
    
    u1 = strrchr(path, '/');
    u2 = strrchr(path, '\\');
    if (u1 > u2) return u1+1;
    else if (u2 > u1) return u2+1;
    else return path; /* both are NULL */
}

/* Get directory from file name.
 * We support both Unix-style and MS-style; note that people with shells on NT
 * can use Unix-style paths, so we can't have it platform dependent.
 */
char *dirname(char *path) 
{
    static char tmp[MAX_LEN];
    char *u1, *u2;

    strcpy(tmp, path);
    u1 = strrchr(tmp, '/');
    u2 = strrchr(tmp, '\\');
    if (u1 > u2) *(u1+1) = '\0';
    else if (u2 > u1) *(u2+1) = '\0';
    else tmp[0] = '\0'; /* both are NULL */
    return tmp;
}

/* create .h include file name from .fl include statement */
char *gethname(char *flname)
{
    char *hname;
    int len_hname;

    hname = (char *)getmem(MAX_LEN);

    /* absolute or relative path? */
    if (!isabsolute(flname)) {
        /* get dirname of current file, and append the new file name (may include relative path) */
        strcpy(hname, dirname(filename));
        strcat(hname, flname);
    }
    else strcpy(hname, flname);

    /* change suffic to .h */
    len_hname = strlen(hname);

    /* is it long enough? */
    if (len_hname > 3) {
        hname[len_hname-3] = '\0';
        strcat(hname, ".h");
    }
    return hname;
}

int is_verbatim(int type) {
	if (flavor::Parser::token::VERBATIM <= type && type <= flavor::Parser::token::VERBATIM_BOTH_J) return 1;
	else return 0;
	
}
