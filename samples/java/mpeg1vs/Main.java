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

 
package mpeg1vs;
import flavor.*;
import java.io.*;


public class Main
{
	public static void main(String args[])
	{
		if (args.length != 1) {
			System.out.println("Usage: java mpeg1vs.Main <MPEG1 Video file>");
			System.exit(-1);
        }

        try {
            // Create an input bitstream
            Bitstream bs = new Bitstream(args[0], Bitstream.BS_INPUT);
        
            // Create our MPEG-1 Video object
            MPEG1Video mv = new MPEG1Video();

            mv.get(bs);
        }
        catch(IOException e) {
	        System.out.println(e.toString());
       		return;
       	}
    }
}
