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
 * Alexandros Eleftheriadis <eleft@ee.columbia.edu>
 *
 */

/* mp3bitstream.cpp */

// BuffBitstream IO implementation

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include "port.h"
#include "buffbitstream.h"


// masks for bitstring manipulation
static const unsigned int mask[33] = {
    0x00000000, 0x00000001, 0x00000003, 0x00000007,
    0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
    0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
    0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
    0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
    0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
    0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
    0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
    0xffffffff
};

// complement masks (used for sign extension)
static const unsigned int cmask[33] = {
    0xffffffff, 0xfffffffe, 0xfffffffc, 0xfffffff8,
    0xfffffff0, 0xffffffe0, 0xffffffc0, 0xffffff80,
    0xffffff00, 0xfffffe00, 0xfffffc00, 0xfffff800,
    0xfffff000, 0xffffe000, 0xffffc000, 0xffff8000,
    0xffff0000, 0xfffe0000, 0xfffc0000, 0xfff80000,
    0xfff00000, 0xffe00000, 0xffc00000, 0xff800000,
    0xff000000, 0xfe000000, 0xfc000000, 0xf8000000,
    0xf0000000, 0xe0000000, 0xc0000000, 0x80000000,
    0x00000000
};

// sign masks (used for sign extension)
static const unsigned int smask[33] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000004,
    0x00000008, 0x00000010, 0x00000020, 0x00000040,
    0x00000080, 0x00000100, 0x00000200, 0x00000400,
    0x00000800, 0x00001000, 0x00002000, 0x00004000,
    0x00008000, 0x00010000, 0x00020000, 0x00040000,
    0x00080000, 0x00100000, 0x00200000, 0x00400000,
    0x00800000, 0x01000000, 0x02000000, 0x04000000,
    0x08000000, 0x10000000, 0x20000000, 0x40000000,
    0x80000000
};

// standard constructor
BuffBitstream::BuffBitstream()
{
    type=BS_NOTINIT;
    close_fd=0;
    end=0;

    cur_buf=0;  // current buffer is i/o buffer
    buf=NULL;
    buf_len=0;
    cur_bit=0;
    tot_bits=0;
    
    ibuf=NULL;
    ibuf_len=0;
    icur_bit=0;
    itot_bits=0;
    
    dbuf=NULL;
    dbuf_len=0;
    dcur_bit=0;
    dtot_bits=0;
    dsize=0;

    err_code=E_NONE;
}

// file-based constructor
BuffBitstream::BuffBitstream(const char *filename, Bitstream_t t)
{
    Create(filename, t);
}

