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

#ifndef _FLAVOR_H_
#define _FLAVOR_H_


#include "flavori.h"
#include "port.h"
#include "buffbitstream.h"


// XML functions
void CreateXmlHeader(const char* pcFileName, const char* pcName, const char* pcSchema=NULL);
void EndXml(const char* pcName);
void WriteXmlElement(char* pcFmt, ...);
void IntoAClass(const char* pcName, unsigned int nAlign);
void OutOfClass(char* pcFmt, ...);


#endif /* ! _FLAVOR_H_ */
