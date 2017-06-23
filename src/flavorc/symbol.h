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

#ifndef _symbol_h_
#define _symbol_h_


/* defined in ptree.h, crosslinked with symbol_t below */
typedef struct _node_t node_t;
typedef struct _list_t list_t;

/* native Flavor types not supported by C/C++/Java */
typedef struct {
    unsigned int val;   /* actual value */
    int len;            /* length */
} bit_t;

/* verbatim section */
typedef struct {
    char *str;  /* the code */
    int lineno; /* the source line it starts at */
} verbatim_t;

/* Symbol Table
 * Note: The symbol table contains all reserved words, as well as  built-in types.
 */

/* variable modifier flags */
typedef enum {
    M_NONE      =0x000,
    M_UNSIGNED  =0x001,
    M_SIGNED    =0x002,
    M_SHORT     =0x004,
    M_LONG      =0x008,
    M_CONST     =0x010,
    M_STATIC    =0x020,
	M_LITTLE	=0x040,
	M_BIG		=0x080,
} modifier_t;

/* forward references */
typedef struct _symbol_t symbol_t;
typedef struct _context_t context_t;
typedef struct _symlink_t symlink_t;

/* symbol table entry type (entered in hashed array of linked lists) */
struct _symbol_t {

    /* name */
    char *name;

    /* line where it was defined */
    int lineno;

    /* Type indication.
     * Member declaration (including methods), parameter in
     * a method or class, reserved word or built-in type
     * (the different types are declared as tokens in the parser).
     */
    int ident;

    /* set to 1 for internal symbol table entries */
    short int internal;

    /* Set if we should treat this node as if it is an external symbol, even if 'internal' is set. 
     * This is used in the lexer (getsymbol()) to decide if it should be passed to the parser as an <sp> or <ival>.
     * Currently all internals except built-in types have it set.
     */
    short int exported;

    /* owning class (or NULL, if global scope var) */
    symbol_t *owner;

    /* for variables, set (by the code generator) if they have been declared in the class methods */
    short int declared;
    short int declared_get;
    short int declared_put;
    short int declared_putxml;
    short int declared_xsd;

    /* set if the symbol is parsable */
    short int parsable;

    /* for constant parsable vars that are not classes, a node_t * to the parse expression */
    node_t *parse;

    /* When a parsable variable has an initial expression, <prefix>parse
     * is used as a tmp variable in the code generator to save the parse
     * size in the put() method. This ensure that the order of evaluation
     * of parse and init expressions is preserved (parse first). When no
     * variable has initial value, the <prefix>parse generates an
     * 'unused variable' warning. The following is a flag that is set
     * when we are generating the class variable declarations, and tells us
     * if the put() method should declare a <prefix>parse.
     * This is only meaningful for class symbols.
     */
    int put_needs_parse_var;

    /* Similarly to 'put_needs_parse_var', when we are tracing, a parsable
     * variable requires that <prefix>parse is defined to save the parse size
     * (so that it can be passed along to the tracing function without
     * evaluating the expression twice).
     */
    int get_needs_parse_var;

    /* When we want to trace vlc codes, we need to save the code value.
     * A <prefic>code variable is used, and is declared when the flag below is set.
     */
    int get_needs_code_var;

    /* Similarly to 'needs_parse_var', when we are parsing arrays we need
     * tmp variables. Since we don't know the number of dimensions, we
     * use this variable that is set when we are generating class variable
     * declarations to create the right number of them.
     */
    int max_dims;

    /* To avoid declaration of unused variables in arrays (the dimx_end)
     * we keep an array of flags that tell us if dimension x has at least one
     * non-partial use (so it needs the dimx_end var).
     */
    int *dimx_end_var;

    /* If a variable declaration, pointer to type definition (includes built-in types); 
     * if a class declaration, pointer to base class (if any).
     */
    symbol_t *ptype;

    /* if a class variable, pointer to its context */
    context_t *cxt;

    /* For all variables, context level they are defined.
     * This is used to figure out in the code generator which
     * are class variables (they are at level 1).
     */
    int cxt_level;

    /* if a class with parameter types, pointer to list with symbol pointers, used only if var is a constant */
    list_t *param;

    /* For classes, this is set to 1 when we see a forward declaration.
     * When we find the actual declaration, it is set to 2.
     * NOTE: it will be ***reset*** in the code generator, after it skips the first encounter of this class. 
     */
    int forward;

    /* Flag to signal an abstract polymorphic class -- i.e., its use triggers a nextbits() in the
     * space of the IDs of only its derived (non-abstract) classes.
     */
    int abstract;

    /* In order to detect cyclic references with forward declarations, the following NULL
     * terminated array of pointers is used to store in a forward-declared class all classes
     * that use it as a member.
     */
    symbol_t **forwardref;

    /* pointer to initial value expression, used only if var is a constant */
    node_t *init;

