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

/* cid.c */

#include "cdef.h"


/* check if a class or a base class has an ID; returns the class where an ID was first found */
symbol_t *c_find_id(symbol_t *sp)
{
    if (sp == NULL) return NULL;
    if (sp->id != NULL) return sp;
    if (sp->ptype == NULL) return NULL;
    else return c_find_id(sp->ptype);
}

/* same as above, but abstract classes are ignored */
symbol_t *c_find_id_notabstract(symbol_t *sp)
{
    /* if (sp->abstract) return NULL; */
    if (sp == NULL) return NULL;
    if (sp->id != NULL) return sp;
    if (sp->ptype == NULL) return NULL;
    else return c_find_id_notabstract(sp->ptype);
}

/* get the ID of a class */
void c_class_id_get(symbol_t *sp)
{
    /* generate get ID code, but not for abstract classes */
    if (sp == NULL || sp->abstract || sp->id == NULL) return;
    
    c_outi("if (%sid_parsed == 0) {\n", prefix);
    c_identlevel++;
    c_var_decl_get(sp->id, NULL, NULL, NULL, NULL, 0, NULL, 0);
    c_outi("%sid_parsed = 1;\n", prefix);
    c_identlevel--;
    c_outi("}\n");
}

/* put the ID of a class */
void c_class_id_put(symbol_t *sp)
{
    /* generate put ID code, but not for abstract classes */
    if (sp == NULL || sp->abstract || sp->id == NULL) return;

    c_outi("if (%sid_parsed == 0) {\n", prefix);
    c_identlevel++;
    c_var_decl_put(sp->id, NULL, NULL, NULL, NULL, 0, NULL, 0);
    c_outi("%sid_parsed = 1;\n", prefix);
    c_identlevel--;
    c_outi("}\n");
}

/* get the ID of a class and put it in XML form */
void c_class_id_putxml(symbol_t *sp)
{
    /* generate putxml ID code, but not for abstract classes */
    if (sp == NULL || sp->abstract || sp->id == NULL) return;
    
    c_outi("if (%sid_parsed == 0) {\n", prefix);
    c_identlevel++;
    c_var_decl_putxml(sp->id, NULL, NULL, NULL, NULL, 0, NULL, 0);
    c_outi("%sid_parsed = 1;\n", prefix);
    c_identlevel--;
    c_outi("}\n");
}

/* declare temp variable to hold probed id value and then probe the id value calling the nextbits function */
void c_var_decl_get_class_probe_id_simple(symbol_t *sp)
{
    switch (sp->ptype->id->ptype->ident) {
    case flavor::Parser::token::BIT:
    case flavor::Parser::token::CHAR:
    case flavor::Parser::token::INT:
        /* if we are tracing, save size */
        if (gen_trace) {
            c_outi("%sparse = ", prefix);
            c_expression(sp->ptype->id->parse, 0);
            c_out(";\n");
			
            if (java() && sp->ptype->id->modifiers & M_LONG) 
                c_outi("%s%s_id = %sbs.%s%snextlong( %sparse );\n", 
                       prefix, sp->name, prefix, 
				       (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""),
				       (sp->ptype->id->modifiers & M_UNSIGNED ? "" : "s"), prefix);
            else
                c_outi("%s%s_id = %sbs.%s%snextbits( %sparse );\n", 
                       prefix, sp->name, prefix, 
				       (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""),
				       (sp->ptype->id->modifiers & M_UNSIGNED ? "" : "s"), prefix);
        }
        else {
            if (java() && sp->ptype->id->modifiers & M_LONG) 
                c_outi("%s%s_id = %sbs.%s%snextlong(", 
                       prefix, sp->name, prefix, 
				       (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""),
                       (sp->ptype->id->modifiers & M_UNSIGNED ? "" : "s"));
            else
			    c_outi("%s%s_id = %sbs.%s%snextbits(", 
				       prefix, sp->name, prefix, 
				       (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""),
				       (sp->ptype->id->modifiers & M_UNSIGNED ? "" : "s"));

            c_expression(sp->ptype->id->parse, 0);
            c_out(");\n");
        }
        break;
        
    case flavor::Parser::token::FLOAT:
        /* if we are tracing save size */
        if (gen_trace) 
            c_outi("%sparse = 32;\n", prefix);
        c_outi("%s%s_id = %sbs.%snextfloat();\n", 
               prefix, sp->name, prefix, 
               (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""));        
        break;
        
    case flavor::Parser::token::DOUBLE:
        if (!(sp->modifiers & M_LONG)) {
            /* if we are tracing save size */
            if (gen_trace) 
                c_outi("%sparse = 64;\n", prefix);
            c_outi("%s%s_id = %sbs.%snextdouble();\n", 
                   prefix, sp->name, prefix,
                   (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""));
        }
        else {
            /* if we are tracing save size */
            if (gen_trace) 
                c_outi("%sparse = 64;\n", prefix);
            c_outi("%s%s_id = %sbs.%snextldouble();\n", 
                   prefix, sp->name, prefix, 
                   (sp->ptype->id->modifiers & M_LITTLE ? "little_" : ""));
        }
        break;

    default:
        fatal("Internal error CI%d: Unknown ID type: '%s'", __LINE__, sp->ptype->id->ptype->name);
    }
}

