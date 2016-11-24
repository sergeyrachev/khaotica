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

#ifndef _cout_h_
#define _cout_h_


/* out identation level */
extern int c_identlevel;

/* generate correct identation, according to c_identlevel */
void c_ident(void);

/* output text */
void c_out(char *fmt, ...);

/* output idented text */
void c_outi(char *fmt, ...);


#endif /* ! _cout_h_ */
