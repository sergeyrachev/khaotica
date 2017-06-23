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

/* ctrace.c */

#include "cdef.h"


/* trace the failed ID probing of a class sp */
void c_trace_failed_id_simple(symbol_t *sp, int dims)
{
    int i;
    
    /* cast the variable to 'unsigned int' or 'double' */
    if (sp->ptype->id->ptype->ident <= flavor::Parser::token::INT)
        if (java() && sp->ptype->id->modifiers & M_LONG)
            c_outi("%s(%sbs.getpos(), %sparse, (long)%s%s_id, \"ID %s", trace_func, prefix, prefix, prefix, sp->name, sp->name);
        else
            c_outi("%s(%sbs.getpos(), %sparse, (%sint)%s%s_id, \"ID %s", trace_func, prefix, prefix, s_mods2str(M_UNSIGNED), prefix, sp->name, sp->name);
	else if (sp->ptype->id->ptype->ident <= flavor::Parser::token::DOUBLE)
        c_outi("%s(%sbs.getpos(), %sparse, (double)%s%s_id, \"ID %s", trace_func, prefix, prefix, prefix, sp->name, sp->name);
    else
        fatal("Internal error CT%d: Expected simple type", __LINE__);
    
    if (cpp()) {
        for (i=0; i<dims; i++) 
            c_out("[%%d]");
        c_out(".%s lookup failed [ERROR]\"", sp->ptype->id->name);
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
        for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
        c_out(".%s lookup failed [ERROR]\"", sp->ptype->id->name);
    }
    c_out(");\n");
}

/* trace the failed map-based ID probing of a class sp */
void c_trace_failed_id_map(symbol_t *sp, symbol_t *map, int dims)
{
    int i;
    
    if (java() && sp->ptype->id->modifiers & M_LONG)
        c_outi("%s(%sbs.getpos(), %sparse, (long)%scode, \"ID %s", trace_func, prefix, prefix, prefix, sp->name);
    else
        c_outi("%s(%sbs.getpos(), %sparse, (%sint)%scode, \"ID %s", trace_func, prefix, prefix, s_mods2str(M_UNSIGNED), prefix, sp->name);

    if (cpp()) {
        for (i=0; i<dims; i++)
            c_out("[%%d]");
        c_out(".%s lookup via %s failed [ERROR]\"", sp->ptype->id->name, map->name);
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
        for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
        c_out(".%s lookup via %s failed [ERROR]\"", sp->ptype->id->name, map->name);
    }
    c_out(");\n");
}

/* trace the onset of a class get() call */
void c_trace_entering_class(symbol_t *sp, int dims)
{
    int i;
    
    c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"processing %s %s", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->ptype->name, sp->name);

    if (cpp()) {
        for (i=0; i<dims; i++) 
            c_out("[%%d]");
        c_out("\"");
        if (dims > 0) 
            c_out(", ");
        for (i=0; i<dims; i++) {
            c_out("%s", c_gentmp_dim(i));
            if (i != dims-1)
                c_out(", ");
        }
    }
    else if (java()) {
        for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("\"");
	}
    c_out(");\n");
}

/* trace the onset of a map-based class parsing */
void c_trace_entering_mapped_class(symbol_t *sp, int dims, symbol_t *map)
{
    int i;
    
    c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"processing %s %s", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->ptype->name, sp->name);

    if (cpp()) {
        for (i=0; i<dims; i++) 
            c_out("[%%d]");
        c_out(" via map %s\"", map->name);

        if (dims > 0) 
            c_out(", ");
        for (i=0; i<dims; i++) {
            c_out("%s", c_gentmp_dim(i));
            if (i != dims-1) 
                c_out(", ");
        }
    }
    else if (java()) {
        for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
  	    c_out(" via map %s\"", map->name);
    }
    c_out(");\n");
}

/* Trace the parsing of a variable with simple parse expr; 'incorrect' is set when 
 * there is a difference with the expected value (if any).
 */
