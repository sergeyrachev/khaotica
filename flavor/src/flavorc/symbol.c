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

/* symbol.c */

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "port.h"
#include "symbol.h"
#include "ptree.h"
#include "util.h"
#include "error.h"

#include "y.tab.h"

/*
 * Reserved words and types
 */

/* type for initializing symbol table with reserved words and types */
typedef struct {
    char *name;
    int token;
} reserved_t;

static reserved_t Reserved[]={
    /* reserved keywords - 0 means not allowed */
    "struct",	    0,
    "typedef",	    0,
    "class",        CLASS,
    "if",           IF,
    "else",         ELSE,
    "do",           DO,
    "while",        WHILE,
    "for",          FOR,
    "goto",         GOTO,
    "continue",     CONTINUE,
    "break",        BREAK,
    "return",       0,
    "repeat",       REPEAT,
    "map",          MAP,
    "extends",      EXTENDS,
    "switch",       SWITCH,
    "case",         CASE,
    "default",      DEFAULT,
    "bac",          BAC,
    /* access modifiers */
    "public",       0,
    "protected",    0,
    "private",      0,
    "package",      0,
    /* modifiers */
    "aligned",      ALIGNED,
    "unsigned",     UNSIGNED,
    "signed",       SIGNED,
    "short",        SHORT,
    "long",         LONG,
    "const",        CONST,
    "abstract",     ABSTRACT,
    "static",       0,
    "extern",       0,
    "inline",       0,
	"little",	    LITTLE,
	"big",		    BIG,
	/* built-in functions */
    "lengthof",	    LENGTHOF,
    "isidof",       ISIDOF,
    "skipbits",     SKIPBITS,
    "nextbits",     NEXTBITS,
    "numbits",      NUMBITS,
    "nextcode",     NEXTCODE,
    /* end */
    (char *)NULL,   0
};

/* these are treated as user-defined types */
static reserved_t Reserved_Exported[]={
    /* built-in types -- 0 means not allowed */
    "void",         0,
    "bit",          BIT,
    "char",         CHAR,
    "int",          INT,
    "float",        FLOAT,
    "double",       DOUBLE,
    /* end */
    (char *)NULL,   0
};

/* these are the pragma keywords */
static reserved_t Reserved_Pragma[]={
    /* reserved keywords -- 0 means not allowed */
    "get",	        PRAGMA_GET,
    "noget",	    PRAGMA_NOGET,
    "put",	        PRAGMA_PUT,
    "noput",	    PRAGMA_NOPUT,
    "putxml",       PRAGMA_PUTXML,
    "noputxml",	    PRAGMA_NOPUTXML,
    "trace",	    PRAGMA_TRACE,
    "notrace",	    PRAGMA_NOTRACE,
    "line",	        PRAGMA_LINE,
    "noline",	    PRAGMA_NOLINE,
    "array",	    PRAGMA_ARRAY,
    "nullstrings",  PRAGMA_NULLSTRINGS,
    "nonullstrings",PRAGMA_NONULLSTRINGS,
    "includes",     PRAGMA_INCLUDES,
    "noincludes",   PRAGMA_NOINCLUDES,
    "bitstream",    PRAGMA_BITSTREAM,
    "prefix",	    PRAGMA_PREFIX,
    "efunc",	    PRAGMA_ERROR_FUNC,
    "tfunc",	    PRAGMA_TRACE_FUNC,
    /*
    "public",       PRAGMA_PUBLIC,
    "private",      PRAGMA_PRIVATE,
    "protected",    PRAGMA_PROTECTED,
    "package",      PRAGMA_PACKAGE, 
    */
    /* end */
    (char *)NULL,   0
};

