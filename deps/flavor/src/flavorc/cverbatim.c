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

/* cverbatim.c */

#include "cdef.h"


/* output verbatim code */
void verbatim(verbatim_t *vp)
{
    if (vp == NULL || vp->str == NULL) return;
    
    /* output preprocessor directive for file/line info */
    if (!supp_lineno && cpp()) {
        c_out("#");
        c_out("line %d \"%s\"\n", vp->lineno, filename);
    }
    c_outi("%s\n", vp->str);
}
