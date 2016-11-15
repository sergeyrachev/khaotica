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

/* cvar.c */

#include "cdef.h"


/* generate variable declarations/definitions */
void c_var_decl(symbol_t *sp, int dims, node_t **dim)
{
    unsigned int mod;
    char *type;
    
    if (sp == NULL) return;
    
    /* convert 'bit' to 'unsigned int' */
    if (strcmp(sp->ptype->name, "bit") == 0) {
        type = "int";
    }
    else 
        type = sp->ptype->name;
    
    /* ignore M_CONST if var not global (it is modified by get()/put()) */
    if (sp->cxt_level > 0) {
        mod = sp->modifiers & (~M_CONST);
        
        if (cpp()) {
            c_outi("%s%s ", s_mods2str(mod), type);
            /* if a class var use pointers */
            if (sp->ptype->ident == CLASS_TYPE && cpp()) 
                c_out("*");
        }
        else if (java()) {
            c_outi("%s%s ", s_mods2str(mod), (sp->modifiers & M_LONG ? "long" : type));
        }
        c_out("%s", sp->name);
        c_array_decl(sp, dims, dim);
        c_out(";\n");
    }
    /* global context */
    else {
        mod = sp->modifiers;
        if (cpp())
            c_outi("%s%s %s", s_mods2str(mod), type, sp->name);
        else if (java())
            c_outi("static %s%s %s", s_mods2str(mod), (sp->modifiers & M_LONG ? "long" : type), sp->name);
        
        c_array_decl_actual(sp, dims, dim);
        
        /* if a global var, it must have an initial value */
        if (sp->init != NULL) {
            c_out(" = ");
            c_expression(sp->init, 0);
        }
        c_out(";\n");
    }
    /* declare lengthof variable, if needed */
    if (sp->ptype->ident != CLASS_TYPE && sp->lengthof) {
        c_outi("int ");
        c_out(sp->plengthof->name, prefix);
        c_array_decl(sp->plengthof, dims, dim);
        c_out(";\n");
    }
}

/* generate external variable declarations for C++ */
void c_var_decl_ext(symbol_t *sp, int dims, node_t **dim)
{
    char *type;
    
    if (sp == NULL) return;
    
    /* convert 'bit' to 'unsigned int' for C++ and 'int' for Java */
    if (strcmp(sp->ptype->name, "bit") == 0) {
        type = "int";
    }
    else
        type = sp->ptype->name;

    /* only global scope vars can be defined as extern */
    if (sp->cxt_level > 0)
        fatal("Internal error CV%d: Encountered extern declaration of non-global variable", __LINE__);

    c_outi("extern %s%s %s", s_mods2str(sp->modifiers), type, sp->name);
    c_array_decl_actual(sp, dims, dim);
    c_out(";\n");
}

/* generate variable declarations for those vars that have class or deeper scope */
void c_var_decl_inclass(symbol_t *sp, node_t *init, node_t *parse, int dims, node_t **dim)
{
    if (sp == NULL) 
        return;

    if (sp->owner == NULL)
        fatal("Internal error CV%d: Expected class member variable", __LINE__);

    /* figure out if we need a tmp parse var -- only relevant for parsables */
    if (sp->parsable) {
        /* for put(), if doing lengthof for non-class, or we have init value and type is 'int' or less */
        if ((sp->lengthof && sp->ptype->ident != CLASS_TYPE) || (init != NULL && sp->ptype->ident <= INT))
            sp->owner->put_needs_parse_var = 1;
        /* For get(), if we are tracing and it is not a class, if we are doing lengthof
         * and is not a class, or if it is a class with an ID and not pure abstract.
         */
        if ((sp->ptype->ident != CLASS_TYPE && (gen_trace || sp->lengthof)) ||
            (sp->ptype->ident == CLASS_TYPE && sp->id != NULL && !c_is_pure_abstract(sp->ptype))) 
            sp->owner->get_needs_parse_var = 1;
    }
    
    /* if parsable, we are tracing, and uses map, then get needs code var */
    if (gen_trace && sp->parsable && parse != NULL && parse->op == MAP_TYPE) {
        if (sp->owner == NULL)
            fatal("Internal error CV%d: Expected class member variable", __LINE__);
 
        sp->owner->get_needs_code_var = 1;
    }

    /* Save maximum nr of dimensions (so that the appropriate number of tmp.\
     * vars is generated in put/get). Only parsables, variables with initialization,
     * or classes are relevant here.
     *
     * NOTE: This is needed regardless if the variable is a class member or
     *       has deeper scope. These tmps are also used in, e.g., int b[10] = 1;
     */
    if (sp->owner != NULL) 
        if (init != NULL || sp->parsable || sp->ptype->ident == CLASS_TYPE) 
            if (sp->owner->max_dims < dims)
                sp->owner->max_dims = dims;
    
    if (!sp->declared && sp->cxt_level == 1) {
        c_var_decl(sp, dims, dim);
        sp->declared = 1;
        sp->declared_get = 1;
        sp->declared_put = 1;
        sp->declared_putxml = 1;
    }
}

/***********************************************/
/* variable declarations/definitions for get() */
/***********************************************/