/* these are the BAC keywords */
static reserved_t Reserved_BAC[]={
    /* reserved keywords -- 0 means not allowed */
    "prec",	        BAC_PREC,
    "ooc",	        BAC_OOC,
    "soc",	        BAC_SOC,
    "te",	        BAC_TE,
    "norm",         BAC_RENORM,
    "bs",	        BAC_BS,
    "init",	        BAC_INIT,
    "end",	        BAC_END,
    "rtable",	    BAC_RTABLE,
    "nexti",	    BAC_NEXTI,
    "trans",        BAC_TRANS,
    "exch",         BAC_EXCH,
    /* end */
    (char *)NULL,   0
};

/*
 * Symbol tables
 */

/* top of contexts stack (a doubly linked list) */
static context_t *s_cxt_top = NULL;

/* currently active context */
static context_t *s_cxt = NULL;

/* pragma context (only one, statically defined) */
static context_t s_pragma_cxt;

/* BAC context (only one, statically defined) */
static context_t s_bac_cxt;

/* currently defined class (or map) */
static symbol_t *s_cur_class = NULL;

/* P. J. Weinberger's hash function
 * See:
 *    - A. Aho et al., "Compilers, Principles, Techniques, and Tools",
 *      Addison Wesley, 1986.
 *
 *    - A. I. Holub, "Compiler Design in C", Prentice Hall, 1990.
 *    
 *    - MKS Lex and Yacc Version Reference Manual, 1993.
 */
#define NUM_BITS	    (sizeof(unsigned) * 8)
#define THREE_FOURTHS	(NUM_BITS * 3 / 4)
#define ONE_EIGHTH	    (NUM_BITS / 8)
#define HIGH_BITS	    (~((unsigned)~0>>ONE_EIGHTH))

static int s_hash(char *s)
{
    unsigned hash, tmp;
    for (hash=0; *s; s++) {
        hash = (hash<<ONE_EIGHTH)+*s;
        if ((tmp = hash & HIGH_BITS) != 0)
            hash = (hash^(tmp>>THREE_FOURTHS)) & ~HIGH_BITS;
    }
    return hash % ST_SIZE;
}

/* eliminate a symbol table entry and all its following links (don't destroy symbols) */
static void s_destroy_entry(symlink_t *slp)
{
    symlink_t *p, *next;

    p = slp;
    while (p != NULL) {
        next = p->next;
        /* note: we don't destroy the symbol, only the symlink */
        free(p);
        p = next;
    }
}

/* eliminate entire context */
void s_destroy_context(context_t *c) 
{
    int i;

    if (c == NULL) return;
    /* sanity check; don't remove global (top) context */
    if (c->prev == NULL || c->level == 0) 
        fatal("Internal error S%d: Attempted to remove global context", __LINE__);
    for (i=0; i<ST_SIZE; i++) {
        s_destroy_entry(c->st[i]);
        c->st[i] = NULL;
    }
}

/* add a new context of a given type */
context_t *s_newcontext(int type)
{
    context_t *c;

    /* create new context */
    c = (context_t *)getmem(sizeof(context_t));
    memset(c, 0, sizeof(context_t));
    c->type = type;

    /* link */
    c->next = NULL;
    c->prev = s_cxt;
    s_cxt->next = c;

    /* set level */
    c->level = c->prev->level+1;

    /* mark as currently active one */
    s_cxt = c;

    return s_cxt;
}

/* insert (append) given context */
void s_inscontext(context_t *cxt)
{
    s_cxt->next = cxt;
    cxt->prev = s_cxt;
    cxt->next = NULL;
    cxt->level = s_cxt->level+1;
    s_cxt = cxt;
}

/* remove the most recent context (but don't delete) */
void s_remcontext(void)
{
    context_t *c = s_cxt->prev;
    if (c != NULL) {
        c->next = NULL;
        s_cxt = c;
    }
}

/* get current context */
context_t *s_get_cur_cxt(void) { return s_cxt; } 

/* get current context level */
int s_get_cur_cxt_level(void) { return s_cxt->level; }

/* get currently defined class, if any */
symbol_t *s_get_cur_class(void) { return s_cur_class; }

