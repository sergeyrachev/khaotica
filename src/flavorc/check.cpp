/*
 * Copyright (c) 1997-2005 Alexandros Eleftheriadis and Danny Hong.
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
 *
 */

/* check.c */

#include <stdio.h>
#include <string.h>

#include "port.h"
#include "util.h"
#include "globals.h"
#include "error.h"

#include "check.h"

#include "y.tab.h"


/*
 * Checks for errors that cannot be caught by the grammar
 */

/* get the lvalue (actual symbol_t *) from an expression */
static symbol_t *get_lvalue(node_t *exp)
{
    /* The only caveat here is arrays and rescoped vars; to find the actual symbol,
     * you have to go all the way to the left when you hit a '[', or to the immediate
     * right when you hit a '.'; this is because of the way parsing is performed.
     * If you hit a unary operator, you just move over it.
     */
    if (exp->op == flavor::Parser::token::VARIABLE) return exp->left.sp;
    if (exp->op == '.') return exp->right.np->left.sp;
    /* recursively look for base of array */
    if (exp->op == '[') return get_lvalue(exp->left.np);
    if (is_literal(exp->op)) return NULL;
    /* we hit a unary operator - go either left or right (only one should be valid) */
    if (exp->left.np != NULL && exp->right.np == NULL) return get_lvalue(exp->left.np);
    else if (exp->left.np == NULL && exp->right.np != NULL) return get_lvalue(exp->right.np);
    /* lvalue not found */
    else if (exp->left.np == NULL && exp->right.np == NULL) return NULL;
    /* fatal("Internal error CH%d: Lvalue contained binary operator", __LINE__); */
    else return NULL;
}

/* checks that lengthof() operator gets meaningful argument */
int check_lengthof(node_t *args)
{
    symbol_t *sp;
    
	/* sanity check */
	if (args == NULL) {
		yyerror("Argument required for lengthof() operator");
		return 0;
	}
    /* find variable, and make sure it is either a class or a parsable var */
	sp = get_lvalue(args);
    if (sp == NULL || sp->ptype == NULL) return 0;

	if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE || sp->parsable) {
		/* set lengthof flag in this variable; if a class, set in class as well */
		sp->lengthof = 1;
        if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE) sp->ptype->lengthof = 1;
		return 1;
	}
	else {
		/* invalid argument */
		yyerror("Operator lengthof() requires a parsable class or variable as argument");
		return 0;
	}
}

/* checks that isidof() operator gets meaningful arguments */
int check_isidof(symbol_t *c, node_t *args)
{
    symbol_t *sp;
    
    if (is_literal(args->op)) return 1;

    /* sanity check */
	if (c == NULL || args == NULL) {
		yyerror("Two arguments required for isidof() operator");
		return 0;
	}
    /* class must have an id */
    if (c->id == NULL) {
        yyerror("Operator isidof() requires a class with an id");
        return 0;
    }

    /* the nextbits function returns integers, so it's ok */
    if (args->op == flavor::Parser::token::NEXTBITS || args->op == flavor::Parser::token::SNEXTBITS || args->op == flavor::Parser::token::LITTLE_NEXTBITS || args->op == flavor::Parser::token::LITTLE_SNEXTBITS)
        return 1;

    /* find variable, and make sure the type is compatible with the id */
	sp = get_lvalue(args);
    if (sp == NULL) return 0;
    if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE || !check_type_compat(c->id->ptype, sp->ptype)) {
        yyerror("Type of argument of isidof() is incompatible with id type of class '%s'", c->name);
        return 0;
    }
    return 1;
}

/* Return the dominant type between two types (used when constructing arrays defs, to figure out
 * a type spec for the entire array; e.g. {1, 2.0, 3.141592654 } - type should be double.
 */
symbol_t *check_type_promote(symbol_t *type1, symbol_t *type2)
{
    if (type1->ident == flavor::Parser::token::CLASS_TYPE) return type1;
    else if (type2->ident == flavor::Parser::token::CLASS_TYPE) return type2;

    /* 'largest' type prevails */
    if (type1->ident>type2->ident) return type1;
    else return type2;
}

/* check if type2 has type 1 as a base */
int check_type_compat_class(symbol_t *type1, symbol_t *type2)
{
    if (type2 == type1) return 1;
    if (type2->ptype != NULL) return check_type_compat_class(type1, type2->ptype);
    else return 0;
}

/* check that type2 can be assigned to type1 */
int check_type_compat(symbol_t *type1, symbol_t *type2)
{
    if (type1->ident == flavor::Parser::token::CLASS_TYPE) {
        /* Assignment to classes not allowed, but this function is also used to check 
         * actual to formal parameter assignment, so here we consider it ok.
         */

        /* check if type2 is derived from type1 */
        return check_type_compat_class(type1, type2);
    }
    /* not a class */
    if (type1->ident < type2->ident) {
        /* incompatible types: class assigned to a plain type, or > int assigned to <= int */
        if (type2->ident == flavor::Parser::token::CLASS_TYPE || (type1->ident <= flavor::Parser::token::INT && type2->ident > flavor::Parser::token::INT)) return 0;
        else return 1;
    }
    return 1;
}

/* check that a new and current modifier spec make sense (returns sanitized mod) */
unsigned int check_mod(unsigned int mod, unsigned int newmod)
{
    if (mod & newmod) yyerror("Duplicate modifier: %s", s_tok2str(s_mod2tok(newmod)));
    if (((mod & M_UNSIGNED) && (newmod & M_SIGNED)) ||
        ((mod & M_SIGNED) && (newmod & M_UNSIGNED))) {
        yyerror("Cannot use both 'signed' and 'unsigned'");
        return mod;
    }
    if (((mod & M_SHORT) && (newmod & M_LONG)) ||
        ((mod & M_LONG) && (newmod & M_SHORT))) {
        yyerror("Cannot use both 'short' and 'long'");
        return mod;
    }
    if (((mod & M_LITTLE) && (newmod & M_BIG)) ||
        ((mod & M_BIG) && (newmod & M_LITTLE))) {
        yyerror("Cannot use both 'little' and 'big'");
        return mod;
    }
    
	return mod | newmod;
}

/* check that a combination of modifier and type makes sense; returns sanitized modifiers */
unsigned int check_modtype(unsigned int mod, symbol_t *type)
{
    unsigned int m = mod;
    
    /* signed/unsigned only valid for INT and CHAR types */
    if (type->ident != flavor::Parser::token::INT && type->ident != flavor::Parser::token::CHAR) {
        if ((m & M_SIGNED) || (m & M_UNSIGNED)) {
            yyerror("Sign specification meaningless for type '%s'", type->name);
            m &= (~(M_SIGNED | M_UNSIGNED));
        }
    }
    
    /* short/long meaningful only for INT and CHAR; long meaningful for double */
    if (type->ident != flavor::Parser::token::INT && type->ident != flavor::Parser::token::CHAR && ((m & M_SHORT) || (m & M_LONG))) {
        if (type->ident != flavor::Parser::token::DOUBLE || !(m & M_LONG)) {
            yyerror("Short/long specification meaningless for type '%s'", type->name);
            m &= (~(M_SHORT | M_LONG));
        }
    }
     
    /* if BIT, treat as unsigned */
    if (type->ident == flavor::Parser::token::BIT) m |= M_UNSIGNED;

    return m;
}

/* check that an expression tree contains only non-parsables */
int check_nonparse(node_t *np)
{
    int left, right;
    
    if (np == NULL) return 0;
    
    if (np->op == flavor::Parser::token::VARIABLE) {
        if (np->left.sp->parsable) return 1;
        else return 0;
    }
    if (!np->leaf) {
        left = check_nonparse(np->left.np);
        right = check_nonparse(np->right.np);
        return left+right;
    }
    else return 0;
}


