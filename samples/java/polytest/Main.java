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
 
 
package polytest;
import flavor.*;
import java.io.*;


public class Main 
{
	public static void main(String args[]) 
    {
		String file_name;
		if (args.length > 0) 
            file_name = args[0];
		else 
            file_name = "polytest.bs";
		
        try {
            encode(file_name);  // Call encode first
            decode(file_name);  // Then decode
    		(new File(file_name)).delete();
		} catch(IOException e) {
			System.out.println(e.toString());
		}
	}
	
	public static void encode(String f) throws IOException 
    {
		// Create an output bitstream
		Bitstream bs = new Bitstream(f, IBitstream.BS_OUTPUT);
    
        // Create the object and set its values
		PolyTest pt = new PolyTest();
    
		String s = null;

		pt.b[0] = new Base();
		s = new String("Base");
		pt.b[0].length = s.length();
		s.getChars(0, pt.b[0].length, pt.b[0].data, 0);
		
		pt.b[1] = new Derived1();
		s = new String("Derived1");
		pt.b[1].length = s.length();
		s.getChars(0, pt.b[1].length, pt.b[1].data, 0);
		   
		pt.b[2]=new Derived2();
		s = new String("Derived2");
		pt.b[2].length = s.length();
		s.getChars(0, pt.b[2].length, pt.b[2].data, 0);

		// Output the bits
		pt.put(bs);
		bs.close();
	}

	public static void decode(String f) throws IOException 
    {
        // Create an input bitstream
		Bitstream bs = new Bitstream(f, IBitstream.BS_INPUT);
		
		// Create an object
		PolyTest pt = new PolyTest();
 	    
		// Get the bits
		pt.get(bs);
        		
		bs.close();
	}
}

