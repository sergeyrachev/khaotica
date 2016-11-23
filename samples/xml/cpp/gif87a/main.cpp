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


#include <stdio.h>
#include <string.h>

// Must include the run-time library before the Flavor-generated file
#include <flavor.h>

// Include the Flavor-generated file
#include "gif87a.h"

static char* gpcInputFile   = 0;
static char* gpcOutputFile  = 0;
static int   gnSchema       = 0;

void usage()
{
    fprintf(stdout, 
            "\nUsage:\n"
            "    gif87a -ofile=<out file> -schema=[y|n] <GIF file>\n\n"
            "This program parses a GIF file and then converts it into an XML document.\n"
            "Options:\n"
            "    -ofile   Specify the output file to write the XML data.\n"
            "    -schema  Assume corresponding schema is available.\n"
            "    -?       Show this help.\n\n");
}

void main(int argC, char *argV[])
{
    if (argC != 4) {
        usage();
        exit(1);
    }
    
    // Process parameters
    int parmInd;
    for (parmInd = 1; parmInd < argC; parmInd++) {
        // Break out on first parm not starting with a dash
        if (argV[parmInd][0] != '-') break;

        if (!strcmp(argV[parmInd], "-?")) {
            usage();
            exit(1);
        }
        else if (!strncmp(argV[parmInd], "-ofile=", 7)) 
            gpcOutputFile = &(argV[parmInd][7]);
        else if (!strncmp(argV[parmInd], "-schema=", 8)) { 
            char schema = argV[parmInd][8];
            if (schema == 'Y' || schema == 'y')
                gnSchema = 1;
            else
                gnSchema = 0;
        }
        else 
            fprintf(stderr, "Unknown option '%s', ignoring it.\n", argV[parmInd]);
    }

    // And now we have to have only one parameter left and it must be the file name.
    if (parmInd + 1 != argC) {
        usage();
        exit(1);
    }
    gpcInputFile = argV[parmInd];

    // Create an input bitstream
    Bitstream bs(gpcInputFile, BS_INPUT);
    
    // Create our GIF87a object
    GIF87a gif;

    if (gnSchema) {
        // Create the root element <GIF87a> and include the XML schema - gif87a.xsd
        CreateXmlHeader(gpcOutputFile, "GIF87a", "gif87a.xsd");
    
        // If schema is used, then, the data type attributes may not be needed
        gif.putxml(bs, 0);
    }
    else {
        // Create the root element <GIF87a>
        CreateXmlHeader(gpcOutputFile, "GIF87a");

        // Create the XML document and explicitly generate the data type attributes
        gif.putxml(bs, 1);
    }
   
    // Make sure the end tag matches the root element tag
    EndXml("GIF87a");
}