/* check an expression for unassigned/undeclared variables; second arg is the root of the tree */
node_t *check_unassigned(node_t *exp, node_t *root)
{
    if (exp == NULL) return exp;
    
    if (exp->op == flavor::Parser::token::VARIABLE) {
        if (exp->left.sp == NULL)
            fatal("Internal error CH%d: Variable without a symbol", __LINE__);
        if (exp->left.sp->ptype == NULL) {
            yyerror("## Variable '%s' used before being defined", exp->left.sp->name);
            /* set it as 'int' so that we can continue */
            exp->left.sp->ptype = s_lookup("int");
            exp->type = exp->left.sp->ptype->ident;
            exp->ptype = exp->left.sp->ptype;
            /* also set it as assigned - no reason to report this */
            exp->left.sp->assigned = 1;
            /* also set the root of the expression */
            root->type = exp->left.sp->ptype->ident;
            exp->ptype = exp->left.sp->ptype;
        }
        if (!exp->left.sp->assigned && !exp->left.sp->owner->assigned) 
            yyerror("Variable '%s' used before being assigned", exp->left.sp->name);
    }
    else if (exp->op == flavor::Parser::token::ISIDOF) check_unassigned(exp->right.np, root);
    else if (!is_literal(exp->op)) check_unassigned(exp->left.np, root);

    if (exp->right.np != NULL) check_unassigned(exp->right.np, root);
    if (exp->third.np != NULL) check_unassigned(exp->third.np, root);
    
    return exp;
}

/* Check that an expression is a valid lvalue (used by check_assign()).
 * Returns symbol pointer to lvalue.
 */
static symbol_t *check_lvalue(node_t *exp)
{
    symbol_t *sp;
    
    if (exp == NULL) return NULL;
    
    sp = get_lvalue(exp);
    if (sp == NULL) {
        yyerror("Invalid lvalue (constant or literal)");
        return NULL;
    }
    
    /* set it as assigned */
    sp->assigned = 1;
    
    if (sp->ptype == NULL) {
        yyerror("Variable '%s' assigned before being defined", sp->name);
        /* set it as 'int' so that we can continue */
        sp->ptype = s_lookup("int");
        return sp;
    }
    if (sp->parsable) {
        yyerror("Assignment to parsable variable '%s' not allowed", sp->name);
        return sp;
    }
    if (sp->modifiers & M_CONST) {
        yyerror("Assignment to constant variable '%s' not allowed", sp->name);
        return sp;
    }
    
    return sp;
}

/* check an assignment operation */
node_t* check_assign(int op, node_t* left, node_t* right)
{
    /* check that we have a valid lvalue */
    if (check_lvalue(left) == NULL) goto check_assign_end;
    
    /* assignment between classes not supported */
    if (left->type == flavor::Parser::token::CLASS_TYPE || right->type == flavor::Parser::token::CLASS_TYPE) {
        yyerror("Assignment operator '%s' not supported for classes", s_op2str(op));
        goto check_assign_end;
    }
    
    /* check that operator makes sense for lvalue type */
    if (left->type>flavor::Parser::token::INT) {
        if (op == flavor::Parser::token::MOD_ASSIGN || op == flavor::Parser::token::LS_ASSIGN  || op == flavor::Parser::token::RS_ASSIGN ||
            op == flavor::Parser::token::AND_ASSIGN || op == flavor::Parser::token::XOR_ASSIGN || op == flavor::Parser::token::OR_ASSIGN)
            yyerror("Assignment operator '%s' cannot be applied on type 'double'", s_op2str(op));
    }
    /* check type compatibility between left and right side */
    if (!check_type_compat(left->ptype, right->ptype))
        yyerror("Incompatible types in assignment");
    
check_assign_end:
    return node(op, left, right);
}

/* build a string with i times '[]' (to generate error message below) */
static char *check_build_brackets(int i)
{
    int j;
    char *str = (char *)getmem(i*2+1);
    for (j=0; j<2*i; j+=2) {
        str[j]   = '[';
        str[j+1] = ']';
    }
    str[j] = '\0';
    return str;
}

/* check that dimensions of formal and actual params match */
static void check_actual_param_dims(symbol_t *sp, node_t *p)
{
    /* if the symbol has dimensions, then p must point to a variable that has the same dims */
    if (sp->dims > 0)
        if (p->op != flavor::Parser::token::VARIABLE || p->left.sp->dims != sp->dims)
            yyerror("Formal parameter must have type '%s%s'", sp->ptype->name, check_build_brackets(sp->dims));
        return;
}

/* compare parameter lists of a class (declared vs. provided) */
void check_actual_params(symbol_t *c, list_t *def, node_t *given)
{
    /* def has all symbols; given may have expressions */
    list_t *s = def;
    node_t *p = given;
    
    while (s != NULL) {
        if (p == NULL) {
            yyerror("Incorrect number of parameters for class '%s'", c->name);
            return;
        }
        if (p->op != ',') {
            /* must be a single variable */
            
            /* danny */
            if (p->op == '.') {
                /* check dimensions */
                if (s->sp->dims > 0)
                    check_actual_param_dims(s->sp, p->right.np);
                /* check type compatibility */
                if (!check_type_compat(s->sp->ptype, p->ptype))
                    yyerror("Different types for formal and actual class parameter '%s' ('%s' to '%s')", 
                            s->sp->name, p->ptype->name, s->sp->ptype->name);
            }
            else {
                /* check dimensions */
                if (s->sp->dims > 0) check_actual_param_dims(s->sp, p);
                /* check type compatibility */
                if (!check_type_compat(s->sp->ptype, p->ptype))
                    yyerror("Different types for formal and actual class parameter '%s' ('%s' to '%s')", 
                            s->sp->name, p->ptype->name, s->sp->ptype->name);
            }
            /* no next p */
            p = NULL;
        }
        else {
            /* multiple variables */
            if (p->left.np == NULL) {
                yyerror("Internal error CH%d", __LINE__);
                return;
            }

            /* danny */
            if (p->left.np->op == '.') {
                /* check dimensions */
                if (s->sp->dims > 0) check_actual_param_dims(s->sp, p->left.np->right.np);
                /* check type compatibility */
                if (!check_type_compat(s->sp->ptype, p->left.np->ptype))
                    yyerror("Different types for formal and actual class parameter '%s' ('%s' to '%s')", 
                            s->sp->name, p->left.np->ptype->name, s->sp->ptype->name);
            }
            else {
                /* check dimensions */
                if (s->sp->dims > 0) check_actual_param_dims(s->sp, p->left.np);
                /* check type compatibility */
                if (!check_type_compat(s->sp->ptype, p->left.np->ptype))
                    yyerror("Different types for formal and actual class parameter '%s' ('%s' to '%s')", 
                            s->sp->name, p->left.np->ptype->name, s->sp->ptype->name);
            }
            /* go to next variable */
            p = p->right.np;
        }
        s = s->next;
    }
    if (p != NULL) 
        yyerror("Incorrect number of parameters for class '%s'", c->name);
}

/* compare formal parameter lists of two classes (for derived classes) 'c' is the derived class */
void check_formal_params(symbol_t *c, list_t *derived, list_t *base)
{
    list_t *s = base;
    list_t *p = derived;
    if (s == NULL && p != NULL) {
        yyerror("Derived class '%s' cannot have parameters (base class '%s' does not)", c->name, c->ptype->name);
        return;
    }
    while (s != NULL) {
        if (p == NULL) {
            yyerror("Derived class '%s' must have same number and type of parameters as base class '%s'", c->name, c->ptype->name);
            return;
        }
        /* check that dimensions match */
        if (s->sp->dims != p->sp->dims)
            yyerror("Different dimensions for formal parameter '%s' of base class '%s' (%d to %d)", s->sp->name, c->ptype->name, p->sp->dims, s->sp->dims);
        /* check type compatibility */
        if (!check_type_compat(s->sp->ptype, p->sp->ptype))
            yyerror("Different type for formal parameter '%s' of base class '%s' ('%s' to '%s')", s->sp->name, c->ptype->name, p->sp->ptype->name, s->sp->ptype->name);
        s = s->next;
        p = p->next;
    }
}

