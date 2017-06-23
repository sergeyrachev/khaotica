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

/* cmap.c */

#include "cdef.h"
 

/*********************************************/
/* map declarations/definitions for getvlc() */
/*********************************************/

/* get an escape value */
static void c_map_get_assign_esc(list_t *entry)
{
    node_t *esc_type = entry->e2;
    unsigned mod = entry->e1->left.ival;

    /* no map cascades at this time */
    if (entry->type == flavor::Parser::token::ESC_MAP)
        fatal("Internal error CM%d: Map cascading not supported", __LINE__);
    
    /* explicit casting is needed for Java */
	if (java()) 
        if (esc_type->ptype->ident != flavor::Parser::token::BIT && esc_type->ptype->ident != flavor::Parser::token::INT)
            c_out("(%s) ", esc_type->ptype->name);

    /* parse length should be a literal */
    if (!is_literal(entry->e3->op)) {
        c_out("0"); /* error already reported - just assign 0 */
        return;
    }

    c_out("%sbs.%s%sget", prefix, 
	      (mod & M_LITTLE ? "little_" : ""),
	      (mod & M_UNSIGNED || esc_type->ptype->ident > flavor::Parser::token::INT ? "" : "s"));

    if (esc_type->ptype->ident <= flavor::Parser::token::INT) {
        if (java() && esc_type->ptype->modifiers & M_LONG)
            c_out("long(%sesc_bits)", prefix);
        else 
            c_out("bits(%sesc_bits)", prefix);
    }
    else if (esc_type->ptype->ident == flavor::Parser::token::FLOAT)
        c_out("float()");
    else if (esc_type->ptype->ident == flavor::Parser::token::DOUBLE && (esc_type->ptype->modifiers & M_LONG))
        c_out("ldouble()");
    else 
        c_out("double()");    
}

/* output assignment statements for a simple map entry */
static void c_map_get_assign_simple(symbol_t* sp, list_t* entry)
{
    list_t *p = entry->sub1;

    if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP) {
        c_outi("%sesc_bits = ", prefix);
        c_expression(p->e3, 0);
        c_out(";\n");
    }

    if (cpp()) 
        c_outi("*%sarg = ", prefix);
    else if (java())
        c_outi("%sarg = ", prefix);

    /* figure out if we have simple assignment or escape */
    if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP)
        c_map_get_assign_esc(p);
    else {
		if (sp->ptype->ident == flavor::Parser::token::FLOAT && p->e1->type == flavor::Parser::token::DOUBLE)
            c_out("(float)");
		c_expression(p->e1, 0);
	}
    c_out(";\n");
}

/* output assignment statements for a class map entry */
static void c_map_get_assign_class(symbol_t *sp, list_t *entry)
{
    list_t *p;

    /* find the statements for this class */
    list_t *stmts = find_class_stmts(sp->ptype);
    if (stmts == NULL) 
        fatal("Internal error CM%d: Cannot find declaration of class '%s'", __LINE__, sp->name);

    if (entry->sub1->type == flavor::Parser::token::ESC_FTYPE || entry->sub1->type == flavor::Parser::token::ESC_MAP)
        p = entry->sub1;
    else
        p = entry->sub1->sub1;

    /* traverse declarations and output assignment code for each variable */
	while (stmts != NULL) {
        switch (stmts->type) {
        case flavor::Parser::token::DECL:
			if (p == NULL) return;
            if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP) {
                c_outi("%sesc_bits = ", prefix);
                c_expression(p->e3, 0);
                c_out(";\n");
            }

            c_outi("%sarg%s%s = ", prefix, c_scope(), stmts->sp->name);

            /* figure out if we have simple assignment or escape */
            if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP)
                c_map_get_assign_esc(p);
            else {
		        if (stmts->sp->ptype->ident == flavor::Parser::token::FLOAT && p->e1->type == flavor::Parser::token::DOUBLE)
                    c_out("(float)");
                c_expression(p->e1, 0);
            }
            c_out(";\n");

            p = p->next;
            break;
            
        default:
            return;
        }
        stmts = stmts->next;
    }
}

/* output assignment statements for a map entry */
static void c_map_get_assign(symbol_t *sp, list_t *entry)
{
    /* different handling depending on if we have simple or class type */
    if (sp->ptype->ident != flavor::Parser::token::CLASS_TYPE)
        c_map_get_assign_simple(sp, entry);
    else 
        c_map_get_assign_class(sp, entry);
}

