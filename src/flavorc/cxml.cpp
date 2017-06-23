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
 * Danny Hong <danny@ee.columbia.edu>
 *
 */

/* cxml.c */

#include "cdef.h"

/* helper function called by the function below */
static void c_xml_var_helper(symbol_t* sp, node_t *align, int dims, node_t **dim)
{
    if (align != NULL) {
        c_out(" aligned=\\\"");
        c_expression(align, 0);
        c_out("\\\"");
    }

    if (sp->ptype->ident != flavor::Parser::token::CHAR)
        c_out("%s", (sp->modifiers & M_LITTLE ? " big=\\\"false\\\"" : ""));
    
    if (cpp()) {
        c_out(" bitLen=\\\"%%d\\\">");
        /* output the content (between the element tags) in the correct format */
		if (sp->ptype->ident <= flavor::Parser::token::INT) {
			if (sp->ptype->ident == flavor::Parser::token::CHAR)
                c_out("%%c");
			else if (sp->modifiers & M_UNSIGNED) 
                c_out("%%u");
            else 
                c_out("%%d");
		}
		else if (sp->ptype->ident <= flavor::Parser::token::DOUBLE)
            c_out("%%g");
		else
            fatal("Internal error CX%d: Expected simple type", __LINE__);

        c_out("</%s>\", %sparse, %s", sp->name, prefix, sp->name);
        if (dims > 0)
            c_array_proper(sp, dims);
        c_out(");\n");
    }
    else if (java()) {
        c_out(" bitLen=\\\"");
        c_out("\" + %sparse + \"\\\">", prefix);
        /* output the content (between the element tags) in the correct format */
        c_out("\" + %s", sp->name);
		if (dims > 0) 
            c_array_proper(sp, dims);
		c_out(" + \"</%s>\");\n", sp->name);
    }
}

/* put the parsing of a variable with simple parse expr as an XML element */
void c_xml_var_simple(symbol_t* sp, node_t *align, int dims, node_t **dim)
{
    c_outi("if (bAttr) {\n");
    c_identlevel++;    

    if (cpp())
        c_outi("%s(\"<%s", xml_func3, sp->name);
    else if (java())
        c_outi("XML.%s(\"<%s", xml_func3, sp->name);
 
    /* bit values are accessed as integer values */
    if (sp->ptype->ident == flavor::Parser::token::CHAR) {
        if (sp->modifiers & M_UNSIGNED) 
            c_out(" type=\\\"flUChar\\\"");
        else 
            c_out(" type=\\\"flSChar\\\"");
    }
    else if (sp->ptype->ident <= flavor::Parser::token::INT) {
        if (sp->modifiers & M_UNSIGNED) 
            c_out(" type=\\\"flUInt\\\"");
        else 
            c_out(" type=\\\"flSInt\\\"");
    }
    else if (sp->ptype->ident == flavor::Parser::token::FLOAT)
        c_out(" type=\\\"flFloat\\\"");
    else if (sp->ptype->ident == flavor::Parser::token::DOUBLE)
        c_out(" type=\\\"flDouble\\\"");
	else 
        fatal("Internal error CX%d: Expected simple type", __LINE__);

    c_xml_var_helper(sp, align, dims, dim);

    c_identlevel--;    
    c_outi("}\n");

    // !bAttr

    c_outi("else {\n");
    c_identlevel++; 

    if (cpp())
        c_outi("%s(\"<%s", xml_func3, sp->name);
    else if (java())
        c_outi("XML.%s(\"<%s", xml_func3, sp->name);

    c_xml_var_helper(sp, align, dims, dim);

    c_identlevel--;    
    c_outi("}\n");
}

/* helper function called by the function below */
static void c_xml_map_var_helper(symbol_t* sp, int simple)
{
    if (sp->ptype->ident != flavor::Parser::token::CHAR)
        c_out("%s", (sp->modifiers & M_LITTLE ? " big=\\\"false\\\"" : ""));
    
    if (cpp()) {
        c_out(" bitLen=\\\"%%d\\\">");
        /* output the content (between the element tags) in the correct format */
		if (sp->ptype->ident <= flavor::Parser::token::INT) {
			if (sp->ptype->ident == flavor::Parser::token::CHAR)
                c_out("%%c");
			else if (sp->modifiers & M_UNSIGNED) 
                c_out("%%u");
            else 
                c_out("%%d");
		}
		else if (sp->ptype->ident <= flavor::Parser::token::DOUBLE)
            c_out("%%g");
		else
            fatal("Internal error CX%d: Expected simple type", __LINE__);
 
        if (simple)
            c_out("</%s>\", %sesc_bits, *%sarg);\n", "value", prefix, prefix);
        else
            c_out("</%s>\", %sesc_bits, %sarg%s%s);\n", sp->name, prefix, prefix, c_scope(), sp->name);
    }
    else if (java()) {
        c_out(" bitLen=\\\"");
        c_out("\" + %sesc_bits + \"\\\">", prefix);
        /* output the content (between the element tags) in the correct format */
        if (simple)
            c_out("\" + %sarg", prefix);
        else
            c_out("\" + %sarg%s%s", prefix, c_scope(), sp->name);

        c_out(" + \"</%s>\");\n", (simple ? "value" : sp->name));
    }
}

/* put the parsing of a variable (escape value) with simple expr as an XML element */
void c_xml_map_var_simple(symbol_t* sp, int simple)
{
    c_outi("if (bAttr) {\n");
    c_identlevel++;    

    if (cpp())
        c_outi("%s(\"<%s", xml_func3, (simple ? "value" : sp->name));
    else if (java())
        c_outi("XML.%s(\"<%s", xml_func3, (simple ? "value" : sp->name));
 
    /* bit values are accessed as integer values */
    if (sp->ptype->ident == flavor::Parser::token::CHAR) {
        if (sp->modifiers & M_UNSIGNED) 
            c_out(" type=\\\"flUChar\\\"");
        else 
            c_out(" type=\\\"flSChar\\\"");
    }
    else if (sp->ptype->ident <= flavor::Parser::token::INT) {
        if (sp->modifiers & M_UNSIGNED) 
            c_out(" type=\\\"flUInt\\\"");
        else 
            c_out(" type=\\\"flSInt\\\"");
    }
    else if (sp->ptype->ident == flavor::Parser::token::FLOAT)
        c_out(" type=\\\"flFloat\\\"");
    else if (sp->ptype->ident == flavor::Parser::token::DOUBLE)
        c_out(" type=\\\"flDouble\\\"");
	else 
        fatal("Internal error CX%d: Expected simple type", __LINE__);

    c_xml_map_var_helper(sp, simple);

    c_identlevel--;    
    c_outi("}\n");

    // !bAttr

    c_outi("else {\n");
    c_identlevel++; 

    if (cpp())
        c_outi("%s(\"<%s", xml_func3, (simple ? "value" : sp->name));
    else if (java())
        c_outi("XML.%s(\"<%s", xml_func3, (simple ? "value" : sp->name));

    c_xml_map_var_helper(sp, simple);

    c_identlevel--;    
    c_outi("}\n");
}