/* set currently defined class, if any */
symbol_t *s_set_cur_class(symbol_t *c) { return s_cur_class = c; }

/* check if we are in class context */
int s_in_class(void)
{
    if (s_cxt_top == NULL || s_cxt_top->next == NULL || s_cxt_top->next->type != CLASS) return 0;
    else return 1;
}

/* copy symbol table (target assumed empty) */
void s_copy_st(context_t *src, context_t *tgt)
{
    int i;
    symlink_t *srcp, *tgtp, *n;

    for (i=0; i<ST_SIZE; i++) {
        /* sanity check */
        if (tgt->st[i] != NULL) 
            fatal("Internal error S%d: Non-empty symbol table entry", __LINE__);
        /* if src is empty, nothing to copy */
        if (src->st[i] == NULL) continue;
        /* build the head of the list and anchor it */
        srcp = src->st[i];
        tgtp = (symlink_t *)getmem(sizeof(symlink_t));
        tgtp->sp = srcp->sp;
        tgtp->next = NULL;
        tgt->st[i] = tgtp;
        /* copy remaining list nodes, if any */
        srcp=srcp->next;
        while (srcp != NULL) {
            n = (symlink_t *)getmem(sizeof(symlink_t));
            n->sp = srcp->sp;
            n->next = NULL;
            tgtp->next = n;
            srcp = srcp->next;
            tgtp = tgtp->next;
        }
        tgtp->next = NULL;
    }
}

/* create a new symbol */
symbol_t *s_create(char *name, int type)
{
    symbol_t *sp = (symbol_t *)getmem(sizeof(symbol_t));
    sp->name = (char *)getmem(strlen(name)+1);
    sp->lineno = lineno;
    strcpy(sp->name, name);
    sp->ident = type;
    sp->internal = 0;
    sp->exported = 1;
    sp->owner = s_get_cur_class();
    sp->declared = 0;
    sp->declared_get = 0;
    sp->declared_put = 0;
    sp->declared_putxml = 0;
    sp->declared_xsd = 0;
    sp->put_needs_parse_var = 0;
    sp->get_needs_parse_var = 0;
    sp->get_needs_code_var = 0;
    sp->dimx_end_var = NULL;
    sp->parsable = 0;
    sp->parse = NULL;
    sp->align = NULL;
    sp->id = NULL;
    sp->range = 0;
    sp->accessed = 1;
    sp->assigned = 0;
    sp->ptype = NULL;
    sp->forward = 0;
    sp->forwardref = NULL;
    sp->abstract = 0;
    sp->init = NULL;
    sp->modifiers = M_NONE;
    sp->param = NULL;
    sp->dims = 0;
    sp->max_dims = 0;
    sp->dim = NULL;
    sp->derived = NULL;
	sp->lengthof = 0;
    sp->plengthof = NULL;
    sp->isidof = 0;
    sp->escape = 0;
    sp->isparam = 0;
    sp->included = including();   
    return sp;
}

/* add a symbol to the current context */
symbol_t *s_add(char *name, int type)
{
    int hash;
    symbol_t *sp;
    symlink_t *slp;
    context_t *cp;
    
    /* create new */
    sp = s_create(name, type);
    
    /* create symlink and assigned symbol */
    slp = (symlink_t *)getmem(sizeof(symlink_t));
    slp->sp = sp;

    /* link to current context; if context has level 3, place in class context (level 1) */
    hash = s_hash(name);
    if (s_cxt->level == 3) cp = s_cxt_top->next;
    else cp = s_cxt;
    slp->next = cp->st[hash];
    cp->st[hash] = slp;
    /* set context level of this variable */
    sp->cxt_level = cp->level;
    sp->cxt = cp;
    
    /* if we add a class type, mark currently defined class */
    if (type == CLASS_TYPE) s_cur_class = sp;
    
    return sp;
}