void BuffBitstream::Create(const char *filename, Bitstream_t t)
{
    type=t;
    close_fd=1;
    end=0;

    ibuf_len=BS_BUF_LEN;
    icur_bit=0;
    itot_bits=0;
    ibuf = new unsigned char[BS_BUF_LEN];
    memset(ibuf, 0, BS_BUF_LEN);

    dbuf_len=BS_BUF_LEN;
    dcur_bit=0;
    dtot_bits=0;
    dsize=0; 
    dbuf = new unsigned char[BS_BUF_LEN];
    memset(dbuf, 0, BS_BUF_LEN);

    cur_buf=0;  // current buffer is i/o buffer
    buf=ibuf;
    buf_len=ibuf_len;
    cur_bit=icur_bit;
    tot_bits=itot_bits;


    err_code=E_NONE;

    // copy file name -- only if it fits
    if (strlen(filename) < sizeof(fn)) strcpy(fn, filename);
    
    switch (type) {
        
    case BS_INPUT:
        if ((fd = open(fn, O_RDONLY)) < 0) {
            fprintf(stderr, "BuffBitstream::BuffBitstream: cannot open file '%s'\n", fn);
            exit(1);
        }
#ifdef WIN32		
        // switch to binary mode
        setmode(fd, O_BINARY);
#endif
        cur_bit = BS_BUF_LEN << 3;  // fake we are at the end of buffer
        fill_buf();
        break;
        
    case BS_OUTPUT:
        if ((fd = open(fn, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
            fprintf(stderr, "BuffBitstream::BuffBitstream: cannot create file '%s'\n", fn);
            exit(1);
        }
#ifdef WIN32		
        // switch to binary mode
        setmode(fd, O_BINARY);
#endif
        break;
        
    default:
        fprintf(stderr, "BuffBitstream::BuffBitstream: unknown bitstream type %d\n", t);
        exit(1);
        break;
    }
}
 
// file descriptor-based constructor
BuffBitstream::BuffBitstream(int fd, Bitstream_t t)
{
    Create(fd, t);
}

void BuffBitstream::Create(int fd, Bitstream_t t)
{
    type=t;
    close_fd=0;
    end=0;

    ibuf_len=BS_BUF_LEN;
    icur_bit=0;
    itot_bits=0;
    ibuf = new unsigned char[BS_BUF_LEN];
    memset(ibuf, 0, BS_BUF_LEN);

    dbuf_len=BS_BUF_LEN;
    dcur_bit=0;
    dtot_bits=0;
    dsize=0;
    dbuf = new unsigned char[BS_BUF_LEN];
    memset(dbuf, 0, BS_BUF_LEN);

    cur_buf=0;  // current buffer is i/o buffer
    buf=ibuf;
    buf_len=ibuf_len;
    cur_bit=icur_bit;
    tot_bits=itot_bits;

    this->fd=fd;
    
    err_code=E_NONE;

    switch(type) {
    
    case BS_INPUT:
        cur_bit = BS_BUF_LEN << 3;  // fake we are at the end of buffer
        fill_buf();
        break;
        
    case BS_OUTPUT:
        break;
        
    default:
        fprintf(stderr, "BuffBitstream::BuffBitstream: unknown bitstream type %d\n", t);
        exit(1);
        break;
    }
#ifdef WIN32		
    // switch to binary mode
    setmode(fd, O_BINARY);
#endif
}

// standard destructor
BuffBitstream::~BuffBitstream()
{  
    Destroy();
}

void BuffBitstream::Destroy()
{
    // make sure all data is out
    if (buf) {
        // prevent double flush, which caused the WriteFailed exception before when Destructor is called after Destroy()
        if (type == BS_OUTPUT) flushbits();	
        delete[] buf;
        buf = (unsigned char*)0;
    }

    if (cur_buf) {
        delete[] ibuf;
        ibuf = (unsigned char*)0;
    }
    else { 
        delete[] dbuf;
        dbuf = (unsigned char*)0;
    }

    if (close_fd) close(fd); 
}

// convert error code to text message
char* const BuffBitstream::err2msg(Error_t code)
{
    switch (code) {
    case E_NONE:				return "<None>";
    case E_END_OF_DATA:			return "End of data";
    case E_INVALID_ALIGNMENT:	return "Invalid alignment";
    case E_READ_FAILED:			return "Read failed";
    case E_WRITE_FAILED:		return "Write failed";
    default:					return "Unknown error";
    }
}

////////////////
// Big endian //
////////////////

// returns 'n' bits as unsigned int; does not advance bit pointer
unsigned int BuffBitstream::nextbits(int n)
{
    register unsigned int x;    // the value we will return
    unsigned char *v;           // the byte where cur_bit points to
    register int s;             // number of bits to shift 

    // make sure we have enough data
    if (cur_bit + n > (buf_len << 3)) fill_buf();
    // starting byte in buffer
    v = buf + (cur_bit >> 3);
    // load 4 bytes, a byte at a time - this way endianess is automatically taken care of
    x = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    // figure out how much shifting is required
    s = 32 - ((cur_bit % 8) + n);

    if (s >= 0) x = (x >> s);   // need right adjust 
    else {                      // shift left and read an extra byte
        x = x << -s;
        x |= v[4] >> (8+s);
    }
    return (x & mask[n]);
}

// returns 'n' bits as unsigned int with sign extension; does not advance bit pointer (sign extension only if n>1)
unsigned int BuffBitstream::snextbits(int n) 
{
    register unsigned int x = nextbits(n);
    if (n>1 && (x & smask[n])) return x | cmask[n];
    else return x;
}

// returns 'n' bits as unsigned int; advances bit pointer
unsigned int BuffBitstream::getbits(int n)
{
    register unsigned int x = nextbits(n);
    cur_bit += n;
    tot_bits += n;
    return (x & mask[n]);
}

// returns 'n' bits as unsigned int with sign extension; advances bit pointer (sign extension only if n>1)
unsigned int BuffBitstream::sgetbits(int n)
{
    register unsigned int x = getbits(n);
    if (n>1 && (x & smask[n])) return x | cmask[n];
    else return x;
}

// probe a float
float BuffBitstream::nextfloat(void)
{
    float f;
    *(int *)&f = nextbits(32);
    return f;
}

// get a float
float BuffBitstream::getfloat(void)
{
    float f;
    *(int *)&f = getbits(32);
    return f;
}

// probe a double
double BuffBitstream::nextdouble(void)
{
    double d;
    // make sure we have enough data (so that we can go back)
    if (cur_bit + 64 > (buf_len<<3)) fill_buf();
    *(int *)&d = nextbits(32);
    cur_bit += 32;
    *(((int *)&d)+1) = nextbits(32);
    cur_bit -= 32;
    return d;
}

// get a double
double BuffBitstream::getdouble(void)
{
    double d;
    *(((int *)&d)+1) = getbits(32);
    *(int *)&d = getbits(32);
    return d;
}

// can only write at least one byte to a file at a time; returns the output value
int BuffBitstream::putbits(unsigned int value, int n)
{
    int delta;          // required input shift amount
    unsigned char *v;   // current byte
    unsigned int tmp;   // temp value for shifted bits
    unsigned int val;   // the n-bit value

    val = value & mask[n];

    if (cur_bit + n > (buf_len << 3)) flush_buf();
    
    delta = 32 - n - (cur_bit % 8);
    v = buf + (cur_bit >> 3);
    if (delta >= 0) {
        tmp = val << delta;
        v[0] |= tmp >> 24;
        v[1] |= tmp >> 16;
        v[2] |= tmp >> 8;
        v[3] |= tmp;
    } 
    else {
        tmp = val >> (-delta);
        v[0] |= tmp >> 24;
        v[1] |= tmp >> 16;
        v[2] |= tmp >> 8;
        v[3] |= tmp;
        v[4] |= value << (8+delta);
    }
    cur_bit += n;
    tot_bits += n;
    return value;
}

// put a float
float BuffBitstream::putfloat(float value)
{
    putbits(*((int *)&value), 32);
    return value;
}

// put a double 
double BuffBitstream::putdouble(double value)
{
    putbits(*(((int *)&value)+1), 32);
    putbits(*(int *)&value, 32);
    return value;
}

///////////////////
// Little endian //
///////////////////

// returns 'n' bits as unsigned int; does not advance bit pointer
unsigned int BuffBitstream::little_nextbits(int n)
{
    register unsigned int x = 0;    // the value we will return
    int bytes = n >> 3;             // number of bytes to read
    int leftbits = n % 8;             // number of left-over bits to read
    unsigned int byte_x = 0;
    int i = 0;
    for (; i < bytes; i++) {
        byte_x = nextbits(8);
        cur_bit += 8;
        byte_x <<= (8*i);
        x |= byte_x;
    }
    /* Note that it doesn't make much sense to have a number in little-endian
     * byte-ordering, where the number of bits used to represent the number is
     * not a multiple of 8.  Neverthless, we provide a way to take care of 
     * such case.
     */
    if (leftbits > 0) {
        byte_x = nextbits(leftbits);
        byte_x <<= (8*i);
        x |= byte_x;
    }
    cur_bit -= i*8;
    return x;
}

// returns 'n' bits as unsigned int with sign extension; does not advance bit pointer (sign extension only if n>1)
unsigned int BuffBitstream::little_snextbits(int n) 
{
    register unsigned int x = little_nextbits(n);
    if (n>1 && (x & smask[n])) return x | cmask[n];
    else return x;
}

// returns 'n' bits as unsigned int; advances bit pointer
unsigned int BuffBitstream::little_getbits(int n)
{
    register unsigned int x = 0;        // the value we will return
    int bytes = n >> 3;                 // number of bytes to read 
    int leftbits = n % 8;                 // number of bits to read
    unsigned int byte_x = 0;
    int i = 0;
    for (; i < bytes; i++) {
        byte_x = getbits(8);
        byte_x <<= (8*i);
        x |= byte_x;
    }
    if (leftbits > 0) {
        byte_x = getbits(leftbits);
        byte_x <<= (8*i);
        x |= byte_x;
    }
    return x;	
}

// returns 'n' bits as unsigned int with sign extension; advances bit pointer (sign extension only if n>1)
unsigned int BuffBitstream::little_sgetbits(int n)
{
    register unsigned int x = little_getbits(n);
    if (n>1 && (x & smask[n])) return x | cmask[n];
    else return x;
}

// probe a float
float BuffBitstream::little_nextfloat(void)
{
    float f;
    *(int *)&f = little_nextbits(32);
    return f;
}

// get a float
float BuffBitstream::little_getfloat(void)
{
    float f;
    *(int *)&f = little_getbits(32);
    return f;
}

// probe a double
double BuffBitstream::little_nextdouble(void)
{
    double d;
    // make sure we have enough data (so that we can go back)
    if (cur_bit + 64 > (buf_len<<3)) fill_buf();
    *(int *)&d = little_nextbits(32);
    cur_bit += 32;
    *(((int *)&d)+1) = little_nextbits(32);
    cur_bit -= 32;
    return d;
}

// get a double
double BuffBitstream::little_getdouble(void)
{
    double d;
    *(int *)&d = little_getbits(32);
    *(((int *)&d)+1) = little_getbits(32);
    return d;
}

// can only write at least one byte to a file at a time; returns the output value
int BuffBitstream::little_putbits(unsigned int value, int n)
{
    unsigned int tmp = value;
    int bytes = n >> 3;         // number of bytes to write
    int leftbits = n % 8;       // number of bits to write
    unsigned int byte_x = 0;
    int i = 0;
    for (; i < bytes; i++) {
        byte_x = (value >> (8*i)) & mask[8];
        putbits(byte_x, 8);
    }
    if (leftbits > 0) {
        byte_x = (value >> (8*i)) & mask[leftbits];
        putbits(byte_x, leftbits);
    }
    return value;
}

// put a float
float BuffBitstream::little_putfloat(float value)
{
    little_putbits(*((int *)&value), 32);
    return value;
}

// put a double 
double BuffBitstream::little_putdouble(double value)
{
    little_putbits(*(int *)&value, 32);
    little_putbits(*(((int *)&value)+1), 32);
    return value;
}

// advance by some bits ignoring the value
void BuffBitstream::skipbits(int n)
{
    register int x = n;
    register int buf_size = buf_len << 3;

    // make sure we have enough data
    while (cur_bit + x > buf_size) {
        x -= buf_size - cur_bit;
        cur_bit = buf_size;
        if (type == BS_INPUT) fill_buf();
        else flush_buf();
    }
    cur_bit += x;
    tot_bits += n;
    return;
}

// align bitstream
int BuffBitstream::align(int n)
{
    int s = 0;
    // we only allow alignment on multiples of bytes
    if (n % 8) {
#ifdef USE_EXCEPTION
        throw InvalidAlignment();
#else
        seterror(E_INVALID_ALIGNMENT);
        return 0;
#endif
        return 0;
    }
    // align on next byte
    if (tot_bits % 8) {
        s = 8-(tot_bits % 8);
        skipbits(s);
    }
    while (tot_bits % n) {
        s += 8;
        skipbits(8);
    }
    return s;
}

/* Probe next n bits (input) or return 0 (output); in either case, the bitstream is (alen-bit) aligned.
 * If big=0, then the number is represented using the little-endian method; otherwise, big-endian byte-ordering.
 * If sign=0, then no sign extension; otherwise, sign extension.
 * If alen>0, then the number is probed at alen-bit boundary (alen must be multiple of 8).
 */
unsigned int BuffBitstream::next(int n,  int big, int sign, int alen)
{
    if (alen > 0) align(alen);
    if (type == BS_INPUT) {
        if (big) {
            if (sign) return snextbits(n);
            else return nextbits(n);
        }
        else {
            if (sign) return little_snextbits(n);
            else return little_nextbits(n);
        }
    }
    else return 0;
}

/* Search for the specified code (input); returns number of bits skipped, excluding code.
 * If alen > 0, then output bits up to the specified alen-bit boundary (output); returns number of bits written
 * The code is represented using n bits at alen-bit boundary.
 */
unsigned int BuffBitstream::nextcode(unsigned int code, int n, int alen)
{
    unsigned int s = 0;

    if (type == BS_INPUT) {
        if (!alen) {
            while (code != nextbits(n)) {
                s += 1;
                skipbits(1);
            }
        }
        else {
            s += align(alen);
            while (code != nextbits(n)) {
                s += alen;
                skipbits(alen);
            }
        }
    }
    else if (type == BS_OUTPUT) s += align(alen);
    
    return s;
}

// flush buffer; left-over bits are also output with zero padding
void BuffBitstream::flushbits()
{
    flush_buf();

    if (cur_bit == 0) return;

    int l = write(fd, buf, 1);
    if (l != 1) {
#ifdef USE_EXCEPTION
        throw WriteFailed();
#else
        seterror(E_WRITE_FAILED);
        return;
#endif
    }
    buf[0] = 0;
    cur_bit = 0;    // now only the left-over bits
}

// output the buffer excluding the left-over bits.
void BuffBitstream::flush_buf()
{
    int l = (cur_bit >> 3);             // number of bytes to write
    int n = write(fd, buf, l);
    if (n != l) {
#ifdef USE_EXCEPTION
        throw WriteFailed();
#else
        seterror(E_WRITE_FAILED);
        return;
#endif
    }
    // are there any left-over bits?
    if (cur_bit & 0x7) {
        buf[0] = buf[l];                // copy the left-over bits
        memset(buf+1, 0, BS_BUF_LEN-1); // zero-out rest of buffer
    }
    else memset(buf, 0, BS_BUF_LEN);    // zero-out entire buffer
    // keep left-over bits only
    cur_bit &= 7;
}

// get the next chunk of data from whatever the source is
void BuffBitstream::fill_buf()
{
    int	n;	// how many bytes we must fetch (already read)
    int	l;	// how many bytes we will fetch (available)
    int	u;	// how many are still unread

    // buffer
    if (cur_buf) {
#ifdef USE_EXCEPTION
        throw EndOfData();
#else
        seterror(E_END_OF_DATA);
        return;
#endif
    }

    n = (cur_bit >> 3);
    u = buf_len - n;

    // move unread contents to the beginning of the buffer
    memmove(buf, buf+n, u);
    // clear the rest of buf
    memset(buf+u, 0, n);

    l = read(fd, buf+u, n);

    // check for end of data
    if (l == 0) {
        end = 1;
#ifdef USE_EXCEPTION
        throw EndOfData();
#else
        seterror(E_END_OF_DATA);
        return;
#endif
    }
    else if (l < 0) {
        end = 1;
#ifdef USE_EXCEPTION
        throw ReadFailed();
#else
        seterror(E_READ_FAILED);
        return;
#endif
    }
    else if (l < n) {
        end = 1;
        buf_len = u+l;
    }
    // now we are at the first byte
    cur_bit &= 7;
}

// buffer

// copy the next 'n' bytes from the i/o buffer to the data buffer for later use
void BuffBitstream::buffer(int n)
{
    // should not call this method if current buffer is not i/o buffer
    if (cur_buf) return;

    align(8);
    if (cur_bit+n*8 > (buf_len<<3)) fill_buf();
    
    memmove(dbuf+dsize, buf+(cur_bit>>3), n);
    dsize += n;

    skipbits(n*8);    
}

void BuffBitstream::swap()
{
    int n; // used data (in bytes)
    int u; // unused data (in bytes)

    // current buffer is dbuf
    if (cur_buf) {
        n = (cur_bit >> 3);
        if (cur_bit & 7) n += 1;
        u = dsize - n;     

        // move unused contents to the beginning of the buffer
        // fprintf(stdout, "curbit = %d, n = %d, u = %d, dsize = %d\n", cur_bit, n, u, dsize);        
        memmove(buf, buf+n, u);
        // clear the rest of buf
        memset(buf+u, 0, n);

        dcur_bit = 0;
        dtot_bits = tot_bits;
        dsize = u;

        buf = ibuf;
        buf_len = ibuf_len;
        cur_bit = icur_bit;
        tot_bits = itot_bits;

        cur_buf = 0;
    }
    // current buffer is ibuf
    else {
        ibuf_len = buf_len;
        icur_bit = cur_bit;
        itot_bits = tot_bits;

        buf = dbuf;
        buf_len = dbuf_len;
        cur_bit = dcur_bit;
        tot_bits = dtot_bits;
 
        cur_buf = 1;
    }
}