/* Core code for getvlc/nextvlc - checks for a match against the map codes.  The routine uses 
 * two variables: _data and _size, that contain the matched code.  If no match is found, then 
 * the last code entry is used.  If we get a hit, the _hit variable is assigned 1, otherwise 
 * it is left untouched (previously set to 0).
 */
static void c_map_next_core(symbol_t *sp, list_t *stmts, int usenext)
{
    list_t *lp = stmts;
    int size;

    /* sanity check */
    if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::BIT_LITERAL || lp->e1->left.bit == NULL)
        fatal("Internal error CM%d: Expected code entry with bit literal", __LINE__);
    
    size = lp->e1->left.bit->len;
    
    /* get data */
    c_outi("%sdata = %sbs.nextbits(%d);\n", prefix, prefix, size);
    c_outi("switch (%sdata) {\n", prefix);
    while (lp != NULL) {
        /* sanity check */
        if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::BIT_LITERAL || lp->e1->left.bit == NULL)
            fatal("Internal error CM%d: Expected code entry with bit literal", __LINE__);

        /* did the size change? */
        if (lp->e1->left.bit->len != size) {
            c_outi("default:\n");
            c_identlevel++;
            c_map_next_core(sp, lp, usenext);
            c_identlevel--;
            break;
        }

        /* check for match */
        c_outi("case %d:\n", lp->e1->left.bit->val);
        c_identlevel++;

        /* skip bits, if we are doing get - must be done here so that escapes read correctly */
        if (!usenext)
            c_outi("%sbs.skipbits(%d);\n", prefix, size);

        /* output assignment code */
        c_map_get_assign(sp, lp);
        
        /* save size */
        c_outi("%ssize = %d;\n", prefix, lp->e1->left.bit->len);

        c_outi("break;\n");
        c_identlevel--;

        /* if at the end, set hit to 0 and use last entry */
        if (lp->next == NULL) {
            c_outi("default:\n");
            c_identlevel++;
            if (cpp()) 
                c_outi("%shit = 0;\n", prefix);
			else if (java())
                c_outi("%shit = false;\n", prefix);
            c_identlevel--;
        }
        /* next code */
        lp = lp->next;
    }
    /* close switch statement */
    c_outi("}\n");
}

/* generate map nextvlc method */
static void c_map_next(symbol_t* sp, list_t* stmts)
{
    c_out("\n");
    if (cpp()) {
        c_outi("int %s::nextvlc(%s &%sbs, %s%s *%sarg, int *%sparse, int *%scode) {\n", 
               sp->name, bitstream_class, prefix, 
               ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""), 
               sp->ptype->name, prefix, prefix, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        /* no escape code allowed if look-ahead parsing */
		c_outi("int %shit = 1;\n\n", prefix);
	}
    else if (java()) {
		c_outi("public static %s nextvlc(%s %sbs) throws IOException {\n", 
               map_result, bitstream_class, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        /* no escape code allowed if look-ahead parsing */
        c_outi("boolean %shit = true;\n", prefix);
		if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE)
			c_outi("%s %sarg = new %s();\n\n", sp->ptype->name, prefix, sp->ptype->name);
		else
			c_outi("%s %sarg = %s%s;\n\n",
                   sp->ptype->name, prefix, ((sp->ptype->ident == flavor::Parser::token::FLOAT) ? "(float)" : ""),
                   ((sp->ptype->ident <= flavor::Parser::token::INT) ? "0" : "0.0"));
    }

    /* output the core nextvlc code (series of nextbits with case statements) */
    c_map_next_core(sp, stmts, 1);

    /* return code and size, if requested */
    if (cpp()) {
        c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
        c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		c_outi("return %shit;\n", prefix);
	}
    else if (java()) 
		c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);
    
    c_identlevel--;
    c_outi("}\n");
}

/* generate map getvlc method */
static void c_map_get(symbol_t *sp, list_t *stmts)
{
	c_out("\n");
    if (cpp()) {
        c_outi("int %s::getvlc(%s &%sbs, %s%s *%sarg, int *%sparse, int *%scode) {\n", 
               sp->name, bitstream_class, prefix, 
               ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
               sp->ptype->name, prefix, prefix, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        c_outi("int %sesc_bits = 0;\n", prefix);
		c_outi("int %shit = 1;\n\n", prefix);
	}
    else if (java()) {	
		c_outi("public static %s getvlc(%s %sbs) throws IOException {\n", 
               map_result, bitstream_class, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        c_outi("int %sesc_bits = 0;\n", prefix);
		c_outi("boolean %shit = true;\n", prefix);
		if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE)
			c_outi("%s %sarg = new %s();\n\n", sp->ptype->name, prefix, sp->ptype->name);
		else
			c_outi("%s %sarg = %s%s;\n\n",
                   sp->ptype->name, prefix, ((sp->ptype->ident == flavor::Parser::token::FLOAT) ? "(float)" : ""),
                   ((sp->ptype->ident <= flavor::Parser::token::INT) ? "0":"0.0"));
	}
    
    /* output the core getvlc code (series of getbits with case statements) */
    c_map_next_core(sp, stmts, 0);

    /* return code and size, if requested */
    if (cpp()) {
        c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
        c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		c_outi("return %shit;\n", prefix);
	}
    else if (java())
        c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);
    
    c_identlevel--;
    c_outi("}\n");
}