/* compare formal parameter lists of forward declaration and definition of a classes */
void check_forward_params(symbol_t *c, list_t *def, list_t *decl)
{
    list_t *s = decl, *p = def;
    
    if (s == NULL && p != NULL) {
        yyerror("Class '%s' definition/declaration has parameters but previous declaration did not", c->name);        
        return;
    }
    while (s != NULL) {
        if (p == NULL) {
            yyerror("Definition/declaration of class '%s' must have same number and type of parameters as its declaration", c->name, c->ptype->name);
            return;
        }
        /* check that dimensions match */
        if (s->sp->dims != p->sp->dims)
            yyerror("Different dimensions for formal parameter '%s' in definition and declaration of class '%s' (%d to %d)", s->sp->name, c->ptype->name, p->sp->dims, s->sp->dims);
        /* check type compatibility */
        if (!check_type_compat(s->sp->ptype, p->sp->ptype))
            yyerror("Different type for formal parameter '%s' in definition and declaration of class '%s' ('%s' to '%s')", s->sp->name, c->ptype->name, p->sp->ptype->name, s->sp->ptype->name);
        s = s->next;
        p = p->next;
    }
}

/* check that array init fits within parse size */
int check_fit_array(symbol_t *var, node_t *init, node_t *parse)
{
    node_t *p;
    int err = 0;

    if (!check_type_compat(var->ptype, init->ptype)) return 0;
    
    /* go through the various init values and call check_fit */
  
    if (init->op != flavor::Parser::token::ARRAY || init->left.np == NULL)
        fatal("Internal error CH%d: Expected array initializer", __LINE__);
    
    p = init->left.np;
    while (p != NULL) {
        if (p->op == ',') {
            if (p->right.np == NULL) 
                fatal("Internal error CH%d: No subexpression in array initializer", __LINE__);
            if (p->right.np->op == flavor::Parser::token::ARRAY)
                err += !check_fit_array(var, p->right.np, parse);
            else
                err += !check_fit(var, p->right.np, parse);
        }
        else if (p->op == flavor::Parser::token::ARRAY) {
            err += !check_fit_array(var, p, parse);
            break;
        }
        if (p->leaf) {
            err += !check_fit(var, p, parse);
            break;
        }
        else p = p->left.np;
    }
    return err ? 0 : 1;
}

/* check that init fits within parse size (assumes that type checking on init and parse has been done) */
int check_fit(symbol_t* var, node_t* init, node_t* parse)
{
    int parse_size, val, min_val, max_val;
    
    /* sanity check */
    if (var==NULL || init==NULL || parse==NULL)
        fatal("Internal error CH%d: Invalid arguments", __LINE__);
    
    /* if init or parse are constant variables, we can reduce */
    if (init->op == flavor::Parser::token::VARIABLE && !var->range) {
        if (init->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (init->left.sp->init == NULL)
                fatal("Internal error CH%d: Constant without initial value", __LINE__);
            else return check_fit(var, init->left.sp->init, parse);
        }
        else return 1;
    }
    if (parse->op == flavor::Parser::token::VARIABLE) {
        if (parse->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (parse->left.sp->init == NULL)
                fatal("Internal error CH%d: Constant without initial value", __LINE__);
            else return check_fit(var, init, parse->left.sp->init);
        }
        else return 1;
    }

    /* at this point both must be literals */
    if (init->type>flavor::Parser::token::INT || parse->type>flavor::Parser::token::INT)
        /* TBD: check fit for doubles/floats */
        return 1;
    
    /* figure out parse size */
    if (parse->op == flavor::Parser::token::BIT_LITERAL) parse_size = parse->left.bit->val;
    else parse_size = parse->left.ival;
    
    /* parse size must be a non-negative number */
    if (parse_size == 0) {
        /* if 0, everything "fits" */
        return 1;
    }
    else if (parse_size < 0) {
        yyerror("Parse size must be a non-negative number (%d)", parse_size);
        return 0;
    }

    /* figure out init value */
    if (init->op == flavor::Parser::token::BIT_LITERAL) val=init->left.bit->val;
    else val=init->left.ival;

    /* figure out min/max values for this parse size */
	if (parse_size == 32) return 1;

    if (var->modifiers & M_UNSIGNED) {
        min_val = 0;
        max_val = (1<<parse_size)-1;
    }
    else {
        min_val = -(1<<(parse_size-1));
        max_val = (1<<(parse_size-1))-1;
    }
    if (val<min_val || val>max_val) {
        yyerror("Initial value for '%s' (%d) %sdoes not fit parse size (%d)", 
                var->name, val, (var->modifiers & M_UNSIGNED ? "" : "as signed "), parse_size);
        return 0;
    }
    else return 1;
}

/* check if an expression is suitable as an operand for INC or DEC operation */
int check_unary(node_t* expr)
{
    symbol_t* sp;
    
    if (expr == NULL) return 0;

    sp = get_lvalue(expr);
    /* lvalue not found */
    if (sp == NULL) return 0;
    
    /* sanity check */
    if (sp->ptype == NULL)
        /* will be caught be check_lvalue() later on */
        return 1;

    if (sp->ptype->internal && !(sp->modifiers & M_CONST) && !sp->parsable) return 1;
    else return 0;
}

/* check that alignment expression is an int, multiple of 8 */
node_t* check_align_arg(node_t* expr)
{
    if (!is_literal(expr->op)) {
        yyerror("Non-constant expression in 'align' argument");
        return expr;
    }
    if (expr->type > flavor::Parser::token::INT) {
        yyerror("Non-integer argument in 'align' argument");
        return expr;
    }
    if (expr->left.ival % 8) {
        yyerror("Argument in 'align' is not a multiple of 8 (%d)", expr->left.ival);
        return expr;
    }
    return expr;
}

/* check array index (increase max_array_size if necessary) */
node_t *check_array_index(node_t *expr)
{
    /* we can check only if expr is a literal */
    if (!is_literal(expr->op)) return expr;
    
    if (expr->type > flavor::Parser::token::INT) {
        yyerror("Invalid array index type ('%s')", expr->ptype->name);
        return expr;
    }
    if (expr->type != flavor::Parser::token::INT)
        fatal("Internal error CH%d: Expected integer type", __LINE__);
    if (expr->left.ival > max_array_size-1) {
        yyerror("Warning: Maximum array size (%d) exceeded: %d", max_array_size, expr->left.ival+1);
    }
    if (expr->left.ival < 0) {
        yyerror("Negative array index: %d", expr->left.ival);
        return expr;
    }
    return expr;
}

/* check array size (increase max_array_size if necessary) */
node_t *check_array_size(node_t *expr)
{
    /* we can check only if expr is a literal */
    if (!is_literal(expr->op)) return expr;
    
    if (expr->type > flavor::Parser::token::INT) {
        yyerror("Invalid array size type ('%s')", expr->ptype->name);
        return expr;
    }
    if (expr->type != flavor::Parser::token::INT) {
        yyerror("Internal error CH%d: Expected integer type", __LINE__);
        return expr;
    }
    if (expr->left.ival > max_array_size) {
        yyerror("Warning: Maximum array size (%d) exceeded: %d", max_array_size, expr->left.ival);
    }
    if (expr->left.ival <= 0) {
        yyerror("Zero or negative array size: %d", expr->left.ival);
        return expr;
    }
    return expr;
    
}

