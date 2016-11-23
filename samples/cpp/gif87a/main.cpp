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

// Must include the run-time library before the Flavor-generated file
#include <flavor.h>

// Include the Flavor-generated file
#include "gif87a.h"


int main(int argC, char *argV[])
{
    if (argC != 2) {
        fprintf(stdout, "Usage: %s <GIF87a file>\n", argV[0]);
        exit(1);
    }
    
    // Create an input bitstream
    Bitstream bs(argV[1], BS_INPUT);
    
    // Create our GIF87a object
    GIF87a gif;
    
    // Parse (will automagically print out the values)
#ifdef USE_EXCEPTION
    try {
        gif.get(bs); // Get the bits
    }
    // Catch exception from bitstream I/O library
    catch (Bitstream::EndOfData e) {
		fprintf(stdout, "End of file\n");
		exit(1);
	}
    catch (Bitstream::Error e) {
        fprintf(stderr, "%s: Error: %s\n", argV[0], e.getmsg());
        exit(1);
    }
#else  /* ! USE_EXCEPTION */
    gif.get(bs); // Get the bits
    if (bs.geterror() == E_END_OF_DATA) {
        fprintf(stdout, "End of file\n");
        exit(1);
    }
    else if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", argV[0], bs.getmsg());
        exit(1);
    }
#endif /* ! USE_EXCEPTION */

    return 0;
}