/* generate the get part of parsable variable declarations (coded using a bac) */
static void c_var_decl_get_bac(symbol_t *sp, node_t *param, node_t *parse, int dims, node_t **dim, int usenext)
{
    symbol_t *bac = parse->left.sp;
    
    if (gen_trace) 
        c_trace_entering_mapped_class(sp, dims, bac);

    c_outi("%s", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    c_out(" = %s%sad_code(%sbs, ", bac->name, c_static_scope(), prefix);
    c_expression(param, 0);
    c_out(");\n");
}

/* generate the get part of variable declarations for simple parsables */
static void c_var_decl_get_parse_simple(symbol_t *sp, node_t *ip, node_t *pp, int dims, node_t **dim, int usenext)
{
    int i;
   
    /* if we are tracing or need lengthof, save parse size */
    if (gen_trace || sp->lengthof) {
        c_outi("%sparse = ", prefix);
        c_expression(pp, 0);
        c_out(";\n");
    }
    
    c_outi("%s", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    
    /* see which get function to call */
    c_out(" = ");

    /* java needs explicit cast */
    if (java())
        if (sp->ptype->ident != BIT && sp->ptype->ident != INT) 
            c_out("(%s) ", sp->ptype->name);

    c_out("%sbs.%s%s%s", prefix, 
	                     (sp->modifiers & M_LITTLE ? "little_" : ""),
	                     (sp->modifiers & M_UNSIGNED || sp->ptype->ident > INT  ? "" : "s"), 
                         (usenext ? "next" : "get"));
   
    if (sp->ptype->ident <= INT) {
        if (java() && sp->modifiers & M_LONG) 
            c_out("long(");
        else 
            c_out("bits(");
        if (gen_trace || sp->lengthof) 
            c_out("%sparse", prefix);
        else 
            c_expression(pp, 0);
    }
    else if (sp->ptype->ident == FLOAT) 
        c_out("float(");
    else if (sp->ptype->ident == FLOAT && (sp->modifiers & M_LONG)) 
        c_out("ldouble(");
    else 
        c_out("double(");
    
    c_out(");\n");
    
    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi(sp->plengthof->name, prefix);
        if (dims > 0) 
            c_array_proper(sp->plengthof, sp->plengthof->dims);
        c_out(" = %sparse;\n", prefix);
    }

    /* if a value is specified, check the parsed value against it */
    if (ip != NULL) {

        /* id range; in this case, the ? : operator is not used */
        if (sp->range) {
            c_outi("if (!(");
            c_expression(ip, 0);
            c_out(" <= %s && %s <= ", sp->name, sp->name);
            c_expression(ip->third.np, 0);
            c_out(")) {\n");
            c_identlevel++;
           
            c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        
            if (cpp()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[%%d]");
                c_out("'\"");
                if (dims > 0) {
                    c_out(", ");
                    for (i=0; i<dims; i++) {
                        c_out("%s", c_gentmp_dim(i));
                        if (i != dims-1) 
                            c_out(", ");
                    }
                }
            }
            else if (java()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[\" + %s + \"]", c_gentmp_dim(i));
                c_out("'\"");
            }
            c_out(");\n");

            /* increase error count */
            c_outi("%sret++;\n", prefix);
        
            if (gen_trace) 
                c_trace_var_simple(sp, dims, usenext, 1);
            
            c_identlevel--;
            c_outi("}\n");

            if (gen_trace) {
                c_outi("else {\n");
                c_identlevel++;
                c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
        }
        else {
            c_outi("if (%s", sp->name);
            if (dims > 0) 
                c_array_proper(sp, dims);
            c_out(" != ");
            if (sp->ptype->ident == FLOAT) 
                c_out("(float)");
            c_expression(ip, 0);
            c_out(") {\n");
            c_identlevel++;
 
            c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        
            if (cpp()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[%%d]");
                c_out("'\"");
                if (dims > 0) {
                    c_out(", ");
                    for (i=0; i<dims; i++) {
                        c_out("%s", c_gentmp_dim(i));
                        if (i != dims-1) 
                            c_out(", ");
                    }
                }
            }
            else if (java()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[\" + %s + \"]", c_gentmp_dim(i));
                c_out("'\"");
            }
            c_out(");\n");

            /* increase error count */
            c_outi("%sret++;\n", prefix);
        
            if (gen_trace) 
                c_trace_var_simple(sp, dims, usenext, 1);
        
            c_identlevel--;
            c_outi("}\n");
                
            if (gen_trace) {
                c_outi("else {\n");
                c_identlevel++;
                c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
        }    
    }
    else {
        if (gen_trace)
            c_trace_var_simple(sp, dims, usenext, 0);
    }
}

/* generate the get part of variable declarations for map-based parsables */
static void c_var_decl_get_parse_map(symbol_t *sp, node_t *ip, symbol_t *map, int dims, node_t **dim, int usenext)
{
    int i;

	if (gen_trace) 
        c_trace_entering_mapped_class(sp, dims, map);

	if cpp() {
       	c_outi("if (%s%s%s(%sbs, &%s", map->name, c_static_scope(), (usenext ? "nextvlc" : "getvlc"), prefix, sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
		/* if we are tracing or need lengthof, get number of bits */
		if (gen_trace || sp->lengthof) 
            c_out(", &%sparse", prefix);
		/* if we are tracing, we also need code value */
		if (gen_trace) 
            c_out(", &%scode", prefix);
		c_out(") == 0) {\n");
	}
    else if (java()) {
  		c_outi("%smr = %s%s%s(%sbs);\n", prefix, map->name, c_static_scope(), (usenext ? "nextvlc" : "getvlc"), prefix);
		
        if (gen_trace) 
            c_outi("%scode = %smr.getCode();\n",prefix,prefix);
		if (gen_trace || sp->lengthof) 
            c_outi("%sparse = %smr.getCodeLength();\n",prefix,prefix);
		c_outi("if (!%smr.isHit()) {\n",prefix);
    }

    /* lookup failed */
    c_identlevel++;
    c_outi("%s(\"Map '%s' lookup for '%s", err_func, map->name, sp->name);
    if (cpp()) {
        if (dims > 0) 
            for (i=0; i<dims; i++) 
                c_out("[%%d]");
        c_out("' failed\"");
        if (dims > 0) {
            c_out(", ");
            for (i=0; i<dims; i++) {
                c_out("%s", c_gentmp_dim(i));
                if (i != dims-1) 
                    c_out(", ");
            }
        }
        c_out(");\n");
    }
    else if (java()) {
		for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("' failed\");\n");
    }
    /* increase error count */
    c_outi("%sret++;\n", prefix);

	if (java()) {
		c_identlevel--;
		c_outi("}\n");
		c_outi("else {\n");
		c_identlevel++;
		
		c_outi(sp->name);
		for (i=0; i<dims; i++) 
            c_out("[%s]", c_gentmp_dim(i));
		
		c_out(" = %s%s.",prefix,"mr");

		if (sp->ptype->ident <= INT)
            c_out("_iout");
		else if (sp->ptype->ident == FLOAT)
            c_out("_fout");
		else if (sp->ptype->ident == DOUBLE) 
            c_out("_dout");
		else if (sp->ptype->ident == CLASS_TYPE) 
            c_out("(%s)_o",sp->ptype->name);
		c_out(";\n");
	}

    /* trace failed lookup */
    if (gen_trace) 
        c_trace_failed_map(map, sp, dims);

    c_identlevel--;
    c_outi("}\n");
   
    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi(sp->plengthof->name, prefix);
        if (dims > 0) 
            c_array_proper(sp->plengthof, sp->plengthof->dims);
        c_out(" = %sparse;\n", prefix);
    }

    /* if a value is specified, check the parsed value against it */
    if (ip != NULL) {
        c_outi("if (%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" != ");
        c_expression(ip, 0);
        c_out(") {\n");
        c_identlevel++;
        c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        if (cpp()) {
            if (dims > 0) 
                for (i=0; i<dims; i++) 
                    c_out("[%%d]");
            c_out("'\"");
            if (dims > 0) {
                c_out(", ");
                for (i=0; i<dims; i++) {
                    c_out("%s", c_gentmp_dim(i));
                    if (i != dims-1) 
                        c_out(", ");
                }
            }
        }
        else if (java()) {
            if (dims > 0) 
                for (i=0; i<dims; i++) 
                    c_out("[\" + %s + \"]", c_gentmp_dim(i));
            c_out("'\");\n");
        }

        /* trace, flagging mismatch */
        if (gen_trace) 
            c_trace_var_map(sp, map, dims, usenext, 1);

        c_identlevel--;
        c_outi("}\n");

        if (gen_trace) {
            c_outi("else {\n");
            c_identlevel++;
            c_trace_var_map(sp, map, dims, usenext, 0);
            c_identlevel--;
            c_outi("}\n");
        }
    }
    else 
        if (gen_trace) 
            c_trace_var_map(sp, map, dims, usenext, 0);
}

/* generate the get part of parsable variable declarations (no class or bac) */
static void c_var_decl_get_parse(symbol_t *sp, node_t *init, node_t *parse, int dims, node_t **dim, int usenext)
{
    node_t *pp, *ip;
    if (sp->modifiers & M_CONST || sp->range) {
        pp = sp->parse;
        ip = sp->init;
    }
    else {
        pp = parse;
        ip = init;
    }

    /* sanity check */
    if (pp == NULL) 
        fatal("Internal error CV%d: Expected parsable variable", __LINE__);

    /* if parse size is 0, we don't call getbits(); we just set the initial value, if any */
    if (is_literal(pp->op)) {
        if (pp->op != INT_LITERAL) 
            fatal("Internal error CV%d: Expected integer literal", __LINE__);

        if (pp->left.ival == 0) {
            if (ip != NULL) {
                /* just set initial value, if any */
                if (dims > 0) 
                    c_array_prologue(sp, dims, dim);
                c_outi("%s", sp->name);
                if (dims > 0) 
                    c_array_proper(sp, dims);
                c_out(" = ");
                c_expression(ip, 0);
                c_out(";\n");
                /* update lengthof */
                if (sp->lengthof) {
                    c_outi(sp->plengthof->name, prefix);
                    if (dims > 0)
                        c_array_proper(sp->plengthof, sp->plengthof->dims);
                    c_out(" = 0;\n");
                }
                /* done */
                if (dims > 0) 
                    c_array_epilogue(sp, dims, dim);
            }
            return;
        }
        /* we fall over to the code below */
    }

    /* parse size non-zero or not a constant */
    
    if (dims > 0) 
        c_array_prologue(sp, dims, dim);

    /* call appropriate function depending on map/simple expression for parse expression */
    if (pp->op == MAP_TYPE) 
        c_var_decl_get_parse_map(sp, ip, pp->left.sp, dims, dim, usenext);
    else 
        c_var_decl_get_parse_simple(sp, ip, pp, dims, dim, usenext);

    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/* generate variable declarations/definitions (get() part) */
void c_var_decl_get(symbol_t    *sp,        /* symbol to be declared/defined */
                    node_t      *init,      /* initialization expression */ 
                    node_t      *parse,     /* parse expression */
                    node_t      *param,     /* parameters */
                    node_t      *align,     /* alignment expression */ 
                    int         dims,       /* array dimensions for the symbol */
                    node_t      **dim, 
                    int         usenext)    /* if > 0 --> lookahead parsing */ 
{
    if (sp == NULL) return;
    
    /* declare, if necessary */
    if (!sp->declared_get) {
        c_var_decl(sp, dims, dim);
        sp->declared_get = 1;
    }

    /* see if we need to align */
    if (align != NULL) {
        if (gen_trace) {
            /* save parse size */
            c_outi("%sparse = ", prefix);
            c_expression(align, 0);
            c_out(";\n");
            /* do the align */
            c_outi("%sbs.align(%sparse);\n", prefix, prefix);
            /* trace */
            c_trace_aligned();
        }
        else {
            c_outi("%sbs.align(", prefix);
            c_expression(align, 0);
            c_out(");\n");
        }
    }

    /* class variable */
    if (sp->ptype->ident == CLASS_TYPE) {
        c_var_decl_get_class(sp, parse, param, dims, dim, usenext);
        return;
    }
    /* binary-arithmetically coded variable */
    if (sp->parsable && parse && parse->op == BAC_TYPE) {
        c_var_decl_get_bac(sp, param, parse, dims, dim, usenext);
        return;
    }
    /* all other parsable variables */
    if (sp->parsable) {
        c_var_decl_get_parse(sp, init, parse, dims, dim, usenext);
        return;
    }
    /* regular variable; only relevant if initial value is given */
    if (init != NULL || sp->init != NULL) {
        if (dims > 0) 
            c_array_prologue(sp, dims, dim);
        c_outi("%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" = ");
        /* if a constant, use the symbol's init; otherwise the expression's */
        if (sp->modifiers & M_CONST) 
            c_expression(sp->init, 0);
        else
            c_expression(init, 0);
        c_out(";\n");
        if (dims > 0) 
            c_array_epilogue(sp, dims, dim);
    }
}

/***********************************************/
/* variable declarations/definitions for put() */
/***********************************************/

/* generate the put part of parsable variable declarations (coded using a bac) */
static void c_var_decl_put_bac(symbol_t *sp, node_t *param, node_t *parse, int dims, node_t **dim, int usenext)
{
    symbol_t *bac = parse->left.sp;

    if (dims > 0) 
        c_array_prologue(sp, dims, dim);
    
    c_outi("%s%sae_code(%sbs, ", bac->name, c_static_scope(), prefix);

    c_out("%s, ", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);

    c_expression(param, 0);
    c_out(");\n");

    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/* generate the put part of variable declarations for simple parsables */
static void c_var_decl_put_parse_simple(symbol_t *sp, node_t *ip, node_t *pp, int dims, node_t **dim)
{
    /* see which put function to call */
    c_outi("%sbs.", prefix);
    if (sp->ptype->ident <= INT) {
        if (java() && sp->modifiers & M_LONG) 
            c_out("%sputlong(%s", (sp->modifiers & M_LITTLE ? "little_" : ""), sp->name);
        else 
            c_out("%sputbits(%s", (sp->modifiers & M_LITTLE ? "little_" : ""), sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(", ");
        if (sp->lengthof || ip != NULL) 
            c_out("%sparse", prefix);
        else 
            c_expression(pp, 0);
    }
    else if (sp->ptype->ident == FLOAT) {
        c_out("%sputfloat(%s", (sp->modifiers & M_LITTLE ? "little_" : ""), sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
    }
    else if (sp->ptype->ident == DOUBLE && (sp->modifiers & M_LONG)) {
        c_out("%sputldouble(%s", (sp->modifiers & M_LITTLE ? "little_" : ""), sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
    }
    else {
        c_out("%sputdouble(%s", (sp->modifiers & M_LITTLE ? "little_" : ""), sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
    }
    c_out(");\n");
}

/* generate the put part of variable declarations for map-based parsables */
static void c_var_decl_put_parse_map(symbol_t *sp, node_t *ip, symbol_t *map, int dims, node_t **dim, int usenext)
{
	int i;

    /* call putvlc() */
    if (cpp()) {
		c_outi("if (%s%s%s(%sbs, %s", map->name, c_static_scope(), "putvlc", prefix, sp->name);
		if (dims > 0)
            c_array_proper(sp, dims);
		/* if we need lengthof, get number of bits */ 
		if (sp->lengthof) 
            c_out(", &%sparse", prefix);
		c_out(") == 0) {\n");
	}
	else if (java()) {
		c_outi("%smr = %s%s%s(%sbs, %s", prefix, map->name, c_static_scope(), "putvlc", prefix, sp->name);
		if (dims > 0) 
            c_array_proper(sp, dims);
		c_out(");\n");
		c_outi("if (!%smr.isHit()) {\n",prefix);
	}

	/* lookup failed */
	c_identlevel++;
	c_outi("%s(\"Map '%s' lookup for '%s", err_func, map->name, sp->name);
	if (cpp()) {
		if (dims > 0) 
            for (i=0; i<dims; i++) 
                c_out("[%%d]");
		c_out("' failed\"");
		if (dims > 0) {
			c_out(", ");
			for (i=0; i<dims; i++) {
				c_out("%s", c_gentmp_dim(i));
				if (i != dims-1) 
                    c_out(", ");
			}
		}
		c_out(");\n");
	}
	else if (java()) {
		for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("' failed\");\n");
	}
    /* increase error count */ 
    c_outi("%sret++;\n", prefix);
    c_identlevel--;
    c_outi("}\n");
}

/* generate the put part of parsable variable declarations (no class or bac) */
static void c_var_decl_put_parse(symbol_t *sp, node_t *init, node_t *parse, int dims, node_t **dim, int usenext)
{
    node_t *pp, *ip;
    if (sp->modifiers & M_CONST || sp->range) {
        pp = sp->parse;
        ip = sp->init;
    }
    else {
        pp = parse;
        ip = init;
    }
    
    /* if parse size is 0, we don't call putbits(); we just set the initial value, if any */
    if (is_literal(pp->op)) {
        if (pp->op != INT_LITERAL) 
            fatal("Internal error CV%d: Expected integer literal", __LINE__);

        if (pp->left.ival == 0) {
            if (ip != NULL) {
                /* just set initial value, if any */
                if (dims > 0) 
                    c_array_prologue(sp, dims, dim);
                c_outi("%s", sp->name);
                if (dims > 0) 
                    c_array_proper(sp, dims);
                c_out(" = ");
                if (sp->ptype->ident == FLOAT)
                    c_out("(float)");
                c_expression(ip, 0);
                c_out(";\n");
                /* update lengthof */
                if (sp->lengthof) {
                    c_outi(sp->plengthof->name, prefix);
                    if (dims > 0) 
                        c_array_proper(sp->plengthof, sp->plengthof->dims);
                    c_out(" = 0;\n");
                }
                /* done */
                if (dims > 0) 
                    c_array_epilogue(sp, dims, dim);
            }
            return;
        }
        /* we fall over to the code below */
    }

    /* parse size non-zero */
    
    if (dims > 0) 
        c_array_prologue(sp, dims, dim);
    
    /* if we use lengthof, or there is initial value, save parse (if not a map) */
    if (pp->op != MAP_TYPE && (sp->lengthof || ip != NULL)) {
        /* Set the parse size before computing the init.
         * This is crucial, to preserve the order of expression
         * evaluation between get and put (parse expressions come first).
         * Not needed if type is greater than 'int', unless doing lengthof().
         */
        if (sp->lengthof || sp->ptype->ident <= INT) {
            c_outi("%sparse = ", prefix);
            c_expression(pp, 0);
            c_out(";\n");
        }
    }
    /* if a value is specified, set the var beforehand */
    if (ip != NULL) {
        /* set init */
        if (ip != NULL) {
            /* id range; in this case, the ? : operator is not used */
            if (sp->range) {
                c_outi("if (!(");
                c_expression(ip, 0);
                c_out(" <= %s && %s <= ", sp->name, sp->name);
                c_expression(ip->third.np, 0);
                c_out(")) {\n");
                c_identlevel++;
                c_outi("%s", sp->name);
                if (dims > 0) 
                    c_array_proper(sp, dims);
                c_out(" = ");
                c_expression(ip, 0);
                c_out(";\n");
                c_identlevel--;
                c_outi("}\n");
            }
            else {
                c_outi("%s", sp->name);
                if (dims > 0) 
                    c_array_proper(sp, dims);
                c_out(" = ");
                if (sp->ptype->ident == FLOAT)
                    c_out("(float)");
                c_expression(ip, 0);
                c_out(";\n");
            }
        }
    }

    if (pp->op == MAP_TYPE) 
        /* we have to do map no matter what so that it can be checked */
        c_var_decl_put_parse_map(sp, ip, pp->left.sp, dims, dim, usenext);
    else
        /* do simple only of we are not doing nextbits */
        if (!usenext) 
            c_var_decl_put_parse_simple(sp, ip, pp, dims, dim);
    
    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi(sp->plengthof->name, prefix);
        if (dims > 0) 
            c_array_proper(sp->plengthof, sp->plengthof->dims);
        c_out(" = %sparse;\n", prefix);
    }
    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/* generate variable declarations/definitions (put() part) */
void c_var_decl_put(symbol_t    *sp,        /* symbol to be declared/defined */
                    node_t      *init,      /* initialization expression */
                    node_t      *parse,     /* parse expression */
                    node_t      *param,     /* parameters */
                    node_t      *align,     /* alignment expression */
                    int         dims,       /* array dimensions for the symbol */
                    node_t      **dim,  
                    int         usenext)    /* if > 0 --> lookahead parsing */ 
{
    if (sp == NULL) return;
    
    /* declare, if necessary */
    if (!sp->declared_put) {
        c_var_decl(sp, dims, dim);
        sp->declared_put = 1;
    }

    /* see if we need to align */
    if (align != NULL) {
        c_outi("%sbs.align(", prefix);
        c_expression(align, 0);
        c_out(");\n");
    }
    
    /* class variable */
    if (sp->ptype->ident == CLASS_TYPE) {
        c_var_decl_put_class(sp, param, parse, dims, dim, usenext);
        return;
    }
    /* binary-arithmetically coded variable */
    if (sp->parsable && parse && parse->op == BAC_TYPE) {
        c_var_decl_put_bac(sp, param, parse, dims, dim, usenext);
        return;
    }
    /* all other parsable variables */
    if (sp->parsable) {
        c_var_decl_put_parse(sp, init, parse, dims, dim, usenext);
        return;
    }
    /* regular variable; only relevant if initial value is given */
    if (init != NULL || sp->init != NULL) {
        if (dims > 0) 
            c_array_prologue(sp, dims, dim);
        c_outi("%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" = ");
        /* if a constant, use the symbol's init; otherwise the expression's */
        if (sp->modifiers & M_CONST) 
            c_expression(sp->init, 0);
        else 
            c_expression(init, 0);
        c_out(";\n");
        if (dims > 0) 
            c_array_epilogue(sp, dims, dim);
    }
}

/**************************************************/
/* variable declarations/definitions for putxml() */
/**************************************************/

/* generate the putxml part of parsable variable declarations (coded using a bac) */
static void c_var_decl_putxml_bac(symbol_t *sp, node_t *param, node_t *parse, int dims, node_t **dim, int usenext)
{
    symbol_t *bac = parse->left.sp;
    
    if (gen_trace) 
        c_trace_entering_mapped_class(sp, dims, bac);

    /* signal the beginning of parsing the class */
    if (cpp()) 
        c_outi("%s(\"%s\", 0);\n", xml_func4, sp->name);
    else if (java())
        c_outi("XML.%s(\"%s\", 0);\n", xml_func4, sp->name);

    c_outi("%s", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    c_out(" = %s%sad_code(%sbs, ", bac->name, c_static_scope(), prefix);
    c_expression(param, 0);
    c_out(");\n");

    /* signal the end of parsing the class */
    if (cpp()) 
        c_outi("%s(\"</%s>\");\n", xml_func5, sp->name);
    else if (java())
        c_outi("XML.%s(\"</%s>\");\n", xml_func5, sp->name);
}

/* generate the putxml part of variable declarations for simple parsables */
static void c_var_decl_putxml_parse_simple(symbol_t *sp, node_t *ip, node_t *pp, node_t *align, int dims, node_t **dim, int usenext)
{
    int i;
   
    /* for generating the putxml() method, save parse size */
    c_outi("%sparse = ", prefix);
    c_expression(pp, 0);
    c_out(";\n");
    
    c_outi("%s", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    
    /* see which get function to call */
    c_out(" = ");

    /* java needs explicit cast */
    if (java())
        if (sp->ptype->ident != BIT && sp->ptype->ident != INT) 
            c_out("(%s) ", sp->ptype->name);

    c_out("%sbs.%s%s%s", prefix, 
	                     (sp->modifiers & M_LITTLE ? "little_" : ""),
	                     (sp->modifiers & M_UNSIGNED || sp->ptype->ident > INT ? "" : "s"), 
                         (usenext ? "next" : "get"));
   
    if (sp->ptype->ident <= INT) {
        if (java() && sp->modifiers & M_LONG) 
            c_out("long(");
        else 
            c_out("bits(");
        c_out("%sparse", prefix);
    }
    else if (sp->ptype->ident == FLOAT) 
        c_out("float(");
    else if (sp->ptype->ident == FLOAT && (sp->modifiers & M_LONG)) 
        c_out("ldouble(");
    else 
        c_out("double(");
    
    c_out(");\n");
    
    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi(sp->plengthof->name, prefix);
        if (dims > 0) 
            c_array_proper(sp->plengthof, sp->plengthof->dims);
        c_out(" = %sparse;\n", prefix);
    }

    /* if a value is specified, check the parsed value against it */
    if (ip != NULL) {

        /* id range; in this case, the ? : operator is not used */
        if (sp->range) {
            c_outi("if (!(");
            c_expression(ip, 0);
            c_out(" <= %s && %s <= ", sp->name, sp->name);
            c_expression(ip->third.np, 0);
            c_out(")) {\n");
            c_identlevel++;
           
            c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        
            if (cpp()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[%%d]");
                c_out("'\"");
                if (dims > 0) {
                    c_out(", ");
                    for (i=0; i<dims; i++) {
                        c_out("%s", c_gentmp_dim(i));
                        if (i != dims-1) 
                            c_out(", ");
                    }
                }
            }
            else if (java()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[\" + %s + \"]", c_gentmp_dim(i));
                c_out("'\"");
            }
            c_out(");\n");

            /* increase error count */
            c_outi("%sret++;\n", prefix);
        
            /* output the corresponding XML element */
            if (!usenext) 
                c_xml_var_simple(sp, align, dims, dim);

            if (gen_trace) 
                c_trace_var_simple(sp, dims, usenext, 1);
            
            c_identlevel--;
            c_outi("}\n");
        
            if (!usenext) {
                c_outi("else {\n");
                c_identlevel++;
                c_xml_var_simple(sp, align, dims, dim);
                if (gen_trace)
                    c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
            else if (gen_trace) {
                c_outi("else {\n");
                c_identlevel++;
                c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
        }
        else {
            c_outi("if (%s", sp->name);
            if (dims > 0)
                c_array_proper(sp, dims);
            c_out(" != ");
            if (sp->ptype->ident == FLOAT) 
                c_out("(float)");
            c_expression(ip, 0);
            c_out(") {\n");
            c_identlevel++;
 
            c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        
            if (cpp()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[%%d]");
                c_out("'\"");
                if (dims > 0) {
                    c_out(", ");
                    for (i=0; i<dims; i++) {
                        c_out("%s", c_gentmp_dim(i));
                        if (i != dims-1) 
                            c_out(", ");
                    }
                }
            }
            else if (java()) {
                if (dims > 0) 
                    for (i=0; i<dims; i++) 
                        c_out("[\" + %s + \"]", c_gentmp_dim(i));
                c_out("'\"");
            }
            c_out(");\n");

            /* increase error count */
            c_outi("%sret++;\n", prefix);
        
            if (!usenext) 
                c_xml_var_simple(sp, align, dims, dim);
            else if (gen_trace) 
                c_trace_var_simple(sp, dims, usenext, 1);
        
            c_identlevel--;
            c_outi("}\n");
                
            if (!usenext) {
                c_outi("else {\n");
                c_identlevel++;
                c_xml_var_simple(sp, align, dims, dim);
                if (gen_trace)
                    c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
            else if (gen_trace) {
                c_outi("else {\n");
                c_identlevel++;
                c_trace_var_simple(sp, dims, usenext, 0);
                c_identlevel--;
                c_outi("}\n");
            }
        }    
    }
    else {
        if (!usenext) 
            c_xml_var_simple(sp, align, dims, dim);
        else if (gen_trace)
            c_trace_var_simple(sp, dims, usenext, 0);
    }
}

/* generate the putxml part of variable declarations for map-based parsables */
static void c_var_decl_putxml_parse_map(symbol_t *sp, node_t *ip, symbol_t *map, int dims, node_t **dim, int usenext)
{
    int i;

	if (gen_trace) 
        c_trace_entering_mapped_class(sp, dims, map);

    /* signal the beginning of parsing the class */
    if (cpp())
        c_outi("%s(\"%s\", 0);\n", xml_func4, sp->name);
    else if (java()) 
        c_outi("XML.%s(\"%s\", 0);\n", xml_func4, sp->name);

	if cpp() {
    	c_outi("if (%s%s%s(%sbs, bAttr, &%s", map->name, c_static_scope(), "putxml", prefix, sp->name);
    
        if (dims > 0) 
            c_array_proper(sp, dims);
		/* if we are tracing or need lengthof, get number of bits */
		if (gen_trace || sp->lengthof) 
            c_out(", &%sparse", prefix);
		/* if we are tracing, we also need code value */
		if (gen_trace) 
            c_out(", &%scode", prefix);
		c_out(") == 0) {\n");
	}
    else if (java()) {
   		c_outi("%smr = %s%s%s(%sbs, bAttr);\n", prefix, map->name, c_static_scope(), "putxml", prefix);
		
        if (gen_trace) 
            c_outi("%scode = %smr.getCode();\n",prefix,prefix);
		if (gen_trace || sp->lengthof) 
            c_outi("%sparse = %smr.getCodeLength();\n",prefix,prefix);
		c_outi("if (!%smr.isHit()) {\n",prefix);
    }

    /* lookup failed */
    c_identlevel++;
    c_outi("%s(\"Map '%s' lookup for '%s", err_func, map->name, sp->name);
    if (cpp()) {
        if (dims > 0) 
            for (i=0; i<dims; i++) 
                c_out("[%%d]");
        c_out("' failed\"");
        if (dims > 0) {
            c_out(", ");
            for (i=0; i<dims; i++) {
                c_out("%s", c_gentmp_dim(i));
                if (i != dims-1) 
                    c_out(", ");
            }
        }
        c_out(");\n");
    }
    else if (java()) {
		for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("' failed\");\n");
    }
    /* increase error count */
    c_outi("%sret++;\n", prefix);

	if (java()) {
		c_identlevel--;
		c_outi("}\n");
		c_outi("else {\n");
		c_identlevel++;
		
		c_outi(sp->name);
		for (i=0; i<dims; i++) 
            c_out("[%s]", c_gentmp_dim(i));
		
		c_out(" = %s%s.", prefix, "mr");

		if (sp->ptype->ident <= INT) 
            c_out("_iout");
		else if (sp->ptype->ident == FLOAT) 
            c_out("_fout");
		else if (sp->ptype->ident == DOUBLE) 
            c_out("_dout");
		else if (sp->ptype->ident == CLASS_TYPE) 
            c_out("(%s)_o",sp->ptype->name);
		c_out(";\n");
	}

    /* trace failed lookup */
    if (gen_trace) 
        c_trace_failed_map(map, sp, dims);

    c_identlevel--;
    c_outi("}\n");
   
    /* signal the end of parsing the class */
    if (cpp()) 
        c_outi("%s(\"</%s>\");\n", xml_func5, sp->name);
    else if (java())
        c_outi("XML.%s(\"</%s>\");\n", xml_func5, sp->name);

    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi(sp->plengthof->name, prefix);
        if (dims > 0) 
            c_array_proper(sp->plengthof, sp->plengthof->dims);
        c_out(" = %sparse;\n", prefix);
    }

    /* if a value is specified, check the parsed value against it */
    if (ip != NULL) {
        c_outi("if (%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" != ");
        c_expression(ip, 0);
        c_out(") {\n");
        c_identlevel++;
        c_outi("%s(\"Const value mismatch for '%s", err_func, sp->name);
        if (cpp()) {
            if (dims > 0) 
                for (i=0; i<dims; i++) 
                    c_out("[%%d]");
            c_out("'\"");
            if (dims > 0) {
                c_out(", ");
                for (i=0; i<dims; i++) {
                    c_out("%s", c_gentmp_dim(i));
                    if (i != dims-1) 
                        c_out(", ");
                }
            }
        }
        else if (java()) {
            if (dims > 0) 
                for (i=0; i<dims; i++) 
                    c_out("[\" + %s + \"]", c_gentmp_dim(i));
            c_out("'\");\n");
        }

        /* trace, flagging mismatch */
        if (gen_trace)
            c_trace_var_map(sp, map, dims, usenext, 1);

        c_identlevel--;
        c_outi("}\n");

        if (gen_trace) {
            c_outi("else {\n");
            c_identlevel++;
            c_trace_var_map(sp, map, dims, usenext, 0);
            c_identlevel--;
            c_outi("}\n");
        }
    }
    else 
        if (gen_trace) 
            c_trace_var_map(sp, map, dims, usenext, 0);
}

/* generate the putxml part of parsable variable declarations (no class or bac) */
static void c_var_decl_putxml_parse(symbol_t *sp, node_t *init, node_t *parse, node_t *align, int dims, node_t **dim, int usenext)
{
    node_t *pp, *ip;
    if (sp->modifiers & M_CONST || sp->range) {
        pp = sp->parse;
        ip = sp->init;
    }
    else {
        pp = parse;
        ip = init;
    }

    /* sanity check */
    if (pp == NULL) 
        fatal("Internal error CV%d: Expected parsable variable", __LINE__);

    /* if parse size is 0, we don't call getbits(); we just set the initial value, if any */
    if (is_literal(pp->op)) {
        if (pp->op != INT_LITERAL) 
            fatal("Internal error CV%d: Expected integer literal", __LINE__);
        if (pp->left.ival == 0) {
            if (ip != NULL) {
                /* just set initial value, if any */
                if (dims > 0) 
                    c_array_prologue(sp, dims, dim);
                c_outi("%s", sp->name);
                if (dims > 0) 
                    c_array_proper(sp, dims);
                c_out(" = ");
                c_expression(ip, 0);
                c_out(";\n");
                /* update lengthof */
                if (sp->lengthof) {
                    c_outi(sp->plengthof->name, prefix);
                    if (dims > 0)
                        c_array_proper(sp->plengthof, sp->plengthof->dims);
                    c_out(" = 0;\n");
                }
                /* done */
                if (dims > 0) 
                    c_array_epilogue(sp, dims, dim);
            }
            return;
        }
        /* we fall over to the code below */
    }

    /* parse size non-zero or not a constant */
    
    if (dims > 0) 
        c_array_prologue(sp, dims, dim);

    /* call appropriate function depending on map/simple expression for parse expression */
    if (pp->op == MAP_TYPE) 
        c_var_decl_putxml_parse_map(sp, ip, pp->left.sp, dims, dim, usenext);
    else 
        c_var_decl_putxml_parse_simple(sp, ip, pp, align, dims, dim, usenext);

    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/* generate variable declarations/definitions (putxml() part) */
void c_var_decl_putxml(symbol_t    *sp,        /* symbol to be declared/defined */
                       node_t      *init,      /* initialization expression */ 
                       node_t      *parse,     /* parse expression */
                       node_t      *param,     /* parameters */
                       node_t      *align,     /* alignment expression */ 
                       int         dims,       /* array dimensions for the symbol */
                       node_t      **dim, 
                       int         usenext)    /* if > 0 --> lookahead parsing */ 
{
    if (sp == NULL) return;
    
    /* declare, if necessary */
    if (!sp->declared_putxml) {
        c_var_decl(sp, dims, dim);
        sp->declared_putxml = 1;
    }

    /* see if we need to align */
    if (align != NULL) {
        if (gen_trace) {
            /* save parse size */
            c_outi("%sparse = ", prefix);
            c_expression(align, 0);
            c_out(";\n");
            /* do the align */
            c_outi("%sbs.align(%sparse);\n", prefix, prefix);
            /* trace */
            c_trace_aligned();
        }
        else {
            c_outi("%sbs.align(", prefix);
            c_expression(align, 0);
            c_out(");\n");
        }
    }

    /* class variable */
    if (sp->ptype->ident == CLASS_TYPE) {
        c_var_decl_putxml_class(sp, parse, param, dims, dim, usenext);
        return;
    }
    /* binary-arithmetically coded variable */
    if (sp->parsable && parse && parse->op == BAC_TYPE) {
        c_var_decl_putxml_bac(sp, param, parse, dims, dim, usenext);
        return;
    }
    /* all other parsable variables */
    if (sp->parsable) {
        c_var_decl_putxml_parse(sp, init, parse, align, dims, dim, usenext);
        return;
    }
    /* regular variable; only relevant if initial value is given */
    if (init != NULL || sp->init != NULL) {
        if (dims > 0) 
            c_array_prologue(sp, dims, dim);
        c_outi("%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" = ");
        /* if a constant, use the symbol's init; otherwise the expression's */
        if (sp->modifiers & M_CONST) 
            c_expression(sp->init, 0);
        else
            c_expression(init, 0);
        c_out(";\n");
        if (dims > 0) 
            c_array_epilogue(sp, dims, dim);
    }
}

/***********************************************/
/* variable declarations/definitions for xsd() */
/***********************************************/

/* generate the XML element description of variable declarations for simple parsables */
static void c_var_decl_xsd_parse_simple(symbol_t *sp, node_t *ip, node_t *pp, node_t *align, int dims)
{
    c_outi("<xsd:element name=\"%s\" type=", sp->name);

	if (sp->ptype->ident == CHAR) {
		if (sp->modifiers & M_UNSIGNED)
			c_out("\"flUChar\"");
		else
			c_out("\"flSChar\"");
	}
	else {
        if (sp->modifiers & M_LITTLE) {
            if (sp->ptype->ident <= INT) {
   	            if (sp->modifiers & M_UNSIGNED)
        			c_out("\"flUInt\"");
                else 
        			c_out("\"flSInt\"");
            }
            else if (sp->ptype->ident == FLOAT)
                c_out("\"flFloat\"");
			else if (sp->ptype->ident == DOUBLE)
                c_out("\"flDouble\"");
            else 
                fatal("Internal error CV%d: Expected simple type", __LINE__);
		}
		else {
    		if (sp->ptype->ident <= INT) {
   	            if (sp->modifiers & M_UNSIGNED)
        			c_out("\"flUInt\"");
                else 
        			c_out("\"flSInt\"");
            }
            else if (sp->ptype->ident == FLOAT)
                c_out("\"flFloat\"");
			else if (sp->ptype->ident == DOUBLE)
                c_out("\"flDouble\"");
            else 
                fatal("Internal error CV%d: Expected simple type", __LINE__);
		}
	}

    if (dims > 0 && ip == NULL)
        c_out(" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n");
    else
        c_out("/>\n");
}

/* generate the XML schema description of variable declarations for map-based parsables */
static void c_var_decl_xsd_parse_map(symbol_t *sp, node_t *ip, symbol_t *map, node_t *align, int dims)
{
    if (dims > 0 && ip == NULL)
        c_outi("<xsd:element name=\"%s\" type=\"%s\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n", sp->name, map->name);
    else 
	    c_outi("<xsd:element name=\"%s\" type=\"%s\"/>\n", sp->name, map->name);
}

/* generate the XML element schema description for parsable variable declarations */
static void c_var_decl_xsd_parse(symbol_t *sp, node_t *init, node_t *parse, node_t *align, int dims, node_t **dim)
{
    node_t *pp, *ip;
    if (sp->modifiers & M_CONST || sp->range) {
        pp = sp->parse;
        ip = sp->init;
    }
    else {
        pp = parse;
        ip = init;
    }
    /* sanity check */
    if (pp == NULL) 
        fatal("Internal error CV%d: Expected parsable variable", __LINE__);

    /* call appropriate function depending on map/simple expression for parse expression */
    if (pp->op == MAP_TYPE) 
        c_var_decl_xsd_parse_map(sp, ip, pp->left.sp, align, dims);
    else 
        c_var_decl_xsd_parse_simple(sp, ip, pp, align, dims);
}

/* generate the XML schema description for variable declarations */
void c_var_decl_xsd(symbol_t *sp,
                    node_t *init, 
                    node_t *parse, 
                    node_t *param, 
                    node_t *align, 
                    int dims, 
                    node_t **dim, 
                    int usenext)
{
	if (!usenext) {
		if (sp == NULL) return;
		/* if class variable, special handling (to resolve IDs etc.) */
		if (sp->ptype->ident == CLASS_TYPE) {
	        c_var_decl_xsd_class(sp, parse, param, dims);
		    return;
		}
		/* not a class; if it is parsable, special handling */
		if (sp->parsable) {
			c_var_decl_xsd_parse(sp, init, parse, align, dims, dim);
			return;
		}
	}
}

