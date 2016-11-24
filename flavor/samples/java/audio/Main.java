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
 

package audio;
import flavor.*;
import java.io.*;


public class Main
{
    static char option;

	public static void main(String args[])
	{
		if ((args.length != 2) || (args[0].charAt(0) != '-')) {
			usage();
			System.exit(-1);
        }
        else {
            option = args[0].charAt(1);
            if (option == 'h') {
			    usage();
			    System.exit(-1);
            }

            try {
        	    // Create an input bitstream
                Bitstream bs = new Bitstream(args[1], Bitstream.BS_INPUT);

                // Automatic detection
                if (option == '-') {
                    // Create our audio object
                    Audio audio = new Audio();

                    // Parse 
                    while (true) {
 	        		    audio.get(bs);
                        switch (audio.type) {
                        case 3: {   // WAV format
                            // For the data chunk, read one data element at a time
                            if (audio.wav.ckData.available == 1) {
                                WAVData wd = new WAVData();
                                for (int i = 0; i < audio.wav.ckData.ckSize; i=i+audio.wav.ckFormat.bitsPerSample/8) 
                                    wd.get(bs, audio.wav.ckFormat.bitsPerSample);
                                audio.wav.ckData.available = 0;
                            }
                            break;
                        }
                        case 2: {   // AIFF/AIFC format
                            // For the sound data chunk, read one data element at a time
                            if (audio.aiff.ckSound.available == 1) {
                                SoundData sd = new SoundData();
                                for (int i = 0; i < audio.aiff.ckSound.ckSize; i++) 
                                    sd.get(bs);
                                audio.aiff.ckSound.available = 0;
                            }
                            break;
                        }
                        case 1: {   // NeXT/Sun audio format
                            // Read one data element at a time
                            AUData aud = new AUData();
                            for (int i = 0; i < audio.auh.dataSize; i++)
                                aud.get(bs);
                            break;
                        }
                        default:    // 8-bit raw format
                            break;
                        }
                    }
	            }

                // WAV
                else if (option == 'w') {
                    System.out.println("Reading a WAV file\n");
 
                    // Create our audio object
                    WAV wav = new WAV();
    
            		// Parse
                    while (true) {
	        		    wav.get(bs);
                        // Read one data element at a time
                        if (wav.ckData.available == 1) {
                            WAVData wd = new WAVData();
                            for (int i = 0; i < wav.ckData.ckSize; i=i+wav.ckFormat.bitsPerSample/8) 
                                wd.get(bs, wav.ckFormat.bitsPerSample);
                            wav.ckData.available = 0;
                        }
                    }
                }

                // AIFF/AIFC
                else if (option == 'a') {
                System.out.println("Reading an AIFF/AIFC file\n");

                    // Create our audio object
                    AIFF aiff = new AIFF();
    
            		// Parse
                    while (true) {
	        		    aiff.get(bs);
                        // Read one data element at a time
                        if (aiff.ckSound.available == 1) {
                            SoundData sd = new SoundData();
                            for (int i = 0; i < aiff.ckSound.ckSize; i++) 
                                sd.get(bs);
                            aiff.ckSound.available = 0;
                        }
                    }
                }

                // NeXT/Sun
                else if (option == 'n') {
                    System.out.println("Reading a NeXT/Sun audio file\n");

                    // Create our audio object
                    AUHeader auh = new AUHeader();
                    AUData aud = new AUData();

              		// Parse
                    while (true) {
                        auh.get(bs);
                        // Read one data element at a time
                        for (int i = 0; i < auh.dataSize; i++)
                            aud.get(bs);
                     }
                }

                // Raw
                else if (option == 'r') {
                    switch(args[0].charAt(2)) {
                    case '1':
                    {
                        System.out.println("Reading an 8-bit raw audio file\n");
        
                        // Create our audio object
                        Raw8 raw = new Raw8();
    
            		    // Parse (will automagically print out the values)
	        		    raw.get(bs);
        		    }
                    break;
                    case '2':
                    {
                        System.out.println("Reading a 16-bit raw audio file\n");

                        // Create our audio object
                        Raw16 raw = new Raw16();
    
            		    // Parse (will automagically print out the values)
	        		    raw.get(bs);
        		    }
                    break;
                    case '3':
                    {
                        System.out.println("Reading a 16-bit raw audio file (little-endian)\n");
                
                        // Create our audio object
                        Raw16Little raw = new Raw16Little();
    
            		    // Parse (will automagically print out the values)
	        		    raw.get(bs);
        		    }
                    break;
                    default:
                        usage();
            			System.exit(-1);
                    }
                }
                else {
                    usage();
    			    System.exit(-1); 
                }
       		} catch(IOException e) {
	        	System.out.println(e.toString());
       			return;
       		}
        }
    }

    public static void usage()
    {
        System.out.println("Usage: java audio.Main -option <audio file>");
        System.out.println("\t-h          print this message\n");
        System.out.println("\t--          automatically detect the format of the audio_file");
        System.out.println("\t-a          the audio_file is in the AIFF/AIFC format");
        System.out.println("\t-n          the audio_file is in the NeXT/Sun audio format");
        System.out.println("\t-w          the audio_file is in the WAV format");
        System.out.println("\t-r1         the audio_file is in the 8-bit raw format");
        System.out.println("\t-r2         the audio_file is in the 16-bit raw format");
        System.out.println("\t-r3         the audio_file is in the 16-bit raw format (little-endian)");
    }
}
