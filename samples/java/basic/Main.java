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


package basic;
import flavor.*;
import java.io.*;


public class Main
{
	public static void main(String args[])
    {
        String file_name = null;
    	if (args.length != 0)
	        file_name = args[0];
	    else
	        file_name = "basic.bs";

		try {
            // Create an output file
	        Bitstream bs1 = new Bitstream(file_name, IBitstream.BS_OUTPUT);
    	    Basic b1 = new Basic();
            b1.next_parse_len = 24;
            b1.num2 = -15;
            b1.num3 = 5;
	        b1.put(bs1);    // Write the Basic bitstream into the output file
 	        bs1.close();
        
            // Open the input file
	        Bitstream bs2 = new Bitstream(file_name, IBitstream.BS_INPUT);
	        Basic b2 = new Basic();
	        b2.get(bs2);    // Read from the input file the bits for the Basic object

            /* Let's check that we really read -15 for 'num2' (note that Flavor 
             * could directly do that, e.g., Flavor makes sure that the value
             * read for 'num1' is always 0xFF0)
             */
            if (b2.num2 != -15)
                System.out.println("Error: value read was not -15 but " + b2.num2);
	        else
		        System.out.println("No Error: value read was -15");

            // Also check that 'num3' has the value 5
            if (b2.num3 != 5)
                System.out.println("Error: value read was not 5 but " + b2.num3);
	        else
		        System.out.println("No Error: value read was 5");
        }
        catch(IOException e) {
			System.out.println(e.toString());
		}
    }
}

