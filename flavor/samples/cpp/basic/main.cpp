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

// Must include the run-time library before the Flavor-generated files
#include <flavor.h>

// Include the Flavor-generated file
#include "basic.h"


// Creates an output file and then writes the Basic object bitstream
void encode(char *pcFileName, char *pcProgName)
{
    // Create an output bitstream
    Bitstream bs(pcFileName, BS_OUTPUT);
    
    // Create the object and set its values
    Basic b;
    b.next_parse_len = 22;
    b.num2 = -15;
    b.num3 = 5;

    // Output the bits
#ifdef USE_EXCEPTION
    try {
        b.put(bs); 
    } 
    catch (Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else  /* ! USE_EXCEPTION */
    b.put(bs);  
    if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", pcProgName, bs.getmsg());
        exit(1);
    }
#endif /* ! USE_EXCEPTION */	
}


// Reads the bits from the input file into the Basic object
void decode(char *pcFileName, char *pcProgName)
{
    // Create an input bitstream
    Bitstream bs(pcFileName, BS_INPUT);
    
    // Create the object
    Basic b;
    
    // Get the bits
#ifdef USE_EXCEPTION
    try {
        b.get(bs); // Get the bits
    } 
    catch (Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else  /* ! USE_EXCEPTION */
    b.get(bs); // Get the bits
    if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", pcProgName, bs.getmsg());
        exit(1);
    }
#endif /* ! USE_EXCEPTION */
    
    /* Let's check that we really read -15 for 'num2' (note that Flavor 
     * could directly do that, e.g., Flavor makes sure that the value
     * read for 'num1' is always 0xFF0)
     */
    if (b.num2 != -15)
        fprintf(stderr, "Error: value read was not -15 but %d\n", b.num2);
	else
		fprintf(stderr, "No Error: value read was -15\n");

    // Also check that 'num3' has the value 5
    if (b.num3 != 5)
        fprintf(stderr, "Error: value read was not 5 but %d\n", b.num3);
	else
		fprintf(stderr, "No Error: value read was 5\n");
}


int main(int argC, char *argV[])
{
    char *pcFileName;
	
	if (argC > 1)
        pcFileName = argV[1];
	else
		pcFileName = "basic.bs";
    
    // Call encode first
    encode(pcFileName, argV[0]);
    
    // Then decode
    decode(pcFileName, argV[0]);

    return 0;
}
