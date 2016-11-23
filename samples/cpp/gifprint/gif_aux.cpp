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


#include <stdio.h>

// Must include the run-time library before the Flavor-generated file
#include <flavor.h>

// Include the Flavor-generated file
#include "gifprint.h"


// Print methods for all GIF87a classes (declared in sample2.fl)

void ColorEntry::print(void)
{
    printf("\tr: %d\tg: %d\tb: %d\n", r, g, b);
}

void ColorMap::print(void)
{
    printf("Color Map:\n");
}

void ScreenDescriptor::print(void)
{
    printf("Screen Descriptor:\n");
    printf("\twidth:      %d\n"
           "\theight:     %d\n"
           "\tM:          %d\n"
           "\tcr:         %d\n"
           "\tmarker:     %d\n"
           "\tpixel:      %d\n"
           "\tbackground: %d\n",
           width, height, M, cr, marker, pixel, background);
}

void ExtensionBlock::print(void)
{
    printf("Extension Block:\n"
           "\tfunction_code: %d\n"
           "\ttotal bytes:   %d\n",
           function_code, total);
}

void ImageData::print(void)
{
    printf("Image Data:\n"
           "\tcode_size:   %d\n"
           "\ttotal bytes: %d\n",
           code_size, total);
}

void ImageDescriptor::print(void)
{
    printf("Image Descriptor:\n"
           "\tleft:    %d\n"
           "\ttop:     %d\n"
           "\twidth:   %d\n"
           "\theight:  %d\n"
           "\tM:       %d\n"
           "\tI:       %d\n"
           "\tmarkers: %d\n"
           "\tpixel:   %d\n",
           left, top, width, height, M, I, markers, pixel);
}

void GIF87a::print(void)
{
    printf("GIF87a\n");
}