/* recursive check of number of elements in array initializations */
static int check_array_init_nelem(int *dim_values, int dims, node_t *init, int level)
{
    node_t *p;
    int cnt;
    static int max_level;

    /* max level reached - to check for fewer dimensions than declared */
    if (level == 0)
        max_level = 0;
    else if (level > max_level)
        max_level = level;

    if (init->op != flavor::Parser::token::ARRAY || init->left.np == NULL)
        fatal("Internal error CH%d: Expected array initializer", __LINE__);
    
    /* we want the innermost dimension errors to be reported first, so we do a double scan */
    
    /* first scan to count number of elements in this dimension */
    p = init->left.np;
    cnt = 0;
    while (p != NULL) {
        /* regardless if it's a leaf or ',', we add 1 */
        cnt++;
        if (p->op == flavor::Parser::token::ARRAY) {
            break;
        }
        else if (p->leaf)
            break;
        else
            p = p->left.np;
    }
    
    /* only check number of elements if we are within the number of dimensions declared */
    if (level < dims && cnt != dim_values[level]) {
        yyerror("Invalid number of elements for dimension %d in array initializer (%d instead of %d)", dims-level, cnt, dim_values[level]);
        return 0;
    }

    /* second scan to go to deeper dimensions */
    p = init->left.np;
    while (p != NULL) {
        if (p->op == ',') {
            if (p->right.np == NULL)
                fatal("Internal error CH%d: No subexpression in array initializer", __LINE__);
            if (p->right.np->op == flavor::Parser::token::ARRAY && !check_array_init_nelem(dim_values, dims, p->right.np, level+1))
                return 0;
        }
        else if (p->op == flavor::Parser::token::ARRAY) {
            if (!check_array_init_nelem(dim_values, dims, p, level+1))
                return 0;
            break;
        }
        if (p->leaf)
            break;
        else
            p = p->left.np;
    }
  
    /* if back at level 0, check number of levels reached */
    if (level == 0) {
        if (max_level+1 > dims) {
            yyerror("Array initializer has more dimensions than declared (%d should be %d)", max_level+1, dims);
            return 0;
        }
        else if (max_level+1 < dims) {
            yyerror("Array initializer has fewer dimensions than declared (%d should be %d)", max_level+1, dims);
            return 0;
        }
    }
    return 1;
}

/* check that init value for array matches dimensions (the dims *must* be literals) */
int check_array_init(symbol_t *sp, node_t **dim, int dims, node_t *init)
{
    int i, j, *dim_values, ret=1;
    node_t *p;
    /* first, figure out the dimensions of the symbol */
    dim_values = (int *)getmem(dims*sizeof(int));
    
    for (i=0, j=0; i<dims; i++) {
        p = dim[i];
        if (p == NULL)
            fatal("Internal error CH%d: Unexpected end of dimensioning array", __LINE__);
        /* skip double braces */
        if (p->op == flavor::Parser::token::DBL_BRACE_L)
            continue;
        else if (p->op != '[')
            fatal("Internal error CH%d: Expected array expresion", __LINE__);
        if (p->left.np == NULL)
            fatal("Internal error CH%d: Array expression without value", __LINE__);
        if (p->left.np->op != flavor::Parser::token::INT_LITERAL) {
            /* invalid array size type has already been reported - warn only if we have a variable */
            if (!is_literal(p->left.np->op))
                yyerror("Cannot initialize array '%s' with non-constant dimensions", sp->name);
            ret = 0;
            goto check_array_init_end;
        }
        dim_values[j] = p->left.np->left.ival;
        j++;
    }

    /* now traverse the array tree and check number of elements */
    if (!check_array_init_nelem(dim_values, j, init, 0))
        ret = 0;

check_array_init_end:
    free(dim_values);
    return ret;
}

/* check that a list of dimension expr contain no double braces */
int check_no_double_brace(node_t **dim)
{
    node_t *d;
    
    if (dim == NULL) return 1;
    
    while ((d = *dim++) != NULL) {
        if (d->op == flavor::Parser::token::DBL_BRACE_L) return 0;
    }
    return 1;
}

/* if a variable is constant parsable or non-parsable, check indices */
static int check_indices(symbol_t *sp, node_t *expr)
{
    int i, err=0;
    node_t *n;
    node_t *dimp;
    
    if (sp==NULL || sp->dims==0 || expr==NULL)
        fatal("Internal error CH%d: Invalid argument", __LINE__);
    
    /* if non-constant parsable, can't do */
    if (!(sp->modifiers & M_CONST) && sp->parsable)
        return 1;
    
    /* sanity check */
    if (sp->dim == NULL)
        fatal("Internal error CH%d: No array expression in array symbol", __LINE__);
    
    /* go through the indices, checking things out if they are literals */
    n = expr;
    for (i=sp->dims-1; i>=0; i--) {
        dimp = (node_t *)sp->dim[i];
        if (dimp->op == flavor::Parser::token::DBL_BRACE_L)
            continue;
        if (dimp->left.np->op == flavor::Parser::token::INT_LITERAL && n->right.np->op == flavor::Parser::token::INT_LITERAL)
            if (dimp->left.np->left.ival-1 < n->right.np->left.ival) {
                yyerror("Array index (%d) exceeds declared size (%d)",
                    n->right.np->left.ival, dimp->left.np->left.ival);
                err++;
            }
            n = n->left.np;
    }
    return !err;
}

/* check an expression for proper array formation (places symbol table entry in spp, sets literal if all indices are literals) */
int check_array(node_t *expr, symbol_t **spp, int *literal)
{
    node_t *n;
    int levels;
    int err = 0;

    if (expr == NULL || spp == NULL || literal == NULL) fatal("Internal error CH%d: Invalid argument", __LINE__);
    /* set to 1, and reset if we hit a non-literal index */
    *literal = 1;
    /* look for '[' */
    n = expr;
    /* descent to the base of the array, counting levels */
    levels = 0;
    while (n->op == '[') {
        levels++;
        if (n->right.np == NULL) fatal("Internal error CH%d: Unexpected end of array expression", __LINE__);
        if (n->right.np->op != flavor::Parser::token::INT_LITERAL) *literal=0;
        n = n->left.np;
    }

    /* here the only thing we can find is rescoping operators */
    if (n->op == '.') n = n->right.np;
    
    /* at this point we must have reached the actual variable */
    if (n->op != flavor::Parser::token::VARIABLE) return 0;
    
    /* pass along symbol table entry */
    *spp = n->left.sp;

    /* if it is an internal variable, don't check */
    if (n->left.sp->internal) return 1;

    /* ARRAY PARAM */

    /* if level == 0, then we don't have to check */
    if (levels == 0) return 1;

    /* check for undefined variable */
    if (n->left.sp->ptype == NULL) {
	yyerror("Variable '%s' used before being defined", n->left.sp->name);
        n->left.sp->ptype = s_lookup("int");
        n->left.sp->assigned = 1;
        return 0;
    }

    /* check dims */
    if (n->left.sp->dims != levels) {
        yyerror("Number of dimensions for '%s' (%d) does not match declaration (%d)", n->left.sp->name, levels, n->left.sp->dims);
        err++;
    }
    else {
        /* we may be able to check the actual indices (only for constants) */
        if (n->left.sp->dims>0) err += !check_indices(n->left.sp, expr);
    }
    
    return !err;
}


