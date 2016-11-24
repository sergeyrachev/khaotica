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
#include <flavor.h>
#include "mpeg1vs.h"


int main(int argc, char *argv[])
{
    if (argc !=2 ) {
        fprintf(stderr, "Usage: %s mpeg1vs_file\n", argv[0]);
        exit(1);
    }

    Bitstream bs(argv[1], BS_INPUT);

    // Create our MPEG-1 Video object
    MPEG1Video mv;

    // Get the data
#ifdef USE_EXCEPTION
	try {
		mv.get(bs);
    }
	catch (Bitstream::EndOfData e) {
		fprintf(stdout, "%s: %s\n", argv[0], e.getmsg());
		exit(1);
	}
	catch (Bitstream::Error e) {
		fprintf(stderr, "%s: Error: %s\n", argv[0], e.getmsg());
	    exit(1);
	}
#else
    mv.get(bs);
    if (bs.geterror() == E_END_OF_DATA) {
        fprintf(stdout, "End of file\n");
        exit(1);
    }
    else if (bs.geterror() != E_NONE) {
        fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
        exit(1);
    }
#endif 

    return 0;
}