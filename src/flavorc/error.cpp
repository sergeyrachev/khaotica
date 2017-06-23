/*
 * Copyright (c) 1997-2005 Alexandros Eleftheriadis, Danny Hong and 
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

/* error.c */

/* error handling */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "port.h"
#include "globals.h"
#include "error.h"


/* error handlers (note: almost all errors as handled via yyerror) */
void error(char *fmt, ...)
{
    va_list ap;
    
    /* keep track of how many errors we encountered */
    nerrors++;
    
    va_start(ap, fmt);
    
    if (!msvc) 
        fprintf(stderr, "%s: ", program);
    else 
        fprintf(stderr, "%s: error: ", program);
    
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/* fatal error handler */
void fatal(char *fmt, ...)
{
    va_list ap;
    
    /* keep track of how many errors we encountered */
    nerrors++;
    
    va_start(ap, fmt);
    
    if (!msvc)
        fprintf(stderr, "%s: ", program);
    else
        fprintf(stderr, "%s: error: ", program);
    
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    
    exit(nerrors);
}
