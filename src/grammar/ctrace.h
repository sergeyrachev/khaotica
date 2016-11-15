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

#ifndef _ctrace_h_
#define _ctrace_h_


/* trace the failed ID probing of a class sp */
void c_trace_failed_id_simple(symbol_t *sp, int dims);

/* trace the failed map-based ID probing of a class sp */
void c_trace_failed_id_map(symbol_t *sp, symbol_t *map, int dims);

/* trace the onset of a class get() call */
void c_trace_entering_class(symbol_t *sp, int dims);

/* trace the onset of a map-based class parsing */
void c_trace_entering_mapped_class(symbol_t *sp, int dims, symbol_t *map);

/* Trace the parsing of a variable with simple parse expr; 'incorrect' is set when there is a difference with
 * the expected value (if any).
 */
void c_trace_var_simple(symbol_t *sp, int dims, int usenext, int incorrect);

/* Trace the parsing of a variable with map parse expr; 'incorrect' is set when there is a difference with
 * the expected value (if any).
 */
void c_trace_var_map(symbol_t *sp, symbol_t *map, int dims, int usenext, int incorrect);

/* trace the parsing of a class with map parse expr */
void c_trace_class_map(symbol_t *sp, symbol_t *map, int dims, int usenext);

/* trace failed map lookup */
void c_trace_failed_map(symbol_t *map, symbol_t *sp, int dims);

/* trace align modifier */
void c_trace_aligned(void);

/* trace the beginning of a get() method */
void c_trace_get_begin(symbol_t *sp);

/* trace the end of a get() method */
void c_trace_get_end(symbol_t *sp);


#endif /* ! _ctrace_h_ */
