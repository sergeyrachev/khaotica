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

#ifndef _BUFFBITSTREAM_H_
#define _BUFFBITSTREAM_H_


#include "flavori.h"
#include "port.h"


/* This bitstream class has been specifically designed to support
 * parsing of MP3 files.  As shown in Figure A.7 of the MPEG-1 Audio 
 * specification, the coded data must be first stored in a data buffer
 * for random access in the bitstream.  For this, we introduce three
 * new methods: buffer(), swap(), and getdsize().  
 */


const int BS_BUF_LEN=2048;  // buffer size, in bytes


/* BuffBitstreamclass */
class BuffBitstream: public IBitstream
{
private:
    Bitstream_t type;       // type of bitstream (input/output)
    int fd;                 // file descriptor (for file-based reading/writing)
    char fn[MAX_LEN];       // file name (when given)
    char close_fd;          // set to 1 by the constructor if the fd needs to be closed
    unsigned char end;      // end of data flag

    int cur_buf;            // 0: current buffer is ibuf; 1: current buffer is dbuf
    
    // current buffer
    unsigned char *buf;     // buffer	
    int buf_len;		    // usable buffer size (for partially filled buffers), in bytes
    int cur_bit;            // current bit position in buf
    unsigned int tot_bits;  // total bits read/written
    
    // i/o buffer
    unsigned char *ibuf;    // i/o buffer	
    int ibuf_len; 		    // usable i/o buffer size (for partially filled buffers)
    int icur_bit;           // current bit position in ibuf
    unsigned int itot_bits; // total bits read/written

    // data buffer
    unsigned char *dbuf;    // data buffer
    int dbuf_len;           // usable data buffer size
    int dcur_bit;           // current bit position in dbuf
    unsigned int dtot_bits; // total bits read/written
    unsigned int dsize;     // the actual data size, in bits


    Error_t err_code;       // error code (useful when exceptions are not supported)
    
private:
    // functions            
    void fill_buf();        // fills buffer
    void flush_buf();       // flushes buffer
    
    // sets error code
    void seterror(Error_t err) { err_code=err; } 	
    
public:
    // convert error code to text message
    static char* const err2msg(Error_t code);
    
public:
    // default constructor - uninitialized bitstream
    BuffBitstream();
    // bitstream as a file, given a file name
    BuffBitstream(const char* filename, Bitstream_t t);
    void Create(const char* filename, Bitstream_t t);
    // bitstream as a file descriptor
    BuffBitstream(int fd, Bitstream_t t);
    void Create(int fd, Bitstream_t t);
    // default destructor
    ~BuffBitstream();
    void Destroy();
    
    // get fd
    int getfd() { return fd; }
    // get mode
    Bitstream_t getmode() { return type; }

    // bitstream operations
    
	////////////////
	// Big endian //
	////////////////

    // probe next 'n' bits, do not advance
    unsigned int nextbits(int n);
    // probe next 'n' bits with sign extension, do not advance (sign extension only if n>1)
    unsigned int snextbits(int n);
    // get next 'n' bits, advance
    unsigned int getbits(int n);
    // get next 'n' bits with sign extension, advance (sign extension only if n>1)
    unsigned int sgetbits(int n);
    // float
    float nextfloat(void);
    float getfloat(void);
    // double
    double nextdouble(void);
    double getdouble(void);
    // long double 
    long double nextldouble(void) { return nextdouble(); }
    long double getldouble(void) { return getdouble(); }
    
    // put 'n' bits 
    int putbits(unsigned int value, int n);
 	// float
    float putfloat(float value);
    // double
    double putdouble(double value);
    // long double 
    long double putldouble(double value) { return putdouble(value); }
	
  	///////////////////
	// Little endian //
	///////////////////

	unsigned int little_nextbits(int n);
	unsigned int little_snextbits(int n);
    unsigned int little_getbits(int n);
    unsigned int little_sgetbits(int n);
    float little_nextfloat(void);
    float little_getfloat(void);
    double little_nextdouble(void);
    double little_getdouble(void);
    long double little_nextldouble(void) { return little_nextdouble(); }
    long double little_getldouble(void) { return little_getdouble(); }
    int little_putbits(unsigned int value, int n);
    float little_putfloat(float value);
    double little_putdouble(double value);
    long double little_putldouble(double value) { return little_putdouble(value); }

 
    // skip next 'n' bits (both input/output); n>=0
    void skipbits(int n);
	// align bitstream (n must be multiple of 8, both input/output); returns bits skipped
    int align(int n);
    // probe next 'n' bits (input)
    unsigned int next(int n, int big, int sign, int alen);
    // search for a specified code; returns the number of bits skipped/written (both input/output)
    unsigned int nextcode(unsigned int code, int n, int alen);
    // get current position (both input/output)
    unsigned int getpos(void) { return tot_bits; }


    // buffer next 'n' bytes for later use
    void buffer(int n);
    // swap between ibuf and dbuf
    void swap();
    // return actual data size in the buffer
    unsigned int getdsize(void) { return dsize; }
  
    
    // flush buffer; left-over bits are also output with zero padding (output only)
    void flushbits();
    // returns 1 if reached end of data
    inline int atend() { return end; }

    // get last error
    inline int geterror(void) { return err_code; }
    // get last error in text form
    char* const getmsg(void) { return err2msg(err_code); }

#ifdef USE_EXCEPTION
    // exception handling (when supported)
    
    // our base exception
    class Error {
        Error_t code;   // exception code, from Error_t
    public:
        Error(Error_t c) : code(c) {}
        inline int geterror(void)   { return code; }
        inline char *const getmsg(void)  { return BuffBitstream::err2msg(code); }
    };
    class EndOfData : public Error {
    public:
        EndOfData(void) : Error(E_END_OF_DATA) {}
    };
    class InvalidAlignment : public Error {
    public:
        InvalidAlignment(void) : Error(E_INVALID_ALIGNMENT) {}
    };
    class ReadFailed : public Error {
    public:
        ReadFailed(void) : Error(E_READ_FAILED) {}
    };
    class WriteFailed : public Error {
    public:
        WriteFailed(void) : Error(E_WRITE_FAILED) {}
    };
#endif /* USE_EXCEPTION */
    
};


#endif /* ! _BUFFBITSTREAM_H_ */
