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

#ifndef _globals_h_
#define _globals_h_


#include "ptree.h"

/* program name */
extern const char program[];

/* current file being processed */
extern char *filename;

/* language used */
typedef enum {
	L_CPP,
	L_JAVA,
	L_XSD
} lang_t;
extern lang_t lang;

/* macros to test which language we are using */
#define cpp() (lang == L_CPP)
#define java() (lang == L_JAVA)
#define xsd() (lang == L_XSD)

/* access type used */
typedef enum {
    A_PUBLIC,
    A_PROTECTED,
    A_PRIVATE,
    A_PACKAGE
} access_t;
extern access_t access_type;

/* macros totest which access type we are using */
#define apublic() (access_type == A_PUBLIC)
#define aprotected() (access_type == A_PROTECTED)
#define aprivate() (access_type == A_PRIVATE)
#define apackage() (access_type == A_PACKAGE)

/* suppress line number info from verbatim sections */
extern int supp_lineno;

/* parse only - don't generate any code */
extern int parse_only;

/* generate put method */
extern int gen_put;

/* generate get method */
extern int gen_get;

/* initial maximum size of arrays */
extern int init_max_array_size;

/* maximum size of arrays -- may change dynamically via pragma statements */
extern int max_array_size;

/* generate MSVC-style error messages */
extern int msvc;

/* prefix to use for Flavor variables */
extern char *prefix;

/* function to use for reporting errors on the bitstream */
extern char *err_func;

/* function to use for tracing */
extern char* trace_func;

/* class to use for bitstream I/O */
extern char* bitstream_class;

/* class to use for global contants in Java */
extern char *global_class;
extern int first_global;

/* name of the Flavor library package */
extern char *flavor_pkg;

/* class to use for the result of MAP operation in Java (getvlc(), putvlc() ... */
extern char map_result[];

/* generate include directives */
extern int gen_includes;

/* generate tracing code */
extern int gen_trace;

/* do we need to declare tracing functions; this is set when we hit a 'trace' pragma */
extern int decl_trace;

/* number of errors encountered */
extern int nerrors;

/* current line number */
extern int lineno;

/* option settings */
extern int w_abstract;
extern int w_cyclic;
extern int w_includes;

/* generate null-terminated strings for arrays */
extern int null_strings;

/* file names and pointers used by the code generator */
extern char ofile[], ofile2[], opkg[];
extern FILE *ofp, *ofp2;

/* this is the list of parsed statements; the entire input file ends up in this list */
extern list_t *parsed_code;

/* are we currently processing an include file? (defined in lexer.l) */
int including(void);


/* XML specific */

/* generate the putxml() method */
extern int gen_putxml;

/* function to use for xml */
extern char default_xml_func1[];
extern char default_xml_func2[];
extern char default_xml_func3[];
extern char default_xml_func4[];
extern char default_xml_func5[];
extern char *xml_func1;
extern char *xml_func2;
extern char *xml_func3;
extern char *xml_func4;
extern char *xml_func5;

char *root_xsd_name;


#endif /* ! _globals_h_ */