/* check a parse expression for type and value */
node_t* check_parse(symbol_t* type, unsigned int mods, node_t* parse)
{
    /* if no parse info, ok */
    if (parse == NULL) return NULL;

    /* if parse is a map, check type compatibility */
    if (parse->op == flavor::Parser::token::MAP_TYPE) {
        /* if classes, they must be identical */
        if (type->ident == flavor::Parser::token::CLASS_TYPE || parse->ptype->ident == flavor::Parser::token::CLASS_TYPE) {
            if (type != parse->ptype) yyerror("Incompatible type between map and variable");
        }
        else if (!check_type_compat(type, parse->ptype))
            yyerror("Incompatible type between map and variable");
        return parse;
    }
    /* not a map; check that parsable expression evaluates to 'int' or less */
    if (parse->type > flavor::Parser::token::INT) {
        yyerror("Invalid type for parsed length expression ('%s'); setting to 0", parse->ptype->name);
        /* return something useful so that we can safely continue */
        return leafi(flavor::Parser::token::INT_LITERAL, 0);
    }
    /* if variable, see if we can reduce */
    if (parse->op == flavor::Parser::token::VARIABLE) {
        if (parse->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (parse->left.sp->init == NULL) fatal("Internal error CH%d: Constant without initial value", __LINE__);
            else return check_parse(type, mods, parse->left.sp->init);
        }
        else return parse;
    }
    /* if parse is a bit literal, convert first to int */
    if (parse->op == flavor::Parser::token::BIT_LITERAL) {
        parse = leafi(flavor::Parser::token::INT_LITERAL, parse->left.bit->val);
    }
    /* if double or float, parse size must be a constant */
    if (type->ident == flavor::Parser::token::FLOAT || type->ident == flavor::Parser::token::DOUBLE) {
        if (parse->op != flavor::Parser::token::INT_LITERAL)
            yyerror("Type '%s' must have a fixed parse length (of %d bits)", type->name, get_double_size(type->ident, mods));
        else if (parse->left.ival != get_double_size(type->ident, mods) && parse->left.ival != 0)
            yyerror("Type '%s' must have a fixed parse length of %d bits", type->name, get_double_size(type->ident, mods));
        return parse;
    }
    /* if char, check that the parse value is not larger than 8 */
    if (type->ident == flavor::Parser::token::CHAR && parse->op == flavor::Parser::token::INT_LITERAL) {
        if (parse->left.ival > 8)
            yyerror("Parsed length for 'char' larger than 8 (%d)", parse->left.ival);
    }
    /* if parse is int literal, check value (ignore size of double; will check that in set_var later on) */
    if (parse->op == flavor::Parser::token::INT_LITERAL) {
        if (cpp()) {
            if (parse->left.ival > 32 && parse->left.ival != get_double_size(flavor::Parser::token::DOUBLE, 0))
                yyerror("Parsed length larger than 32 bits (%d)", parse->left.ival);
            if (parse->left.ival < 0) 
                yyerror("Invalid parsed length value: %d", parse->left.ival);
        }
        else if (java()) {
            if (parse->left.ival > 64)
                yyerror("Parsed length larger than 64 bits (%d)", parse->left.ival);
            if (parse->left.ival < 0) 
                yyerror("Invalid parsed length value: %d", parse->left.ival);
        }
    }
    return parse;
}

/* check that float/double/long double gets appropriate fixed parse size */
node_t* check_double_parse(symbol_t* sp, node_t* parse)
{
    int parse_size;
    
    /* we have already checked that expr evaluates to 'int' */
    if (parse->type != flavor::Parser::token::INT) return parse;
    
    /* if variable, see if we can reduce */
    if (parse->op == flavor::Parser::token::VARIABLE) {
        if (parse->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (parse->left.sp->init == NULL)
                fatal("Internal error CH%d: Constant without initial value", __LINE__);
            else
                return check_double_parse(sp, parse->left.sp->init);
        }
        else {
            /* warn about fixed-length requirement for this type */
            yyerror("Type '%s' only supports a fixed parsed length of %d bits", sp->ptype->name, get_double_size(sp->ptype->ident, (sp->modifiers & M_CONST)));
            return parse;
        }
    }
    /* at this point it must be a literal */
    
    /* figure out parse size */
    if (parse->op == flavor::Parser::token::BIT_LITERAL) parse_size = parse->left.bit->val;
    else parse_size = parse->left.ival;
    
    if (parse_size != get_double_size(sp->ptype->ident, (sp->modifiers & M_CONST)) && parse_size!=0)
        yyerror("Type '%s' only supports a fixed parse length of %d bits",  sp->ptype->name, get_double_size(sp->ptype->ident, (sp->modifiers & M_CONST)));
    
    return parse;
}

/* check that a given ID does not clash with the ID of a given class (called from check_id_unique and check_id_unique_down below) */
static void check_id_unique_class(node_t *id_init, symbol_t *base)
{
    int ivalue;
    double dvalue;
    node_t *np;
    
    if (id_init == NULL || base == NULL)
        fatal("Internal error CH%d: Invalid argument", __LINE__);
    
    /* if class has no ID, or the base is abstract, nothing to do */
    if (base->id == NULL || base->abstract)
        return;
    
    /* it has an ID, check its value */
    
    /* first get the value of the ID we are checking against */
    if (id_init->op == flavor::Parser::token::BIT_LITERAL)
        ivalue = id_init->left.bit->val;
    else if (id_init->op == flavor::Parser::token::INT_LITERAL)
        ivalue = id_init->left.ival;
    else if (id_init->op == flavor::Parser::token::DOUBLE_LITERAL)
        dvalue = id_init->left.dval;
    else
        /* mangled ID declaration, ignore */
        return;
    
    /* now compare with the value of the ID in the base class */
    np = base->id->init;
    if (np == NULL) {
        fatal("Internal error CH%d: ID without initial value", __LINE__);
    }
    else if (np->op == flavor::Parser::token::BIT_LITERAL) {
        if ((unsigned)ivalue == np->left.bit->val)
            yyerror("Non-unique class ID value '%d' (clashes with class '%s')", ivalue, base->name);
    }
    else if (np->op == flavor::Parser::token::INT_LITERAL) {
        if (ivalue == np->left.ival)
            yyerror("Non-unique class ID value '%d' (clashes with class '%s')", ivalue, base->name);
    }
    else if (np->op == flavor::Parser::token::DOUBLE_LITERAL) {
        if (dvalue == np->left.dval)
            yyerror("Non-unique class ID value '%g' (clashes with class '%s')", dvalue, base->name);
    }
    else
        /* mangled base ID declaration, ignore */
        return;
}

/* recursive part of check_id_unique, when we check downwards (from a class going to its derived ones) */
static void check_id_unique_down(node_t *id_init, symbol_t *var)
{
    symbol_t *sp, **spp;
    
    /* if no var, or the var is not derived, nothing to do */
    if (var == NULL)
        return;
    
    /* check uniqueness in base class */
    check_id_unique_class(id_init, var);
    
    /* continue in next parent, as well as all classes derived from this one (careful: except the one we got in) */
    
    /* check in other derived classes */
    if (var->derived != NULL) {
        spp = var->derived;
        while ((sp =* spp++) != NULL) {
            if (sp == var)
                continue;
            else
                check_id_unique_down(id_init, sp);
        }
    }
}


/* Check that an ID value in a class hierarchy is unique. Init is assumed to be 
 * a literal, and var is the class from which we will start searching (recursive).
 * Note that the ID value of abstract classes is ignored.
 */
static void check_id_unique(node_t *id_init, symbol_t *var)
{
    symbol_t *sp, **spp;
    
    /* if no var, or the var is not derived, nothing to do */
    if (var == NULL || var->ptype == NULL) return;
    
    /* check uniqueness in base class */
    check_id_unique_class(id_init, var->ptype);
    
    /* continue in next parent, as well as all classes derived from this one (careful: except the one we got in) */
    
    /* check in other derived classes */
    if (var->ptype->derived != NULL) {
        spp = var->ptype->derived;
        while ((sp = *spp++) != NULL) {
            if (sp == var)
                continue;
            else 
                check_id_unique_down(id_init, sp);
        }
    }

    /* check in parent class of our base, if any */
    if (var->ptype->ptype != NULL)
        check_id_unique(id_init, var->ptype->ptype);
}


/* Get most recent base class that has an ID; if none, return NULL
 * note that abstract parents are not ignored in this search
 * (to be able to check size and type of id).
 */
static symbol_t *check_id_get_base(symbol_t *base)
{
    if (base == NULL) return NULL;
    
    if (base->id != NULL) return base;
    else if (base->ptype != NULL) return check_id_get_base(base->ptype);
    else return NULL;
}


/* Check that class ID has a constant value, and that it matches the type
 * and name of that of the base class, if any.
 */