/*********************************************/
/* map declarations/definitions for putvlc() */
/*********************************************/

/* put the simple value for the escape code */
static void c_map_put_esc_simple(symbol_t *sp, list_t *entry)
{
	node_t *pp = entry->e3;
	if (!is_literal(pp->op)) {
        c_out("0");
        return;
    }

	/* no map cascades at this time */
    if (entry->type == flavor::Parser::token::ESC_MAP)
        fatal("Internal error CM%d: Map cascading not supported", __LINE__);
	
    c_outi("%sbs.%s%s%s", prefix, 
	       (sp->modifiers & M_LITTLE ? "little_" : ""),
	       (sp->modifiers & M_UNSIGNED || sp->ptype->ident > flavor::Parser::token::INT ? "" : "s"), "put");

    if (sp->ptype->ident <= flavor::Parser::token::INT) {
        if (java() && sp->modifiers & M_LONG) 
            c_out("long(");
        else
            c_out("bits(%sarg,", prefix);
        c_expression(pp, 0);
		c_out(");\n");
    }
    else if (sp->ptype->ident == flavor::Parser::token::FLOAT)
        c_out("float(%sarg);\n", prefix);
    else if (sp->ptype->ident == flavor::Parser::token::DOUBLE && (sp->modifiers & M_LONG))
        c_out("ldouble(%sarg);\n", prefix);
    else
        c_out("double(%sarg);\n", prefix);
}

/* put the class value for the escape code */
static void c_map_put_esc_class(symbol_t *sp, list_t *entry)
{
	list_t *pp = entry;
    list_t *stmts = find_class_stmts(sp->ptype);
	if (stmts == NULL) 
        fatal("Internal error CM%d: Cannot find declaration of class '%s'", __LINE__, sp->name);
	
    /* no map cascades at this time */
    if (entry->type == flavor::Parser::token::ESC_MAP)
        fatal("Internal error CM%d: Map cascading not supported", __LINE__);
		
    while (stmts != NULL) {
        switch (stmts->type) {
		case flavor::Parser::token::DECL:
            /* declarations are ok, even parsable ones, but not with classes */
            if (stmts->sp->ptype->ident == flavor::Parser::token::CLASS_TYPE) return;
            if (pp == NULL) return;
          	if (!is_literal(pp->e3->op)) {
                c_out("0");
                return;
            }

            c_outi("%sbs.%sput", prefix, (stmts->sp->modifiers & M_LITTLE ? "little_" : ""));

            if (stmts->sp->ptype->ident <= flavor::Parser::token::INT) {
                if (java() && stmts->sp->modifiers & M_LONG)
                    c_out("long(%sarg%s%s,", prefix, c_scope(), stmts->sp->name);
                else 
                    c_out("bits(%sarg%s%s,", prefix, c_scope(), stmts->sp->name);
                c_expression(pp->e3, 0);
		        c_out(");\n");
            }
            else if (stmts->sp->ptype->ident == flavor::Parser::token::FLOAT)
                c_out("float(%sarg%s%s);\n", prefix, c_scope(), stmts->sp->name);
            else if (stmts->sp->ptype->ident == flavor::Parser::token::DOUBLE && (stmts->sp->modifiers & M_LONG))
                c_out("ldouble(%sarg%s%s);\n", prefix, c_scope(), stmts->sp->name);
            else 
                c_out("double(%sarg%s%s);\n", prefix, c_scope(), stmts->sp->name);

            pp = pp->next;
            break;
            
        default:
            /* everything else, isn't - has been reported already */
            return;
        }
        stmts = stmts->next;
    }
}

/* compare statements for a simple map entry */
static void c_map_put_compare_simple(symbol_t *sp, list_t *entry)
{
    c_out("%sarg == ", prefix);
	if (sp->ptype->ident == flavor::Parser::token::FLOAT && entry->e1->type == flavor::Parser::token::DOUBLE)
        c_out("(float)");
	c_expression(entry->e1, 0);
}

