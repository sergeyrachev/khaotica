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
 

#include <stdio.h>
#include <string.h>

// Must include the run-time library before the Flavor-generated file
#include <flavor.h>

// Include the Flavor-generated file
#include "maptest.h"


void encode(char *pcFileName, char *pcProgName)
{
    // Create an output bitstream
    Bitstream bs(pcFileName, BS_OUTPUT);
    
    // Create the object and set its values
    MapTest mt;
    
    mt.a = 1001;
    
    mt.fm[0] = (float)1.3;
    mt.fm[1] = (float)2.2;
    mt.fm[2] = (float)3.3;
    mt.fm[3] = (float)6.7;

    mt.chroma_format[0] = new YUVblocks(14,-1,1);
    mt.chroma_format[1] = new YUVblocks(-4,22,2);
    mt.chroma_format[2] = new YUVblocks(4,2,-33);
    mt.chroma_format[3] = new YUVblocks(0,6,99);

    printf("Input ...\n");
    printf("mt.a = %d\n", mt.a);

    int i;
    for (i=0; i<4; i++)
        printf("fm[%d] = %g\n", i, mt.fm[i]);
    for (i=0; i<4; i++) {
        printf("chroma_format[%d] = { %d, %d, %d }\n",
               i,
               mt.chroma_format[i]->Yblocks,
               mt.chroma_format[i]->Ublocks, 
			   mt.chroma_format[i]->Vblocks);
    }

    // Output the bits
#ifdef USE_EXCEPTION
    try {
        mt.put(bs);
    } catch(Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else  /* ! USE_EXCEPTION */
    mt.put(bs); 
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
    MapTest mt;
    
#ifdef USE_EXCEPTION
    // Get the bits
    try {
        mt.get(bs); // Get the bits
        
        printf("Output ...\n");
        printf("mt.a = %d\n", mt.a);
        
        int i;
        for (i=0; i<4; i++)
            printf("fm[%d] = %g\n", i, mt.fm[i]);
        for (i=0; i<4; i++) {
            printf("chroma_format[%d] = { %d, %d, %d }\n",
                   i,
                   mt.chroma_format[i]->Yblocks,
                   mt.chroma_format[i]->Ublocks,
                   mt.chroma_format[i]->Vblocks);
		}
    } catch(Bitstream::Error e) {
        // Catch exception from bitstream I/O library
        fprintf(stderr, "%s: Error: %s\n", pcProgName, e.getmsg());
        exit(1);
    }
#else /* ! USE_EXCEPTION */
    mt.get(bs); // Get the bits
        
    printf("Output ...\n");
    printf("mt.a = %d\n", mt.a);
    
    int i;
    for (i=0; i<4; i++)
        printf("fm[%d] = %g\n", i, mt.fm[i]);
    for (i=0; i<4; i++) {
        printf("chroma_format[%d] = { %d, %d, %d }\n",
               i,
               mt.chroma_format[i]->Yblocks,
               mt.chroma_format[i]->Ublocks,
               mt.chroma_format[i]->Vblocks);
	}
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
        pcFileName = "maptest.bs";
    else 
        pcFileName = argV[1];

    // Call encode first
    encode(pcFileName, argV[0]);

    // Then decode
    decode(pcFileName, argV[0]);

    return 0;
}

