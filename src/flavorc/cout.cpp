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

/* cout.c */

#include "cdef.h"


/* our identation level */
int c_identlevel = 0;

/* generate correct identation, according to c_identlevel */
void c_ident(void)
{
    /* our identation string (4 chars) */
    static const char c_ident_str[] = "    ";
    int i = c_identlevel;
    
    if (ofp == NULL) return;

    while (i-- > 0) {
        fprintf(ofp, c_ident_str);
#ifdef _PRINTOUT_
		fprintf(stdout, c_ident_str);
#endif
	}
}

/* output text */
void c_out(const char *fmt, ...)
{
    va_list ap;
    
    if (ofp == NULL) return;

    va_start(ap, fmt);
    vfprintf(ofp, fmt, ap);
	
#ifdef _PRINTOUT_
	vfprintf(stdout, fmt, ap);
#endif 
	
    va_end(ap);
}

/* output idented text */
void c_outi(const char *fmt, ...)
{
    va_list ap;
    
    if (ofp == NULL) return;
    
    c_ident();
    va_start(ap, fmt);
    vfprintf(ofp, fmt, ap);
	
#ifdef _PRINTOUT_
	vfprintf(stdout, fmt, ap);
#endif

    va_end(ap);
}