symbol_t *check_id(symbol_t *var)
{
    symbol_t *base;
    
    if (var->id == NULL)
        fatal("Internal error CH%d: No ID", __LINE__);

    if (var->id->init == NULL)
        fatal("Internal error CH%d: ID without value", __LINE__);
    
    if (!var->id->range)
        /* the expression must evaluate to a literal */
        if (!is_literal(var->id->init->op)) {
            yyerror("ID variable '%s' value for class '%s' must evaluate to a constant", var->id->name, var->name);
            return var;
        }

    /* for abstract classes, the ID must evaluate to 0, and is otherwise ignored */
    if (var->abstract && var->id->init->left.ival != 0) {
        yyerror("ID variable '%s' value for abstract class '%s' must be 0", var->id->name, var->name);
        return var;
    }

    if (!var->id->range)
        /* Check that parse size is non-zero; we only check int literals
         * since this is already type-checked and reduced by check_parse() early on.
         */
        if (var->id->parse->op == flavor::Parser::token::INT_LITERAL) {
            if (var->id->parse->left.ival == 0)
                yyerror("ID variable '%s' parse size cannot be 0", var->id->name);
        }

    /* Now check that it matches with base class, if any;
     * match must be on type, modifiers, parsable size, and name, and 
     * the initial values must be different across all derived
     * classes in the same tree.
     */
    base = check_id_get_base(var->ptype);
    if (base == NULL) {
        /* no base class with ID or no base class, no further checks necessary */
        return var;
    }
    
    /* type, modifiers, name, parse, and align must match */
    if (var->id->ptype != base->id->ptype)
        yyerror("ID types in derived and base classes must be identical");
    if (!var->id->range)
        if (var->id->modifiers != base->id->modifiers)
            yyerror("ID type modifiers in derived and base classes must be identical");
    if (strcmp(var->id->name, base->id->name) != 0)
        yyerror("ID names in derived and base classes must be identical");

    /* check parse sizes */
    if (var->id->parse == NULL || base->id->parse == NULL)
        fatal("Internal error CH%d: IDs without parse sizes", __LINE__);
    
    /* if int, check sizes, else check that maps are identical */
    if (var->id->parse->op == flavor::Parser::token::INT_LITERAL) {
        if (base->id->parse->op == flavor::Parser::token::INT_LITERAL) {
            if (var->id->parse->left.ival != base->id->parse->left.ival)
                yyerror("Parse size of ID must match that of base classes ('%d' should be '%d')",
                        var->id->parse->left.ival, base->id->parse->left.ival);
        }
        else
            yyerror("Base class '%s' uses map '%s' for ID parse size", 
                    base->name, base->id->parse->left.sp->name);
    }
    else if (var->id->parse->op == flavor::Parser::token::MAP_TYPE) {
        if (base->id->parse->op == flavor::Parser::token::INT_LITERAL)
            yyerror("Base class '%s' uses literal instead of map '%s' for ID parse size", 
                    var->name, var->id->parse->left.sp->name);
        else {
            if (var->id->parse->left.sp != base->id->parse->left.sp)
                yyerror("Different map used in ID of base class ('%s' instead of '%s')",
                        var->id->parse->left.sp->name, base->id->parse->left.sp->name);
        }
    }
    else {
        fatal("Internal error CH%d: Neither integer literal nor map for ID parse size", __LINE__);
    }

    /* check alignment sizes */
    if ((var->id->align == NULL && base->id->align != NULL) ||
        (var->id->align != NULL && base->id->align == NULL))
        yyerror("Alignment of ID in base and derived classes must be identical");
    else if (var->id->align != NULL && base->id->align != NULL) {
        if (var->id->align->op != flavor::Parser::token::INT_LITERAL || base->id->align->op != flavor::Parser::token::INT_LITERAL)
            fatal("Internal error CH%d: Non-integer literal for ID alignment size", __LINE__);
        if (var->id->align->left.ival != base->id->align->left.ival)
            yyerror("Alignment size of ID must match that of base class ('%d' should be '%d')",
                var->id->align->left.ival, base->id->align->left.ival);
    }

    /* now check initial values for *all* classes in the same tree */
    check_id_unique(var->id->init, var);
    
    return var;
}


/* if ID declaration catches the declaration of the ID of the base class, rescope the ID */
symbol_t *check_rescope_id(symbol_t *id)
{
    symbol_t *sp;
    /* see if it is already defined */
    if (id->ptype != NULL) {
        /* check that the scope is indeed that of an ID variable */
        if (id->cxt_level < 2) {
            sp = s_add(id->name, flavor::Parser::token::VARIABLE);
            sp->range = id->range;
            return sp;
        }
        else {
            yyerror("ID name '%s' hidden by parameter name", id->name);
            sp = s_add(id->name, flavor::Parser::token::VARIABLE);
            sp->range = id->range;
            return sp;
        }
    }
    return id;
}


/*
 * Check cyclic declarations:
 * These can only occur with forward declarations, and can trigger infinite loops
 * of get() or put() calls. To detect them, the following algorithm is used:
 * 1) if a class X has as a member a forward-declared class F, it is added in a list
 *    in F.
 * 2) When we process the definition of a forward-declared class, we check that
 *    a. F does not extend X or a class derived from X.
 *    b. F does not contain a variable of type X, or of a class derived from X.
 */

/* 
 * This is part (a).
 * (sp is F, type is the base class, and derived is a list of classes to check against)
 */
int check_cyclic_decl_derived(symbol_t *sp, symbol_t *type, symbol_t **derived)
{
    symbol_t *p;

    /* make sure that sp does not extend classes that use it, or classes derived from them */
    if (sp == NULL || derived == NULL)
        return 0;

    while ((p =* derived++)!=NULL) {
        if (p == type) {
            yyerror("Using '%s' as a base class creates a cyclic reference in '%s'", type->name, sp->name);
            return 1;
        }
        /* recurse into derived classes */
        if (p->derived != NULL)
            return check_cyclic_decl_derived(sp, type, p->derived);
    }
    /* not reached */
    return 0;
}

/*
 * This is part (b)
 */

/* recursive helper: only checks one symbol table entry */
static void check_cyclic_decl_contained_rec(symbol_t *sp, symbol_t **derived)
{
    symbol_t **ppd, *pd;
    int tmpline;

    ppd = derived;
    while ((pd =* ppd++)!=NULL) {
        if (sp->ptype == pd) {
            tmpline = lineno;
            lineno = sp->lineno;
            yyerror("Declaration of '%s' creates cyclic reference in class '%s'", sp->name, sp->owner->name);
            lineno = tmpline;
        }
        else if (pd->derived != NULL) {
            /* recurse into derived classes */
            check_cyclic_decl_contained_rec(sp, pd->derived);
        }
    }
}

void check_cyclic_decl_contained(symbol_t *sp, symbol_t **derived)
{
    symlink_t *slp;
    int i;

    if (sp == NULL || derived == NULL)
        return;

    /* make sure that sp does not contain classes that use it, or classes derived from them */

    /* we only need to search in the first-level context of a class */
    for (i=0; i<ST_SIZE; i++) {
        slp = sp->cxt->st[i];
        while (slp != NULL) {
            if (slp->sp == NULL)
                fatal("Internal error CH%d: Symbol link without symbol", __LINE__);
            check_cyclic_decl_contained_rec(slp->sp, derived);
            slp = slp->next;
        }
    }
}

