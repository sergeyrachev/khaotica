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


/* cgetput.c */


#include "cdef.h"


/* process statements for vars that go to class scope */
static void c_stmts(list_t *stmts)
{
    list_t *lp = stmts;
    while (lp != NULL) {
        /* skip included statements (and their sub-statements) */
        if (lp->included) {
            lp = lp->next;
            continue;
        }
        switch (lp->type) {
        case flavor::Parser::token::DECL:
            /* only declare level 1 variables as class members */
            c_var_decl_inclass(lp->sp, lp->e1, lp->e2, lp->dims, lp->dim);
            break;
        case '{':
            /* block of statements */
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::EXPR:
            break;
        case flavor::Parser::token::LABEL:
            break;
        case flavor::Parser::token::CASE:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::DEFAULT:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::IF:
            c_stmts(lp->sub1);
            if (lp->sub2 != NULL) c_stmts(lp->sub2);
            break;
        case flavor::Parser::token::SWITCH:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::WHILE:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::DO:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::FOR:
            c_stmts(lp->sub1);
            break;
        case flavor::Parser::token::GOTO:
            break;
        case flavor::Parser::token::CONTINUE:
            break;
        case flavor::Parser::token::BREAK:
            break;
        case flavor::Parser::token::VERBATIM:
            verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_C:
            if (cpp()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_J:
            if (java()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_GET:
        case flavor::Parser::token::VERBATIM_PUT:
        case flavor::Parser::token::VERBATIM_BOTH:
        case flavor::Parser::token::VERBATIM_C_CPP:
        case flavor::Parser::token::VERBATIM_GET_C:
        case flavor::Parser::token::VERBATIM_PUT_C:
        case flavor::Parser::token::VERBATIM_BOTH_C:
        case flavor::Parser::token::VERBATIM_GET_J:
        case flavor::Parser::token::VERBATIM_PUT_J:
        case flavor::Parser::token::VERBATIM_BOTH_J:
            break;
        case flavor::Parser::token::PRAGMA:
            pragma(lp->sub1);
            break;
        case flavor::Parser::token::INCLUDE:
            if (cpp()) 
                c_out("#include \"%s\"\n", lp->e1->left.string);
            break;
        case flavor::Parser::token::IMPORT:
            c_outi("// import \"%s\"\n", lp->e1->left.string);
            break;
        case flavor::Parser::token::BAC_TYPE:
            break;
        default:
            fatal("Internal error CG%d: Unknown statement type %d", __LINE__, lp->type);
        }
        lp = lp->next;
    }
}

/* process statements (get() part) */
static void c_stmts_get(list_t *stmts)
{
    list_t *lp = stmts; 
    while (lp != NULL) {
        /* skip included statements (and their sub-statements) */
        if (lp->included) {
            lp = lp->next;
            continue;
        }
        switch (lp->type) {
        case flavor::Parser::token::DECL:
            c_var_decl_get(lp->sp, lp->e1, lp->e2, lp->e3, lp->align, lp->dims, lp->dim, lp->usenext);
            break;
        case '{':
            c_outi("{\n");
            c_identlevel++;
            c_stmts_get(lp->sub1);
            c_identlevel--;
            c_outi("}\n");
            break;
        case flavor::Parser::token::EXPR:
            c_ident();
            c_expression(lp->e1, 0);
            c_out(";\n");
            break;
        case flavor::Parser::token::LABEL:
            c_identlevel--;
            c_outi("%s:\n", lp->sp->name);
            c_identlevel++;
            break;
        case flavor::Parser::token::CASE:
            c_identlevel--;
            c_outi("case ");
            c_expression(lp->e1, 0);
            c_out(":\n");
            c_identlevel++;
            c_stmts_get(lp->sub1);
            break;
        case flavor::Parser::token::DEFAULT:
            c_identlevel--;
            c_outi("default:\n");
            c_identlevel++;
            c_stmts_get(lp->sub1);
            break;
        case flavor::Parser::token::IF:
			c_outi("if (");
			c_expression(lp->e1, 1);
			
            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_get(lp->sub1);
                c_identlevel--;
                c_outi("}\n"); 
            }
            else {
                c_out(")\n");
                c_stmts_get(lp->sub1);
            }
    		if (lp->sub2 != NULL) {
                if (lp->sub2->type != '{') {
                    c_outi("else {\n");
                    c_identlevel++;
                    c_stmts_get(lp->sub2);
                    c_identlevel--;
                    c_outi("}\n"); 
                }
                else {
                    c_outi("else\n"); 
                    c_stmts_get(lp->sub2);
                }
            }
			c_outi("\n");
			break;
            
        case flavor::Parser::token::SWITCH:
            c_outi("switch (");
            c_expression(lp->e1, 0);

            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_get(lp->sub1);
                c_identlevel--;
                c_outi("}\n"); 
            }
            else {
			    c_out(")\n");
                c_stmts_get(lp->sub1);
            }
			c_outi("\n");
            break;
            
        case flavor::Parser::token::WHILE:
            c_outi("while (");
            c_expression(lp->e1, 1);
            c_out(") {\n");
            c_identlevel++;
            if (lp->sub1->type != '{')
                c_stmts_get(lp->sub1);
            else
                c_stmts_get(lp->sub1->sub1);
            
            /* In the case of an infinit loop, the following statement is required in order
             * for the compiler to go through the rest of the statements after the loop. 
             */
            if (java()) 
                c_outi("if (1==0) break;\n");
            
		    c_identlevel--;
            c_outi("}\n"); 
            break;
            
        case flavor::Parser::token::DO:
            c_outi("do {\n");
            c_identlevel++;
            if (lp->sub1->type != '{') 
                c_stmts_get(lp->sub1);
            else
                c_stmts_get(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("} while (");
            c_expression(lp->e1, 1);
            c_out(");\n");
            break;
            
        case flavor::Parser::token::FOR:
            c_outi("for (");
            c_expression(lp->e1, 0);
            c_out("; ");
            c_expression(lp->e2, 1);
            c_out("; ");
            c_expression(lp->e3, 0);
            c_out(") {\n");
   	        c_identlevel++;
            if (lp->sub1->type != '{') 
                c_stmts_get(lp->sub1);
            else
                c_stmts_get(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("}\n");
            break;
            
        case flavor::Parser::token::GOTO:
            c_outi("goto %s;\n", lp->sp->name);
            break;
        case flavor::Parser::token::CONTINUE:
            c_outi("continue;");
            break;
        case flavor::Parser::token::BREAK:
            c_outi("break;\n");
            break;
        case flavor::Parser::token::VERBATIM_GET:
        case flavor::Parser::token::VERBATIM_BOTH:
            verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_GET_C:
        case flavor::Parser::token::VERBATIM_BOTH_C:
            if (cpp()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_GET_J:
        case flavor::Parser::token::VERBATIM_BOTH_J:
            if (java()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM:
        case flavor::Parser::token::VERBATIM_C:
        case flavor::Parser::token::VERBATIM_J:
        case flavor::Parser::token::VERBATIM_PUT:
        case flavor::Parser::token::VERBATIM_PUT_C:
        case flavor::Parser::token::VERBATIM_PUT_J:
        case flavor::Parser::token::VERBATIM_C_CPP:
            break;
        case flavor::Parser::token::PRAGMA:
            break;
        case flavor::Parser::token::INCLUDE:
            break;
        case flavor::Parser::token::IMPORT:
            c_outi("// import \"%s\"\n", lp->e1->left.string);
            break;
        case flavor::Parser::token::BAC_TYPE:
            if (lp->e2->left.ival)
                c_outi("%s::ad_init(%sbs);\n", lp->e1->left.sp->name, prefix);
            else
                c_outi("%s::ad_end(%sbs);\n", lp->e1->left.sp->name, prefix);
            break;
        default:
            fatal("Internal error CG%d: Unknown statement type %d", __LINE__, lp->type);
        }
        lp = lp->next;
    }
}

/* process statements (put() part) */
static void c_stmts_put(list_t *stmts)
{
    list_t *lp = stmts;
    while (lp != NULL) {
        /* skip included statements (and their sub-statements) */
        if (lp->included) {
            lp = lp->next;
            continue;
        }
        switch (lp->type) {
        case flavor::Parser::token::DECL:
            c_var_decl_put(lp->sp, lp->e1, lp->e2, lp->e3, lp->align, lp->dims, lp->dim, lp->usenext);
            break;
        case '{':
            c_outi("{\n");
            c_identlevel++;
            c_stmts_put(lp->sub1);
            c_identlevel--;
            c_outi("}\n");
            break;
        case flavor::Parser::token::EXPR:
            c_ident();
            c_expression(lp->e1, 0);
            c_out(";\n");
            break;
        case flavor::Parser::token::LABEL:
            c_identlevel--;
            c_outi("%s:\n", lp->sp->name);
            c_identlevel++;
            break;
        case flavor::Parser::token::CASE:
            c_identlevel--;
            c_outi("case ");
            c_expression(lp->e1, 0);
            c_out(":\n");
            c_identlevel++;
            c_stmts_put(lp->sub1);
            break;
        case flavor::Parser::token::DEFAULT:
            c_identlevel--;
            c_outi("default:\n");
            c_identlevel++;
            c_stmts_put(lp->sub1);
            break;
        case flavor::Parser::token::IF:
            c_outi("if (");
            c_expression(lp->e1, 1);

            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_put(lp->sub1);
                c_identlevel--;
                c_outi("}\n");
            }
            else {
                c_out(")\n");
                c_stmts_put(lp->sub1);
            }
            if (lp->sub2 != NULL) {
                if (lp->sub2->type != '{') {
                    c_outi("else {\n");
                    c_identlevel++;
                    c_stmts_put(lp->sub2);
                    c_identlevel--;
                    c_outi("}\n");
                }
                else {
                    c_outi("else\n");
                    c_stmts_put(lp->sub2);
                }
            }
			c_outi("\n");
			break;
            
        case flavor::Parser::token::SWITCH:
            c_outi("switch (");
            c_expression(lp->e1, 0);
			
            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_put(lp->sub1);
			    c_identlevel--;
                c_outi("}\n");
            }
            else {
                c_out(")\n");
                c_stmts_put(lp->sub1);
            }
			c_outi("\n");
            break;
            
        case flavor::Parser::token::WHILE:
            c_outi("while (");
            c_expression(lp->e1, 1);
            c_out(") {\n");
            c_identlevel++;
            if (lp->sub1->type != '{') 
                c_stmts_put(lp->sub1);
            else 
                c_stmts_put(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("}\n");
            break;
            
        case flavor::Parser::token::DO:
            c_outi("do {\n");
            c_identlevel++;
            if (lp->sub1->type != '{') 
                c_stmts_put(lp->sub1);
            else 
                c_stmts_put(lp->sub1->sub1);

            if (java())
                c_outi("if (1==0) break;\n");
               
            c_identlevel--;
            c_outi("} while (");
			c_expression(lp->e1, 1);
            c_out(");\n");
            break;
            
        case flavor::Parser::token::FOR:
            c_outi("for (");
            c_expression(lp->e1, 0);
            c_out("; ");
            c_expression(lp->e2, 1);
            c_out("; ");
            c_expression(lp->e3, 0);
            c_out(") {\n");
            c_identlevel++;
            if (lp->sub1->type != '{')
                c_stmts_put(lp->sub1);
            else 
                c_stmts_put(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("}\n");
            break;
            
        case flavor::Parser::token::GOTO:
            c_outi("goto %s;\n", lp->sp->name);
            break;
        case flavor::Parser::token::CONTINUE:
            c_outi("continue;");
            break;
        case flavor::Parser::token::BREAK:
            c_outi("break;\n");
            break;
        case flavor::Parser::token::VERBATIM_PUT:
        case flavor::Parser::token::VERBATIM_BOTH:
            verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_PUT_C:
        case flavor::Parser::token::VERBATIM_BOTH_C:
            if (cpp()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_PUT_J:
        case flavor::Parser::token::VERBATIM_BOTH_J:
            if (java()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM:
        case flavor::Parser::token::VERBATIM_C:
        case flavor::Parser::token::VERBATIM_J:
        case flavor::Parser::token::VERBATIM_GET:
        case flavor::Parser::token::VERBATIM_GET_C:
        case flavor::Parser::token::VERBATIM_GET_J:
        case flavor::Parser::token::VERBATIM_C_CPP:
            break;
        case flavor::Parser::token::PRAGMA:
            break;
        case flavor::Parser::token::INCLUDE:
            break;
        case flavor::Parser::token::IMPORT:
            c_outi("// import \"%s\"\n", lp->e1->left.string);
            break;
        case flavor::Parser::token::BAC_TYPE:
            if (lp->e2->left.ival)
                c_outi("%s::ae_init(%sbs);\n", lp->e1->left.sp->name, prefix);
            else
                c_outi("%s::ae_end(%sbs);\n", lp->e1->left.sp->name, prefix);
            break;
        default:
            fatal("Internal error CG%d: Unknown statement type %d", __LINE__, lp->type);
        }
        lp = lp->next;
    }
}

/* process statements (putxml() part) */
static void c_stmts_putxml(list_t *stmts)
{
    list_t *lp = stmts; 
    while (lp != NULL) {
        /* skip included statements (and their sub-statements) */
        if (lp->included) {
            lp = lp->next;
            continue;
        }
        switch (lp->type) {
        case flavor::Parser::token::DECL:
            c_var_decl_putxml(lp->sp, lp->e1, lp->e2, lp->e3, lp->align, lp->dims, lp->dim, lp->usenext);
            break;
        case '{':
            c_outi("{\n");
            c_identlevel++;
            c_stmts_putxml(lp->sub1);
            c_identlevel--;
            c_outi("}\n");
            break;
        case flavor::Parser::token::EXPR:
            c_ident();
            c_expression(lp->e1, 0);
            c_out(";\n");
            break;
        case flavor::Parser::token::LABEL:
            c_identlevel--;
            c_outi("%s:\n", lp->sp->name);
            c_identlevel++;
            break;
        case flavor::Parser::token::CASE:
            c_identlevel--;
            c_outi("case ");
            c_expression(lp->e1, 0);
            c_out(":\n");
            c_identlevel++;
            c_stmts_putxml(lp->sub1);
            break;
        case flavor::Parser::token::DEFAULT:
            c_identlevel--;
            c_outi("default:\n");
            c_identlevel++;
            c_stmts_putxml(lp->sub1);
            break;
        case flavor::Parser::token::IF:
			c_outi("if (");
			c_expression(lp->e1, 1);
			
            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_putxml(lp->sub1);
                c_identlevel--;
                c_outi("}\n"); 
            }
            else {
                c_out(")\n");
                c_stmts_putxml(lp->sub1);
            }
    		if (lp->sub2 != NULL) {
                if (lp->sub2->type != '{') {
                    c_outi("else {\n");
                    c_identlevel++;
                    c_stmts_putxml(lp->sub2);
                    c_identlevel--;
                    c_outi("}\n"); 
                }
                else {
                    c_outi("else\n"); 
                    c_stmts_putxml(lp->sub2);
                }
            }
			c_outi("\n");
			break;
            
        case flavor::Parser::token::SWITCH:
            c_outi("switch (");
            c_expression(lp->e1, 0);

            if (lp->sub1->type != '{') {
                c_out(") {\n");
                c_identlevel++;
                c_stmts_putxml(lp->sub1);
                c_identlevel--;
                c_outi("}\n"); 
            }
            else {
			    c_out(")\n");
                c_stmts_putxml(lp->sub1);
            }
			c_outi("\n");
            break;
            
        case flavor::Parser::token::WHILE:
            c_outi("while (");
            c_expression(lp->e1, 1);
            c_out(") {\n");
            c_identlevel++;
            if (lp->sub1->type != '{')
                c_stmts_putxml(lp->sub1);
            else 
                c_stmts_putxml(lp->sub1->sub1);
            
            /* In the case of an infinit loop, the following statement is required in order
             * for the compiler to go through the rest of the statements after the loop. 
             */
            if (java()) 
                c_outi("if (1==0) break;\n");
            
		    c_identlevel--;
            c_outi("}\n"); 
            break;
            
        case flavor::Parser::token::DO:
            c_outi("do {\n");
            c_identlevel++;
            if (lp->sub1->type != '{') 
                c_stmts_putxml(lp->sub1);
            else 
                c_stmts_putxml(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("} while (");
            c_expression(lp->e1, 1);
            c_out(");\n");
            break;
            
        case flavor::Parser::token::FOR:
            c_outi("for (");
            c_expression(lp->e1, 0);
            c_out("; ");
            c_expression(lp->e2, 1);
            c_out("; ");
            c_expression(lp->e3, 0);
            c_out(") {\n");
   	        c_identlevel++;
            if (lp->sub1->type != '{')
                c_stmts_putxml(lp->sub1);
            else 
                c_stmts_putxml(lp->sub1->sub1);

            if (java()) 
                c_outi("if (1==0) break;\n");

            c_identlevel--;
            c_outi("}\n");
            break;
            
        case flavor::Parser::token::GOTO:
            c_outi("goto %s;\n", lp->sp->name);
            break;
        case flavor::Parser::token::CONTINUE:
            c_outi("continue;");
            break;
        case flavor::Parser::token::BREAK:
            c_outi("break;\n");
            break;
        case flavor::Parser::token::VERBATIM_GET:
        case flavor::Parser::token::VERBATIM_BOTH:
            verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_GET_C:
        case flavor::Parser::token::VERBATIM_BOTH_C:
            if (cpp()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM_GET_J:
        case flavor::Parser::token::VERBATIM_BOTH_J:
            if (java()) 
                verbatim(lp->vp);
            break;
        case flavor::Parser::token::VERBATIM:
        case flavor::Parser::token::VERBATIM_C:
        case flavor::Parser::token::VERBATIM_J:
        case flavor::Parser::token::VERBATIM_PUT:
        case flavor::Parser::token::VERBATIM_PUT_C:
        case flavor::Parser::token::VERBATIM_PUT_J:
        case flavor::Parser::token::VERBATIM_C_CPP:
            break;
        case flavor::Parser::token::PRAGMA:
            break;
        case flavor::Parser::token::INCLUDE:
            break;
        case flavor::Parser::token::IMPORT:
            c_outi("// import \"%s\"\n", lp->e1->left.string);
            break;
        case flavor::Parser::token::BAC_TYPE:
            if (lp->e2->left.ival)
                c_outi("%s::ad_init(%sbs);\n", lp->e1->left.sp->name, prefix);
            else
                c_outi("%s::ad_end(%sbs);\n", lp->e1->left.sp->name, prefix);
            break;
        default:
            fatal("Internal error CG%d: Unknown statement type %d", __LINE__, lp->type);
        }
        lp = lp->next;
    }
}

/* process statements (xsd part) */
static void c_stmts_xsd(list_t *stmts)
{
    list_t *lp = stmts;
    while (lp != NULL) {
        /* skip included statements (and their sub-statements) */
        if (lp->included) {
            lp = lp->next;
            continue;
        }
        switch (lp->type) {
        case flavor::Parser::token::DECL:
            c_var_decl_xsd(lp->sp, lp->e1, lp->e2, lp->e3, lp->align, lp->dims, lp->dim, lp->usenext);
            break;
        case '{':
            c_stmts_xsd(lp->sub1);
            break;
        case flavor::Parser::token::EXPR:
            break;
        case flavor::Parser::token::LABEL:
            break;
        case flavor::Parser::token::CASE:
        case flavor::Parser::token::DEFAULT:
            c_outi("<xsd:sequence minOccurs=\"0\">\n");
            c_identlevel++;
            c_stmts_xsd(lp->sub1);
            c_identlevel--;
            c_outi("</xsd:sequence>\n");
            break;
        case flavor::Parser::token::IF:
            c_outi("<xsd:sequence minOccurs=\"0\">\n");
            c_identlevel++;
            c_stmts_xsd(lp->sub1);
            c_identlevel--;
            c_outi("</xsd:sequence>\n");
			/* else */
    		if (lp->sub2 != NULL) {
				c_outi("<xsd:sequence minOccurs=\"0\">\n");
				c_identlevel++;
				c_stmts_xsd(lp->sub2);
				c_identlevel--;
				c_outi("</xsd:sequence>\n");
            }
			break;
            
        case flavor::Parser::token::SWITCH:
			c_outi("<xsd:choice>\n");
			c_identlevel++;
			c_stmts_xsd(lp->sub1);
			c_identlevel--;
			c_outi("</xsd:choice>\n");
            break;
            
        case flavor::Parser::token::WHILE:
			c_outi("<xsd:sequence minOccurs=\"0\" maxOccurs=\"unbounded\">\n");
			c_identlevel++;
			c_stmts_xsd(lp->sub1);
			c_identlevel--;
			c_outi("</xsd:sequence>\n");
            break;
            
        case flavor::Parser::token::DO:
			c_outi("<xsd:sequence minOccurs=\"1\" maxOccurs=\"unbounded\">\n");
			c_identlevel++;
			c_stmts_xsd(lp->sub1);
			c_identlevel--;
			c_outi("</xsd:sequence>\n");
            break;
            
        case flavor::Parser::token::FOR:
			c_outi("<xsd:sequence minOccurs=\"0\" maxOccurs=\"unbounded\">\n");
			c_identlevel++;
			c_stmts_xsd(lp->sub1);
			c_identlevel--;
			c_outi("</xsd:sequence>\n");
            break;
            
        case flavor::Parser::token::GOTO:
        case flavor::Parser::token::CONTINUE:
        case flavor::Parser::token::BREAK:
        case flavor::Parser::token::VERBATIM_GET:
        case flavor::Parser::token::VERBATIM_BOTH:
        case flavor::Parser::token::VERBATIM_GET_C:
        case flavor::Parser::token::VERBATIM_GET_J:
        case flavor::Parser::token::VERBATIM_BOTH_J:
        case flavor::Parser::token::VERBATIM:
        case flavor::Parser::token::VERBATIM_C:
        case flavor::Parser::token::VERBATIM_J:
        case flavor::Parser::token::VERBATIM_PUT:
        case flavor::Parser::token::VERBATIM_PUT_C:
        case flavor::Parser::token::VERBATIM_PUT_J:
        case flavor::Parser::token::VERBATIM_C_CPP:
        case flavor::Parser::token::PRAGMA:
        case flavor::Parser::token::INCLUDE:
        case flavor::Parser::token::IMPORT:
            break;
        case flavor::Parser::token::BAC_TYPE:
            if (lp->e2->left.ival)
                c_outi("%s::ad_init(%sbs);\n", lp->e1->left.sp->name, prefix);
            else
                c_outi("%s::ad_end(%sbs);\n", lp->e1->left.sp->name, prefix);
            break;
        default:
            fatal("Internal error CG%d: Unknown statement type %d", __LINE__, lp->type);
        }
        lp = lp->next;
    }
}

/* generate class parameter list */
static void c_param_list(list_t *vars)
{
    int i;
    list_t *lp = vars;
    while (lp != NULL) {
        if (lp->type != flavor::Parser::token::DECL)
            fatal("Internal error CG%d: Expected declaration in parameter list", __LINE__);
        c_out("%s%s", s_mods2str(lp->sp->modifiers), lp->sp->ptype->name);
        /* class variable */
        if (lp->sp->ptype->ident == flavor::Parser::token::CLASS_TYPE) {
            if (cpp()) 
                c_out(" *%s", lp->sp->name);
            else if (java())
                c_out(" %s", lp->sp->name);
        }
        /* regular variable */
        else {
            if (lp->sp->dims == 0) { 
                c_out(" %s", lp->sp->name);
            }
            else {
                if (cpp()) {
                    c_out(" (&%s)", lp->sp->name);
                    if (lp->sp->dims > 0) 
                        for (i=0; i<lp->sp->dims; i++) 
                            c_out("[%d]", max_array_size);
                }
                else if (java()) {
                    if (lp->sp->dims > 0) 
                        for (i=0; i<lp->sp->dims; i++) 
                            c_out("[]");
                    c_out(" %s", lp->sp->name);
                }
            }
        }
        if (lp->next != NULL) 
            c_out(", ");
        lp = lp->next;
    }
}

/* generate class parameter list for put/get */
static void c_param_list_getput(list_t *vars)
{
    list_t *lp = vars;
    while (lp != NULL) {
        if (lp->type != flavor::Parser::token::DECL)
            fatal("Internal error CG%d: Expected declaration in parameter list", __LINE__);
        c_out("%s", lp->sp->name);
        if (lp->next != NULL) 
            c_out(", ");
        lp = lp->next;
    }
}

/* declare id probe vars for all contained class vars with id */
static void c_class_get_decl_probe_id_vars(symbol_t *sp)
{
    int i;
    symbol_t *p;
    symlink_t *slp;

    /* Here, we do not use the statements; since we are processing only parsable 
     * variables, we can find them in the top context of this class.
     */
    for (i=0; i<ST_SIZE; i++) {
        slp = sp->cxt->st[i];
        while (slp != NULL) {
            if (slp->sp == NULL)
                fatal("Internal error CG%d: Symbol link without symbol", __LINE__);
            p = slp->sp;
            if (p->ptype != NULL && p->ptype->ident == flavor::Parser::token::CLASS_TYPE &&
                !p->isparam && p->ptype->id != NULL && p->owner == sp) {
                /* declare tmp for id */
                c_outi("%s%s %s%s_id;\n", s_mods2str(p->ptype->id->modifiers & !M_CONST), p->ptype->id->ptype->name, prefix, p->name);
				
                /* tracing, then the parent class needs parse var for id probing */
				if (gen_trace) 
                    sp->get_needs_parse_var = 1;
            }
            slp = slp->next;
        }
    }
}

/* declare get()/put()/putxml() methods (C++ only) */
static void c_class_decl_methods(symbol_t *sp)
{
    /* get */
    if (gen_get) {
        c_outi("public: virtual int get(%s &%sbs", bitstream_class, prefix);
        /* if we have parameters, declare them */
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        /* we need to pass id parse info if this or a superclass has ID */
        if (c_find_id_notabstract(sp) != NULL)
            c_out(", int %sid_parsed = 0", prefix);
        
        c_out(");\n");
    }
    /* put */
    if (gen_put) {
        c_outi("public: virtual int put(%s &%sbs", bitstream_class, prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        if (c_find_id_notabstract(sp) != NULL)
            c_out(", int %sid_parsed = 0", prefix);
        c_out(");\n");
    }
    /* putxml */
    if (gen_putxml) {
        c_outi("public: virtual int putxml(%s &%sbs, short int bAttr", bitstream_class, prefix);
       if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        if (c_find_id_notabstract(sp) != NULL)
            c_out(", int %sid_parsed = 0", prefix);
        c_out(");\n");
    }
}

/*******************************/
/* generate class get() method */
/*******************************/

static void c_class_get(symbol_t *sp, list_t *stmts)
{
    c_out("\n");

    /* We need to pass ID parse info if this or a superclass has an ID.
     * For Java, this requires an extra get() method declared without the ID parse info.
     */
    if (java() && c_find_id_notabstract(sp) != NULL) {
        c_outi("public int get(%s %sbs", bitstream_class, prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        c_out(") throws IOException {\n");
		c_identlevel++;
        c_outi("return get(%sbs", prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }
        c_out(", 0);\n");
        c_identlevel--;
        c_outi("}\n\n");
    }

    if (cpp()) 
        c_outi("int %s::get(%s &%sbs", sp->name, bitstream_class, prefix);
    else if (java()) 
        c_outi("public int get(%s %sbs", bitstream_class, prefix);
    
    if (sp->param != NULL) {
        c_out(", ");
        c_param_list(sp->param);
    }

    /* we need to pass id parse info if this or a superclass (non-abstract) has ID */
    if (c_find_id_notabstract(sp) != NULL) 
        c_out(", int %sid_parsed", prefix);
    c_out(")");
	
    if (java())
        c_out(" throws IOException");

    c_out(" {\n");
	
    /* now we are inside the get() method */

    c_identlevel++;

    /* declare our return value (0 means no errors) */
    c_outi("int %sret = 0;\n", prefix);

    /* declare our map return value */
    if (java()) 
        c_outi("MapResult %smr;\n", prefix);
    
    /* if the class contains array variables, then we need to declare appropriate tmp variables */
    if (sp->max_dims > 0) 
        c_array_tmp_decl(sp->max_dims, sp->dimx_end_var);

    /* declare id probe vars for all contained class vars with id */
    c_class_get_decl_probe_id_vars(sp);    

    /* we may need to declare tmp var that saves parse size (so that parse expressions are evaluated once) */
    if (sp->get_needs_parse_var) 
        c_outi("int %sparse = 0;\n", prefix);
    if (sp->get_needs_code_var)
        c_outi("int %scode;\n", prefix);

    /* if tracing, signal the beginning of the get method */
    if (gen_trace) 
        c_trace_get_begin(sp);

    /* align, if needed; note that the top clas-level align is *NOT* included in bit accounting */
	if (sp->align != NULL) {
		if (gen_trace) {
			/* wave parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->align, 0);
			c_out(";\n");
			/* do the align */
    	    c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
            c_outi("%sbs.align(", prefix);
            c_expression(sp->align, 0);
            c_out(");\n");
		}
	}

    /* if lengthof requested, mark bit position of start of class */
    if (sp->lengthof) 
        c_outi("%slengthof = %sbs.getpos();\n", prefix, prefix);

    /* get our id, if needed */
    c_class_id_get(sp);
    
    /* see if it is a derived class */
    if (sp->ptype != NULL) {
        if (cpp()) 
            c_outi("%sret += %s::get(%sbs", prefix, sp->ptype->name, prefix);
        else if (java()) 
            c_outi("%sret += super.get(%sbs", prefix, prefix);

        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }

        /* pass along id parse info */
        if (c_find_id_notabstract(sp->ptype) != NULL) c_out(", %sid_parsed", prefix);
        c_out(");\n");
    }

    c_stmts_get(stmts);
    
    /* if lengthof requested, compute total bits */
    if (sp->lengthof) c_outi("%slengthof = %sbs.getpos()-%slengthof;\n", prefix, prefix, prefix);

    /* if tracing, signal the end of the get() */
    if (gen_trace) c_trace_get_end(sp);

    /* output return statement */
    c_outi("return %sret;\n", prefix);
    
    c_identlevel--;
    c_outi("}\n");
}

/*******************************/
/* generate class put() method */
/*******************************/

static void c_class_put(symbol_t *sp, list_t *stmts)
{
    c_out("\n");

    /* We need to pass ID parse info if this or a superclass has an ID.
     * For Java, this requires an extra put() method declared without this.
     */
    if (java() && c_find_id_notabstract(sp) != NULL) {
        c_outi("public int put(%s %sbs", bitstream_class, prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        c_out(") throws IOException {\n");
        c_identlevel++;
        c_outi("return put(%sbs", prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }
        c_out(", 0);\n");
        c_identlevel--;
        c_outi("}\n\n");
    }

    if (cpp()) 
        c_outi("int %s::put(%s &%sbs", sp->name, bitstream_class, prefix);
    else if (java())
        c_outi("public int put(%s %sbs", bitstream_class, prefix);
    
    /* if we have parameters, declare them */
    if (sp->param != NULL) {
        c_out(", ");
        c_param_list(sp->param);
    }

    /* we need to pass id parse info if this or a base class has ID */
    if (c_find_id_notabstract(sp) != NULL)
        c_out(", int %sid_parsed", prefix);
  
    c_out(")");

    if (java())	
        c_out(" throws IOException");
	
    c_out(" {\n");
    c_identlevel++;
    
    /* declare our return value (0 means no errors) */
    c_outi("int %sret = 0;\n", prefix);
    
    /* declare our map return value */
    if (java()) 
        c_outi("MapResult %smr;\n", prefix);

    /* if the class contains array variables, then we need to declare appropriate tmp variables */
    if (sp->max_dims > 0) 
        c_array_tmp_decl(sp->max_dims, sp->dimx_end_var);
    
    /* declare tmp var that saves parse size (to preserve order of parse and init evaluations) */
    if (sp->put_needs_parse_var) 
        c_outi("int %sparse = 0;\n", prefix);

    /* align, if necessary; note that the top clas-level align is *NOT* included in bit accounting */
    if (sp->align != NULL) {
        c_outi("%sbs.align(", prefix);
        c_expression(sp->align, 0);
        c_out(");\n");
    }
   
    /* if lengthof requested, mark bit position of start of class */
    if (sp->lengthof) 
        c_outi("%slengthof = %sbs.getpos();\n", prefix, prefix);

    /* output our id, if needed */
    c_class_id_put(sp);
    
    /* see if it is a derived class */
    if (sp->ptype != NULL) {
        if (cpp()) 
            c_outi("%sret += %s::put(%sbs", prefix, sp->ptype->name, prefix);
        else if (java())
            c_outi("%sret += super.put(%sbs", prefix, prefix);
        
        /* if we have params, include them */
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }
        
        /* pass along id parse info */
        if (c_find_id_notabstract(sp->ptype) != NULL)
            c_out(", %sid_parsed", prefix);
        c_out(");\n");
    }

    c_stmts_put(stmts);

    /* if lengthof requested, compute total bits */
    if (sp->lengthof) 
        c_outi("%slengthof = %sbs.getpos()-%slengthof;\n", prefix, prefix, prefix);
    
    /* output return statement */
    c_outi("return %sret;\n", prefix);
    
    c_identlevel--;
    c_outi("}\n");
}

/**********************************/
/* generate class putxml() method */
/**********************************/

static void c_class_putxml(symbol_t* sp, list_t* stmts)
{
    c_out("\n");

    /* We need to pass ID parse info if this or a superclass has an ID.
     * For Java, this requires an extra putxml() method declared without this.
     */
    if (java() && c_find_id_notabstract(sp) != NULL) {
        c_outi("public int putxml(%s %sbs, boolean bAttr", bitstream_class, prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list(sp->param);
        }
        c_out(") throws IOException {\n");
		c_identlevel++;
        c_outi("return putxml(%sbs, bAttr", prefix);
        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }
        c_out(", 0);\n");
        c_identlevel--;
        c_outi("}\n\n");
    }

    if (cpp()) 
        c_outi("int %s::putxml(%s &%sbs, short int bAttr", sp->name, bitstream_class, prefix);
    else if (java())
        c_outi("public int putxml(%s %sbs, boolean bAttr", bitstream_class, prefix);
    
    if (sp->param != NULL) {
        c_out(", ");
        c_param_list(sp->param);
    }

    /* we need to pass id parse info if this or a superclass (non-abstract) has ID */
    if (c_find_id_notabstract(sp) != NULL)
        c_out(", int %sid_parsed", prefix);

    c_out(")");
	
    if (java()) 
        c_out(" throws IOException");

    c_out(" {\n");
	
    /* now we are inside the putxml() method */

    c_identlevel++;

    /* declare our return value (0 means no errors) */
    c_outi("int %sret = 0;\n", prefix);

    /* declare our map return value */
    if (java()) 
        c_outi("MapResult %smr;\n", prefix);

    /* if the class contains array variables, then we need to declare appropriate tmp variables */
    if (sp->max_dims > 0) 
        c_array_tmp_decl(sp->max_dims, sp->dimx_end_var);

    /* declare id probe vars for all contained class vars with id */
    c_class_get_decl_probe_id_vars(sp);    

    /* we may need to declare tmp var that saves parse size (so that parse expressions are evaluated *once*) */
    c_outi("int %sparse = 0;\n", prefix);
    if (sp->get_needs_code_var) 
        c_outi("int %scode;\n", prefix);

     /* align, if needed; note that the top clas-level align is *NOT* included in bit accounting */
	if (sp->align != NULL) {
		if (gen_trace) {
			/* wave parse size */
			c_outi("%sparse = ", prefix);
			c_expression(sp->align, 0);
			c_out(";\n");
			/* do the align */
    	    c_outi("%sbs.align(%sparse);\n", prefix, prefix);
			/* trace */
			c_trace_aligned();
		}
		else {
            c_outi("%sbs.align(", prefix);
            c_expression(sp->align, 0);
            c_out(");\n");
		}
	}

    /* if lengthof requested, mark bit position of start of class */
    if (sp->lengthof) 
        c_outi("%slengthof = %sbs.getpos();\n", prefix, prefix);

    /* get our id, if needed */
    c_class_id_putxml(sp);
    
    /* see if it is a derived class */
    if (sp->ptype != NULL) {
        if (cpp()) 
            c_outi("%sret += %s::putxml(%sbs, bAttr", prefix, sp->ptype->name, prefix);
        else if (java())
            c_outi("%sret += super.putxml(%sbs, bAttr", prefix, prefix);

        if (sp->param != NULL) {
            c_out(", ");
            c_param_list_getput(sp->param);
        }

        /* pass along id parse info */
        if (c_find_id_notabstract(sp->ptype) != NULL)
            c_out(", %sid_parsed", prefix);
        c_out(");\n");
    }

    c_stmts_putxml(stmts);

    /* if lengthof requested, compute total bits */
    if (sp->lengthof)
        c_outi("%slengthof = %sbs.getpos()-%slengthof;\n", prefix, prefix, prefix);
   
    /* output return statement */
    c_outi("return %sret;\n", prefix);
    
    c_identlevel--;
    c_outi("}\n");   
}


/* generate isidof() method */
static void c_class_isidof(symbol_t *sp)
{
    int cnt;

    c_outi("\n");

    if (cpp()) 
        c_outi("int %s::%sisidof(%s tag) {\n", sp->name, prefix, sp->id->ptype->name);
    else if (java())
        c_outi("public static int %sisidof(%s tag) {\n", prefix, sp->id->ptype->name);

    c_identlevel++;
    cnt = c_class_isidof_check_id(sp, 0);
    if (cnt > 0) {
        c_outi("else\n");
        c_identlevel++;
        c_outi("return 0;\n");
        c_identlevel--;
    }
    else
        c_outi("return 0;\n");
    c_identlevel--;
    c_outi("}\n");
}

/*****************************************/
/* generate an XML element for the class */
/*****************************************/

/* generate the XML schema element descriptions for the class */
static void c_class_xsd(symbol_t* sp, list_t* stmts)
{
    /* get our id only once for the base class or the derived class */
    if (sp->ptype == NULL) {
        if (!(sp == NULL || sp->id == NULL))
	        c_var_decl_xsd(sp->id, NULL, NULL, NULL, NULL, 0, NULL, 0);
    }
    else {
        if (sp->ptype->id == NULL)
	        c_var_decl_xsd(sp->id, NULL, NULL, NULL, NULL, 0, NULL, 0);
    }
    
    c_stmts_xsd(stmts);
}

/*****************************/
/* generate class definition */
/*****************************/

/* generate class definition */
void class_decl(symbol_t *sp, list_t *stmts)
{
    FILE *tmp;
	int extend = 0;

    if (sp == NULL) 
        return;
    
    /* see if a forward declaration was given, but no definition */
    if (sp->forward > 0) {
        if (sp->forward == 1) {
            lineno = sp->lineno;
            yyerror("class '%s' was declared but never defined", sp->name);
        }
        else if (sp->forward == 2)
            sp->forward = 0;
        
        if (cpp())
            c_out("\nclass %s;\n", sp->name);
        
        return;
    }
 
    /* see if it is a derived class or not */
    if (sp->ptype == NULL) {
        if (cpp()) 
            c_out("\nclass %s {\npublic:\n", sp->name);
        else if (java())
            c_out("\npublic class %s {\n", sp->name);
		else if (xsd()) {
            root_xsd_name = sp->name;
			extend = 0;
			c_out("\n<xsd:complexType name=\"%s\">\n", sp->name);
			c_identlevel++;
			c_outi("<xsd:sequence>\n");
		}
    } else {
        if (cpp()) 
            c_out("\nclass %s : public %s {\npublic:\n", sp->name, sp->ptype->name);
        else if (java())
            c_out("\npublic class %s extends %s {\n", sp->name, sp->ptype->name);
		else if (xsd()) {
            root_xsd_name = sp->name;
            extend = 1;
			c_out("\n<xsd:complexType name=\"%s\">\n", sp->name);
			c_identlevel++;
			c_outi("<xsd:complexContent>\n");
			c_identlevel++;
			c_outi("<xsd:extension base=\"%s\">\n", sp->ptype->name);
			c_identlevel++;
			c_outi("<xsd:sequence>\n");
		}
    }
    c_identlevel++;
    
	/* don't need to declare anything for xsd */
	if (!xsd()) {
        /* declare id, if available, and if not defined by a base class */
        if (sp->id != NULL) {
            if (!sp->abstract) {
                /* parse var is needed in both get and put, unless the id has type larger than 'int' (float or double) */
                if (sp->id->ptype->ident <= flavor::Parser::token::INT) {
                    sp->put_needs_parse_var = 1;
                    sp->get_needs_parse_var = 1;
                }
            }
            if (c_find_id(sp->ptype) == NULL) {
                c_var_decl_inclass(sp->id, sp->id->init, NULL, 0, NULL);
                /* if abstract, reset put_needs_parse_var, which is set by the above call */
                if (sp->abstract) {
                    sp->put_needs_parse_var = 0;
                    sp->get_needs_parse_var = 0;
                }
            }
            else {
                /* declare lengthof variable, if needed */
                if (sp->id->lengthof) {
                    c_outi("int ");
                    c_out(sp->id->plengthof->name, prefix);
                    c_out(";\n");
                }
                /* mark the id as declared */
                sp->id->declared = 1;
                sp->id->declared_get = 1;
                sp->id->declared_put = 1;
                sp->id->declared_putxml = 1;
            }
        }

        /* declare lengthof variable, if needed */
        if (sp->lengthof) 
            c_outi("int %slengthof;\n", prefix);

        /* process class variable declarations */
        c_stmts(stmts);

        /* declare isidof method, if needed */
        if (sp->isidof && cpp()) 
            c_outi("public: static int %sisidof(%s tag);\n", prefix, sp->id->ptype->name);

        /* if doing C++, declare get()/put()/putxml() and switch output files */
        if (cpp()) {
            c_class_decl_methods(sp);
            c_identlevel = 0;
            tmp = ofp;
            ofp = ofp2; /* now ofp points to the .cpp file */
        }

        /* generate get() method */
        if (gen_get) 
            c_class_get(sp, stmts);

        /* generate put() method */
        if (gen_put) 
            c_class_put(sp, stmts);

        /* generate putxml() method */
        if (gen_putxml) 
            c_class_putxml(sp, stmts);

        /* generate isidof() method, if needed */
        if (sp->isidof) 
            c_class_isidof(sp);
    }
    else 
        c_class_xsd(sp, stmts);

    /* switch back */
    if (cpp()) {
        c_identlevel = 1; /* that's where we left off */
        ofp = tmp;        /* ofp is switched to the .h file */
    }

    c_identlevel--;

    if (cpp())
        c_out("};\n");  
    else if (java())
        c_out("}\n");
	else if (xsd()) {
		c_outi("</xsd:sequence>\n");

        /* the derived class */
		if (extend) {
			c_identlevel--;
			c_outi("</xsd:extension>\n");
			c_identlevel--;
			c_outi("</xsd:complexContent>\n");
		}
        /* the base class; the aligned attr only needs to be defined in the base class */
        else 
            c_outi("<xsd:attribute name=\"aligned\" type=\"xsd:unsignedInt\" use=\"optional\"/>\n");

		c_identlevel--;
        c_out("</xsd:complexType>\n");
	}
}