    /* pointer to align expression, used only if var is a constant and for class ID variables */
    node_t *align;

    /* for class types, pointer to class ID symbol table entry */
    symbol_t *id;
    
    /* for expected range of values */
    short int range;

    /* how many times this symbol has been accessed */
    unsigned int accessed;

    /* if a variable, it it has been assigned to */
    short int assigned;

    /* type modifiers ('const', 'static', etc.); see 'modifier_t' above */
    unsigned int modifiers;

    /* dimensions, if it is an array */
    int dims;

    /* if dims>0, array of ptrs to dimension expressions (node_t *'s), used only for constants */
    node_t **dim;

    /* if a class, NULL-terminated array of classes directly derived from this one */
    symbol_t **derived;

	/* set if lengthof() is used for this variable (set by check_lengthof()) */
	int lengthof;

    /* Set to point to the lengthof variable of this symbol.
     * These variables always shadow their parent variables; they are generated
     * by the parser (eval_builtin) when a 'lengthof' operator is found for them.
     */
    symbol_t *plengthof;

    /* set if isidof() is used for this variable */
    int isidof;

    /* for maps, set if an escape is present (so that 'type(map)*' can be flagged as an error) */
    int escape;

    /* Set if this is a parameter type var (this is used for class parameter vars, to avoid declaring their
     * probe id variable - it is not needed).
     */
    int isparam;

    /* set if symbol declared from included file (no code will be generated) */
    int included;
};

/* linked list of symbols at a given hash position */
struct _symlink_t {
    symbol_t *sp;
    symlink_t *next;
};

/* symbol table size (PJW hashing works best if it is a prime number) */
#define ST_SIZE (257)

/* Context (doubly linked list).
 * Contexts work as follows. Top is the global one where only class decls, 
 * consts, and maps are allowed. First level holds class variables. Second
 * level holds parameter variables for classes. Third level is always empty.
 * This is because the variables that would go there, are put in the 1st 
 * level (class variables). Fourth and so on contain local scope variables
 * (defined within blocks). Note that all parsable variables have class
 * scope (i.e., are placed in the 1st level context).
 */
struct _context_t {
    
    /* context type; CLASS or regular context (blocks etc.) */
    int type;       
    
    /* context level (how deep we are) */
    int level;

    /* the symbol table */
    symlink_t *st[ST_SIZE];

    /* forward/backward pointers */
    context_t *prev;
    context_t *next;
};

/*
 * Symbol table management functions
 */

/* initialize contexts stack */
void s_init(void);

/* create new context */
context_t *s_newcontext(int type);

/* insert (append) given context */
void s_inscontext(context_t *cxt);

/* remove most recent context (but don't delete) */
void s_remcontext(void);

/* get current context */
context_t *s_get_cur_cxt(void);

/* get current context level */
int s_get_cur_cxt_level(void);

/* check if we in class context */
int s_in_class(void);

/* get currently defined class, if any */
symbol_t *s_get_cur_class(void);

/* set currently defined class, if any */
symbol_t *s_set_cur_class(symbol_t *c);

/* copy symbol table (target assumed empty) */
void s_copy_st(context_t *src, context_t *tgt);

/* create a new symbol */
symbol_t *s_create(char *name, int type);

/* add a symbol to the current context */
symbol_t *s_add(char *name, int type);

/* transfer a symbol to class (1st level) context */
symbol_t *s_transfer_class(symbol_t *sp);

/* transfer a symbol table entry from the current to a new context */
symbol_t *s_transfer(symbol_t *sp, context_t *tgt);

/* mark the currently defined class as parsable */
void s_mark_class_parsable();

/* lookup the context a symbol belongs to */
context_t *s_lookup_cxt(symbol_t *symbol);

/* lookup a symbol */
symbol_t *s_lookup(char *name);

/* lookup a symbol in the pragma context */
symbol_t *s_pragma_lookup(char *name);

/* lookup a symbol in the BAC context */
symbol_t *s_bac_lookup(char *name);

/* lookup a symbol; add if not already there searches all contexts, and adds to most recent if the symbol is not found */
symbol_t *s_lookupadd(char *name, int type);

/* get modifier constant from modifier token value */
unsigned int s_tok2mod(int mod);

/* get modifier token value from modifier constant */
int s_mod2tok(unsigned int mod);

/* get token string from token value */
const char *s_tok2str(int tok);

/* generate a modifiers string from symbol modifiers (note: uses static memory) */
const char *s_mods2str(unsigned int mod);

/* generate a string from operator token (uses static memory) */
const char *s_op2str(int op);

/* Flag that a particular dimension uses non-partial array; this is used in classes
 * to avoid declaration of uneeded dimx_end variables.
 */
void s_cur_class_dimx_nonpartial(int dim, int val);


#endif /* ! _symbol_h_ */