/* declare temp variable to hold probed id value and then probe the id value calling the nextvlc function */
void c_var_decl_get_class_probe_id_map(symbol_t *sp, int dims)
{
    int i;
    symbol_t *map = sp->ptype->id->parse->left.sp;

    /* call getvlc() */
    c_outi("if (%s%snextvlc(%sbs, &%s%s_id", map->name, c_static_scope(), prefix, prefix, sp->name);
    
    /* if we are tracing, get number of bits and code value */
    if (gen_trace) 
        c_out(", &%sparse, &%scode", prefix, prefix);
    
    c_out(") != 0) {\n");
    
    /* lookup failed */
    c_identlevel++;
    c_outi("%s(\"Map '%s' lookup for ID '%s", err_func, map->name, sp->name);
    
    if (cpp()) {
        if (dims > 0)
            for (i=0; i<dims; i++) c_out("[%%d]");

        c_out(".%s' failed\"", sp->ptype->id->name);
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
        c_out(".%s' failed\"", sp->ptype->id->name);
    }
    c_out(");\n");
    
    /* don't increase error count - will be done by get() itself */
    
    /* trace failed lookup */
    if (gen_trace)
        c_trace_failed_id_map(sp, sp->ptype->id->parse->left.sp, dims);
    
    c_identlevel--;
    c_outi("}\n");
}

/* Generate 'if' statements for ID parsing of a variable 'orig' of type class based on its class type 
 *'sp' (used by c_variable_decl_get_class_boot); cnt is the number of comparisons already produced.
 * Returns cnt + number of comparisons produced in this run.
 */
int c_var_decl_get_class_id(symbol_t *orig, int dims, symbol_t *sp, int cnt, int xml)
{
    symbol_t *p, **derivedp;
  
    /* output ID check code, but only if the class is not abstract */
    if (sp->id != NULL && !sp->abstract) {
        if (sp->id->init == NULL) 
            fatal("Internal error CI%d: ID without value", __LINE__);

        c_ident();
    
        /* if we are not the first to output an ID, prepend an 'else' */
        if (cnt > 0) 
            c_out("else ");

        /* id range */
        if (sp->id->range) {
            c_out("if (");
            c_expression(sp->id->init, 0);
            c_out(" <= %s%s_id && %s%s_id <= ", prefix, orig->name, prefix, orig->name);
            c_expression(sp->id->init->third.np, 0);
            c_out(") {\n");
            c_identlevel++;
            c_outi("%s", orig->name);
            if (dims > 0) 
                c_array_proper(orig, dims);
            if (cpp()) 
                c_out(" = new(%s);\n", sp->name);
            else if (java()) 
                c_out(" = new %s();\n", sp->name);
 
            if (xml) {
               /* signal the beginning of parsing the class */
                if (sp->id != NULL) {
                    if (cpp()) 
                        c_outi("%s(\"%s xsi:type=\\\"%s\\\"\", ", xml_func4, orig->name, sp->name);
                    else if (java())
                        c_outi("XML.%s(\"%s xsi:type=\\\"%s\\\"\", ", xml_func4, orig->name, sp->name);
   
                    if (orig->ptype->align != NULL)
	                    c_expression(orig->ptype->align, 0);
                    else
                        c_out("0");

                    c_out(");\n");        
                }
            }

            c_identlevel--;
            c_outi("}\n");
        }
        else {
            c_out("if (%s%s_id == ", prefix, orig->name);
            c_expression(sp->id->init, 0);
            c_out(") {\n");
            c_identlevel++;
            c_outi("%s", orig->name);
            if (dims > 0) 
                c_array_proper(orig, dims);
            if (cpp()) 
                c_out(" = new(%s);\n", sp->name);
            else if (java()) 
                c_out(" = new %s();\n", sp->name);
            
            if (xml) {
               /* signal the beginning of parsing the class */
                if (sp->id != NULL) {
                    if (cpp()) 
                        c_outi("%s(\"%s xsi:type=\\\"%s\\\"\", ", xml_func4, orig->name, sp->name);
                    else if (java())
                        c_outi("XML.%s(\"%s xsi:type=\\\"%s\\\"\", ", xml_func4, orig->name, sp->name);
    
                    if (orig->ptype->align != NULL)
	                    c_expression(orig->ptype->align, 0);
                    else
                        c_out("0");

                    c_out(");\n");        
                }
            }
            
            c_identlevel--;
            c_outi("}\n");
        }
        /* one more comparison */
        cnt++;
    }
    /* try all derived classes as well */
    if ((derivedp = sp->derived) != NULL)
        while ((p = *derivedp++) != NULL) 
            cnt = c_var_decl_get_class_id(orig, dims, p, cnt, xml);
    
    return cnt;
}


/* Generate 'if' statements for ID comparison of a variable of type 'sp'
 * (used by c_class_isidof());
 * cnt is the number of comparisons already produced.
 *
 * Returns:
 *  cnt + number of comparisons it produced in this run
 */
int c_class_isidof_check_id(symbol_t *sp, int cnt)
{
    symbol_t *p, **derivedp;
  
    /* output ID check code, but only if the class is not abstract */
    if (sp->id != NULL && !sp->abstract) {
        if (sp->id->init == NULL) fatal("Internal error CI%d: ID without value", __LINE__);

        c_ident();
        
        /* if we are not the first to output an ID prepend an 'else' */
        if (cnt > 0) 
            c_out("else ");

        if (!sp->id->range) {
            c_out("if (tag == ");
            c_expression(sp->id->init, 0);
        }
        else {
            c_out("if (tag >= ");
            c_expression(sp->id->init, 0);
            c_out(" && tag <= ");
            c_expression(sp->id->init->third.np, 0);
        }
    
        c_out(")\n");
        c_identlevel++;
        c_outi("return 1;\n");
        c_identlevel--;

        /* one more comparison */
        cnt++;
    }
    
    /* try all derived classes as well */
    if ((derivedp = sp->derived) != NULL) 
        while ((p =* derivedp++) != NULL) cnt = c_class_isidof_check_id(p, cnt);
     
    return cnt;
}