/* check that a map code and value is unique (not a prefix of another code) if not, returns 0 */
int check_map(symbol_t *map, list_t *codes, list_t *code)
{
    list_t *cur, *cur_entry, *new_entry;
    bit_t *codebit, *curbit;   /* pointers to input and current bit literal value */
    int lcode, lcur;           /* how much shifting is required for alignment */
    int tmp_lineno;
	int hit;
	int num_of_entry;

    if (code->e1 == NULL || code->e1->op != flavor::Parser::token::BIT_LITERAL || code->e1->left.bit == NULL)
        fatal("Internal error CH%d: Expected code entry with bit literal", __LINE__);

    codebit = code->e1->left.bit;
    cur = codes;
    while (cur != code && cur != NULL) {
        /* make sure current value is bit literal */
        if (cur->e1 == NULL || cur->e1->op != flavor::Parser::token::BIT_LITERAL || cur->e1->left.bit == NULL)
            fatal("Internal error CH%d: Expected code entry with bit literal", __LINE__);

        curbit = cur->e1->left.bit;
        /* normalize to shortest length */
        if (codebit->len <= curbit->len) {
            lcode = 0;
            lcur = curbit->len-codebit->len;
        }
        else {
            lcode = codebit->len-curbit->len;
            lcur = 0;
        }
        /* compare */
        if ((codebit->val>>lcode) == (curbit->val>>lcur)) {
            tmp_lineno = lineno;
            lineno = code->lineno;
            yyerror("Code '%s' conflicts with code '%s' (skipped)", bit2str(codebit), bit2str(curbit));
            lineno = tmp_lineno;
            return 0;
        }

		/* compare value */

		/* integer or character type */
		if (map->ptype->ident <= flavor::Parser::token::INT) {
			if (cur->sub1->e1->left.ival == code->sub1->e1->left.ival && code->sub1->type != flavor::Parser::token::ESC_FTYPE) {
				yyerror("Error: Value '%s' conflicts within map '%s'", node2string(code->sub1->e1), map->name);
				return 0;
			}
		}
		/* double or float type */
		else if (map->ptype->ident <= flavor::Parser::token::DOUBLE) {
			if (cur->sub1->e1->left.dval == code->sub1->e1->left.dval && code->sub1->type != flavor::Parser::token::ESC_FTYPE) {
			    yyerror("Error: Value '%s' conflicts within map '%s'", node2string(code->sub1->e1), map->name);
				return 0;
			}
		}
		/* class type */
		else if (map->ptype->ident != flavor::Parser::token::CLASS_TYPE) {
			fatal("Internal error: Unknown type for map '%s'", map->name);
		}
		else {

			/* now check value conflict */
			
			/* reset helper variable */
			hit = 0;
            num_of_entry = 0;

			cur_entry = cur->sub1->sub1;
			new_entry = code->sub1->sub1;
			while (cur_entry != NULL && new_entry != NULL) {
				switch(cur_entry->e1->type) {
				case flavor::Parser::token::CHAR:
				case flavor::Parser::token::INT:
					if (cur_entry->e1->left.ival == new_entry->e1->left.ival)
						hit++; /* if same value found 'hit' increased */
					break;
				case flavor::Parser::token::FLOAT:
				case flavor::Parser::token::DOUBLE:
					if (cur_entry->e1->left.dval == new_entry->e1->left.dval)
						hit++;
					break;
				default:
					fatal("Internal error CH%d: Unknown type for map variable", __LINE__);
				}
				cur_entry = cur_entry->next;
				new_entry = new_entry->next;
				num_of_entry ++; /* tracking number of values */
			}

			/* cannot happen; danny - maybe it could with escape values being a class
			if (cur_entry != NULL || new_entry != NULL) fatal("Internal error CH%d", __LINE__);
            */

			/* all entry values are same; danny - do we care about this? 
			if (hit == num_of_entry) {
				yyerror("Error: class type value conflict within map '%s'", map->name);
				return 0;
			}
            */
		}
		cur = cur->next;
    }
    return 1;
}

/* check that a class is appropriate for use within a map return 0 if not */
int check_map_class(symbol_t *sp)
{
    list_t *stmts;
    
    /* The following limitations exist:
     *   - only simple variables allowed 
     *   - arrays should be allowed
     *   - only declarations (no if's etc.)
     *   - if a map uses a class, the class becomes parsable, thus, assigned
     */
    
    /* first, find the statements for this class */
    stmts = find_class_stmts(sp);
    if (stmts == NULL)
        fatal("Internal error CH%d: Cannot find declaration of class '%s'", __LINE__, sp->name);
    while (stmts != NULL) {
        switch (stmts->type) {

        case flavor::Parser::token::DECL:
            /* declarations are ok, even parsable ones, but not classes */
            if (stmts->sp->ptype->ident == flavor::Parser::token::CLASS_TYPE) {
                yyerror("Class '%s' contains class variable declarations and cannot be used in a map", sp->name);
                return 0;
            }
            stmts->sp->accessed = 1;

		    break;
		default:
            /* verbatim is ok; everything else, isn't */
            if (flavor::Parser::token::VERBATIM > stmts->type && stmts->type > flavor::Parser::token::VERBATIM_BOTH_J)
                yyerror("Class '%s' is not simple and cannot be used in a map", sp->name);
            return 0;
        }
        stmts = stmts->next;
    }
    /* mark the class as parsable and assigned */
    sp->parsable = 1;
    sp->assigned = 1;

    return 1;
}

/* check map class value type against with class declaration for map type declaration */
list_t* check_map_class_value(symbol_t* map, list_t* val)
{
    list_t* class_stmts = find_class_stmts(map->ptype);
    list_t* val_sub = val->sub1;
	int verbatim_only = 1;
	
	while (val_sub != NULL) {
		if (class_stmts == NULL) {
			yyerror("No variable declaration found in class '%s' for map value '%s' in map '%s'", 
                    map->ptype->name, node2string(val_sub->e1), map->name);
			return val;
		}

		while (class_stmts != NULL && is_verbatim(class_stmts->type)) 
			class_stmts = class_stmts->next;
 
        if (class_stmts == NULL) return val;

		if (val_sub->e1->type != class_stmts->sp->ptype->ident 
            && !(val_sub->e1->type == flavor::Parser::token::DOUBLE && class_stmts->sp->ptype->ident == flavor::Parser::token::FLOAT
                 || val_sub->e1->type == flavor::Parser::token::FLOAT && class_stmts->sp->ptype->ident == flavor::Parser::token::DOUBLE)) {
			yyerror("Invalid map value type '%s' for map '%s'", node2string(val_sub->e1), map->name);
			return val;
		}
		if (class_stmts->sp->modifiers == M_UNSIGNED 
            && class_stmts->sp->ptype->ident <= flavor::Parser::token::INT
            && val_sub->e1->left.ival < 0) {
			yyerror("Invalid map value: use negative value '%s' for unsigned variable '%s' in class '%s' for map '%s'", node2string(val_sub->e1), class_stmts->sp->name, map->ptype->name, map->name);
			return val;
		}
		
		val_sub = val_sub->next;
		class_stmts = class_stmts->next;
	}

	if (class_stmts) {
		while (class_stmts != NULL) {
			if (!is_verbatim(class_stmts->type)) 
                verbatim_only = 0;
			class_stmts = class_stmts->next;
		}
		if (!verbatim_only)
            yyerror("Class '%s' doesn't match with map value (@skipping...)", map->ptype->name);
		return val;
	}
	return val;
}

/* check simple map value type against with MAP type declaration */
list_t* check_map_simple_value(symbol_t* map, list_t* val)
{
	if (map->ptype->ident != val->e1->type &&
        !(map->ptype->ident == flavor::Parser::token::FLOAT && val->e1->type == flavor::Parser::token::DOUBLE || map->ptype->ident == flavor::Parser::token::DOUBLE && val->e1->type == flavor::Parser::token::FLOAT))
		yyerror("Error: '%s' is incompatible type with MAP %s declaration", node2string(val->e1), map->name);
	return val;
}

/* check esc class parsable var was declared with same type as its owning map and parse size */
void check_map_esc_class_value(symbol_t *map, list_t *val)
{
    list_t* class_stmts = find_class_stmts(map->ptype);
    list_t* val_sub = val;
	int verbatim_only = 1;
	
	while (val_sub != NULL) {
		if (class_stmts == NULL) {
			yyerror("No variable declaration found in class '%s' for map value '%s' in map '%s'", 
                    map->ptype->name, node2string(val_sub->e1), map->name);
			return;
		}

		while (class_stmts != NULL && is_verbatim(class_stmts->type)) 
			class_stmts = class_stmts->next;
 
        if (class_stmts == NULL) return;
	    
        /* same type as map? */
        if (val_sub->e2->type != class_stmts->sp->ptype->ident) { 
		    yyerror("Error: '%s' is incompatible type with map '%s' declaration", 
                    val->e2->ptype->name, map->name);
		}
	    /* parse length should be integer */
	    if (val_sub->e3->op != flavor::Parser::token::INT_LITERAL) {
		    yyerror("Error: parse size must be integer");
		    return;
	    }
	    /* no zero or less than zero value for parse length */
	    if (val_sub->e3->left.ival <= 0) {
		    yyerror("Invalid parser length value (%d)", val->e3->left.ival);
		    return;
	    }
	    /* if char, check that the parse value is not larger than 8 */
	    if (val_sub->e2->type == flavor::Parser::token::CHAR && val->e3->left.ival > 8)
		    yyerror("Parsed length for 'char' larger than 8 (%d)", val->e3->left.ival);
	    /* bit length of integer parsable variable cannot be bigger than 32 */
	    else if (val_sub->e2->type == flavor::Parser::token::INT && val->e3->left.ival > 32)
		    yyerror("Parsed length larger than 32 bits (%d)", val->e3->left.ival);
	    /* bit length of float parable variable must be eaqual to 32 */
	    else if (val_sub->e2->type == flavor::Parser::token::FLOAT && val->e3->left.ival != 32)
		    yyerror("Warning: length of float parsable variable must be 32");
	    /* bit length of double parable variable must be eaqual to 64 */
	    else if (val_sub->e2->type == flavor::Parser::token::DOUBLE && val->e3->left.ival != 64)
		    yyerror("Warning: length of double parsable variable must be 64");

		val_sub = val_sub->next;
		class_stmts = class_stmts->next;
	}

	return;
}

