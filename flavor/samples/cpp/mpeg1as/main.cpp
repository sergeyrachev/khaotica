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
#include <flavormp3.h>
#include "mpeg1as.h"


int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s mpeg1as_file\n", argv[0]);
        exit(1);
    }

    BuffBitstream bs(argv[1], BS_INPUT);

    // Create our MPEG-1 Audio frame object
    Frame frame;
    // Main data for Layer III
    MainData md;

#ifdef USE_EXCEPTION
    
    // Get the data
	try {
        int i, N;
        // Read first 15 frames only
        for (i=0; i<15; i++) {
		    frame.get(bs);
            if (frame.h->layer == LAYER1) {
                N = (12 * frame.h->bitrate * 1000) / frame.h->frequency;
                if (frame.h->padding_bit) N += 1;
            }
            else {
                // TBD: must take care of the free bitrate case
                N = (144 * frame.h->bitrate * 1000) / frame.h->frequency;
                if (frame.h->padding_bit) N += 1;
                if (frame.h->layer == LAYER3) {
                    /* In the case of Layer III, frame.size indicates not the size of the 
                     * whole frame, rather only the size of the header.  N-(frame.size>>3)
                     * bytes are the data bytes and we buffer them into the data buffer.
                     */
                    bs.buffer(N-(frame.size>>3));
                    bs.swap();  // Once the header is read, we switch to the data buffer

                    fprintf(stdout, "dsize = %d, N = %d, frame.size = %d, main_data_begin = %d\n", bs.getdsize(), N, (frame.size>>3), frame.adl3->main_data_begin);

                    /* Start reading from the data buffer at -(main_data_begin) bytes from the 
                     * beginning of the last header 
                     */
                    bs.skipbits((bs.getdsize() - (N-(frame.size>>3)+frame.adl3->main_data_begin))*8);
                    
                    // Get the main data
                    md.get(bs, frame.h, frame.adl3);
                    bs.swap();  // Switch back to the i/o buffer
                }
            }
            if (frame.h->layer == LAYER3)
                fprintf(stdout, "The size of frame(%d) header = %d bits, N = %d\n", i, frame.size, N);
            else
                fprintf(stdout, "The size of frame(%d) = %d bits, N = %d\n", i, frame.size, N);
        }
	}
	catch (BuffBitstream::EndOfData e) {
		fprintf(stdout, "%s: %s\n", argv[0], e.getmsg());
		exit(1);
	}
	catch (BuffBitstream::Error e) {
		fprintf(stderr, "%s: Error: %s\n", argv[0], e.getmsg());
	    exit(1);
	}
#else 

    // Get the data
    int i, N;
    // Read first 15 frames only
    for (i=0; i<15; i++) {
        frame.get(bs);
        if (frame.h->layer == LAYER1) {
            N = (12 * frame.h->bitrate * 1000) / frame.h->frequency;
            if (frame.h->padding_bit) N += 1;
        }
        else {
            // TBD: must take care of the free bitrate case
            N = (144 * frame.h->bitrate * 1000) / frame.h->frequency;
            if (frame.h->padding_bit) N += 1;
            if (frame.h->layer == LAYER3) {
                /* In the case of Layer III, frame.size indicates not the size of the 
                 * whole frame, rather only the size of the header.  N-(frame.size>>3)
                 * bytes are the data bytes and we buffer them into the data buffer.
                 */
                bs.buffer(N-(frame.size>>3));
                bs.swap();  // Once the header is read, we switch to the data buffer

                fprintf(stdout, "dsize = %d, N = %d, frame.size = %d, main_data_begin = %d\n", bs.getdsize(), N, (frame.size>>3), frame.adl3->main_data_begin);

                /* Start reading from the data buffer at -(main_data_begin) bytes from the 
                 * beginning of the last header 
                 */
                bs.skipbits((bs.getdsize() - (N-(frame.size>>3)+frame.adl3->main_data_begin))*8);
                
                // Get the main data
                md.get(bs, frame.h, frame.adl3);
                bs.swap();  // Switch back to the i/o buffer
            }
        }
        if (frame.h->layer == LAYER3)
            fprintf(stdout, "The size of frame(%d) header = %d bits, N = %d\n", i, frame.size, N);
        else
            fprintf(stdout, "The size of frame(%d) = %d bits, N = %d\n", i, frame.size, N);

        if (bs.geterror() == E_END_OF_DATA) {
            fprintf(stdout, "End of file\n");
            exit(1);
        }
        else if (bs.geterror() != E_NONE) {
            fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
            exit(1);
        }
    }
#endif

    return 0;
}