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
 * Danny Hong <danny@ee.columbia.edu>
 *
 */


/* xml.cpp */


#include <stdio.h>
#include <stdarg.h>


// Keeps track of indentation
static int g_nIndent = 0;   // The current indentation level
FILE* g_fpXML = NULL;       // The current XML file


/* PutIndents:
 *  Put right amount of spaces for indentation according to g_nIndent.
 */
void PutIndents(void)
{
    for (int i=0; i<=g_nIndent; i++) 
        fprintf(g_fpXML, "    ");
}


/* CreateXmlHeader:
 *  Output the header for the XML file.
 * In:
 *  pcFileName  - The XML file name
 *  pcName      - The XML root tag name
 *  pcSchema    - The XML schema name, if existing
 */
void CreateXmlHeader(const char* pcFileName, const char* pcName, const char* pcSchema=NULL)
{
    // Open a file as an XML document
    g_fpXML = fopen(pcFileName, "w+");

    // Dump header
    if (pcSchema) {
        fprintf(g_fpXML,
                "<?xml version=\"1.0\"?>\n"
		        "<%s\n"
                "    xmlns=\"http://www.ee.columbia.edu/flavor\"\n"
                "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                "    xsi:schemaLocation=\"http://www.ee.columbia.edu/flavor\n"
                "                        %s\"\n"
                ">\n",
                pcName, pcSchema);
    } 
    else {
        fprintf(g_fpXML,
                "<?xml version=\"1.0\"?>\n"
		        "<%s\n"
                "    xmlns=\"http://www.ee.columbia.edu/flavor\"\n"
                "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                ">\n",
                pcName);
    }
}


/* EndXml:
 *  Finish up the XML file.
 * In:
 *  pcName1 - The XML root tag name
 */
void EndXml(const char* pcName)
{
    // Dump header
    fprintf(g_fpXML, "</%s>", pcName);
    fclose(g_fpXML);
    g_fpXML = NULL;
}


/* WriteXmlElement:
 *  Output the XML element for the given parsable variable.
 * In:
 *  pcFmt   - The element to be output
 */
void WriteXmlElement(char* pcFmt, ...)
{
    PutIndents();
    va_list ap;
    va_start(ap, pcFmt);
    vfprintf(g_fpXML, pcFmt, ap);
    fprintf(g_fpXML, "\n");
    va_end(ap);
}


/* IntoAClass:
 *  Indicates that we are entering a class.  Keeps track of indentations
 *  and makes sure to put the begin tag for the class.
 * In:
 *  pcName  - The name of the class
 *  nAlign  - The aligment value (if exists)
 */
void IntoAClass(const char* pcName, unsigned int nAlign)
{
    PutIndents();

    if (nAlign > 0)
        fprintf(g_fpXML, "<%s aligned=\"%d\">\n", pcName, nAlign);
    else
        fprintf(g_fpXML, "<%s>\n", pcName);

    g_nIndent++;
}


/* OutOfClass:
 *  Indicates that we are leaving a class.  Keeps track of indentations
 *  and makes sure to put the end tag for the class.
 * In:
 *  pcFmt   - The end class tag to be output
 */
void OutOfClass(char* pcFmt, ...)
{
    g_nIndent--;
    PutIndents();
    va_list ap;
    va_start(ap, pcFmt);
    vfprintf(g_fpXML, pcFmt, ap);
    fprintf(g_fpXML, "\n");
    va_end(ap);
}