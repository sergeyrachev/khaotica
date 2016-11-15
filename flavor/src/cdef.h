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

#ifndef _def_h_
#define _def_h_


#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "port.h"
#include "globals.h"
#include "symbol.h"
#include "ptree.h"
#include "compile.h"
#include "check.h"
#include "util.h"
#include "error.h"

#include "parser.tab.h"

/* code generation includes */
#include "cout.h"
#include "cverbatim.h"
#include "cpragma.h"
#include "carray.h"
#include "cexpr.h"
#include "ctrace.h"
#include "cclass.h"
#include "cvar.h"
#include "cgetput.h"
#include "cid.h"
#include "cmap.h"
#include "cbac.h"
#include "cxml.h"


/* generate correct scoping operator */
#define c_scope() (cpp() ? "->" : ".")

/* generate correct static scoping operator */
#define c_static_scope() (cpp() ? "::" : ".")


#endif /* ! _def_h_ */