/* compare statements for a class map entry */
static void c_map_put_compare_class(symbol_t *sp, list_t *entry)
{
	 list_t *p = entry->sub1;
 
     /* find the statements for this class */
     list_t *stmts = find_class_stmts(sp->ptype);
     if (stmts == NULL)
        fatal("Internal error CM%d: Cannot find declaration of class '%s'", __LINE__, sp->name);
 
     /* traverse declarations and output assignment code for each variable */
	 while (stmts != NULL) {
        switch (stmts->type) {
		case flavor::Parser::token::DECL:
            /* declarations are ok, even parsable ones, but not with classes */
            if (stmts->sp->ptype->ident == flavor::Parser::token::CLASS_TYPE) return;
            if (p == NULL) return;

            c_out("(%sarg%s%s == ", prefix, c_scope(), stmts->sp->name);

            if (stmts->sp->ptype->ident == flavor::Parser::token::FLOAT && p->e1->type == flavor::Parser::token::DOUBLE)
                c_out("(float)");
            c_expression(p->e1, 0);
			c_out(")");
            if (stmts->next != NULL && !is_verbatim(stmts->next->type)) 
                c_out(" && ");
            p = p->next;
            break;
            
        default:
            /* everything else, isn't - has been reported already */
            return;
        }
        stmts = stmts->next;
    }
}

/* output compare statements for a map entry */
static void c_map_put_compare(symbol_t *sp, list_t *entry)
{
	if (sp->ptype->ident != flavor::Parser::token::CLASS_TYPE)
        c_map_put_compare_simple(sp, entry);
	else
        c_map_put_compare_class(sp, entry);
}

/* Core code for putvlc - checks for a match against the map codes.  The routine uses 
 * two variables: _data and _size, that contain the matched code.  If no match is found, then 
 * the last code entry is used.  If we get a hit, the _hit variable is assigned 1, otherwise 
 * it is left untouched (previously set to 0).
 */
static void c_map_put_core(symbol_t *sp, list_t *stmts)
{
    list_t *lp = stmts;
    list_t *last = NULL;
	int size;    
     
    /* put data */
    while (lp != NULL) {
        /* sanity check */
        if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::BIT_LITERAL || lp->e1->left.bit == NULL)
            fatal("Internal error CM%d: Expected code entry with bit literal", __LINE__);
		
        if (lp->sub1->type == flavor::Parser::token::ESC_FTYPE || lp->sub1->type == flavor::Parser::token::ESC_MAP) {
            /* save one and only escape code for last */
            last = lp;
            lp = lp->next;
            continue;
        }

		/* get bit length */
		size = lp->e1->left.bit->len;
		
        /* check for match */
        c_outi("if (");
        c_map_put_compare(sp, lp->sub1);
        c_out(") {\n");
        c_identlevel++;

		/* put bits */
        if (!lp->sp) {
            if (java() && sp->ptype->modifiers & M_LONG) 
                c_outi("%sbs.putlong(%d,%d);\n", prefix, lp->e1->left.bit->val, size);
            else
                c_outi("%sbs.putbits(%d,%d);\n", prefix, lp->e1->left.bit->val, size);
        }
        else {
            if (java() && sp->ptype->modifiers & M_LONG) 
                c_outi("%sbs.%sputlong(%d,%d);\n", prefix, 
			           (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""), lp->e1->left.bit->val, size);
            else
                c_outi("%sbs.%sputbits(%d,%d);\n", prefix, 
			           (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""), lp->e1->left.bit->val, size);
        }

        /* save size */
        c_outi("%ssize = %d;\n", prefix, lp->e1->left.bit->len);
        /* save data */
		c_outi("%sdata = %d;\n", prefix, lp->e1->left.bit->val);
       
        /* return code and size, if requested */
        if (cpp()) {
            c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
            c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		    c_outi("return %shit;\n", prefix);
	    }
        else if (java()) 
            c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);

        c_identlevel--;
        c_outi("}\n");
	    
        /* next code */
        lp = lp->next;
    }
    /* no escape code */
    if (last == NULL) {
        if (cpp()) {
            c_outi("%shit = 0;\n", prefix);
            c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
            c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		    c_outi("return %shit;\n", prefix);
		}
        else if (java()) {
            c_outi("%shit = false;\n", prefix);
            c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);
        }
    }
    else {
		/* get bit length */
		size = last->e1->left.bit->len;

		/* put bits */
        if (!last->sp) {
            if (java() && sp->ptype->modifiers & M_LONG) 
                c_outi("%sbs.putlong(%d,%d);\n", prefix, last->e1->left.bit->val, size);
            else
                c_outi("%sbs.putbits(%d,%d);\n", prefix, last->e1->left.bit->val, size);
        }
        else {
            if (java() && sp->ptype->modifiers & M_LONG) 
                c_outi("%sbs.%sputlong(%d,%d);\n", prefix, 
			           (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""), last->e1->left.bit->val, size);
            else
                c_outi("%sbs.%sputbits(%d,%d);\n", prefix, 
			           (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""), last->e1->left.bit->val, size);
        }

        /* save size */
        c_outi("%ssize = %d;\n", prefix, last->e1->left.bit->len);
        /* save data */
		c_outi("%sdata = %d;\n", prefix, last->e1->left.bit->val);

        if (sp->ptype->ident != flavor::Parser::token::CLASS_TYPE)
            c_map_put_esc_simple(sp, last->sub1);
        else
            c_map_put_esc_class(sp, last->sub1);

        if (cpp()) {
            c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
            c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		    c_outi("return %shit;\n", prefix);
		}
        else if (java()) {
            c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);
        }
    }
}