void c_trace_var_simple(symbol_t *sp, int dims, int usenext, int incorrect)
{
    int i;
    
    if (usenext) 
        c_outi("%s(%sbs.getpos(), %sparse, ", trace_func, prefix, prefix);
    else
        c_outi("%s(%sbs.getpos()-%sparse, %sparse, ", trace_func, prefix, prefix, prefix);
    
    /* cast the variable to 'unsigned int' or 'double' */
    if (sp->ptype->ident <= flavor::Parser::token::INT)
        if (java() && sp->modifiers & M_LONG) 
            c_out("(long)%s", sp->name);
        else 
            c_out("(%sint)%s", s_mods2str(M_UNSIGNED), sp->name);
    else if (sp->ptype->ident <= flavor::Parser::token::DOUBLE)
        c_out("(double)%s", sp->name);
    else 
        fatal("Internal error CT%d: Expected simple type", __LINE__);
    
    if (dims > 0)
        c_array_proper(sp, dims);
    c_out(", \"%s", sp->name);
   
    for (i=0; i<dims; i++) {
		if (cpp())
            c_out("[%%d]");
		else if (java()) {
			c_out("[\" + ");
			c_out("%s", c_gentmp_dim(i));
			c_out(" + \"]");
		}
	}
  	if (java()) 
        c_out("\" + ");

    /* put a '*' if it uses next */
    if (usenext) {
        if (cpp())
            c_out("*");
        else if (java())
            c_out("\"*\" + ");
    }

    /* since it is a simple value, we can output it in the correct format as well (after the name) */
    if (cpp()) {
		if (sp->ptype->ident <= flavor::Parser::token::INT) {
			if (sp->ptype->ident == flavor::Parser::token::CHAR)
				c_out(" (%%c)");	    /* char */
			else
				if (sp->modifiers & M_UNSIGNED)
					c_out(" (%%u)");	/* unsigned */
				else
					c_out(" (%%d)");	/* signed */
		}
		else if (sp->ptype->ident <= flavor::Parser::token::DOUBLE)
            c_out(" (%%g)");
		else 
            fatal("Internal error CT%d: Expected simple type", __LINE__);
    }
	else if (java()) {
		c_out(" \" (\" + %s",sp->name);
		if (dims > 0) 
            c_array_proper(sp, dims);
		c_out(" + \")");
	}

    /* flag variables with incorrect values */
    if (incorrect)
        c_out(" [ERROR]\"");
    else
        c_out("\"");
	
    if (cpp() && dims > 0) {
        c_out(", ");
        for (i=0; i<dims; i++) {
            c_out("%s", c_gentmp_dim(i));
            if (i != dims-1)
                c_out(", ");
        }
    }
    /* now we need the actual variable itself (to print the actual - nonhex - value) */
    if (cpp()) {
		c_out(", %s", sp->name);
		if (dims > 0) 
            c_array_proper(sp, dims);
	}
    c_out(");\n");
}

/* Trace the parsing of a variable with map parse expr; 'incorrect' is set when 
 * there is a difference with the expected value (if any).
 */
