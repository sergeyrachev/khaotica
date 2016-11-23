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


package gif87a;
import flavor.*;
import java.io.*;


public class Main
{
	public static void main(String args[]) throws IOException 
    {
		if (args.length != 3) {
			System.out.println("Usage:");
            System.out.println("    java gif87a.Main -ofile=<out file> -schema=[y|n] <GIF file>");
            System.out.println("    ");
            System.out.println("    -ofile   Specify the output file to write the XML data.");
            System.out.println("    -schema  Assume corresponding schema is available.");
            System.out.println("    -?       Show this help.");
			System.exit(-1);
		}

		try {
			// Create an input bitstream
			Bitstream bs = new Bitstream(args[2], Bitstream.BS_INPUT);

			// Create our GIF87a object
			GIF87a gif = new GIF87a();

            if (args[1].charAt(8) == 'y' || args[1].charAt(8) == 'Y') {
                // Create the root element <GIF87a> and include the XML schema - gif87a.xsd
                XML.CreateXmlHeader(args[0].substring(7), "GIF87a", "gif87a.xsd");
    
                // If schema is used, then, the data type attributes may not be needed
                gif.putxml(bs, false);
            }
            else {
                // Create the root element <GIF87a>
                XML.CreateXmlHeader(args[0].substring(7), "GIF87a");

                // Create the XML document and explicitly generate the data type attributes
                gif.putxml(bs, true);
            }
            // Make sure the end tag matches the root element tag
            XML.EndXml("GIF87a");
        }
        catch(IOException e) {
			System.out.println(e.toString());
		}
    }
}