/* transfer a symbol to class (1st level) context */
symbol_t *s_transfer_class(symbol_t *sp) { return s_transfer(sp, s_cxt_top->next); }

/* transfer a symbol table entry from the current to a new context */
symbol_t *s_transfer(symbol_t *sp, context_t *tgt)
{
    int hash;
    symlink_t *slp=NULL, *prev;
    context_t *cp;

    hash = s_hash(sp->name);
    
    /* find context */
    cp = s_lookup_cxt(sp);
    if (cp == NULL) fatal("Internal error S%d: Symbol lookup failed during transfer", __LINE__);
    
    /* take out from current context */
    if (cp->st[hash] != NULL && cp->st[hash]->sp == sp) {
        slp = cp->st[hash];
        cp->st[hash] = cp->st[hash]->next;
    }
    else {
        prev = cp->st[hash];
        while (prev->next != NULL) {
            if (prev->next->sp == sp) {
                slp = prev->next;
                prev->next = prev->next->next;
                break;
            }
            prev = prev->next;
        }
        if (slp == NULL) fatal("Internal error S%d: Symbol transfer to new context failed", __LINE__);
    }
    
    /* add to new context */
    slp->next = tgt->st[hash];
    tgt->st[hash] = slp;
    sp->cxt_level = tgt->level;
    
    return sp;
}

/* mark the currently defined class as parsable */
void s_mark_class_parsable() { if (s_cur_class != NULL) s_cur_class->parsable = 1; }

/* lookup the context a symbol belongs to */
context_t *s_lookup_cxt(symbol_t *symbol)
{
    int hash;
    context_t *cp;
    symlink_t *slp;

    if (symbol == NULL) return NULL;

    hash=s_hash(symbol->name);

    cp = s_cxt;
    while (cp != NULL) {
        for (slp=cp->st[hash]; slp!=NULL; slp=slp->next)
            if (slp->sp == symbol) return cp;
        cp = cp->prev;
    }

    /* not found */
    return NULL;

}

/* lookup a symbol */
symbol_t *s_lookup(char *name)
{
    int hash;
    symlink_t *slp;
    context_t *c;

    hash = s_hash(name);

    c = s_cxt;
    while (c != NULL) {
        for (slp=c->st[hash]; slp!=NULL; slp=slp->next)
            if (strcmp(slp->sp->name, name) == 0) {
                slp->sp->accessed++;
                return slp->sp;
            }
        c = c->prev;
    }

    /* not found */
    return NULL;
}

/* Lookup a symbol; add if not already there.
 * Searches all contexts, and adds to most recent if the symbol is not found.
 */
symbol_t *s_lookupadd(char *name, int type)
{
    symbol_t *sp;

    if ((sp = s_lookup(name)) != NULL) return sp;
    else return s_add(name, type);
}

/* add a symbol to the current context (used only for installing reserved keywords by s_init) */
static symbol_t *s_init_add(char *name, int type, int internal, int export)
{
    symbol_t *sp = s_add(name, type);

    /* set flags */
    sp->internal = internal;
    sp->exported = export;

    return sp;
}

/* lookup a symbol in the pragma context */
symbol_t *s_pragma_lookup(char *name)
{
    int hash;
    symlink_t *slp;
    
    hash = s_hash(name);
    for (slp=s_pragma_cxt.st[hash]; slp!=NULL; slp=slp->next)
        if (strcmp(slp->sp->name, name) == 0) return slp->sp;
        
    /* not found */
    return NULL;
}

/* add a symbol to the pragma context (used only for installing pragma keywords by s_init) */
static symbol_t *s_init_pragma_add(char *name, int type)
{
    symbol_t *sp;
    symlink_t *slp;
    int hash;
    
    /* create new */
    sp = s_create(name, type);
    /* create symlink */
    slp = (symlink_t *)getmem(sizeof(symlink_t));
    slp->sp = sp;

    /* link to pragma context */
    hash = s_hash(name);
    slp->next = s_pragma_cxt.st[hash];
    s_pragma_cxt.st[hash] = slp;
    
    /* set flags (treat as internal) */
    sp->internal = 1;
    sp->exported = 0;
    
    return sp;
}

