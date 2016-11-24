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
 

package maptest;
import flavor.*;
import java.io.*;


public class Main
{
	public static void main(String args[])
    {
    	String file_name;
		if (args.length > 0 ) 
            file_name = args[0];
		else 
            file_name = "maptest.bs";

        try {
            encode(file_name);  // Call encode first
		    decode(file_name);  // Then decode
		} catch(IOException e) {
			System.out.println(e.toString());
		}
	}
	
	public static void encode(String f) throws IOException 
    {
        // Create an output bitstream
    	Bitstream bs = new Bitstream(f, IBitstream.BS_OUTPUT);
		
        // Create the object and set its values
		MapTest mt = new MapTest();

		mt.a = 1001;

		mt.fm[0] = (float)1.3;
		mt.fm[1] = (float)2.2;
		mt.fm[2] = (float)3.3;
		mt.fm[3] = (float)6.7;
			
		mt.chroma_format[0] = new YUVblocks(14,-1,1);
		mt.chroma_format[1] = new YUVblocks(-4,22,2);
		mt.chroma_format[2] = new YUVblocks(4,2,-33);
        mt.chroma_format[3] = new YUVblocks(0,6,99);

		System.out.println("Input ...");
		System.out.println("mt.a = " + mt.a);

		for (int i=0; i<4; i++)
			System.out.println("fm[" + i + "] = " + mt.fm[i]);
		for (int i=0; i<4; i++) {
			System.out.println("chroma_format[" + i + "] = { " + 
			                   mt.chroma_format[i].Yblocks + ", " + 
                               mt.chroma_format[i].Ublocks + ", " + 
			                   mt.chroma_format[i].Vblocks + " }");
		}
		mt.put(bs);
		bs.close();
	}
    
	public static void decode(String f) throws IOException 
    {
        // Create an input bitstream
    	Bitstream bs = new Bitstream(f, IBitstream.BS_INPUT);
		
        // Create an object
		MapTest mt = new MapTest();

		// Get the bits
		mt.get(bs);
	
		System.out.println("Output ...");
		System.out.println("mt.a = " + mt.a);

		for (int i=0; i<4; i++)
			System.out.println("fm[" + i + "] = " + mt.fm[i]);
        for (int i=0; i<4; i++) {
			System.out.println("chroma_format[" + i + "] = { " + 
			                   mt.chroma_format[i].Yblocks + ", " + 
                               mt.chroma_format[i].Ublocks + ", " + 
                               mt.chroma_format[i].Vblocks + " }");
		}
		bs.close();
	}
}
