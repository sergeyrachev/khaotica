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

// Must include the run-time library before the Flavor-generated files
#include <flavor.h>

// Include the Flavor-generated files
#include "audio.h"


void usage(void)
{
    fprintf(stdout, "Usage: audio -option <audio file>\n");
    fprintf(stdout, "\t-h          print this message\n");
    fprintf(stdout, "\t--          automatically detect the format of the audio_file\n");
    fprintf(stdout, "\t-w          the audio_file is in the WAV format\n");
    fprintf(stdout, "\t-a          the audio_file is in the AIFF/AIFC format\n");
    fprintf(stdout, "\t-n          the audio_file is in the NeXT/Sun audio format\n");
    fprintf(stdout, "\t-r1         the audio_file is in the 8-bit raw format\n");
    fprintf(stdout, "\t-r2         the audio_file is in the 16-bit raw format\n");
    fprintf(stdout, "\t-r3         the audio_file is in the 16-bit raw format (little-endian)\n");
}


int main(int argc, char *argv[])
{
    char *option;

    if ((argc != 3) || (*argv[1] != '-')) {
        usage();
        exit(1);
    }
    else {
        option = argv[1] + 1;
        if (*option == 'h') {
            usage();
            exit(1);
        }
        
        // Create an input bitstream
        Bitstream bs(argv[2], BS_INPUT);

        // Automatic detection
        if (*option == '-') {

            // Create our Audio object
            Audio audio;
        
            // Parse
            while (!bs.atend()) {
#ifdef USE_EXCEPTION
                try {
                    audio.get(bs); 
                    switch (audio.type) {
                    case 3: {   // WAV format
                        // For the data chunk, read one data element at a time
                        if (audio.wav->ckData->available) {
                            WAVData wd;
                            for (int i = 0; i < audio.wav->ckData->ckSize; i=i+audio.wav->ckFormat->bitsPerSample/8) 
                                wd.get(bs, audio.wav->ckFormat->bitsPerSample);
                            audio.wav->ckData->available = 0;
                        }
                        break;
                    }
                    case 2: {   // AIFF/AIFC format
                        // For the sound data chunk, read one data element at a time
                        if (audio.aiff->ckSound->available) {
                            SoundData sd;
                            for (int i = 0; i < audio.aiff->ckSound->ckSize; i++) 
                                sd.get(bs);
                            audio.aiff->ckSound->available = 0;
                        }
                        break;
                    }
                    case 1: {   // NeXT/Sun audio format
                        // Read one data element at a time
                        AUData aud;
                        for (int i = 0; i < audio.auh->dataSize; i++)
                            aud.get(bs);
                        break;
                    }
                    default:    // 8-bit raw format
                        break;
                    }
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
                audio.get(bs);
                switch (audio.type) {
                case 3: {   // WAV format
                    if (audio.wav->ckData->available) {
                        WAVData wd;
                        for (int i = 0; i < audio.wav->ckData->ckSize; i=i+audio.wav->ckFormat->bitsPerSample/8) 
                            wd.get(bs, audio.wav->ckFormat->bitsPerSample);
                        audio.wav->ckData->available = 0;
                    }
                    break;
                }
                case 2: {   // AIFF/AIFC format
                    if (audio.aiff->ckSound->available) {
                        SoundData sd;
                        for (int i = 0; i < audio.aiff->ckSound->ckSize; i++) 
                            sd.get(bs);
                        audio.aiff->ckSound->available = 0;
                    }
                    break;
                }
                case 1: {   // NeXT/Sun audio format
                    AUData aud;
                    for (int i = 0; i < audio.auh->dataSize; i++)
                        aud.get(bs);
                    break;
                }
                default:    // 8-bit raw format
                    break;
                }
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
            }
        }

        // WAV format
        else if (*option == 'w') {
            printf("Reading a WAV file\n\n");

            WAV wav;
        
            // Parse
            while (1) {
#ifdef USE_EXCEPTION
                try {
                    wav.get(bs);
                    // Read one data element at a time
                    if (wav.ckData->available) {
                        WAVData wd;
                        for (int i = 0; i < wav.ckData->ckSize; i=i+wav.ckFormat->bitsPerSample/8) 
                            wd.get(bs, wav.ckFormat->bitsPerSample);
                        wav.ckData->available = 0;
                    }
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else  
                wav.get(bs);
                // Read one data element at a time
                if (wav.ckData->available) {
                    WAVData wd;
                    for (int i = 0; i < wav.ckData->ckSize; i=i+wav.ckFormat->bitsPerSample/8) 
                        wd.get(bs, wav.ckFormat->bitsPerSample);
                    wav.ckData->available = 0;
                }
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
            }
        }
        // AIFF/AIFC format
        else if (*option == 'a') {
            printf("Reading an AIFF/AIFC file\n\n");

            AIFF aiff;
        
            // Parse
            while (1) {
#ifdef USE_EXCEPTION
                try {
                    aiff.get(bs);
                    // Read one data element at a time
                    if (aiff.ckSound->available) {
                        SoundData sd;
                        for (int i = 0; i < aiff.ckSound->ckSize; i++) 
                            sd.get(bs);
                        aiff.ckSound->available = 0;
                    }
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
                aiff.get(bs);
                // Read one data element at a time
                if (aiff.ckSound->available) {
                    SoundData sd;
                    for (int i = 0; i < aiff.ckSound->ckSize; i++)
                        sd.get(bs);
                }
                aiff.ckSound->available = 0;
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
            }
        }
        // NeXT/Sun audio format
        else if (*option == 'n') {
            printf("Reading a NeXT/Sun audio file\n\n");

            AUHeader auh;
            AUData aud;
        
            // Parse
            while (1) {
#ifdef USE_EXCEPTION
                try {
                    auh.get(bs);
                    // Read one data element at a time
                    for (int i = 0; i < auh.dataSize; i++)
                        aud.get(bs);
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
        	    auh.get(bs);
                // Read one data element at a time
                for (int i = 0; i < auh.dataSize; i++)
                    aud.get(bs);
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
            }
        }
        // Raw format
        else if (*option == 'r') {
            switch(*(argv[1] + 2)) {
            case '1':   
            {
                printf("Reading an 8-bit raw audio file\n\n");

                Raw8 raw;
        
                // Parse
#ifdef USE_EXCEPTION
                try {
                    raw.get(bs);
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
        	    raw.get(bs);
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
                break;
            }

            case '2':
            {
                printf("Reading a 16-bit raw audio file\n\n");

                Raw16 raw;
        
                // Parse
#ifdef USE_EXCEPTION
                try {
                    raw.get(bs);
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
        	    raw.get(bs);
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
                break;
            }

            case '3':
            {
                printf("Reading a 16-bit raw audio file (little-endian)\n\n");

                Raw16Little raw;
        
                // Parse
#ifdef USE_EXCEPTION
                try {
                    raw.get(bs);
                }
                catch (Bitstream::EndOfData e) {
		            fprintf(stdout, "End of file\n");
		            exit(1);
	            }
                catch (Bitstream::Error e) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], e.getmsg());
                    exit(1);
                }
#else 
        	    raw.get(bs);
                if (bs.geterror() == E_END_OF_DATA) {
                    fprintf(stdout, "End of file\n");
                    exit(1);
                }
                else if (bs.geterror() != E_NONE) {
                    fprintf(stderr, "%s: Error: %s\n", argv[2], bs.getmsg());
                    exit(1);
                }
#endif 
                break;
            }
            default:
                usage();
                exit(1);
            }
        }
        else {
            usage();
            exit(1);
        }
    }

    return 0;
}