/* lookup a symbol in the BAC context */
symbol_t *s_bac_lookup(char *name)
{
    int hash;
    symlink_t *slp;
    
    hash = s_hash(name);
    for (slp=s_bac_cxt.st[hash]; slp!=NULL; slp=slp->next)
        if (strcmp(slp->sp->name, name) == 0) return slp->sp;
        
    /* not found */
    return NULL;
}

/* add a symbol to the BAC context (used only for installing BAC keywords by s_init) */
static symbol_t *s_init_bac_add(char *name, int type)
{
    symbol_t *sp;
    symlink_t *slp;
    int hash;
    
    /* create new */
    sp = s_create(name, type);
    /* create symlink */
    slp = (symlink_t *)getmem(sizeof(symlink_t));
    slp->sp = sp;

    /* link to pragma context */
    hash = s_hash(name);
    slp->next = s_bac_cxt.st[hash];
    s_bac_cxt.st[hash] = slp;
    
    /* set flags (treat as internal) */
    sp->internal = 1;
    sp->exported = 0;
    
    return sp;
}

/* initialize contexts stack */
void s_init(void)
{
    reserved_t *pr;
    context_t *c, *n;

    /* initialize contexts stack */
    c = s_cxt_top;

    /* clean-up old contents */
    while (c != NULL) {
        n = c->next;
        s_destroy_context(c);
        c = n;
    }

    /* create initial (global) context */
    s_cxt_top = (context_t *)getmem(sizeof(context_t));
    memset(s_cxt_top, 0, sizeof(context_t));

    /* set as the currently active one */
    s_cxt = s_cxt_top;

    /* enter reserved keywords and built-in types */
    for (pr=Reserved; pr->name!=NULL; pr++)
        s_init_add(pr->name, pr->token, 1, 0);
    for (pr=Reserved_Exported; pr->name!=NULL; pr++)
        s_init_add(pr->name, pr->token, 1, 1);
    
    /* enter keywords in pragma context */
    for (pr=Reserved_Pragma; pr->name!=NULL; pr++)
        s_init_pragma_add(pr->name, pr->token);

    /* enter keywords in BAC context */
    for (pr=Reserved_BAC; pr->name!=NULL; pr++)
        s_init_bac_add(pr->name, pr->token);
}

/* get modifier constant from modifier token value */
unsigned int s_tok2mod(int mod)
{
    switch (mod) {
    case UNSIGNED:  return M_UNSIGNED;
    case SIGNED:    return M_SIGNED;
    case SHORT:     return M_SHORT;
    case LONG:      return M_LONG;
    case CONST:     return M_CONST;
    case STATIC:    return M_STATIC;
 	case LITTLE:    return M_LITTLE;
 	case BIG:       return M_BIG;
    default:
        fatal("Internal error S%d: Unknown modifier token value", __LINE__);
        return -1;
    }
}

/* get modifier token value from modifier constant */
int s_mod2tok(unsigned int mod)
{
  switch (mod) {
    case M_UNSIGNED:    return UNSIGNED;
    case M_SIGNED:      return SIGNED;
    case M_SHORT:       return SHORT;
    case M_LONG:        return LONG;
    case M_CONST:       return CONST;
	case M_STATIC:      return STATIC;
	case M_LITTLE:      return LITTLE;
	case M_BIG:         return BIG;
    default:
        fatal("Internal error S%d: Unknown modifier constant", __LINE__);
        return -1;
    }
}