void c_trace_var_map(symbol_t *sp, symbol_t *map, int dims, int usenext, int incorrect)
{
    int i;
    
    if (usenext)
        c_outi("%s(%sbs.getpos(), %sparse, (%sint)%scode, ", trace_func, prefix, prefix, s_mods2str(M_UNSIGNED), prefix);
    else
        c_outi("%s(%sbs.getpos()-%sparse, %sparse, (%sint)%scode, ", trace_func, prefix, prefix, prefix, s_mods2str(M_UNSIGNED), prefix);
    
    c_out("\"%s", sp->name);

    if (dims > 0)
        if (cpp()) 
            for (i=0; i<dims; i++) 
                c_out("[%%d]");
        else 
            for (i=0; i<dims; i++) 
                c_out("[\" + %s + \"]", c_gentmp_dim(i));
  
    /* put a '*' if it uses next */
    if (usenext) 
        c_out("*");
 
    /* output map name */
    c_out(" via %s", map->name);

    /* since it is a simple value, we can output it in the correct format as well (after the name) */
    if (cpp()) {
        if (sp->ptype->ident <= flavor::Parser::token::INT) {
			if (sp->ptype->ident == flavor::Parser::token::CHAR)
				c_out(" (%%c)");	/* char */
            if (sp->modifiers & M_UNSIGNED)
                c_out(" (%%u)");	/* unsigned */
            else 
                c_out(" (%%d)");	/* signed */
        }
        else if (sp->ptype->ident <= flavor::Parser::token::DOUBLE)
            c_out(" (%%g)");
        else 
            fatal("Internal error CT%d: Expected simple type", __LINE__);
    }
    else if (java()) {
        c_out(" (\" + %s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(" + \")");
    }
    /* flag variables with incorrect values */
    if (incorrect)
        c_out(" [ERROR]\"");
    else 
        c_out("\"");

    /* if cpp, output variables that have args in printf */
    if (cpp()) {
        if (dims > 0) {
            c_out(", ");
            for (i=0; i<dims; i++) {
                c_out("%s", c_gentmp_dim(i));
                if (i != dims-1)
                    c_out(", ");
            }
        }
        /* now we need the actual variable itself (to print the actual - nonhex - value) */
        c_out(", %s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
    }
    c_out(");\n");
}

/* trace the parsing of a class with map parse expr */
void c_trace_class_map(symbol_t *sp, symbol_t *map, int dims, int usenext)
{
    int i;
    
    if (usenext)
        c_outi("%s(%sbs.getpos(), %sparse, (%sint)%scode, ", trace_func, prefix, prefix, s_mods2str(M_UNSIGNED), prefix);
    else
        c_outi("%s(%sbs.getpos()-%sparse, %sparse, (%sint)%scode, ", trace_func, prefix, prefix, prefix, s_mods2str(M_UNSIGNED), prefix);
    
    c_out("\"%s", sp->name);
    if (cpp()) 
        if (dims > 0) 
            for (i=0; i<dims; i++) 
                c_out("[%%d]");
    else 
        if (dims > 0) 
            for (i=0; i<dims; i++) 
                c_out("[\" + %s + \"]", c_gentmp_dim(i));
    
    /* put a '*' if it uses next */
    if (usenext) 
        c_out("*");
    
    /* output map name */
    c_out(" via %s\"", map->name);
 
    /* if cpp, output variables that have args in printf */
    if (cpp()) {
        if (dims > 0) {
            c_out(", ");
            for (i=0; i<dims; i++) {
                c_out("%s", c_gentmp_dim(i));
                if (i != dims-1)
                    c_out(", ");
            }
        }
    }
    c_out(");\n");
}

/* trace failed map lookup */
void c_trace_failed_map(symbol_t *map, symbol_t *sp, int dims)
{
    int i;

    c_outi("%s(%sbs.getpos(), %sparse, (%sint)%scode, \"%s lookup failed for %s", trace_func, prefix, prefix, s_mods2str(M_UNSIGNED), prefix, map->name, sp->name);
    
    if (cpp())
		for (i=0; i<dims; i++)
            c_out("[%%d]");
	else if (java()) {
		if (dims > 0)
			c_out("\"");
			for (i=0; i<dims; i++) {
				c_out(" + \"[\" + ");
				c_out("%s", c_gentmp_dim(i));
				c_out("+\"]\" + \"");
			}
	}

    c_out(" [ERROR]\"");

    if (cpp() && dims>0) {
        c_out(", ");
        for (i=0; i<dims; i++) {
            c_out("%s", c_gentmp_dim(i));
            if (i != dims-1)
                c_out(", ");
        }
    }
    c_out(");\n");
}

/* trace align modifier */
void c_trace_aligned(void)
{
    if (cpp())
        c_outi("%s(%sbs.getpos(), 0, (unsigned int)0, \"align(%%d)\", %sparse);\n", trace_func, prefix, prefix);
    else if (java())
        c_outi("%s(%sbs.getpos(), 0, 0, \"align(%sparse)\");\n", trace_func, prefix, prefix);
}

/* trace the beginning of a get() method */
void c_trace_get_begin(symbol_t *sp)
{
    if (cpp())
		c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"begin %s\");\n", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->name);
	else if (java())
		c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"begin %s\");\n", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->name);
}

/* trace the end of a get() method */
void c_trace_get_end(symbol_t *sp)
{
    if (cpp())
		c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"end %s\");\n", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->name);
	else if (java())
		c_outi("%s(%sbs.getpos(), 0, (%sint)0, \"end %s\");\n", trace_func, prefix, s_mods2str(M_UNSIGNED), sp->name);
}