/* generate map putvlc method */
static void c_map_put(symbol_t *sp, list_t *stmts)
{
	c_outi("\n");
	if (cpp()) {
		if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE)
			c_outi("int %s::putvlc(%s &%sbs, %s%s *%sarg, int *%sparse, int *%scode) {\n", 
                   sp->name, bitstream_class, prefix, 
                   ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                   sp->ptype->name, prefix, prefix, prefix);
		else
			c_outi("int %s::putvlc(%s &%sbs, %s%s %sarg, int *%sparse, int *%scode) {\n", 
                   sp->name, bitstream_class, prefix, 
                   ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                   sp->ptype->name, prefix, prefix, prefix);
	}
	else if (java()) {
		c_outi("public static %s putvlc(%s %sbs, %s %sarg) throws IOException {\n",
               map_result, bitstream_class, prefix, sp->ptype->name, prefix);
	}
    c_identlevel++;

    /* declare hit variable (used for return value), data and size */
	c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
    c_outi("int %sesc_bits = 0;\n", prefix);
	if (cpp()) 
		c_outi("int %shit = 1;\n\n", prefix);
	else if (java())
		c_outi("boolean %shit = true;\n\n", prefix);

    /* output the core putvlc code (series of putbits with case statements */
    c_map_put_core(sp, stmts);

    c_identlevel--;
    c_outi("}\n");
}

/*********************************************/
/* map declarations/definitions for putxml() */
/*********************************************/

/* output assignment statements for a simple map entry */
static void c_map_putxml_assign_simple(symbol_t* sp, list_t* entry)
{
    list_t *p = entry->sub1;

    if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP) {
        c_outi("%sesc_bits = ", prefix);
        c_expression(p->e3, 0);
        c_out(";\n");
    }

    if (cpp()) 
        c_outi("*%sarg = ", prefix);
    else if (java())
        c_outi("%sarg = ", prefix);

    /* figure out if we have simple assignment or escape */
    if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP)
        c_map_get_assign_esc(p);
    else {
		if (sp->ptype->ident == flavor::Parser::token::FLOAT && entry->sub1->e1->type == flavor::Parser::token::DOUBLE)
            c_out("(float)");
		c_expression(p->e1, 0);
	}
    c_out(";\n");

    /* output an element for the value, which corresponds to the code */
    c_xml_map_var_simple(sp, 1);
}