/* get token string from token value */
const char *s_tok2str(int tok)
{
    reserved_t *r = Reserved;
    while (r->name != NULL) {
        if (r->token == tok) return r->name;
        r++;
    }

    r = Reserved_Exported;
    while (r->name != NULL) {
        if (r->token == tok) return r->name;
        r++;
    }

    fatal("Internal error S%d: Unknown token value %d", __LINE__, tok);
    return NULL;
}

/* generate a modifiers string from symbol modifiers (note: uses static memory) */
const char *s_mods2str(unsigned int mod)
{
    static char s[256];
    
    if (mod == M_NONE) {
         s[0] = '\0';
         return s;
    }
    else s[0] = '\0';
    
    if (cpp()) {
        if (mod & M_CONST) strcat(s, "const ");
        if (mod & M_STATIC) strcat(s, "static ");
        if (mod & M_UNSIGNED) strcat(s, "unsigned ");
        if (mod & M_SIGNED) strcat(s, "signed ");
        if (mod & M_LONG) strcat(s, "long ");
        if (mod & M_SHORT) strcat(s, "short ");
    }
    else if (java()) {
        if (mod & M_CONST) strcat(s, "final ");
        if (mod & M_STATIC) strcat(s, "static ");
    }
    
    return s;
}

/* generate a string from operator token (uses static memory) */
const char *s_op2str(int op)
{
    static char s[2];

    /* single letter operators are coded as their ASCII codes; only need to handle two-letter ones */
    switch (op) {        
    case LS:         return "<<";
    case RS:         return (cpp() ? ">>" : ">>>");
    case GE:         return ">=";
    case LE:         return "<=";
    case NE:         return "!=";
    case EQ:         return "==";
    case OR:         return "||";
    case AND:        return "&&";
    case INC:        return "++";
    case DEC:        return "--";
    case MUL_ASSIGN: return "*=";
    case DIV_ASSIGN: return "/=";
    case MOD_ASSIGN: return "%=";
    case ADD_ASSIGN: return "+=";
    case SUB_ASSIGN: return "-=";
    case LS_ASSIGN:  return "<<=";
    case RS_ASSIGN:  return (cpp() ? ">>=" : ">>>=");
    case AND_ASSIGN: return "&=";
    case OR_ASSIGN:  return "|=";
    case XOR_ASSIGN: return "^=";
    default:
        s[0] = op;
        s[1] = '\0';
        return s;
    }
}

/* Flag that a particular dimension uses non-partial array; 
 * This avoids declaration of uneeded dimx_end variables.  
 * TBD: If the array is not parsable and it is not initialized, 
 *      then the dimx_end variable becomes useless.
 */

static const _dimx_flag = 0xFFFFFFFF;

void s_cur_class_dimx_nonpartial(int dim, int val)
{
    int i, size;
    symbol_t *sp = s_get_cur_class();

    if (sp == NULL) return;

    /* make sure end flag is not used accidentally */
    if (val == _dimx_flag) 
        fatal("Internal error S%d: Invalid argument", __LINE__);

    if (sp->dimx_end_var == NULL) {
        sp->dimx_end_var = (int *)getmem((dim+2)*sizeof(int));
        for (i=0; i<dim; i++) 
            sp->dimx_end_var[i] = 0;
        sp->dimx_end_var[dim] = val;
        sp->dimx_end_var[dim+1] = _dimx_flag;
        return;
    }
    /* figure out size (minus trailing flag 0xFFFFFFFF) */
    size = 0;
    while (sp->dimx_end_var[size++] != _dimx_flag);

    if (dim < size-1) {
        if (sp->dimx_end_var[dim] == 0) 
            sp->dimx_end_var[dim] = val;
    }
    else {
        /* increase size */
        sp->dimx_end_var = (int *)regetmem(sp->dimx_end_var, (dim+2)*sizeof(int));
        for (i=size-1; i<dim; i++) 
            sp->dimx_end_var[i] = 0;
        sp->dimx_end_var[dim] = val;
        sp->dimx_end_var[dim+1] = _dimx_flag;
    }
    return;
}
