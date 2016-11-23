/* @flavorc
 *
 * maptest.h
 * 
 * This file was automatically generated by flavorc
 * from the source file:
 *     '..\\..\\..\\fl\\maptest.fl'
 *
 * For information on flavorc, visit the Flavor Web site at:
 *     http://www.ee.columbia.edu/flavor
 *
 * -- Do not edit by hand --
 *
 */

#ifndef _F_maptest_h_
#define _F_maptest_h_

// Initial size of parsable arrays: 64

// Bitstream error reporting function
void flerror(char* fmt, ...);

// Tracing functions
void trace(int pos, int size, unsigned int val, char* fmt, ...);
void trace(int pos, int size, double val, char* fmt, ...);

// Disable unreferenced local variable warning in MS Visual C++
#pragma warning(disable: 4101)
// pragma: trace

class YUVblocks {
public:
    int Yblocks;
    int Ublocks;
    int Vblocks;
#line 36 "..\\..\\..\\fl\\maptest.fl"
    
	YUVblocks(int y,int u,int v) : Yblocks(y), Ublocks(u), Vblocks(v) {}
	YUVblocks() {} 
    public: virtual int putxml(IBitstream &_F_bs, short int bAttr);
};

class BPC {
    public: static int putxml(IBitstream &_F_bs, short int bAttr, YUVblocks *_F_arg, int *_F_parse=NULL, int *_F_code=NULL);
};

class A {
    public: static int putxml(IBitstream &_F_bs, short int bAttr, float *_F_arg, int *_F_parse=NULL, int *_F_code=NULL);
};

class MapTest {
public:
    int a;
    float fm[64];
    YUVblocks *chroma_format[64];
    public: virtual int putxml(IBitstream &_F_bs, short int bAttr);
};

#endif /* ! _F_maptest_h_ */
