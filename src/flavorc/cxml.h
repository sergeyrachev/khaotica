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
 * Danny Hong <danny@ee.columbia.edu>
 *
 */

#ifndef _cxml_h_
#define _cxml_h_


/* put the parsing of a variable with simple parse expr as an xml element */
void c_xml_var_simple(symbol_t *sp, node_t *align, int dims, node_t **dim);

/* put the parsing of a variable (escape value) with simple expr as an XML element */
void c_xml_map_var_simple(symbol_t *sp, int simple);


#endif /* ! _cxml_h_ */