/* output assignment statements for a class map entry */
static void c_map_putxml_assign_class(symbol_t *sp, list_t *entry)
{
    list_t *p;

    /* find the statements for this class */
    list_t *stmts = find_class_stmts(sp->ptype);
    if (stmts == NULL) 
        fatal("Internal error CM%d: Cannot find declaration of class '%s'", __LINE__, sp->name);

    if (entry->sub1->type == flavor::Parser::token::ESC_FTYPE || entry->sub1->type == flavor::Parser::token::ESC_MAP)
        p = entry->sub1;
    else
        p = entry->sub1->sub1;

    /* output the beginning of the value element for the XML document */ 
    if (cpp())
        c_outi("%s(", xml_func4);
    else if (java())
        c_outi("XML.%s(", xml_func4);
    c_out("\"value\", 0);\n");

    /* traverse declarations and output assignment code for each variable */
	while (stmts != NULL) {
        switch (stmts->type) {
        case flavor::Parser::token::DECL:
			if (p == NULL) return;
            if (p->type == flavor::Parser::token::ESC_FTYPE || p->type == flavor::Parser::token::ESC_MAP) {
                c_outi("%sesc_bits = ", prefix);
                c_expression(p->e3, 0);
                c_out(";\n");
            }
            else
                c_outi("%sesc_bits = 0;\n", prefix);

            c_outi("%sarg%s%s = ", prefix, c_scope(), stmts->sp->name);

            /* figure out if we have simple assignment or escape */
            if (entry->sub1->type == flavor::Parser::token::ESC_FTYPE || entry->sub1->type == flavor::Parser::token::ESC_MAP)
                c_map_get_assign_esc(p);
            else {
		        if (stmts->sp->ptype->ident == flavor::Parser::token::FLOAT && p->e1->type == flavor::Parser::token::DOUBLE)
                    c_out("(float)");
                c_expression(p->e1, 0);
            }
            c_out(";\n");
       
            /* output an element for each class member value, which corresponds to the code */
            c_xml_map_var_simple(stmts->sp, 0);

            p = p->next;
            break;
            
        default:
            break;
        }
        stmts = stmts->next;
    }

    /* output the end of the value element for the xml document */ 
    if (cpp())
        c_outi("%s(", xml_func5);
    else if (java())
        c_outi("XML.%s(", xml_func5);
    c_out("\"</value>\");\n");
}

/* output assignment statements for a map entry */
static void c_map_putxml_assign(symbol_t *sp, list_t *entry)
{
    /* different handling depending on if we have simple or class type */
    if (sp->ptype->ident != flavor::Parser::token::CLASS_TYPE)
        c_map_putxml_assign_simple(sp, entry);
    else 
        c_map_putxml_assign_class(sp, entry);
}

/* Core code for putxml - checks for a match against the map codes.  The routine uses 
 * two variables: _data and _size, that contain the matched code.  If no match is found, then 
 * the last code entry is used.  If we get a hit, the _hit variable is assigned 1, otherwise 
 * it is left untouched (previously set to 0).
 */
static void c_map_putxml_core(symbol_t *sp, list_t *stmts)
{
    list_t *lp = stmts;
    int size;

    /* sanity check */
    if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::BIT_LITERAL || lp->e1->left.bit == NULL)
        fatal("Internal error CM%d: Expected code entry with bit literal", __LINE__);
    
    size = lp->e1->left.bit->len;
    
    /* get data */
    c_outi("%sdata = %sbs.nextbits(%d);\n", prefix, prefix, size);
    c_outi("switch (%sdata) {\n", prefix);
    while (lp != NULL) {
        /* sanity check */
        if (lp->e1 == NULL || lp->e1->op != flavor::Parser::token::BIT_LITERAL || lp->e1->left.bit == NULL)
            fatal("Internal error CM%d: Expected code entry with bit literal", __LINE__);

        /* did the size change? */
        if (lp->e1->left.bit->len != size) {
            c_outi("default:\n");
            c_identlevel++;
            c_map_putxml_core(sp, lp);
            c_identlevel--;
            break;
        }

        /* check for match */
        c_outi("case %d:\n", lp->e1->left.bit->val);
        c_identlevel++;

        /* skip bits, if we are doing get - must be done here so that escapes read correctly */
        c_outi("%sbs.skipbits(%d);\n", prefix, size);

        /* output code element for the XML document */
        c_outi("if (bAttr) {\n");
        c_identlevel++; 
        
        if (cpp())
            c_outi("%s(", xml_func3);
        else if (java())
            c_outi("XML.%s(", xml_func3);
        c_out("\"<code type=\\\"flBit\\\" bitLen=\\\"%d\\\">%d</code>\");\n", lp->e1->left.bit->len, lp->e1->left.bit->val);
        
        c_identlevel--; 
        c_outi("}\n");
        c_outi("else {\n");
        c_identlevel++; 

        if (cpp())
            c_outi("%s(", xml_func3);
        else if (java())
            c_outi("XML.%s(", xml_func3);
        c_out("\"<code bitLen=\\\"%d\\\">%d</code>\");\n", lp->e1->left.bit->len, lp->e1->left.bit->val);
  
        c_identlevel--; 
        c_outi("}\n");

        /* output assignment code */
        c_map_putxml_assign(sp, lp);
        
        /* save size */
        c_outi("%ssize = %d;\n", prefix, lp->e1->left.bit->len);

        c_outi("break;\n");
        c_identlevel--;

        /* if at the end, set hit to 0 and use last entry */
        if (lp->next == NULL) {
            c_outi("default:\n");
            c_identlevel++;
            if (cpp())
                c_outi("%shit = 0;\n", prefix);
			else if (java())
                c_outi("%shit = false;\n", prefix);
            c_identlevel--;
        }
        /* next code */
        lp = lp->next;
    }
    /* close switch statement */
    c_outi("}\n");
}

