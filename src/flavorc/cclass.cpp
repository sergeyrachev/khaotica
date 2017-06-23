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

/* cclass.c */

#include "cdef.h"


/*
 * Check if a class is pure abstract (has no non-abstract derived with an ID)
 */

/* recursive helper -- called by c_is_pure_abstract */
static int c_not_pure_abstract(symbol_t *sp)
{
    symbol_t *p, **derivedp;
    int ret = 0;

    if (sp == NULL) 
        return 0;

    if (sp->id != NULL && !sp->abstract) 
        return 1;

    /* try all derived classes as well */
    if ((derivedp = sp->derived) != NULL)
        while ((p =* derivedp++) != NULL) 
            ret += c_not_pure_abstract(p);

    return ret;
}

/* determine if a class is a pure abstract class */
int c_is_pure_abstract(symbol_t *sp)
{
    if (sp->id == NULL || !sp->abstract || sp->derived != NULL) 
        return 0;
    return 
        !c_not_pure_abstract(sp);
}

/* create a new class */
static void c_new_class(symbol_t *sp, int dims)
{
    c_outi("%s", sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    
    if (cpp()) 
        c_out(" = new(%s);\n", sp->ptype->name);
    else if (java())
        c_out(" = new %s();\n", sp->ptype->name);
}

/* resolve a polymorphic get/putxml: create a class based on the value of the probed id */
static void c_var_decl_class_poly(symbol_t *sp, node_t *param, int dims, node_t **dim, int xml)
{
    int numIDs, i;

    /* if non-pure and has ID, check its value */
    if (sp->ptype->id->init == NULL)
        fatal("Internal error CC%d: ID without value", __LINE__);

    /* align, if needed */
    if (sp->ptype->id->align != NULL) {
        if (gen_trace) {
            /* save parse size */
            c_outi("%sparse = ", prefix);
            c_expression(sp->ptype->id->align, 0);
            c_out(";\n");
            /* do the align */
            c_outi("%sbs.align(%sparse);\n", prefix, prefix);
            /* trace */
            c_trace_aligned();
        }
        else {
            c_outi("%sbs.align(", prefix);
            c_expression(sp->ptype->id->align, 0);
            c_out(");\n");
        }
    }
    
    /* declare temp variable to hold probed id value; call the nextbits/nextvlc function */
    if (sp->ptype->id->parse->op == flavor::Parser::token::MAP_TYPE)
        c_var_decl_get_class_probe_id_map(sp, dims);
    else
        c_var_decl_get_class_probe_id_simple(sp);

    /* Generate comparison statements with IDs of this and all derived classes.
     * We use 'if's so that float and double can be used as IDs.
     */
    numIDs = c_var_decl_get_class_id(sp, dims, sp->ptype, 0, xml);

    /* generate default statement */
    c_outi("else {\n");
    c_identlevel++;

    /* trace failed ID lookup (simple; map-based is reported separately) */
    if (gen_trace) 
        c_trace_failed_id_simple(sp, dims);
    
    /* call error function */
    if (cpp()) {
        c_outi("%s(\"Unknown ID value for class '%s'", err_func, sp->name);
        for (i=0; i<dims; i++) 
            c_out("[%%d]");

        c_out(" of type '%s': ", sp->ptype->name);
        if (sp->ptype->id->ptype->ident <= flavor::Parser::token::INT)
            c_out("%%d");
        else 
            c_outi("%%g");
        c_out("\\n\", ");
        for (i=0; i<dims; i++) 
            c_out("%s, ", c_gentmp_dim(i));
        c_out("%s%s_id);\n", prefix, sp->name);
    }
    else if (java()) {
        c_outi("%s(\"Unknown ID value for class '%s'", err_func, sp->name);
        for (i=0; i<dims; i++) 
            c_out("\" + %s + \"", c_gentmp_dim(i));
        c_out("' of type '%s': \" + %s%s_id);\n", sp->ptype->name, prefix, sp->name);
    }

    /* construct a base class so that we have something to work with */
    c_new_class(sp, dims);
    
    c_identlevel--;
    c_outi("}\n");
}

/********************************************/
/* class declarations/definitions for get() */
/********************************************/

/* generate get part of variable declarations of type class (simple) */
static void c_var_decl_get_class_simple(symbol_t *sp, node_t *param, int dims, node_t **dim)
{
    /* if we are tracing, signal which class we are going to read */
    if (gen_trace) 
        c_trace_entering_class(sp, dims);
    
	/* align, if necessary */
	if (sp->ptype->align != NULL) {
		if (gen_trace) {
			/* save parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(";\n");
			/* do the align */
			c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
			c_outi("%sbs.align(", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(");\n");
		}
	}

    /* check for pure abstract classes */
    if (c_is_pure_abstract(sp->ptype)) {
        if (w_abstract) {
            lineno = sp->lineno;
            yyerror("abstract class '%s' is pure (has no non-abstract parsable derived classes)", sp->ptype->name);
        }
        /* construct a base class so that we have something to work with */
        c_new_class(sp, dims);
    }
    /* figure out which class to create */
    else if (sp->ptype->id != NULL) 
        c_var_decl_class_poly(sp, param, dims, dim, 0);
    /* class without an ID; just create the object */
    else 
        c_new_class(sp, dims);

    /* now call the get() method of the class */
    c_outi("%sret += %s", prefix, sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);

    if (param != NULL) {
        c_out("%sget(%sbs, ", c_scope(), prefix);
        c_expression(param, 0);
        c_out(");\n");
    }
    else 
        c_out("%sget(%sbs);\n", c_scope(), prefix);
}

/* generate get part of variable declarations of type class (map) */
static void c_var_decl_get_class_map(symbol_t *sp, symbol_t *map, int dims, node_t **dim, int usenext)
{
    int i;

    if (gen_trace)
        c_trace_entering_mapped_class(sp, dims, map);

	/* align, if necessary */
	if (sp->ptype->align != NULL) {
		if (gen_trace) {
			/* save parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(";\n");
			/* do the align */
			c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
			c_outi("%sbs.align(", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(");\n");
		}
	}

    /* call nextvlc()/getvlc() - without address-of operator; classes always use pointers */
    if (cpp()) {
		c_new_class(sp, dims);
       	c_outi("if (%s%s%s(%sbs, %s", 
               map->name, c_static_scope(), (usenext ? "nextvlc" : "getvlc"), prefix, sp->name);

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
        c_outi("%smr = %s%s%s(%sbs);\n", 
               prefix, map->name, c_static_scope(), (usenext ? "nextvlc" : "getvlc"), prefix);

		if (gen_trace || sp->lengthof)
            c_outi("%sparse = %smr.getCodeLength();\n", prefix, prefix);

        if (gen_trace)
            c_outi("%scode = %smr.getCode();\n", prefix, prefix);

		c_outi("if (!%smr.isHit()) {\n", prefix);
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
    }
    else if (java()) {
		for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("' failed\"");
	}
    c_out(");\n");
    
    /* increase error count */
    c_outi("%sret++;\n", prefix);

    /* trace failed lookup */
    if (gen_trace) 
        c_trace_failed_map(map, sp, dims);

    c_identlevel--;
    c_outi("}\n");
	if (java()) {
		c_outi("else {\n");
		c_identlevel++;
		c_outi(sp->name);
		for (i=0; i<dims; i++)
            c_out("[%s]", c_gentmp_dim(i));
		c_out(" = (%s)%smr._o;\n", map->ptype->name, prefix);
		c_identlevel--;
		c_outi("}\n");
	}

    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi("%s", sp->name);
        if (dims > 0) 
            c_array_proper(sp, dims);
        c_out(c_scope());
        c_out(sp->plengthof->name, prefix);
        c_out(" = %sparse;\n", prefix);
    }

    /* no expected value for classes -- just trace */
    if (gen_trace) 
        c_trace_class_map(sp, map, dims, usenext);
}

/* generate the get part of variable declarations of type class */
void c_var_decl_get_class(symbol_t *sp, node_t *parse, node_t *param, int dims, node_t **dim, int usenext)
{
    if (dims > 0) 
        c_array_prologue(sp, dims, dim);
    
    if (parse != NULL && parse->op == flavor::Parser::token::MAP_TYPE)
        c_var_decl_get_class_map(sp, parse->left.sp, dims, dim, usenext);
    else
        c_var_decl_get_class_simple(sp, param, dims, dim);
 
    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/********************************************/
/* class declarations/definitions for put() */
/********************************************/

/* generate put part of variable declarations of type class (simple) */
static void c_var_decl_put_class_simple(symbol_t *sp, node_t *param, int dims)
{
    c_outi("%sret += %s", prefix, sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);
    
    if (param != NULL) {
        c_out("%sput(%sbs, ", c_scope(), prefix);
        c_expression(param, 0);
        c_out(");\n");
    }
    else 
        c_out("%sput(%sbs);\n", c_scope(), prefix);
}

/* generate put part of variable declaration of type class (map) */
static void c_var_decl_put_class_map(symbol_t *sp, symbol_t *map, int dims)
{
    int i;

    /* call putvlc/checkvlc */
	if (cpp()) {
    	c_outi("if (%s%sputvlc(%sbs, %s", map->name, c_static_scope(), prefix, sp->name);
		
        if (dims > 0) 
            c_array_proper(sp, dims);

		/* if we need lengthof, get number of bits */
		if (sp->lengthof) {
			c_out(", &%s", sp->name);
			if (dims > 0) 
                for (i=0; i<dims; i++)
                    c_out("[%s]", c_gentmp_dim(i));
			c_out("%s%slengthof", c_scope(), prefix);
		}
		c_out(") == 0) {\n");
	}
	else if (java()) {
        c_outi("%smr = %s%sputvlc(%sbs,%s", prefix, map->name, c_static_scope(), prefix, sp->name);

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
	}
    else if (java()) {
        for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
        c_out("' failed\"");
    }
    c_out(");\n");

	/* increase error count */
    c_outi("%sret++;\n", prefix);
	c_identlevel--;
	c_outi("}\n");
}

/* generate the put part of variable declarations of type class */
void c_var_decl_put_class(symbol_t *sp, node_t *param, node_t *parse, int dims, node_t **dim, int usenext)
{
    if (dims > 0)
        c_array_prologue(sp, dims, dim);

    if (parse == NULL) 
        c_var_decl_put_class_simple(sp, param, dims);
    else if (parse->op == flavor::Parser::token::MAP_TYPE) {
        if (!usenext)
            c_var_decl_put_class_map(sp, parse->left.sp, dims);
    }
    else 
        fatal("Internal error CC%d: Class with non-map parse size", __LINE__);

    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/***********************************************/
/* class declarations/definitions for putxml() */
/***********************************************/

/* generate putxml part of variable declarations of type class (simple) */
static void c_var_decl_putxml_class_simple(symbol_t *sp, node_t *param, int dims, node_t **dim)
{
    /* if we are tracing, signal which class we are going to read */
    if (gen_trace)
        c_trace_entering_class(sp, dims);
    
	/* align, if necessary */
	if (sp->ptype->align != NULL) {
		if (gen_trace) {
			/* save parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(";\n");
			/* do the align */
			c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
			c_outi("%sbs.align(", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(");\n");
		}
	}

    /* check for pure abstract classes */
    if (c_is_pure_abstract(sp->ptype)) {
        if (w_abstract) {
            lineno = sp->lineno;
            yyerror("abstract class '%s' is pure (has no non-abstract parsable derived classes)", sp->ptype->name);
        }
        /* construct a base class so that we have something to work with */
        c_new_class(sp, dims);
    }
    /* figure out which class to create */
    else if (sp->ptype->id != NULL) 
        c_var_decl_class_poly(sp, param, dims, dim, 1);
    /* class without an ID; just create the object */
    else {
        c_new_class(sp, dims);

        /* signal the beginning of parsing the class */
        if (cpp()) 
            c_outi("%s(\"%s\", ", xml_func4, sp->name);
        else if (java()) 
            c_outi("XML.%s(\"%s\", ", xml_func4, sp->name);

        if (sp->ptype->align != NULL)
	        c_expression(sp->ptype->align, 0);
        else
            c_out("0");
        
        c_out(");\n");
    }

    /* now call the putxml() method of the class */
    c_outi("%sret += %s", prefix, sp->name);
    if (dims > 0) 
        c_array_proper(sp, dims);

    if (param != NULL) {
        c_out("%sputxml(%sbs, bAttr, ", c_scope(), prefix);
        c_expression(param, 0);
        c_out(");\n");
    } 
    else 
        c_out("%sputxml(%sbs, bAttr);\n", c_scope(), prefix);
    
    /* signal the end of parsing the class */
    if (cpp()) 
        c_outi("%s(\"</%s>\");\n", xml_func5, sp->name);
    else if (java())
        c_outi("XML.%s(\"</%s>\");\n", xml_func5, sp->name);
}

/* generate putxml part of variable declarations of type class (map) */
static void c_var_decl_putxml_class_map(symbol_t *sp, symbol_t *map, int dims, node_t **dim)
{
    int i;

    if (gen_trace)
        c_trace_entering_mapped_class(sp, dims, map);

	/* align, if necessary */
	if (sp->ptype->align != NULL) {
		if (gen_trace) {
			/* save parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(";\n");
			/* do the align */
			c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
			c_outi("%sbs.align(", prefix);
			c_expression(sp->ptype->align, 0);
			c_out(");\n");
		}
	}

    /* signal the beginning of parsing the class */
    if (cpp())
        c_outi("%s(\"%s\", ", xml_func4, sp->name);
    else if (java())
        c_outi("XML.%s(\"%s\", ", xml_func4, sp->name);
    if (sp->ptype->align != NULL)
	    c_expression(sp->ptype->align, 0);
    else
        c_out("0");
    c_out(");\n");

    /* call getvlc()/nextvlc() - without address-of operator; classes always use pointers */
    if (cpp()) {
		c_new_class(sp, dims);
        c_outi("if (%s%sputxml(%sbs, bAttr, %s", map->name, c_static_scope(), prefix, sp->name);

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
        c_outi("%smr = %s%sputxml(%sbs, bAttr);\n", prefix, map->name, c_static_scope(), prefix);

		if (gen_trace) 
            c_outi("%scode = %smr.getCode();\n", prefix, prefix);
		if (gen_trace || sp->lengthof) 
            c_outi("%sparse = %smr.getCodeLength();\n", prefix, prefix);
		c_outi("if (!%smr.isHit()) {\n", prefix);
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
    }
    else if (java()) {
		for (i=0; i<dims; i++) 
            c_out("[\" + %s + \"]", c_gentmp_dim(i));
		c_out("' failed\"");
	}
    c_out(");\n");
    
    /* increase error count */
    c_outi("%sret++;\n", prefix);

    /* trace failed lookup */
    if (gen_trace) 
        c_trace_failed_map(map, sp, dims);

    c_identlevel--;
    c_outi("}\n");
	if (java()) {
		c_outi("else {\n");
		c_identlevel++;
		c_outi(sp->name);
		for (i=0; i<dims; i++) 
            c_out("[%s]", c_gentmp_dim(i));
		c_out(" = (%s)%smr._o;\n",map->ptype->name,prefix);
		c_identlevel--;
		c_outi("}\n");
	}

    /* signal the end of parsing the class */
    if (cpp()) 
        c_outi("%s(\"</%s>\");\n", xml_func5, sp->name);
    else if (java())
        c_outi("XML.%s(\"</%s>\");\n", xml_func5, sp->name);

    /* update length, if necessary (ignore usenext) */
    if (sp->lengthof) {
        c_outi("%s", sp->name);
        if (dims > 0)
            c_array_proper(sp, dims);
        c_out(c_scope());
        c_out(sp->plengthof->name, prefix);
        c_out(" = %sparse;\n", prefix);
    }

    /* no expected value for classes -- just trace */
    if (gen_trace) 
        c_trace_class_map(sp, map, dims, 0);
}

/* generate the putxml part of variable declarations of type class */
void c_var_decl_putxml_class(symbol_t *sp, node_t *parse, node_t *param, int dims, node_t **dim, int usenext)
{
    if (dims > 0) 
        c_array_prologue(sp, dims, dim);
    
    if (parse != NULL && parse->op == flavor::Parser::token::MAP_TYPE) {
        if (!usenext)
            c_var_decl_putxml_class_map(sp, parse->left.sp, dims, dim);
    }
    else
        c_var_decl_putxml_class_simple(sp, param, dims, dim);
 
    if (dims > 0) 
        c_array_epilogue(sp, dims, dim);
}

/********************************************/
/* class declarations/definitions for xsd() */
/********************************************/

/* generate XML element schema description for variables of type class (simple) */
static void c_var_decl_xsd_class_simple(symbol_t *sp, node_t *param, int dims)
{
    if (dims > 0) {
        if (!sp->declared_xsd) {
            c_outi("<xsd:element name=\"%s\" type=\"%s\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n", sp->name, sp->ptype->name);
            sp->declared_xsd = 1;
        }
        else return;
    }     
    else
        c_outi("<xsd:element name=\"%s\" type=\"%s\"/>\n", sp->name, sp->ptype->name);
}

/* generate XML element schema description for variables of type class (map) */
static void c_var_decl_xsd_class_map(symbol_t *sp, symbol_t *map, int dims)
{
    if (dims > 0) {
        if (!sp->declared_xsd) {
            c_outi("<xsd:element name=\"%s\" type=\"%s\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n", sp->name, map->name);
            sp->declared_xsd = 1;
        }
        else return;
    }     
    else 
        c_outi("<xsd:element name=\"%s\" type=\"%s\"/>\n", sp->name, map->name);
}

/* generate the corresponding XML schema for variable declarations of type class */
void c_var_decl_xsd_class(symbol_t *sp, node_t *parse, node_t *param, int dims)
{
    if (parse != NULL && parse->op == flavor::Parser::token::MAP_TYPE)
        c_var_decl_xsd_class_map(sp, parse->left.sp, dims);
    else 
        c_var_decl_xsd_class_simple(sp, param, dims);
}


