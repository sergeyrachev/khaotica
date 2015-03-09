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

 
package gifprint;
import flavor.*;
import java.io.*;


public class Main
{
	public static void main(String args[])
    {
		if (args.length != 1) {
			System.out.println("Usage: java gifprint.Main <GIF87a file>");
			System.exit(-1);
		}

		try {
			// Create an input bitstream
			Bitstream bs = new Bitstream(args[0], Bitstream.BS_INPUT);

			// Create our GIF87a object
			GIF87a gif = new GIF87a();;

			// Parse (will automagically print out the values)
			gif.get(bs); 
        } 
        catch(IOException e) {
			System.out.println(e.toString());
		}
    }
}