/* generate map putxml method */
static void c_map_putxml(symbol_t* sp, list_t* stmts)
{
	c_out("\n");
    if (cpp()) {
        c_outi("int %s::putxml(%s &%sbs, short int bAttr, %s%s *%sarg, int *%sparse, int *%scode) {\n", 
               sp->name, bitstream_class, prefix, 
               ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
               sp->ptype->name, prefix, prefix, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        c_outi("int %sesc_bits = 0;\n", prefix);
		c_outi("int %shit = 1;\n\n", prefix);
    } 
    else if (java()) {
		c_outi("public static %s putxml(%s %sbs, boolean bAttr) throws IOException {\n", 
               map_result, bitstream_class, prefix, sp->ptype->name, prefix, prefix, prefix);
		c_identlevel++;
		/* declare hit variable (used for return value), data and size */
		c_outi("int %sdata = 0, %ssize = 0;\n", prefix, prefix);
        c_outi("int %sesc_bits = 0;\n", prefix);
		c_outi("boolean %shit = true;\n", prefix);
		if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE)
			c_outi("%s %sarg = new %s();\n\n", sp->ptype->name, prefix, sp->ptype->name);
		else
			c_outi("%s %sarg = %s%s;\n\n",
                   sp->ptype->name, prefix, (sp->ptype->ident == flavor::Parser::token::FLOAT) ? "(float)" : "",
                   (sp->ptype->ident <= flavor::Parser::token::INT) ? "0" : "0.0");
	}
    
    /* output the core putxml code (series of getbits with case statements) */
    c_map_putxml_core(sp, stmts);

    /* return code and size, if requested */
    if (cpp()) {
        c_outi("if (%sparse != NULL) *%sparse = %ssize;\n", prefix, prefix, prefix);
        c_outi("if (%scode != NULL) *%scode = %sdata;\n", prefix, prefix, prefix);
		c_outi("return %shit;\n", prefix);
	}
    else if (java())
        c_outi("return new %s(%shit, %sdata, %ssize, %sarg);\n", map_result, prefix, prefix, prefix, prefix);
	
    c_identlevel--;
    c_outi("}\n");
}

/******************************************/
/* map declarations/definitions for xsd() */
/******************************************/

static void c_map_xsd_assign_class(symbol_t *sp)
{
    /* find the statements for this class */
    list_t *stmts = find_class_stmts(sp->ptype);
    if (stmts == NULL) 
        fatal("Internal error CM%d: Cannot find declaration of class '%s'", __LINE__, sp->name);

    /* traverse declarations and output assignment code for each variable */
	while (stmts != NULL) {
        switch (stmts->type) {
        case flavor::Parser::token::DECL:
            c_outi("<xsd:element name=\"%s\"", stmts->sp->name);
            if (stmts->sp->ptype->ident == flavor::Parser::token::CHAR) {
                if (sp->modifiers & M_UNSIGNED) 
                    c_out(" type=\"flUChar\"/>\n");
                else 
                    c_out(" type=\"flSChar\"/>\n");
            }
            else if (stmts->sp->ptype->ident <= flavor::Parser::token::INT) {
                if (stmts->sp->modifiers & M_UNSIGNED) 
                    c_out(" type=\"flUInt\"/>\n");
                else 
                    c_out(" type=\"flSInt\"/>\n");
            }
            else if (stmts->sp->ptype->ident == flavor::Parser::token::FLOAT)
                c_out(" type=\"flFloat\"/>\n");
            else if (stmts->sp->ptype->ident == flavor::Parser::token::DOUBLE)
                c_out(" type=\"flDouble\"/>\n");
	        else 
                fatal("Internal error CM%d: Expected simple type", __LINE__);

        default:
            break;
        }
        stmts = stmts->next;
    }
}

