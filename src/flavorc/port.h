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


#ifndef _PORT_H_
#define _PORT_H_


/* win32 automatically defined by the Visual C++ build environment */
#if defined(_WIN32_) || defined(WIN32)

#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <io.h>
#include <direct.h>

/* max length for filenames */
#define MAX_LEN _MAX_PATH
#define USE_EXCEPTION
#define DIR_DELIMITOR '\\'
#define DIR_DELIMITOR_S "\\"

#else /* ! WIN32 */

#define DIR_DELIMITOR '/'
#define DIR_DELIMITOR_S "/"

/* most Unix C++ compilers do not support exceptions; if yours does, uncomment the following */
 #define USE_EXCEPTION

#if HAVE_UNISTD_H
#include <sys/types.h>
#include <unistd.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#ifdef PATH_MAX
#define MAX_LEN PATH_MAX
#else
#define MAX_LEN 1024
#endif

#if HAVE_BSTRING_H
#include <bstring.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#endif /* ! WIN32 */


#endif /* ! _PORT_H_ */
