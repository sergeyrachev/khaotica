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
#include "polytest.h"


void encode(char *pcFileName, char *pcProgName)
{
    // Create an output bitstream
    Bitstream bs(pcFileName, BS_OUTPUT);
    
    // Create the object and set its values
    PolyTest pt;
   
    pt.b[0] = new(Base);
    pt.b[0]->length = 4;
    strcpy(pt.b[0]->data, "Base");
 
    pt.b[1] = new(Derived1);
    pt.b[1]->length = 8;
    strcpy(pt.b[1]->data, "Derived1");
    
    pt.b[2] = new(Derived2);
    pt.b[2]->length = 8;
    strcpy(pt.b[2]->data, "Derived2");

    // Output the bits
#ifdef USE_EXCEPTION
    try {
        pt.put(bs);
    } catch(Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else  /* ! USE_EXCEPTION */
    pt.put(bs); 
    if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", pcProgName, bs.getmsg());
        exit(1);
    }
#endif /* ! USE_EXCEPTION */
}


void decode(char *pcFileName, char *pcProgName)
{
    // Create an input bitstream
    Bitstream bs(pcFileName, BS_INPUT);
    
    // Create the object
    PolyTest pt;
    
#ifdef USE_EXCEPTION
    // Get the bits
    try {
        pt.get(bs); // Get the bits
    } catch(Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else /* ! USE_EXCEPTION */
    pt.get(bs); // Get the bits
    if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", pcProgName, bs.getmsg());
        exit(1);
    }
#endif /* ! USE_EXCEPTION */
}


int main(int argC, char *argV[])
{
    char *pcFileName;

	if (argC < 2) 
        pcFileName = "polytest.bs";
	else
		pcFileName = argV[1];
    
    // Call encode first
    encode(pcFileName, argV[0]);

    // Then decode
    decode(pcFileName, argV[0]);

    return 0;
}
