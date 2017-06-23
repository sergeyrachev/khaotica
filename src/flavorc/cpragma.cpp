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

/* cpragma.c */

#include "cdef.h"

/* process pragma statements */
void pragma(list_t *lp)
{
    while (lp != NULL) {
        
        switch (lp->type) {
            
        case flavor::Parser::token::PRAGMA_ARRAY:
            /* make sure argument is integer literal */
            if (lp->e1->op != flavor::Parser::token::INT_LITERAL) {
                fatal("Internal error CP%d: Expected integer literal for array pragma", __LINE__);
                return;
            }
            if (lp->e1->left.ival <= 0) {
                fatal("Internal error CP%d: Invalid size for 'array' pragma: %d", __LINE__, lp->e1->left.ival);
                return;
            }
            max_array_size = lp->e1->left.ival;
            if (cpp()) c_out("// pragma: array=%d\n", max_array_size);
            break;
            
        case flavor::Parser::token::PRAGMA_GET:
            if (cpp()) c_out("// pragma: get\n");
            gen_get = 1;
            break;
        case flavor::Parser::token::PRAGMA_NOGET:
            if (cpp()) c_out("// pragma: noget\n");
            gen_get = 0;
            break;

        case flavor::Parser::token::PRAGMA_PUT:
            if (cpp()) c_out("// pragma: put\n");
            gen_put = 1;
            break;
        case flavor::Parser::token::PRAGMA_NOPUT:
            if (cpp()) c_out("// pragma: noput\n");
            gen_put = 0;
            break;
            
        case flavor::Parser::token::PRAGMA_PUTXML:
            if (cpp()) c_out("// pragma: putxml\n");
            gen_putxml = 1;
            break;
        case flavor::Parser::token::PRAGMA_NOPUTXML:
            if (cpp()) c_out("// pragma: noputxml\n");
            gen_putxml = 0;
            break;
            
        case flavor::Parser::token::PRAGMA_TRACE:
            if (cpp()) c_out("// pragma: trace\n");
            gen_trace = 1; 
            break;
        case flavor::Parser::token::PRAGMA_NOTRACE:
            if (cpp()) c_out("// pragma: notrace\n");
            gen_trace = 0;
            break;
            
        case flavor::Parser::token::PRAGMA_LINE:
            if (cpp()) c_out("// pragma: line\n");
            supp_lineno = 0;
            break;
        case flavor::Parser::token::PRAGMA_NOLINE:
            if (cpp()) c_out("// pragma: noline\n");
            supp_lineno = 1;
            break;
            
        case flavor::Parser::token::PRAGMA_NULLSTRINGS:
            if (cpp()) 
                c_out("// pragma: nullstrings\n");
            break;
        case flavor::Parser::token::PRAGMA_NONULLSTRINGS:
            if (cpp()) 
                c_out("// pragma: nonullstrings\n");
            break;

        case flavor::Parser::token::PRAGMA_INCLUDES:
        case flavor::Parser::token::PRAGMA_NOINCLUDES:
            // these two are processed as soon as they are parsed
            break;

        case flavor::Parser::token::PRAGMA_BITSTREAM:
            if (cpp()) c_out("// pragma: bitstream=\"%s\"\n", lp->e1->left.string);
            bitstream_class = lp->e1->left.string;
            break;
            
        case flavor::Parser::token::PRAGMA_PREFIX:
            if (cpp()) c_out("// pragma: prefix=\"%s\"\n", lp->e1->left.string);
            prefix = lp->e1->left.string;
            break;

        case flavor::Parser::token::PRAGMA_ERROR_FUNC:
            if (cpp()) c_out("// pragma: efunction=\"%s\"\n", lp->e1->left.string);
            err_func = lp->e1->left.string;
            break;
            
        case flavor::Parser::token::PRAGMA_TRACE_FUNC:
            if (cpp()) c_out("// pragma: tfunction=\"%s\"\n", lp->e1->left.string);
            trace_func = lp->e1->left.string;
            break;
            
        /* Danny 07/15/2002 */
        /*
        case PRAGMA_PUBLIC:
            if (cpp()) c_out("// pragma: public\n");
            access_type = 0; 
            break;

        case PRAGMA_PRIVATE:
            if (cpp()) c_out("// pragma: private\n");
            access_type = 1; 
            break;

        case PRAGMA_PROTECTED:
            if (cpp()) c_out("// pragma: protected\n");
            access_type = 2; 
            break;

        case PRAGMA_PACKAGE:
            if (cpp()) {
                c_out("// pragma: package\n");
                access_type = 0;
            }
            else
                access_type = 3; 
            break;
        */
        default:
            fatal("Internal error CP%d: Unknown pragma statement", __LINE__);
        }
        lp = lp->next;
    }
}