/* generate schema description for the map */
static void c_map_xsd(symbol_t* sp, list_t* stmts)
{
    c_outi("<xsd:element name=\"code\" type=\"flBit\"/>\n");

    if (sp->ptype->ident != flavor::Parser::token::CLASS_TYPE) {
        c_outi("<xsd:element name=\"value\"");
        if (sp->ptype->ident == flavor::Parser::token::CHAR) {
            if (sp->modifiers & M_UNSIGNED) 
                c_out(" type=\"flUChar\"");
            else 
                c_out(" type=\"flSChar\"");
        }
        else {
            if (sp->ptype->ident <= flavor::Parser::token::INT) {
                if (sp->modifiers & M_UNSIGNED)
                    c_out(" type=\"flUInt\"");
                else 
                    c_out(" type=\"flSInt\"");
            }
            else if (sp->ptype->ident == flavor::Parser::token::FLOAT)
                c_out(" type=\"flFloat\"");
            else if (sp->ptype->ident == flavor::Parser::token::DOUBLE)
                c_out(" type=\"flDouble\"");
            else
                fatal("Internal error CM%d: Expected simple type", __LINE__);
        }
        c_out("/>\n");
    }
    else {
        c_outi("<xsd:element name=\"value\">\n");
        c_identlevel++;
        c_outi("<xsd:complexType>\n");
        c_identlevel++;
        c_outi("<xsd:sequence>\n");
        c_identlevel++;

        c_map_xsd_assign_class(sp);

        c_identlevel--;
        c_outi("</xsd:sequence>\n");
        c_identlevel--;
        c_outi("</xsd:complexType>\n");
        c_identlevel--;
        c_outi("</xsd:element>\n");
    }
}

/********************************/
/* map declarations/definitions */
/********************************/

/* generate map definition */
void map_decl(symbol_t *sp, list_t *stmts)
{
    FILE *tmp;

    if (sp == NULL) return;
    if (sp->ptype == NULL) 
        fatal("Internal error CM%d: Map without type information", __LINE__);
	
    if (cpp())
        c_out("\nclass %s {\n", sp->name);
    else if (java())
        c_out("\npublic class %s {\n", sp->name); 
    else if (xsd()) {
        c_out("\n<xsd:complexType name=\"%s\">\n", sp->name);
    	c_identlevel++;
		c_outi("<xsd:sequence>\n");
    }
    
    c_identlevel++;

    /* if doing C++, declare nextvlc()/getvlc()/putvlc() and switch output files */
    if (cpp()) {
        /* nextvlc(), getvlc() */
        if (gen_get) {
            c_outi("public: static int nextvlc(%s &%sbs, %s%s *%sarg, int *%sparse=NULL, int *%scode=NULL);\n",
                   bitstream_class, prefix, 
                   ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                   sp->ptype->name, prefix, prefix, prefix);
            c_outi("public: static int getvlc(%s &%sbs, %s%s *%sarg, int *%sparse=NULL, int *%scode=NULL);\n",
                   bitstream_class, prefix, 
                   ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                   sp->ptype->name, prefix, prefix, prefix);
        }
        /* putvlc() */
        if (gen_put) {
			if (sp->ptype->ident == flavor::Parser::token::CLASS_TYPE)
				/* if CLASS_TYPE pass a pointer to argument */
				c_outi("public: static int putvlc(%s &%sbs, %s%s *%sarg, int *%sparse=NULL, int *%scode=NULL);\n",
                       bitstream_class, prefix, 
                       ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                       sp->ptype->name, prefix, prefix, prefix);
			else
				/* otherwise, just value */
				c_outi("public: static int putvlc(%s &%sbs, %s%s %sarg, int *%sparse=NULL, int *%scode=NULL);\n",
                       bitstream_class, prefix, 
                       ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                       sp->ptype->name, prefix, prefix, prefix);

        }
        /* putxml() */
        if (gen_putxml) {
            c_outi("public: static int putxml(%s &%sbs, short int bAttr, %s%s *%sarg, int *%sparse=NULL, int *%scode=NULL);\n",
                   bitstream_class, prefix, 
                   ((sp->modifiers & M_UNSIGNED) ? "unsigned " : ""),
                   sp->ptype->name, prefix, prefix, prefix);
        }
        c_identlevel = 0;
        tmp = ofp;
        ofp = ofp2;
    }

    if (cpp() || java()) {
	    /* generate getvlc function; no next, if the map uses escape */
	    if (gen_get && !sp->escape) 
            c_map_next(sp, stmts);
        /* generate getvlc function */
        if (gen_get) 
            c_map_get(sp, stmts);
        /* generate putvlc function */
        if (gen_put) 
            c_map_put(sp, stmts);
        /* generate putxml function */
        if (gen_putxml) 
            c_map_putxml(sp, stmts);
    }
	else if (xsd()) 
        c_map_xsd(sp, stmts);

    /* switch back to .h file */
    if (cpp()) {
        c_identlevel = 1;
        ofp = tmp;
    }

    c_identlevel--;

    if (cpp()) 
        c_out("};\n");
    else if (java())
        c_out("}\n");
    else if (xsd()) {
		c_outi("</xsd:sequence>\n");
		c_identlevel--;
        c_out("</xsd:complexType>\n");
    }
}

