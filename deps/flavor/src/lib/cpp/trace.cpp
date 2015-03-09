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
 * Author:
 * Alexandros Eleftheriadis <eleft@ee.columbia.edu>
 * Danny Hong <danny@ee.columbia.edu>
 *
 */


/* trace.cpp */


#include <stdio.h>
#include <stdarg.h>


// Flag for triggering printing of trace header
static int trace_print_header=1;

// Output bit position, size, value, and description of an integer quantity.
void trace(int pos, int size, unsigned int val, char* fmt, ...)
{
    int nbytes, i;
    unsigned char byte[4];
    
    /* Print header, if necessary */
    if (trace_print_header) {
        fprintf(stdout, "   At Bit  Size    Value    Description\n");
        trace_print_header=0;
    }
    
    /* Print bit position and size of quantity */
    if (size>0) fprintf(stdout, "%9d: %4d ", pos, size);
    else fprintf(stdout, "%9d:      ", pos);
    
    /* Split value into bytes */
    byte[3]=(unsigned char)(val>>24);
    byte[2]=(unsigned char)(val>>16);
    byte[1]=(unsigned char)(val>>8);
    byte[0]=(unsigned char)val;
    
    /* Output bits */
    if (size==0) nbytes=0;
    else nbytes=(size-1)/8+1;
    
    for (i=4; i>nbytes; i--) fprintf(stdout, "   ");
    for (i=nbytes-1; i>=0; i--) fprintf(stdout, "%.2X ", byte[i]);
    
    /* Print description */
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    
    fprintf(stdout, "\n");
}

// Output bit position, size, value, and description of a double quantity
void trace(int pos, int size, double val, char* fmt, ...)
{
    /* Print header, if necessary */
    if (trace_print_header) {
        fprintf(stdout, "   At Bit  Size    Value    Description\n");
        trace_print_header=0;
    }
    
    /* Print bit position,size, and value of quantity */
    if (size>0) fprintf(stdout, "%9d: %4d %11g ", pos, size, val);
    else fprintf(stdout, "%9d:                  ", pos);
    
    /* Print description */
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    
    fprintf(stdout, "\n");
}

