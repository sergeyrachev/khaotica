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


#ifndef _FLAVORI_H_
#define _FLAVORI_H_


// bitstream type (not initialized, input or output)
typedef enum {
    BS_NOTINIT,
    BS_INPUT,
    BS_OUTPUT
} Bitstream_t;

// the various exceptions/errors we can expect
typedef enum {
    E_NONE,
	E_END_OF_DATA,
	E_INVALID_ALIGNMENT,
	E_READ_FAILED,
	E_WRITE_FAILED
} Error_t;


/* Flavor interfaces. 
 *
 * This is the interface of the bitstream I/O class that flavorc expects.
 * All methods are declared pure virtual, and *must* be defined in a derived class.
 *
 */

class IBitstream
{
public:
    // bitstream operations
    
	////////////////
	// Big-endian //
	////////////////
    
    // probe next n bits, do not advance
    virtual unsigned int nextbits(int n) = 0;
    // probe next n bits with sign extension, do not advance (sign extension only if n>1)
    virtual unsigned int snextbits(int n) = 0;
    // get next n bits, advance 
    virtual unsigned int getbits(int n) = 0;
    // get next n bits with sign extension, advance (sign extension only if n>1)
    virtual unsigned int sgetbits(int n) = 0;

    // float
    virtual float nextfloat(void) = 0;
    virtual float getfloat(void) = 0;
    // double
    virtual double nextdouble(void) = 0;
    virtual double getdouble(void) = 0;
    // long double 
    virtual long double nextldouble(void) = 0;
    virtual long double getldouble(void) = 0;
 
    // put n bits
    virtual int putbits(unsigned int value, int n) = 0;
    // float
	virtual float putfloat(float value) = 0;
    // double
    virtual double putdouble(double value) = 0;
    // long double 
    virtual long double putldouble(double value) = 0;

  	///////////////////
	// Little-endian //
	///////////////////

    /* The following methods are the same as the above methods, except 
     * that the numbers are represented using the little-endian method.
     */

    virtual unsigned int little_nextbits(int n) = 0;
    virtual unsigned int little_snextbits(int n) = 0;
    virtual unsigned int little_getbits(int n) = 0;
    virtual unsigned int little_sgetbits(int n) = 0;
    virtual float little_nextfloat(void) = 0;
    virtual float little_getfloat(void) = 0;
    virtual double little_nextdouble(void) = 0;
    virtual double little_getdouble(void) = 0;
    virtual long double little_nextldouble(void) = 0;
    virtual long double little_getldouble(void) = 0;
    virtual int little_putbits(unsigned int value, int n) = 0;
	virtual float little_putfloat(float value) = 0;
    virtual double little_putdouble(double value) = 0;
    virtual long double little_putldouble(double value) = 0;
	

    // skip next n bits (both input/output)
    virtual void skipbits(int n) = 0;

    // align bitstream (n must be multiple of 8, both input/output); returns bits skipped
    virtual int align(int n) = 0;
   
    /* Probe next n bits (input) or return 0 (output).
     * If big=0, then the number is represented using the little-endian method; otherwise, big-endian byte-ordering.
     * If sign=0, then no sign extension; otherwise, sign extension.
     * If alen>0, then the number is probed at alen-bit boundary (alen must be multiple of 8).
     */
    virtual unsigned int next(int n, int big, int sign, int alen) = 0;

    /* Search for a specified code (input); returns number of bits skipped, excluding the code.
     * If alen>0, then output bits up to the specified alen-bit boundary (output); returns number of bits written
     * The code is represented using n bits at alen-bit boundary.
     */
    virtual unsigned int nextcode(unsigned int code, int n, int alen) = 0;

    // get current bit position (both input/output)
    virtual unsigned int getpos(void) = 0;
};


#endif /* ! _FLAVORI_H_ */
