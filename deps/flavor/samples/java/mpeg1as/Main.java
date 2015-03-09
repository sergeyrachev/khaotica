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

 
package mpeg1as;
import flavormp3.*;
import java.io.*;


public class Main
{
	public static void main(String args[])
	{
		if (args.length != 1) {
			System.out.println("Usage: java mpeg1as.Main <MPEG1 Audio file>");
			System.exit(-1);
        }

        try {
            // Create an input bitstream
            BuffBitstream bs = new BuffBitstream(args[0], BuffBitstream.BS_INPUT);
        
            // Create our MPEG-1 Audio frame object
            Frame frame = new Frame();
            // Main data for Layer III
            MainData md = new MainData();

            int i, N;
            // Read first 15 frames only
            for (i=0; i<15; i++) {
		        frame.get(bs);
                if (frame.h.layer == 3) {   // Layer I
                    N = (12 * frame.h.bitrate * 1000) / frame.h.frequency;
                    if (frame.h.padding_bit > 0) N += 1;
                }
                else {
                    // TBD: must take care of the free bitrate case
                    N = (144 * frame.h.bitrate * 1000) / frame.h.frequency;
                    if (frame.h.padding_bit > 0) N += 1;
                    if (frame.h.layer == 1) {   // Layer III
                        /* In the case of Layer III, frame.size indicates not the size of the 
                         * whole frame, rather only the size of the header.  N-(frame.size>>3)
                         * bytes are the data bytes and we buffer them into the data buffer.
                         */
                        bs.buffer(N-(frame.size>>>3));
                        bs.swap();  // Once the header is read, we switch to the data buffer

                        System.out.println("dsize = " + bs.getdsize() + ", N = " + N + ", frame.size = " + (frame.size>>>3) + ", main_data_begin = " + frame.adl3.main_data_begin);
                        
                        /* Start reading from the data buffer at -(main_data_begin) bytes from the 
                         * beginning of the last header 
                         */
                        bs.skipbits((bs.getdsize() - (N-(frame.size>>>3)+frame.adl3.main_data_begin))*8);
                        md.get(bs, frame.h, frame.adl3);
                        bs.swap();  // Switch back to the i/o buffer
                    }
                }
                if (frame.h.layer == 1) // Layer III
                    System.out.println("The size of frame(" + i + ") header = " + frame.size + " bits, N = " + N);
                else
                    System.out.println("The size of frame(" + i + ") = " + frame.size + " bits, N = " + N);
            }
        }
        catch(IOException e) {
	        System.out.println(e.toString());
       		return;
       	}
    }
}