/* check esc simple parsable var was declared with same type as its owning map and parse size */
void check_map_esc_simple_value(symbol_t *map, list_t *val)
{
	/* same type as map? */
	if (val->e2->type != map->ptype->ident) {
		yyerror("Error: '%s' is incompatible type with map '%s' declaration", 
                val->e2->ptype->name, map->name);
	}
	/* parse length should be integer */
	if (val->e3->op != flavor::Parser::token::INT_LITERAL) {
		yyerror("Error: parse size must be integer");
		return;
	}
	/* no zero or less than zero value for parse length */
	if (val->e3->left.ival <= 0) {
		yyerror("Invalid parser length value (%d)", val->e3->left.ival);
		return;
	}
	/* if char, check that the parse value is not larger than 8 */
	if (val->e2->type == flavor::Parser::token::CHAR && val->e3->left.ival > 8)
		yyerror("Parsed length for 'char' larger than 8 (%d)", val->e3->left.ival);
	/* bit length of integer parsable variable cannot be bigger than 32 */
	else if (val->e2->type == flavor::Parser::token::INT && val->e3->left.ival > 32)
		yyerror("Parsed length larger than 32 bits (%d)", val->e3->left.ival);
	/* bit length of float parable variable must be eaqual to 32 */
	else if (val->e2->type == flavor::Parser::token::FLOAT && val->e3->left.ival != 32)
		yyerror("Warning: length of float parsable variable must be 32");
	/* bit length of double parable variable must be eaqual to 64 */
	else if (val->e2->type == flavor::Parser::token::DOUBLE && val->e3->left.ival != 64)
		yyerror("Warning: length of double parsable variable must be 64");
}

/*******/
/* BAC */
/*******/

/* check BAC param */
int check_bac_param(char *param)
{
    symbol_t *sp;
    sp = s_bac_lookup(param);

    if (sp != NULL) {
        return sp->ident;
    }
    else {
        yyerror("Error: Invalid keyword '%s' used within the BAC construct", param);
        exit(1);
    }
}

/* check BAC param's value */
void check_bac_val(list_t *lp)
{
    int param = 0;
    node_t *np = NULL;

    /* sanity check */
    if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::INT_LITERAL)
        fatal("Internal error CH%d: Expected a param entry for the bac construct", __LINE__);
    
    param = lp->e1->left.ival;
    np = lp->sub1->e1;
   
    switch (param) {
    case flavor::Parser::token::BAC_PREC:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 3 || np->left.ival > 32) {
            yyerror("Error: Expected an int value in [3,32] for the 'prec' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_OOC:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 2) {
            yyerror("Error: Expected an int value in [0,2] for the 'ooc' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_SOC:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 3) {
            yyerror("Error: Expected an int value in [0,3] for the 'soc' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_TE:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 1) {
            yyerror("Error: Expected an int value in [0,1] for the 'te' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_RENORM:
        /* only one argument is assigned to the norm param */
        if (lp->sub1->type == flavor::Parser::token::EXPR) {
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL ||  np->left.uival < 0) {
                yyerror("Error: Expected an int value >= 0 for the 'norm' param");
                exit(1);
            }
        }
        else {
            np = lp->sub1->sub1->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL ||  np->left.uival < 0) {
                yyerror("Error: Expected an int value >= 0 for the first arg to the 'norm' param");
                exit(1);
            }
            np = lp->sub1->sub1->next->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 1) {
                yyerror("Error: Expected an int value in [0,1] for the second arg to the 'norm' param");
                exit(1);
            }
            if (lp->sub1->sub1->next->next != NULL) {
                np = lp->sub1->sub1->next->next->e1;
                if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival != 1) {
                    yyerror("Error: Expected an int (=1) for the third arg to the 'norm' param");
                    exit(1);
                }
            }
        }
        break;
    case flavor::Parser::token::BAC_BS:
        np = lp->sub1->sub1->e1;
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 2 || np->left.ival > 31) {
            yyerror("Error: Expected an int value in [2,31] for the first arg to the 'bs' param");
            exit(1);
        }
        np = lp->sub1->sub1->next->e1;
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 1 || np->left.ival > 8) {
            yyerror("Error: Expected an int value in [1,8] for the second arg to the 'bs' param");
            exit(1);
        }
        if (lp->sub1->sub1->next->next != NULL) {
            np = lp->sub1->sub1->next->next->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 7) {
                yyerror("Error: Expected an int value in [0,7] for the third arg to the 'bs' param");
                exit(1);
            }
        }
        break;
    case flavor::Parser::token::BAC_INIT:
        if (lp->sub1->type == flavor::Parser::token::EXPR) {
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.uival < 0) {
                yyerror("Error: Expected an int >= 0 value for the 'init' param");
                exit(1);
            }
        }
        else {
            np = lp->sub1->sub1->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.uival < 0) {
                yyerror("Error: Expected an int value >= 0 for the first arg to the 'init' param");
                exit(1);
            }
            np = lp->sub1->sub1->next->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 32) {
                yyerror("Error: Expected an int value [0,32] for the second arg to the 'init' param");
                exit(1);
            }
        }
        break;
    case flavor::Parser::token::BAC_END:
        if (lp->sub1->type == flavor::Parser::token::EXPR) {
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 32) {
                yyerror("Error: Expected an int value in [0,32] for the 'end' param");
                exit(1);
            }
        }
        else {
            np = lp->sub1->sub1->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 32) {
                yyerror("Error: Expected an int value in [0,32] for the first arg to the 'end' param");
                exit(1);
            }
            np = lp->sub1->sub1->next->e1;
            if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 3) {
                yyerror("Error: Expected an int value in [0,3] for the second arg to the 'end' param");
                exit(1);
            }
        }
        break;
    case flavor::Parser::token::BAC_RTABLE:
        np = lp->sub1->sub1->e1;
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival <= 0) {
            yyerror("Error: Expected an int value > 0 for the first arg to the 'rtable' param");
            exit(1);
        }
        np = lp->sub1->sub1->next->e1;
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival <= 0) {
            yyerror("Error: Expected an int value > 0 for the second arg to the 'rtable' param");
            exit(1);
        }
        np = lp->sub1->sub1->next->next->e1;
        if (np == NULL || np->op != flavor::Parser::token::STRING_LITERAL) {
            yyerror("Error: Expected a string value for the third arg to the 'rtable' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_NEXTI:
        if (np == NULL || np->op != flavor::Parser::token::STRING_LITERAL) {
            yyerror("Error: Expected a string value for the 'nexti' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_TRANS:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 1) {
            yyerror("Error: Expected an int value in [0,1] for the 'trans' param");
            exit(1);
        }
        break;
    case flavor::Parser::token::BAC_EXCH:
        if (np == NULL || np->op != flavor::Parser::token::INT_LITERAL || np->left.ival < 0 || np->left.ival > 1) {
            yyerror("Error: Expected an int value in [0,1] for the 'exch' param");
            exit(1);
        }
        break;
    default:
        yyerror("Error: Expected a valid param for the bac construct");
        exit(1);
    }
}
