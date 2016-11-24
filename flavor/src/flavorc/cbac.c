/*
 * Copyright (c) 1997-2005 Alexandros Eleftheriadis and Danny Hong.
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

/* cbac.c */


#include "cdef.h"
 
static const unsigned int PREC_BITS[33] = { 
          0,
/* prec = 1,       2,           3,         4,          5,          6,          7,          8 */
        0x1,      0x3,        0x7,       0xF,       0x1F,       0x3F,       0x7F,       0xFF, 
      0x1FF,    0x3FF,      0x7FF,     0xFFF,     0x1FFF,     0x3FFF,     0x7FFF,     0xFFFF,
    0x1FFFF,   0x3FFFF,   0x7FFFF,   0xFFFFF,   0x1FFFFF,   0x3FFFFF,   0x7FFFFF,   0xFFFFFF,
  0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
};

/* determine the next probability state from the current probability state and symbol */
#define NEXT(INDEX) \
        if (ni_name) { \
            if (cpp()) \
                c_outi("%siNext = %s[st][%d];\n", prefix, ni_name, INDEX); \
            else if (java()) \
                c_outi("%siNext = %s.%s[st][%d];\n", prefix, global_class, ni_name, INDEX); \
        }

/* generate tables needed for semi-quasi-renormalization */
static void quasi_table_gen(symbol_t *sp) 
{
    unsigned int limit = 1<<(prec-1); /* we assume r_min is always greater than HALF */
    unsigned int i, j, k, value, init;

    /* tblOne */
    c_outi("// tblOne[0] corresponds to 0, and tblOne[1] corresponds to 1\n");
    if (cpp()) 
        c_outi("const unsigned int %s::tblOne[2] = { 0, 0xFFFFFFFF };\n\n", sp->name);
    else if (java())
        c_outi("static final long tblOne[] = { 0, 0xFFFFFFFFL };\n\n");

    /* tblLSB */
    if (r_min == 0) {
        /* we use b_bar as an index to the table, rather than n = p - b_bar - 1 */
        c_outi("// tblLSB is used to implement the lsb_n function\n");
        if (cpp()) 
            c_outi("const unsigned int %s::tblLSB[%d] = { 0", sp->name, prec+1);
        else if (java())
            c_outi("static final long tblLSB[] = { 0");
        
        for (i=prec-2; i >= 1; --i) {
            c_out(", %d", (1<<i)-1);
        }        
        c_out(", 0, 0 };\n\n");
    }

    /* tblNumBits */
    c_outi("// tblNumBits returns b_bar, from which b can be calculated\n");
    if (r_min == 0) {   /* the CACM implementation */
        if (cpp()) 
            c_outi("const char %s::tblNumBits[%u] = {\n", sp->name, limit);
        else if (java())
            c_outi("static final char tblNumBits[] = {\n");

        c_outi("\t%d", prec);

        for (i=1, k=1, value=prec-1; i < limit; k<<=1, --value) {
            for (j=0; j < k && i < limit; ++j, ++i) { 
                if (i % 32 == 0) { 
                    c_out(",\n");
                    c_outi("\t%d", value);
                }
                else 
                    c_out(", %d", value);
            }
        }
        c_out("};\n\n");
    }
    else {              /* renormalize whenever R < r_min */
        if (cpp())
            c_outi("const char %s::tblNumBits[%d] = {\n", sp->name, limit);
        else if (java())
            c_outi("static final char tblNumBits[] = {\n");

        c_outi("\t0");

        /* init is the smallest int such that 2^init >= r_min */
        for (init=0; init < (unsigned)prec; ++init) {
            if ((unsigned)(1 << init) >= r_min) break; 
        }          
        for (i=1, value=init; i < r_min; --value) {
            for (; i<<(value-1) < r_min; ++i) {
                if (i % 32 == 0) { 
                    c_out(",\n");
                    c_outi("\t%d", value);
                }
                else
                    c_out(", %d", value);
            }
        }
        for (; i < limit; ++i) {
            if (i % 32 == 0) { 
                c_out(",\n");
                c_outi("\t0");
            }
            else
                c_out(", 0");
        }
        c_out("};\n\n");
    }

    // tblF
    c_outi("// tblF returns the number of follow bits\n");
    if (cpp())
        c_outi("const char %s::tblF[%d] = {\n", sp->name, limit);
    else if (java())
        c_outi("static final char tblF[] = {\n");

    c_outi("\t0");

    for (i=1; i < limit; ++i) {
        if (i % 32 == 0) {
            c_out(",\n");
            c_outi("\t");
        }
        j = i;
        for (value=1; value <= (unsigned)prec; ++value) {
            if (j & 1) {
                if (i % 32 == 0)
                    c_out("%d", value);
                else
                    c_out(", %d", value);
                break;
            }
            j >>= 1;
        }
    }
    c_out("};\n\n");
}

/* output a bit plus follow bits */
static void bpf(bpf_t type, int skip) 
{
    if (skip < 0) 
        fatal("Internal error CB%d: Expected a non-negative number for skip", __LINE__);

    /* if L >= HALF, output 1 1-bit followed by F 0-bits */
    c_outi("if (%sL >= HALF) {\n", prefix); 
    c_identlevel++; 
    if (!skip) {
        c_outi("%sbs.putbits(1, 1);\n", prefix); 
        c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(0, 1);\n", prefix, prefix, prefix); 
    }
    else {
        c_outi("if (%sSkip == 0) %sbs.putbits(1, 1); else %sSkip--;\n", prefix, prefix, prefix); 
        c_outi("for ( ; %sF>0; %sF--) if (%sSkip == 0) %sbs.putbits(0, 1); else %sSkip--;\n", prefix, prefix, prefix, prefix, prefix); 
    }
    c_outi("%sL -= HALF;\n", prefix); 
    c_identlevel--; 
    c_outi("}\n");
    /* else if L+R <= HALF, output 1 0-bit followed by F 1-bits */
    if (type == BPF_CACM) {
        c_outi("else if (%sL+%sR <= HALF) {\n", prefix, prefix); 
        c_identlevel++; 
        if (!skip) {
            c_outi("%sbs.putbits(0, 1);\n", prefix); 
            c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(1, 1);\n", prefix, prefix, prefix); 
        }
        else {
            c_outi("if (%sSkip == 0) %sbs.putbits(0, 1); else %sSkip--;\n", prefix, prefix, prefix); 
            c_outi("for ( ; %sF>0; %sF--) if (%sSkip == 0) %sbs.putbits(1, 1); else %sSkip--;\n", prefix, prefix, prefix, prefix, prefix); 
        }
        c_identlevel--; 
        c_outi("}\n"); 
    }
    /* else if L >= QTR */
    if (type == BPF_CACM) 
        c_outi("else if (%sL >= QTR && %sL+%sR <= HALF+QTR) {\n", prefix, prefix, prefix); 
    else 
        c_outi("else if (%sL >= QTR) {\n", prefix); 
    c_identlevel++; 
    c_outi("%sF++; %sL -= QTR;\n", prefix, prefix); 
    c_identlevel--; 
    c_outi("}\n");
    /* else */
    if (type != BPF_CACM)  {
        c_outi("else {\n"); 
        c_identlevel++; 
        if (!skip) {
            c_outi("%sbs.putbits(0, 1);\n", prefix); 
            c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(1, 1);\n", prefix, prefix, prefix); 
        }
        else {
            c_outi("if (%sSkip == 0) %sbs.putbits(0, 1); else %sSkip--;\n", prefix, prefix, prefix); 
            c_outi("for ( ; %sF>0; %sF--) if (%sSkip == 0) %sbs.putbits(1, 1); else %sSkip--;\n", prefix, prefix, prefix, prefix, prefix); 
        }
        c_identlevel--; 
        c_outi("}\n");
    }
    /* else */
    if (type == BPF_CACM) 
        c_outi("else break;\n");

    c_outi("tbits++; %sL <<= 1; %sR <<= 1;\n", prefix, prefix);
}

/* output the last bit plus follow bits */
static void bpf_end(bpf_t type) 
{
    if (type == BPF_CACM) 
        c_outi("if (%sL<QTR) {\n", prefix); 
    else 
        c_outi("if (%sL<HALF) {\n", prefix);

    c_identlevel++; 
    c_outi("%sbs.putbits(0, 1);\n", prefix); 
    c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(1, 1);\n", prefix, prefix, prefix); 
    c_identlevel--; 
    c_outi("}\n"); 
    c_outi("else {\n"); 
    c_identlevel++; 
    c_outi("%sbs.putbits(1, 1);\n", prefix); 
    c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(0, 1);\n", prefix, prefix, prefix); 
    c_identlevel--; 
    c_outi("}\n");    
}

/* read minimal number of bits needed to disambiguate the last symbol */
static void c_in_end_bits(symbol_t *sp) {
    c_outi("\n");
    c_outi("// returns the (minimal) number of bits read to disambiguate the last symbol\n");
    if (cpp()) {
        c_outi("int %s::read_end_bits(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
        c_identlevel++;
        c_outi("const unsigned int max = HALF + (HALF-1); // we cannot use ONE because ONE may require 33 bits\n");
    }
    else if (java()) {
        c_outi("private static int read_end_bits(%s %sbs) throws IOException {\n", bitstream_class, prefix);
        c_identlevel++;
        c_outi("final long max = HALF + (HALF-1); // we cannot use ONE because ONE may require 33 bits\n");
    }
    
    c_outi("int tbits = 0;\n");

    c_outi("%s l = %sL & %u%s;\n", cpp() ? "unsigned int" : "long", prefix, PREC_BITS[prec], cpp() ? "":"L");
    c_outi("for (int bits=1; ; bits++) {\n");
    c_identlevel++;

    c_outi("%s m = 1 << (%d-bits);\n", cpp() ? "unsigned int" : "long", prec);
    c_outi("%s n1 = 0;\n", cpp() ? "unsigned int" : "long");
    c_outi("%s n2 = m-1;\n", cpp() ? "unsigned int" : "long"); // we want to avoid the case where n2 = ONE
    c_outi("int cases = 1 << bits;\n");
    c_outi("int out = 0;\n");
    c_outi("while (cases > 0) {\n");

    c_identlevel++;
    c_outi("if (l <= n1 && l+(%sR-1) >= n2) {\n", prefix);

    c_identlevel++;
    c_outi("tbits += bits;\n");

    /* this is a special case needed just for MPEG-4 shape coding */
    if (end_type == 2) {
        c_outi("if (n1 == 0 || n2 == max) {\n");
        c_identlevel++;
        c_outi("tbits += 1;\n");
        c_outi("%sbs.skipbits(bits+1);\n", prefix);
        c_identlevel--;
        c_outi("}\n");
        c_outi("else\n");
        c_identlevel++;
        c_outi("%sbs.skipbits(bits);\n", prefix);
        c_identlevel--;
    }
    else 
        c_outi("%sbs.skipbits(bits);\n", prefix);
    if (end_type == 1)
        c_outi("tbits += %sbs.align(8);\n", prefix);
    c_outi("%sT += tbits;\n", prefix);
    c_outi("return tbits;\n");

    c_identlevel--;
    c_outi("}\n");
    c_outi("out++;\n");
    c_outi("n1 += m;\n");
    c_outi("n2 += m;\n");
    c_outi("cases--;\n");
  
    c_identlevel--;
    c_outi("}\n");
    
    c_identlevel--;
    c_outi("}\n");
 
    c_identlevel--;
    c_outi("}\n");
}

/* write minimal number of bits needed to disambiguate the last symbol */
static void c_out_end_bits(symbol_t *sp) {
    c_outi("\n");
    c_outi("// returns the (minimal) number of bits written to disambiguate the last symbol\n");
    if (cpp()) {
        c_outi("int %s::write_end_bits(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
        c_identlevel++;
        c_outi("const unsigned int max = HALF + (HALF-1); // we cannot use ONE because ONE may require 33 bits\n");
    }
    else if (java()) {
        c_outi("private static int write_end_bits(%s %sbs) throws IOException {\n", bitstream_class, prefix);
        c_identlevel++;
        c_outi("final long max = HALF + (HALF-1); // we cannot use ONE because ONE may require 33 bits\n");
    }

    c_outi("int tbits = 0;\n");

    c_outi("%s l = %sL & %u%s;\n", cpp() ? "unsigned int":"long", prefix, PREC_BITS[prec], cpp() ? "":"L");
    c_outi("for (int bits=1; ; bits++) {\n");
    c_identlevel++;

    c_outi("%s m = 1 << (%d-bits);\n", cpp() ? "unsigned int" : "long", prec);
    c_outi("%s n1 = 0;\n", cpp() ? "unsigned int" : "long");
    c_outi("%s n2 = m-1;\n", cpp() ? "unsigned int" : "long");
    c_outi("int cases = 1 << bits;\n");
    c_outi("int out = 0;\n");
    c_outi("while (cases > 0) {\n");

    c_identlevel++;
    c_outi("if (l <= n1 && l+(%sR-1) >= n2) {\n", prefix);
    
    c_identlevel++;
    if (co == CO_FO && r_min <= qtr_plus_2) {
        /* output all the pending bits + the first bit of the current working bits for out */
        c_outi("tbits += %sF+bits;\n", prefix); 
        c_outi("if (out < 1<<(bits-1)) { %sbs.putbits(0, 1); for ( ; %sF>0; %sF--) %sbs.putbits(1, 1); }\n", prefix, prefix, prefix, prefix);
        c_outi("else { %sbs.putbits(1, 1); for ( ; %sF>0; %sF--) %sbs.putbits(0, 1); }\n", prefix, prefix, prefix, prefix);
        /* output the rest of the current working bits for out */
        c_outi("%sbs.putbits(out&PREC_BITS[bits-1], bits-1);\n", prefix);
    }
    else {
        c_outi("// output BU + BLEN-CT bits of CBU + %d spacer bits\n", bs_ns);
        c_outi("int last_cbu_len = BLEN-%sCT;\n", prefix);  
        if ((co == CO_FO && prec <= 27) || (co == CO_BS && bs_n1+bs_ns+prec < 32)) {
            c_outi("%sCBU = (int)(%sL>>%d);\n", prefix, prefix, bs_ns+prec);
            c_outi("%sL &= (1<<%d)-1;\n", prefix, bs_ns+prec);
        }
        if (co == CO_FO) {
            /* output BU */
            c_outi("if (%sCBU > (1 << last_cbu_len)-1) { // there was a carry\n", prefix); 
            c_identlevel++;
            c_outi("%sbs.putbits(%sBU+1, BLEN);\n", prefix, prefix);
            c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(0, BLEN);\n", prefix, prefix, prefix);
            c_identlevel--;
            c_outi("}\n");
            c_outi("else {\n");
            c_identlevel++;
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix); 
            c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(BU_ONES, BLEN);\n", prefix, prefix, prefix); 
            c_identlevel--;
            c_outi("}\n");
            /* output BLEN-CT bits of CBU */
            c_outi("%sbs.putbits(%sCBU, last_cbu_len);\n", prefix, prefix);
            /* output spacer bits */
            c_outi("%sbs.putbits((int)(%sL>>%d), %d);\n", prefix, prefix, prec, bs_ns);
            c_outi("tbits += (1+%sSC)*BLEN + last_cbu_len + %d;\n", prefix, bs_ns);
            /* output the current working bits for out */
            c_outi("%sbs.putbits(out&PREC_BITS[bits], bits);\n", prefix);
        }
        else if (co == CO_BS) { 
            c_outi("int lastbits = %d+bits;\n", bs_ns);
            c_outi("%sL >>= %d;\n", prefix, prec); 
            c_outi("%sL <<= bits;\n", prefix);
            c_outi("%sL += out;           // L consists of lastbits bits\n", prefix);
            c_outi("%sL <<= 32-lastbits;  // and the lastbits are left-aligned\n", prefix);
            c_outi("do {\n");
            c_identlevel++;

            c_outi("%sCBU <<= %sCT;\n", prefix, prefix); 
            c_outi("// make sure that CBU consists of BLEN bits, if possible\n");
            c_outi("if (lastbits >= %sCT) %sCBU += %sL >> (32-%sCT);\n", prefix, prefix, prefix, prefix);
            c_outi("else %sCBU += %sL << (%sCT-lastbits);\n", prefix, prefix, prefix);
            c_outi("%sL <<= %sCT;\n", prefix, prefix);
            c_outi("lastbits -= %sCT;\n", prefix);

            c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
            c_identlevel++;
            
            c_outi("tbits += BLEN;\n");
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix); 
            c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN-1;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
          
            c_identlevel--;
            c_outi("}\n");

            c_outi("else {\n");
            c_identlevel++;
            
            c_outi("%sBU += (%sCBU >> BLEN);\n", prefix, prefix);
            c_outi("%sCBU &= BU_ONES;\n", prefix);

            /* the very first BU value is always 0, and there will not be a carry unless a '0' bit has been output */
            c_outi("tbits += BLEN;\n");
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix);
            
            c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
            c_identlevel++;
            c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN-1;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
            c_identlevel--;
            c_outi("}\n");

            c_outi("else {\n");
            c_identlevel++;
            c_outi("%sBU = %sCBU & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU = 0;\n", prefix);
            c_identlevel--;
            c_outi("}\n");

            c_identlevel--;
            c_outi("}\n");

            c_identlevel--;
            c_outi("} while (lastbits > 0);\n");

            c_outi("tbits += BLEN+lastbits;\n");
            c_outi("%sbs.putbits(%sBU, BLEN+lastbits);\n", prefix, prefix);
            c_outi("if (lastbits==0 && %sCT < BLEN) {\n", prefix);
            c_identlevel++;
            c_outi("tbits++;\n");
            c_outi("%sbs.putbits(%sCBU, 1);\n", prefix, prefix);
            c_identlevel--;
            c_outi("}\n");
        }
    }
    /* byte align with 0-bits */ 
    if (end_type == END_ALIGN_0) 
        c_outi("tbits += %sbs.align(8);\n", prefix);
    /* byte align with 1-bits */ 
    else if (end_type == END_ALIGN_1)
        c_outi("while (%sbs.getpos() %c 8 != 0) { tbits += 1; %sbs.putbits(1, 1); }\n", prefix, '%', prefix);
    /* this is a special case needed just for MPEG-4 shape coding */
    else if (end_type == END_MP4_SHAPE) {
        c_outi("if (n1 == 0 || n2 == max) {\n");
        c_identlevel++;
        c_outi("tbits += 1;\n");
        c_outi("%sbs.putbits(0, 1);\n", prefix);
        c_identlevel--;
        c_outi("}\n");
    }

    c_outi("return tbits;\n");

    c_identlevel--;
    c_outi("}\n");
    c_outi("out++;\n");
    c_outi("n1 += m;\n");
    c_outi("n2 += m;\n");
    c_outi("cases--;\n");

    c_identlevel--;
    c_outi("}\n");

    c_identlevel--;
    c_outi("}\n");
 
    c_identlevel--;
    c_outi("}\n");
}

/*******/
/* get */
/*******/

/* generate the initialization method for decoding */
static void c_bac_ad_init(symbol_t *sp)
{
    c_out("\n");
    c_outi("// returns the number of bits read when initializing\n");
    if (cpp()) 
        c_outi("int %s::ad_init(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
    else if (java()) 	
        c_outi("public static int ad_init(%s %sbs) throws IOException {\n", bitstream_class, prefix);
	
    c_identlevel++;

    c_outi("%sT = 0;\n", prefix);
    if (end_bits == 0 || r_min == 0)
        c_outi("%sL = %u;\n", prefix, l_init);
    c_outi("%sR = %u%s;\n", prefix, r_init, cpp() ? "":"L");
    if (co == CO_FO) 
        c_outi("%sV = %sbs.nextbits(%d);\n", prefix, prefix, v_init);
    else {  // bit-stuffing
        if (v_init >= bs_n1) {
            c_outi("%sV = %sbs.nextbits(%d);\n", prefix, prefix, v_init);
            c_outi("%sCT = %d - (%d %% %d);\n", prefix, bs_n1, v_init, bs_n1);
            c_outi("%sBU = nextbits(%sbs, %sCT);\n", prefix, prefix, prefix); 
        }
        else {
            c_outi("%sBU = %sbs.nextbits(%d);\n", prefix, prefix, bs_n1);
            c_outi("%sV = %sBU >> %d;\n", prefix, prefix, bs_n1-v_init);
            c_outi("%sCT = %d;\n", prefix, bs_n1-v_init);
            c_outi("%sBU &= %u;\n", prefix, (1<<(bs_n1-v_init))-1);
        }
    }

    c_outi("return %d;\n", v_init);
    c_identlevel--;
    c_outi("}\n");
}

/* generate the termination method for decoding */
static void c_bac_ad_end(symbol_t *sp)
{
    c_out("\n");
    c_outi("// returns the number of bits read when terminating\n");
    if (cpp()) 
        c_outi("int %s::ad_end(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
    else if (java())	
        c_outi("public static int ad_end(%s %sbs) throws IOException {\n", bitstream_class, prefix);

    c_identlevel++;

    c_outi("int tbits = 0;\n");
    if (end_bits == 0) {    /* use min # of bits to disambiguate the last symbol */
        c_outi("// read minimal # of bits needed to disambiguate the last symbol\n");
        c_outi("tbits += read_end_bits(%sbs);\n", prefix);
    }
    else {                  /* use end_bits # of bits to disambiguate the last symbol */
        c_outi("// read %d bit(s) to disambiguate the last symbol\n", end_bits);
        c_outi("tbits += %d;\n", end_bits);
        c_outi("%sbs.skipbits(%d);\n", prefix, end_bits);
        if (end_type == END_ALIGN_0 || end_type == END_ALIGN_1) 
            c_outi("tbits += %sbs.align(8); // must byte align\n", prefix);
    }

    c_outi("%sT += tbits;\n", prefix);
    c_outi("return tbits;\n");
    c_identlevel--;
    c_outi("}\n");
}

/* generate methods for binary arithmetic decoding */
static void c_bac_get(symbol_t *sp)
{
    c_bac_ad_init(sp);
    c_bac_ad_end(sp);

    c_out("\n");
    c_outi("// returns the decoded symbol (bit)\n");

    /* the IBAC using multiplication and division */
    if (ooc < OOC_FBAC) {
        if (cpp()) {
            /* if the LPS and MPS are used */
            if (soc == SOC_LM || soc == SOC_ML) {
                c_outi("int %s::ad_code(%s &%sbs, unsigned int c_0, unsigned int c_1) {\n", sp->name, bitstream_class, prefix);
                c_identlevel++;
                c_outi("if (c_0 > c_1) return ad_code(%sbs, c_1, c_0, 0);\n", prefix);
                c_outi("else return ad_code(%sbs, c_0, c_1, 1);\n", prefix);
                c_identlevel--;
                c_outi("};\n\n");
                c_outi("int %s::ad_code(%s &%sbs, unsigned int c_lps, unsigned int c_mps, int mps) {\n", sp->name, bitstream_class, prefix);
            }
            /* else the symbols 0 and 1 are used */
            else
                c_outi("int %s::ad_code(%s &%sbs, unsigned int c_0, unsigned int c_1) {\n", sp->name, bitstream_class, prefix);
        }
        else if (java()) {	
            if (soc < SOC_01) {
    	        c_outi("public static int ad_code(%s %sbs, long c_0, long c_1) throws IOException {\n", bitstream_class, prefix);
                c_identlevel++;
                c_outi("if (c_0 > c_1) return ad_code(%sbs, c_1, c_0, 0);\n", prefix);
                c_outi("else return ad_code(%sbs, c_0, c_1, 1);\n", prefix);
                c_identlevel--;
                c_outi("};\n\n");
                c_outi("public static int ad_code(%s %sbs, long c_lps, long c_mps, int mps) throws IOException {\n", bitstream_class, prefix);
            }
            else
                c_outi("public static int ad_code(%s %sbs, long c_0, long c_1) throws IOException {\n", bitstream_class, prefix);
        }
    }
    /* the FBAC using no multiplicative operations */
    else {
        if (cpp()) {
            if (soc == SOC_LM || soc == SOC_ML)
                c_outi("int %s::ad_code(%s &%sbs, int st, int mps) {\n", sp->name, bitstream_class, prefix);
            else
                c_outi("int %s::ad_code(%s &%sbs, int st) {\n", sp->name, bitstream_class, prefix); 
        }
        else if (java()) {
            if (soc == SOC_LM || soc == SOC_ML)
                c_outi("public static int ad_code(%s %sbs, int st, int mps) throws IOException {\n", bitstream_class, prefix);
            else
                c_outi("public static int ad_code(%s %sbs, int st) throws IOException {\n", bitstream_class, prefix);
        }
    }

    /* output the core code for decoding */
    
    c_identlevel++;
    c_outi("int x; // the decoded symbol\n");
    c_outi("int tbits = 0;\n");
    
    /* if the LPS and MPS are used */
    if (soc < SOC_01) {
        if (ooc == OOC_MULT) {
            if (te == TE_0) {   /* the TE is assigned to the LPS */
                c_outi("%s r_mps = (%sR*c_mps)/(c_lps+c_mps);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_lps = %sR-r_mps;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {              /* the TE is assigned to the MPS */
                c_outi("%s r_lps = (%sR*c_lps)/(c_lps+c_mps);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_mps = %sR-r_lps;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        else if (ooc == OOC_DIV) {
            if (te == TE_0) {   /* the TE is assigned to the LPS */
                c_outi("%s r_mps = (%sR/(c_lps+c_mps))*c_mps;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_lps = %sR-r_mps;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {              /* the TE is assigned to the MPS */
                c_outi("%s r_lps = (%sR/(c_lps+c_mps))*c_lps;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_mps = %sR-r_lps;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        /* no multiplicative operations performed: FBAC */
        else {
            if (cpp()) {
                if (rt_idx2 > 1)  
                    c_outi("unsigned int r_lps = %s[st][(%sR>>%d)&%d];\n", rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("unsigned int r_lps = %s[st];\n", rt_name); 
                c_outi("unsigned int r_mps = %sR-r_lps;\n", prefix);
            }
            else if (java()) {
                if (rt_idx2 > 1)  
                    c_outi("long r_lps = %s.%s[st][(int)(%sR>>%d)&%d];\n", global_class, rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("long r_lps = %s.%s[st];\n", global_class, rt_name); 
                c_outi("long r_mps = %sR-r_lps;\n", prefix);
            }
        }

        /* the LPS over MPS */
        if (soc == SOC_LM) {
            /* if we have the MPS */
            if (end_bits == 0 || r_min == 0) 
                c_outi("if (%sV-%sL < r_mps) { // we have the MPS\n", prefix, prefix);
            else 
                c_outi("if (%sV < r_mps) { // we have the MPS\n", prefix);
            c_identlevel++;
            c_outi("%sR = r_mps;\n", prefix);

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the LPS\n");
                c_identlevel++;
                c_outi("x = 1-mps;\n");
                if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);   
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("x = mps;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1);
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
            c_identlevel--;
            c_outi("}\n");

            /* else we have the LPS */
            c_outi("else { // we have the LPS\n");
            c_identlevel++;
            if (end_bits == 0 || r_min == 0) 
                c_outi("%sL += r_mps;\n", prefix);
            else 
                c_outi("%sV -= r_mps;\n", prefix);
            c_outi("%sR = r_lps;\n", prefix);

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the MPS\n");
                c_identlevel++;
                c_outi("x = mps;\n");
                if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1);   
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("x = 1-mps;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);       
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
        }
        /* the MPS over LPS */
        else {
            /* if we have the LPS */
            if (end_bits == 0 || r_min == 0) 
                c_outi("if (%sV-%sL < r_lps) { // we have the LPS\n", prefix, prefix);
            else 
                c_outi("if (%sV < r_lps) { // we have the LPS\n", prefix);
            c_identlevel++;
            c_outi("%sR = r_lps;\n", prefix);

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the MPS\n");
                c_identlevel++;
                c_outi("x = mps;\n");
                if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1); 
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("x = 1-mps;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);   
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
            c_identlevel--;
            c_outi("}\n");

            /* else we have the MPS */
            c_outi("else { // we have the MPS\n");
            c_identlevel++;
            if (end_bits == 0 || r_min == 0) 
                c_outi("%sL += r_lps;\n", prefix);
            else 
                c_outi("%sV -= r_lps;\n", prefix);
            c_outi("%sR = r_mps;\n", prefix);

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the LPS\n");
                c_identlevel++;
                c_outi("x = 1-mps;\n");
                if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);  
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("x = mps;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1);  
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
        }
    }
    /* else symbols 0 and 1 are used; no reason to have a conditional exchange */
    else {
        if (ooc == OOC_MULT) {
            if (te == TE_0) {   /* the TE is assigned to the symbol 0 */
                c_outi("%s r_1 = (%sR*c_1)/(c_0+c_1);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_0 = %sR-r_1;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {              /* the TE is assigned to the symbol 1 */
                c_outi("%s r_0 = (%sR*c_0)/(c_0+c_1);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_1 = %sR-r_0;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        else if (ooc == OOC_DIV) {
            if (te == TE_0) {   /* the TE is assigned to the symbol 0 */
                c_outi("%s r_1 = (%sR/(c_0+c_1))*c_1;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_0 = %sR-r_1;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {              /* the TE is assigned to the symbol 1 */
                c_outi("%s r_0 = (%sR/(c_0+c_1))*c_0;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_1 = %sR-r_0;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        /* no multiplicative operations performed: FBAC */
        else {
            if (cpp()) {
                if (rt_idx2 > 1) 
                    c_outi("unsigned int r_0 = %s[st][(%sR>>%d)&%d];\n", rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("unsigned int r_0 = %s[st];\n", rt_name);
                c_outi("unsigned int r_1 = %sR-r_lps;\n", prefix);
            }
            else if (java()) {
                if (rt_idx2 > 1) 
                    c_outi("long r_0 = %s.%s[st][(int)(%sR>>%d)&%d];\n", global_class, rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("long r_0 = %s.%s[st];\n", global_class, rt_name);
                c_outi("long r_1 = %sR-r_lps;\n", prefix);
            }
        }

        /* 0 over 1 */
        if (soc == SOC_01) {    /* 0 over 1 */
            /* if we have the symbol 1 */
            if (end_bits == 0 || r_min == 0) 
                c_outi("if (%sV-%sL < r_1) { // we have the symbol 1\n", prefix, prefix);
            else 
                c_outi("if (%sV < r_1) { // we have the symbol 1\n", prefix);
            c_identlevel++;
            c_outi("%sR = r_1;\n", prefix);
            c_outi("x = 1;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1);     
            c_identlevel--;
            c_outi("}\n");

            /* else we have the symbol 0 */
            c_outi("else { // we have the symbol 0\n");
            c_identlevel++;
            if (end_bits == 0 || r_min == 0) 
                c_outi("%sL += r_1;\n", prefix);
            else 
                c_outi("%sV -= r_1;\n", prefix);
            c_outi("%sR = r_0;\n", prefix);
            c_outi("x = 0;\n");
            if (trans == 0) NEXT(0);        /* fast BAC: get the next probability state */
        }
        /* 1 over 0 */
        else {
            /* if we have the symbol 0 */
            if (end_bits == 0 || r_min == 0) 
                c_outi("if (%sV-%sL < r_0) { // we have the symbol 0\n", prefix, prefix);
            else 
                c_outi("if (%sV < r_0) { // we have the symbol 0\n", prefix);
            c_identlevel++;
            c_outi("%sR = r_0;\n", prefix);
            c_outi("x = 0;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);    
            c_identlevel--;
            c_outi("}\n");

            /* else we have the symbol 1 */
            c_outi("else { // we have the symbol 1\n");
            c_identlevel++;
            if (end_bits == 0 || r_min == 0)
                c_outi("%sL += r_0;\n", prefix);
            else 
                c_outi("%sV -= r_0;\n", prefix);
            c_outi("%sR = r_0;\n", prefix);
            c_outi("x = 1;\n");
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1); 
        }
    }

    c_identlevel--;
    c_outi("}\n\n");

    /* renormalization (decoding) */

    c_outi("// renormalization\n");

    if (ooc == OOC_FBAC && trans == TRANS_RENORM) { 
        if (r_min == 0) /* apply renorm as soon as the leading bit is known; the CACM implementation */
            c_outi("if ((%sL+%sR <= HALF) || (%sL >= HALF) || (%sL >= QTR && %sL+%sR <= HALF+QTR) {\n", prefix, prefix, prefix, prefix, prefix, prefix);
        else            /* apply renorm when R is below a certain value */
            c_outi("if (%sR < %u) {\n", prefix, r_min);
        c_identlevel++;
    }

    if (r_min == 0) {   /* the CACM implementation */
        if (cpp()) 
            c_outi("while (1) {\n");
        else if (java())
            c_outi("while (true) {\n");
        c_identlevel++;

        /* here, we need both V and L, even though end_bits != 0 */
        c_outi("if (%sL+%sR <= HALF) {} // do nothing\n", prefix, prefix) ;
        c_outi("else if (%sL >= HALF) { %sL -= HALF; %sV -= HALF; }\n", prefix, prefix, prefix);
        c_outi("else if (%sL >= QTR && %sL+%sR <= HALF+QTR) { %sL -= QTR; %sV -= QTR; }\n", prefix, prefix, prefix, prefix, prefix);
        c_outi("else break;\n");
        c_outi("tbits++; %sL <<= 1; %sR <<= 1; %sV <<= 1; %sV += nextbits(%sbs, 1);\n", prefix, prefix, prefix, prefix, prefix);
    }
    else {           /* apply renorm whenever R is below a certain value */ 
        c_outi("while (%sR < %u) {\n", prefix, r_min);
        c_identlevel++;

        if (co == CO_BS) {
            c_outi("if (%sCT == 0) {\n", prefix);
            c_identlevel++;
            c_outi("%sCT = BLEN;\n", prefix);
            c_outi("tbits += BLEN;\n");
            c_outi("if (%sBU == BU_ONES) %sCT -= 1;\n", prefix, prefix);
            c_outi("%sBU = nextbits(%sbs, BLEN);\n", prefix, prefix);
            c_outi("%sV += (%sBU>>%sCT) & 1;\n", prefix, prefix, prefix);
            c_identlevel--;
            c_outi("}\n");

            /* use V and L separately */
            if (end_bits == 0) 
                c_outi("%sL <<= 1; %sR <<= 1; %sV <<= 1; %sCT--; %sV += (%sBU>>%sCT) & 1;\n", prefix, prefix, prefix, prefix, prefix, prefix, prefix);
            /* just use V=V-L, instead of V and L separately */
            else
                c_outi("%sR <<= 1; %sV <<= 1; %sCT--; %sV += (%sBU>>%sCT) & 1;\n", prefix, prefix, prefix, prefix, prefix, prefix);
        }
        // co == CO_FO
        else { 
            /* use V and L separately */
            if (end_bits == 0) {
                if (r_min <= qtr_plus_2) {
                    c_outi("if (%sL >= HALF) { %sL -= HALF; %sV -= HALF; }\n", prefix, prefix, prefix);
                    c_outi("else if (%sL >= QTR) { %sL -= QTR; %sV -= QTR; }\n", prefix, prefix, prefix);
                    c_outi("tbits++; %sL <<= 1; %sR <<= 1; %sV <<= 1; %sV += nextbits(%sbs, 1);\n", prefix, prefix, prefix, prefix, prefix);
                }
                else 
                    c_outi("tbits++; %sL <<= 1; %sR <<= 1; %sV <<= 1; %sV += nextbits(%sbs, 1);\n", prefix, prefix, prefix, prefix, prefix);
            }
            /* just use V=V-L, instead of V and L separately */
            else 
                c_outi("tbits++; %sR <<= 1; %sV <<= 1; %sV += nextbits(%sbs, 1);\n", prefix, prefix, prefix, prefix);
        }
    }

    c_identlevel--;
    c_outi("}\n");

    if (ooc == OOC_FBAC && trans == TRANS_RENORM) {
        c_outi("if (x == mps)\n");
        c_identlevel++;
        NEXT(1); 
        c_identlevel--;
        c_outi("else\n");
        c_identlevel++;
        NEXT(0);
        c_identlevel--;

        c_identlevel--;
        c_outi("}\n");
    }

    c_outi("%sT += tbits;\n", prefix);
    c_outi("return x;\n");

    c_identlevel--;
    c_outi("}\n");
}

/*******/
/* put */
/*******/

/* generate the initialization method for encoding */
static void c_bac_ae_init(symbol_t *sp)
{
    c_out("\n");
    c_outi("// returns the number of bits written when initializing\n");
    if (cpp()) 
        c_outi("int %s::ae_init(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
    else if (java())	
        c_outi("public static int ae_init(%s %sbs) throws IOException {\n", bitstream_class, prefix);

    c_identlevel++;

    c_outi("%sT = 0;\n", prefix);
    c_outi("%sL = %u;\n", prefix, l_init);
    c_outi("%sR = %u%s;\n", prefix, r_init, cpp() ? "":"L");
    if (co == CO_FO && r_min <= qtr_plus_2)
        c_outi("%sF = 0;\n", prefix);
    else {
        if (co == CO_FO)
            c_outi("%sSC = 0;\n", prefix);
        c_outi("%sCT = BLEN+%d;\n", prefix, bs_ns);
        c_outi("%sBU = 0;\n", prefix);
        c_outi("%sCBU = 0;\n", prefix);
        c_outi("%sFirst = 1;\n", prefix);
    }

    if (prec > v_init) {
        c_outi("%sSkip = %d;\n", prefix, prec-v_init);
        /* update the counter */
        if (!(co == CO_FO && r_min <= qtr_plus_2))
            c_outi("%sCT += %sSkip;\n", prefix, prefix);
    }

    c_outi("return 0;\n");

    c_identlevel--;
    c_outi("}\n");
}

/* generate the termination method for encoding */
static void c_bac_ae_end(symbol_t *sp)
{
    c_out("\n");
    c_outi("// returns the number of bits written when terminating\n");
    if (cpp()) 
        c_outi("int %s::ae_end(%s &%sbs) {\n", sp->name, bitstream_class, prefix);
    else if (java()) 	
        c_outi("public static int ae_end(%s %sbs) throws IOException {\n", bitstream_class, prefix);

    c_identlevel++;

    c_outi("int tbits = 0;\n");

    /* output minimal # of bits to disambiguate the last symbol */
    if (end_bits == 0) {    
        c_outi("// write min num of bits needed to disambiguate the last symbol\n");
        c_outi("tbits += write_end_bits(%sbs);\n", prefix);
        goto finish_c_bac_ae_end;
    }
    /* output all the pending bits + 2 bits; the CACM implementation */
    if (r_min == 0 && end_bits == 2) { /* r_min == 0 => co == CO_FO */
        c_outi("// output all the F pending bits + 2 bits\n");
        c_outi("%sF++;\n", prefix);
        c_outi("tbits += %sF+1;\n", prefix);
        bpf_end(BPF_CACM);
        goto finish_c_bac_ae_end;
    }
    /* first output all the pending bits + the first bit of the current working bits for L */
    if (co == CO_FO && r_min <= qtr_plus_2) {
        c_outi("// output all the F pending bits + the first bit of current L\n");
        c_outi("tbits += %sF+1;\n", prefix);
        bpf_end(BPF_TOIS);
    }
    else {
        c_outi("// output BU + BLEN-CT bits of CBU + %d spacer bits + the first bit of current L\n", bs_ns);
        c_outi("int last_cbu_len = BLEN-%sCT;\n", prefix, bs_ns);  
        if ((co == CO_FO && prec <= 27) || (co == CO_BS && bs_n1+bs_ns+prec < 32)) {
            c_outi("%sCBU = (int)(%sL>>%d);\n", prefix, prefix, bs_ns+prec);
            c_outi("%sL &= (1<<%d)-1;\n", prefix, bs_ns+prec);
        }
        if (co == CO_FO) {
            /* output BU */
            c_outi("if (%sCBU > (1 << last_cbu_len)-1) { // there was a carry\n", prefix); 
            c_identlevel++;
            c_outi("%sbs.putbits(%sBU+1, BLEN);\n", prefix, prefix);
            c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(0, BLEN);\n", prefix, prefix, prefix);
            c_identlevel--;
            c_outi("}\n");
            c_outi("else {\n");
            c_identlevel++;
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix); 
            c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(BU_ONES, BLEN);\n", prefix, prefix, prefix); 
            c_identlevel--;
            c_outi("}\n");
            /* output BLEN-CT bits of CBU */
            c_outi("%sbs.putbits(%sCBU, last_cbu_len);\n", prefix, prefix);
            /* ouput spacer bits + the first bit of current L */
            c_outi("%sbs.putbits((int)(%sL>>%d), %d);\n", prefix, prefix, prec-1, bs_ns+1);
            c_outi("tbits += (1+%sSC)*BLEN + last_cbu_len + %d + 1;\n", prefix, bs_ns);
        }
        else if (co == CO_BS) {
            c_outi("int lastbits = %d;\n", bs_ns+end_bits);
            c_outi("%sL >>= %d;\n", prefix, prec-end_bits); 
            c_outi("%sL <<= 32-lastbits;\n", prefix);
            c_outi("do {\n");
            c_identlevel++;

            c_outi("%sCBU <<= %sCT;\n", prefix, prefix); 
            c_outi("// make sure that CBU consists of BLEN bits, if possible\n");
            c_outi("if (lastbits >= %sCT) %sCBU += (int)(%sL>>(32-%sCT));\n", prefix, prefix, prefix, prefix);
            c_outi("else %sCBU += (int)(%sL<<(%sCT-lastbits));\n", prefix, prefix, prefix);
            c_outi("%sL <<= %sCT;\n", prefix, prefix);
            c_outi("lastbits -= %sCT;\n", prefix);

            c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
            c_identlevel++;
            
            c_outi("tbits += BLEN;\n");
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix); 
            c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN-1;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
          
            c_identlevel--;
            c_outi("}\n");

            c_outi("else {\n");
            c_identlevel++;
            
            c_outi("%sBU += (%sCBU >> BLEN);\n", prefix, prefix);
            c_outi("%sCBU &= BU_ONES;\n", prefix);

            /* the very first BU value is always 0, and there will not be a carry unless a '0' bit has been output */
            c_outi("tbits += BLEN;\n");
            c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix);
            
            c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
            c_identlevel++;
            c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN-1;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
            c_identlevel--;
            c_outi("}\n");

            c_outi("else {\n");
            c_identlevel++;
            c_outi("%sBU = %sCBU & BU_ONES;\n", prefix, prefix);
            c_outi("%sCT = BLEN;\n", prefix);
            if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU = 0;\n", prefix);
            c_identlevel--;
            c_outi("}\n");

            c_identlevel--;
            c_outi("}\n");

            c_identlevel--;
            c_outi("} while (lastbits > 0);\n");

            c_outi("tbits += BLEN+lastbits;\n");
            c_outi("%sbs.putbits(%sBU, BLEN+lastbits);\n", prefix, prefix);
            c_outi("if (lastbits==0 && %sCT < BLEN) {\n", prefix);
            c_identlevel++;
            c_outi("tbits++;\n");
            c_outi("%sbs.putbits(%sCBU, 1);\n", prefix, prefix);
            c_identlevel--;
            c_outi("}\n");
            goto finish_c_bac_ae_end;
        }
    }
    if (end_bits > 1) {
        c_outi("// write %d bit(s) to disambiguate the last symbol\n", end_bits-1);
        c_outi("tbits += %d;\n", end_bits-1);
        if (cpp()) 
            c_outi("%sbs.putbits((%sL>>%d)&%u, %d);\n", prefix, prefix, prec-end_bits, PREC_BITS[end_bits-1], end_bits-1);
        else if (java())
            c_outi("%sbs.putlong((int)(%sL>>%d)&%u, %d);\n", prefix, prefix, prec-end_bits, PREC_BITS[end_bits-1], end_bits-1);

        /* byte align with 0-bits */ 
        if (end_type == END_ALIGN_0) 
            c_outi("tbits += %sbs.align(8);\n", prefix);
        /* byte align with 1-bits */ 
        else if (end_type == END_ALIGN_1)
            c_outi("while (%sbs.getpos() %c 8) { tbits += 1; %sbs.putbits(1, 1); }\n", prefix, '%', prefix);
    }

finish_c_bac_ae_end:
    c_outi("%sT += tbits;\n", prefix);
    c_outi("return tbits;\n");
    c_identlevel--;
    c_outi("}\n");
}

static void renorm_enc_norm() 
{
    if (r_min == 0) {   /* the CACM implementation */
        if (cpp()) 
            c_outi("while (1) {\n");
        else if (java())
            c_outi("while (true) {\n");
        c_identlevel++;

        bpf(BPF_CACM, prec-v_init);
    }
    else  {             /* apply renorm whenever R < r_min */
        c_outi("while (%sR < %u) {\n", prefix, r_min);
        c_identlevel++;

        if (co == CO_FO && r_min <= qtr_plus_2) 
            bpf(BPF_TOIS, prec-v_init);
        else {
            /* if the CBU is maintained in a separate register; L contains both the spacer bits and the actual bits of L */
            if ((co == CO_FO && prec > 27) || (co == CO_BS && bs_n1+bs_ns+prec >= 32)) {
                /* if there was a carry over */
                c_outi("if (%sL >= BS_ONE) { %sCBU++; %sL -= BS_ONE; }\n", prefix, prefix, prefix);
                /* shift a bit from L to CBU */
                c_outi("%sCBU <<= 1;\n", prefix);
                c_outi("if (%sL >= BS_HALF) { %sCBU++; %sL -= BS_HALF; }\n", prefix, prefix, prefix);
            }

            /* the CBU is full; need to output */
            c_outi("if (%sCT == 0) {\n", prefix);
            c_identlevel++;
            /* if the CBU is maintained within L, get the CBU portion from L */
            if ((co == CO_FO && prec <= 27) || (co == CO_BS && bs_n1+bs_ns+prec < 32))
                c_outi("%sCBU = (int)(%sL>>%d);\n", prefix, prefix, bs_ns+prec);
            if (co == CO_FO) {
                c_outi("if (%sCBU > BU_ONES) { // there was a carry\n", prefix);        
                c_identlevel++;
                c_outi("tbits += (%sSC+1)*BLEN;\n", prefix);
                c_outi("%sbs.putbits(%sBU+1, BLEN);\n", prefix, prefix);
                c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(0, BLEN);\n", prefix, prefix, prefix); 
                c_outi("%sBU = %sCBU & BU_ONES;\n", prefix, prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
                c_outi("if (%sCBU == BU_ONES) %sSC++;\n", prefix, prefix);
                c_outi("else {\n");
                c_identlevel++;
                /* the very first BU value is always 0, and there will not be a carry unless a 0-bit has been output */
                c_outi("if (%sFirst == 0) {\n", prefix);
                c_identlevel++;
                c_outi("tbits += BLEN;\n");
                c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else %sFirst = 0;\n", prefix);
                c_outi("tbits += %sSC*BLEN;\n", prefix);
                c_outi("for ( ; %sSC > 0; %sSC--) %sbs.putbits(BU_ONES, BLEN);\n", prefix, prefix, prefix); 
                c_outi("%sBU = %sCBU;\n", prefix, prefix);
                c_identlevel--;
                c_outi("}\n");
                c_identlevel--;
                c_outi("}\n");
                c_outi("%sCT = BLEN;\n", prefix);
                c_outi("%sL &= %u;\n", prefix, PREC_BITS[prec+bs_ns]);
                if (prec > 27) c_outi("%sCBU = 0;\n", prefix);
            }
            else if (co == CO_BS) {
                c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
                c_identlevel++;

                c_outi("tbits += BLEN;\n");
                c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix); 
                c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
                c_outi("%sL &= %u;\n", prefix, PREC_BITS[prec+bs_ns+1]);
                c_outi("%sCT = BLEN-1;\n", prefix);
                if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
                c_identlevel--;
                c_outi("}\n");

                c_outi("else {\n");
                c_identlevel++;

                c_outi("%sBU += (%sCBU >> BLEN);\n", prefix, prefix);
                c_outi("%sCBU &= BU_ONES;\n", prefix);
                /* the very first BU value is always 0, and there will not be a carry unless a 0-bit has been output */
                c_outi("if (%sFirst == 0) {\n", prefix);
                c_identlevel++;
                
                c_outi("tbits += BLEN;\n");
                c_outi("%sbs.putbits(%sBU, BLEN);\n", prefix, prefix);
                c_identlevel--;
                c_outi("}\n");
                
                c_outi("else %sFirst = 0;\n", prefix);
                c_outi("if (%sBU == BU_ONES) { // cannot propagate any carry; need bit-stuffing\n", prefix);
                c_identlevel++;

                c_outi("%sBU = (%sCBU >> 1) & BU_ONES;\n", prefix, prefix);
                c_outi("%sL &= %u;\n", prefix, PREC_BITS[prec+bs_ns+1]);
                c_outi("%sCT = BLEN-1;\n", prefix);
                if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU &= 1;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;

                c_outi("%sBU = %sCBU & BU_ONES;\n", prefix, prefix);
                c_outi("%sL &= %u;\n", prefix, PREC_BITS[prec+bs_ns]);
                c_outi("%sCT = BLEN;\n", prefix);
                if (bs_n1+bs_ns+prec >= 32) c_outi("%sCBU = 0;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_identlevel--;
                c_outi("}\n");
            }
            c_identlevel--;
            c_outi("}\n");
            c_outi("%sL <<= 1; %sR <<= 1; %sCT--;\n", prefix, prefix, prefix);
        }
    }
    c_identlevel--;
    c_outi("}\n");
}

static void renorm_enc_quasi() 
{
    if (r_min == 0) {   /* the CACM implementation */
        /* if R > HALF, then b = 0 (no renormalization) */
        c_outi("%s H=%sL+%sR;\n", cpp() ? "unsigned int":"long", prefix, prefix);
        c_outi("%s renorm = tblOne[(int)(1-(((%sR-1)>>(%d-1))&1))];\n", cpp() ? "unsigned int":"long", prefix, prec);
        c_outi("char b_bar = (char)(renorm & tblNumBits[(int)((%sR-1)&(HALF-1))]);\n", prefix);
        c_outi("char shift = (char)(%d-b_bar);\n", prec);
        c_outi("char b = (char)(b_bar -(((%sL&tblLSB[b_bar])+((%sR-1)&tblLSB[b_bar]))>>(shift-1)));\n", prefix, prefix);
        // f
        c_outi("%s i_f = H >> shift; // index into the F table\n", cpp() ? "unsigned int":"long");
        c_outi("char i_1 = (char)(((%sL-(i_f<<shift))>>31)&(((i_f<<shift)-H)>>31) & 1); // index into the 1 table\n", prefix);
        c_outi("char f = (char)(tblOne[i_1]&(tblF[(int)i_f]-b_bar+b));\n");
        // out
        c_outi("i_1 = (char)(((%sL | ~(H-1) | ( (%sL&~(H-1)) <<1 )) >> %d) &1);\n", prefix, prefix, prec-1);
        c_outi("b &= tblOne[i_1];\n");
        c_outi("f &= tblOne[i_1];\n");
        c_outi("char out = (char)(b-f); \n");
        c_outi("if (out > 0) {\n");
        c_identlevel++;

        c_outi("char out_bit = (char)((%sL>>%d)&1);\n", prefix, prec-1);
        if (prec == v_init) {
            c_outi("%sbs.putbits(out_bit, 1);\n", prefix); 
            c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(1-out_bit, 1);\n", prefix, prefix, prefix); 
        }
        else {
            c_outi("if (%sSkip == 0) %sbs.putbits(out_bit, 1); else %sSkip--;\n", prefix, prefix, prefix); 
            c_outi("for ( ; %sF>0; %sF--) if (%sSkip == 0) %sbs.putbits(1-out_bit, 1); else %sSkip--;\n", prefix, prefix, prefix, prefix, prefix); 
        }
        
        c_outi("%sbs.putbits((int) ( ( (%sL&(HALF-1))>>(%d-out) ) & ((1<<(out-1))-1)), out-1);\n", prefix, prefix, prec);   
        c_outi("%sF = 0;\n", prefix);
        c_identlevel--;
        c_outi("}\n");
        // Update state information
        c_outi("%sL = (%sL<<b) & PREC_BITS[%d];\n", prefix, prefix, prec);
        c_outi("%sL -= tblOne[((int)(0-f)>>31)&1] & HALF;\n", prefix); 
        c_outi("%sR <<= b;\n", prefix);
        c_outi("%sF += f;\n", prefix);
    }
    else  {             /* apply renorm whenever R < r_min */
        /* if R > HALF, then b = 0 (no renormalization) */
        c_outi("%s renorm = tblOne[(int)(1-(((%sR-1)>>(%d-1))&1))];\n", cpp() ? "unsigned int":"long", prefix, prec);
        c_outi("char b = (char)(renorm & tblNumBits[(int)(%sR&(HALF-1))]);\n", prefix);
        
        // Getting the number of follow bits f
        c_outi("char shift = (char)(%d-b);\n", prec); 
        c_outi("%s i_f = (renorm & (%sL+%sR-1)) >> shift;\n", cpp() ? "unsigned int":"long", prefix, prefix);
        c_outi("int diff = (int)(%sL - (i_f << shift)); // diff is negative if L < (i_f << shift)\n", prefix);
        c_outi("char f = (char)(tblOne[(diff>>31)&1]&tblF[(int)i_f]);\n");
        c_outi("char out = (char)(b-f);\n");
        c_outi("if (out > 0) {\n");
        c_identlevel++;
        c_outi("char out_bit = (char)((%sL>>%d)&1);\n", prefix, prec-1);
        if (prec == v_init) {
            c_outi("%sbs.putbits(out_bit, 1);\n", prefix); 
            c_outi("for ( ; %sF>0; %sF--) %sbs.putbits(1-out_bit, 1);\n", prefix, prefix, prefix); 
        }
        else {
            c_outi("if (%sSkip == 0) %sbs.putbits(out_bit, 1); else %sSkip--;\n", prefix, prefix, prefix); 
            c_outi("for ( ; %sF>0; %sF--) if (%sSkip == 0) %sbs.putbits(1-out_bit, 1); else %sSkip--;\n", prefix, prefix, prefix, prefix, prefix); 
        }
        c_outi("%sbs.putbits((int)( ( (%sL&(HALF-1)) >> (%d-out) ) & ((1<<(out-1))-1)), out-1);\n", prefix, prefix, prec);  
        c_outi("%sF = 0;\n", prefix);
        c_identlevel--;
        c_outi("}\n");
        c_outi("%sL = ((%sL<<b) & PREC_BITS[%d]) - (tblOne[(diff>>31)&1] & HALF);\n", prefix, prefix, prec);
        c_outi("%sR <<= b;\n", prefix);
        c_outi("%sF += f;\n", prefix);
    }
}

/* generate methods for binary arithmetic encoding */
static void c_bac_put(symbol_t *sp)
{
    c_bac_ae_init(sp);
    c_bac_ae_end(sp);

    c_outi("\n");
    c_outi("// returns the number of bits written\n");

    /* the IBAC using multiplication and division */
    if (ooc < OOC_FBAC) {
        if (cpp()) {
            /* if the LPS and MPS are used */
            if (soc < SOC_01) {
                c_outi("int %s::ae_code(%s &%sbs, int x, unsigned int c_0, unsigned int c_1) {\n", sp->name, bitstream_class, prefix);
                c_identlevel++;
                c_outi("if (c_0 > c_1) return ae_code(%sbs, x, c_1, c_0, 0);\n", prefix);
                c_outi("else return ae_code(%sbs, x, c_0, c_1, 1);\n", prefix);
                c_identlevel--;
                c_outi("};\n\n");
                c_outi("int %s::ae_code(%s &%sbs, int x, unsigned int c_lps, unsigned int c_mps, int mps) {\n", sp->name, bitstream_class, prefix); 
            }
            /* else the symbols 0 and 1 are used */
            else
                c_outi("int %s::ae_code(%s &%sbs, int x, unsigned int c_0, unsigned int c_1) {\n", sp->name, bitstream_class, prefix); 
        }
        else if (java()) {
            if (soc == SOC_LM || soc == SOC_ML) {
                c_outi("public static int ae_code(%s %sbs, int x, long c_0, long c_1) throws IOException {\n", bitstream_class, prefix);
                c_identlevel++;
                c_outi("if (c_0 > c_1) return ae_code(%sbs, x, c_1, c_0, 0);\n", prefix);
                c_outi("else return ae_code(%sbs, x, c_0, c_1, 1);\n", prefix);
                c_identlevel--;
                c_outi("};\n\n");
                c_outi("public static int ae_code(%s %sbs, int x, long c_lps, long c_mps, int mps) throws IOException {\n", bitstream_class, prefix); 
            }
            else
                c_outi("public static int ae_code(%s %sbs, int x, long c_0, long c_1) throws IOException {\n", bitstream_class, prefix);
        }
    }
    /* the FBAC using no multiplicative operations */
    else {
        if (cpp()) {
            if (soc == SOC_LM || soc == SOC_ML)
                c_outi("int %s::ae_code(%s &%sbs, int x, int st, int mps) {\n", sp->name, bitstream_class, prefix);
            else
                c_outi("int %s::ae_code(%s &%sbs, int x, int st) {\n", sp->name, bitstream_class, prefix); 
        }
        else if (java()) {
            if (soc == SOC_LM || soc == SOC_ML)
		        c_outi("public static int ae_code(%s %sbs, int x, int st, int mps) throws IOException {\n", bitstream_class, prefix);
            else
                c_outi("public static int ae_code(%s %sbs, int x, int st) throws IOException {\n", bitstream_class, prefix);
        }
    }

    /* output the core code for encoding */

    c_identlevel++;
    c_outi("int tbits = 0;\n");

    /* if the LPS and MPS are used */
    if (soc < SOC_01) {
        if (ooc == OOC_MULT) {
            if (te == TE_0) {
                c_outi("%s r_mps = (%sR*c_mps)/(c_lps+c_mps);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_lps = %sR-r_mps;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {
                c_outi("%s r_lps = (%sR*c_lps)/(c_lps+c_mps);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_mps = %sR-r_lps;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        else if (ooc == OOC_DIV) {
            if (te == TE_0) {
                c_outi("%s r_mps = (%sR/(c_lps+c_mps))*c_mps;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_lps = %sR-r_mps;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {
                c_outi("%s r_lps = (%sR/(c_lps+c_mps))*c_lps;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_mps = %sR-r_lps;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        /* no multiplicative operations performed: FBAC */
        else {
            if (cpp()) {
                if (rt_idx2 > 1)
                    c_outi("unsigned int r_lps = %s[st][(%sR>>%d)&%d];\n", rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("unsigned int r_lps = %s[st];\n", rt_name); 
                c_outi("unsigned int r_mps = %sR-r_lps;\n", prefix);
            }
            else if (java()) {
                if (rt_idx2 > 1)
                    c_outi("long r_lps = %s.%s[st][(int)(%sR>>%d)&%d];\n", global_class, rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("long r_lps = %s.%s[st];\n", global_class, rt_name); 
                c_outi("long r_mps = %sR-r_lps;\n", prefix);
            }
        } 

        /* the LPS over MPS */
        if (soc == SOC_LM) {
            /* if we have the MPS */
            c_outi("if (x == mps) { // we have the MPS\n");
            c_identlevel++;

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the LPS\n");
                c_identlevel++;
                c_outi("%sL += r_mps;\n", prefix);  
                c_outi("%sR = r_lps;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("%sR = r_mps;\n", prefix);
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1); 
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
            c_identlevel--;
            c_outi("}\n");

            /* else we have the LPS */
            c_outi("else { // we have the LPS\n");
            c_identlevel++;

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the MPS\n");
                c_identlevel++;
                c_outi("%sR = r_mps;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("%sL += r_mps;\n", prefix);
            c_outi("%sR = r_lps;\n", prefix);
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
        }
        /* the MPS over LPS */
        else {
             /* if we have the MPS */
            c_outi("if (x == mps) { // we have the MPS\n");
            c_identlevel++;

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the LPS\n");
                c_identlevel++;
                c_outi("%sR = r_lps;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("%sL += r_lps;\n", prefix);
            c_outi("%sR = r_mps;\n", prefix);
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1);
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
            c_identlevel--;
            c_outi("}\n");

            /* else we have the LPS */
            c_outi("else { // we have the LPS\n");
            c_identlevel++;

            if (exch) { /* if conditional exchange is required */
                c_outi("if (r_mps < r_lps) { // conditional exchange: we actually have the MPS\n");
                c_identlevel++;
                c_outi("%sL += r_lps;\n", prefix);
                c_outi("%sR = r_mps;\n", prefix);
                c_identlevel--;
                c_outi("}\n");
                c_outi("else {\n");
                c_identlevel++;
            }
            c_outi("%sR = r_lps;\n", prefix);
            if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);
            if (exch) { /* if conditional exchange is required */
                c_identlevel--;
                c_outi("}\n");
            }
        }
    }
    /* else symbols 0 and 1 are used; no reason to have a conditional exchange */
    else {
        if (ooc == OOC_MULT) {
            if (te == TE_0) {
                c_outi("%s r_1 = (%sR*c_1)/(c_0+c_1);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_0 = %sR-r_1;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {
                c_outi("%s r_0 = (%sR*c_0)/(c_0+c_1);\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_1 = %sR-r_0;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        else if (ooc == OOC_DIV) {
            if (te == TE_0) {
                c_outi("%s r_1 = (%sR/(c_0+c_1))*c_1;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_0 = %sR-r_1;\n", cpp() ? "unsigned int": "long", prefix);
            }
            else {
                c_outi("%s r_0 = (%sR/(c_0+c_1))*c_0;\n", cpp() ? "unsigned int": "long", prefix);
                c_outi("%s r_1 = %sR-r_0;\n", cpp() ? "unsigned int": "long", prefix);
            }
        }
        /* no multiplicative operations performed: FBAC */
        else {
            if (cpp()) {
                if (rt_idx2 > 1) 
                    c_outi("unsigned int r_0 = %s[st][(%sR>>%d)&%d];\n", rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("unsigned int r_0 = %s[st];\n", rt_name); 
                c_outi("unsigned int r_1 = %sR-r_lps;\n",  prefix);
            }
            else if (java()) {
                if (rt_idx2 > 1) 
                    c_outi("long r_0 = %s.%s[st][(int)(%sR>>%d)&%d];\n", global_class, rt_name, prefix, prec-fixed_msb_r-m_bits, PREC_BITS[m_bits]); 
                else 
                    c_outi("long r_0 = %s.%s[st];\n", global_class, rt_name); 
                c_outi("long r_1 = %sR-r_lps;\n",  prefix);
            }
        }

        /* if we have the symbol 1 */
        c_outi("if (x == 1) { // we have the symbol 1\n");
        c_identlevel++;
        if (soc == SOC_10) c_outi("%sL += r_0;\n", prefix);
        c_outi("%sR = r_1;\n", prefix);
        if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(1); 
        c_identlevel--;
        c_outi("}\n");

        /* else we have the symbol 0 */
        c_outi("else { // we have the symbol 0\n");
        c_identlevel++;
        if (soc == SOC_01) c_outi("%sL += r_1;\n", prefix, prefix);
        c_outi("%sR = r_0;\n", prefix);
        if (ooc == OOC_FBAC && trans == TRANS_SYM) NEXT(0);
    }

    c_identlevel--;
    c_outi("}\n\n");
     
    /* renormalization (encoding) */

    c_outi("// renormalization\n");

    /* if prob transition is applied after renorm */
    if (ooc == OOC_FBAC && trans == TRANS_RENORM) {
        if (r_min == 0) /* apply renorm as soon as the leading bit is known; the CACM implementation */
            c_outi("if ((%sL+%sR <= HALF) || (%sL >= HALF) || (%sL >= QTR && %sL+%sR <= HALF+QTR) {\n", prefix, prefix, prefix, prefix, prefix, prefix);
        else            /* apply renorm when R is below a certain value */
            c_outi("if (%sR < %u) {\n", prefix, r_min);
        c_identlevel++;
    }

    if (r_type == RENORM_QUASI) 
        renorm_enc_quasi();
    else
        renorm_enc_norm();
    

    /* if prob transition is applied after renorm */
    if (ooc == OOC_FBAC && trans == TRANS_RENORM) {
        c_outi("if (x == mps)\n");
        c_identlevel++;
        NEXT(1); 
        c_identlevel--;
        c_outi("else\n");
        c_identlevel++;
        NEXT(0);
        c_identlevel--;

        c_identlevel--;
        c_outi("}\n");
    }

    c_outi("%sT += tbits;\n", prefix);
    c_outi("return tbits;\n");

    c_identlevel--;
    c_outi("}\n");
}

/*******/
/* xml */
/*******/


/*******/
/* xsd */
/*******/


/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/

/* set the BAC params to their defaults */
static void c_init_bac_params(void)
{
    prec = 16;              /* 16-bit precision */
    ooc = OOC_MULT;         /* multiply first */
    soc = SOC_LM;           /* the LPS over the MPS */
    te = TE_1;              /* the TE is assigned to the MPS */
    
    /* renormalization params */
    r_min = 1<<(prec-2);    /* apply renorm whenever R < QTR */
    r_type = RENORM_ORG;    /* straightforward renormalization */
    r_nb = 1;               /* output 1 bit at a time */
    
    co = CO_FO;             /* apply the follow-on method to prevent the CO problem */

    /* bit-stuffing params */
    bs_n1 = 0;              /* the number of 1 bits accounted when applying stuffing */   
    bs_n0 = 0;              /* the number of 0 bits stuffed */
    bs_ns = 0;              /* the number of spacer bits used */

    r_init = 1<<prec;       /* initially R = ONE */    
    l_init = 0;             /* initially L = 0 */ 
    v_init = prec;          /* initially read prec bits for V */
    end_bits = prec;        /* output prec # of bits to disambiguate the last symbol */
    end_type = 0;           /* no special action is needed */

    /* rtable params; the table lists R values for the LPS or the symbol 0 */  
    rt_idx1 = 0;        
    rt_idx2 = 0;       
    rt_name = NULL;  

    /* nexti param */
    ni_name = NULL;         /* the table lists the prob transition rules */ 

    trans = TRANS_SYM;      /* apply transition after coding of each symbol */
    exch = 0;               /* do not apply conditional exchange */
}

/* get the params for the BAC */
static void c_bac_core(list_t *stmts)
{
    list_t *lp = stmts;
    int r_min_flag = 0, r_init_flag = 0, v_init_flag = 0, end_bits_flag = 0;

    /* set the bac params to their defaults */
    c_init_bac_params();

    while (lp != NULL) {
        node_t *np = lp->sub1->e1;
        
        switch (lp->e1->left.ival) {
        case BAC_PREC:
            prec = np->left.ival;
            break;
        case BAC_OOC:
            ooc = np->left.ival;
            break;
        case BAC_SOC:
            soc = np->left.ival;
            break;
        case BAC_TE:
            te = np->left.ival;
            break;
        case BAC_RENORM:
            r_min_flag = 1;
            /* only one arg is assigned to the norm param */
            if (lp->sub1->type == EXPR) 
                r_min = np->left.ival;
            else {
                np = lp->sub1->sub1->e1;
                r_min = np->left.ival;
                np = lp->sub1->sub1->next->e1;
                r_type = np->left.ival;
                if (lp->sub1->sub1->next->next != NULL) {
                    np = lp->sub1->sub1->next->next->e1;
                    r_nb = np->left.ival;
                }
            }
            break;
        case BAC_BS:
            np = lp->sub1->sub1->e1;
            bs_n1 = np->left.ival;
            np = lp->sub1->sub1->next->e1;
            bs_n0 = np->left.ival;
            if (lp->sub1->sub1->next->next != NULL) {
                np = lp->sub1->sub1->next->next->e1;
                bs_ns = np->left.ival;
            }
            co = CO_BS;
            break;
        case BAC_INIT:
            r_init_flag = 1;
            /* only one arg is assigned to the init param */
            if (lp->sub1->type == EXPR) 
                r_init = np->left.uival;
            else {
                v_init_flag = 1;
                np = lp->sub1->sub1->e1;
                r_init = np->left.uival;
                np = lp->sub1->sub1->next->e1;
                v_init = np->left.ival;
            }
            break;
        case BAC_END:
            end_bits_flag = 1;
            /* only one arg is assigned to the end param */
            if (lp->sub1->type == EXPR) 
                end_bits = np->left.ival;
            else {
                np = lp->sub1->sub1->e1;
                end_bits = np->left.ival;
                np = lp->sub1->sub1->next->e1;
                end_type = np->left.ival;
            }
            break;
        case BAC_RTABLE:
            np = lp->sub1->sub1->e1;
            rt_idx1 = np->left.ival;
            np = lp->sub1->sub1->next->e1;
            rt_idx2 = np->left.ival;
            np = lp->sub1->sub1->next->next->e1;
            rt_name = np->left.string;
            break;
        case BAC_NEXTI:
            ni_name = np->left.string;
            break;
        case BAC_TRANS:
            trans = np->left.ival;
            break;
        case BAC_EXCH:
            exch = np->left.ival;
            break;
        default:
            fatal("Internal error CB%d: Expected a param entry for the bac construct", __LINE__);
        }
        /* next code */
        lp = lp->next;
    }
    
    if (!r_min_flag) r_min = 1<<(prec-2); 
    if (!r_init_flag) {
        if (prec < 32) r_init = 1<<prec;
        else r_init = 0xFFFFFFFF;
    }
    if (!v_init_flag) v_init = prec;
    if (!end_bits_flag) end_bits = prec;

    if (v_init == 0) v_init = prec;
    qtr_plus_2 = (unsigned)(1<<(prec-2))+2;
 
    if (prec < end_bits) {
        error("Error: The end_bits should not be greater than the precision used");
        end_bits = prec;
    }
    if (prec + bs_ns > 32) 
        fatal("Error: The precision, including spacer bits, cannot be more than 32 bits");
    if (prec + bs_ns == 32 && r_min > qtr_plus_2) 
        fatal("Error: The precision + the spacer bits cannot be 32 if the min R value is > QTR+2");
    if (bs_n1 > 31) 
        fatal("Error: The BS procedure can only detect less than 32 consecutive 1-bits");
        /* the MSB is used for carry dectection */
    
    if (r_type == RENORM_QUASI && !(co == CO_FO && r_min <= qtr_plus_2))
        fatal("Error: Can only use semi-quasi-renormalization with the FO procedure");
    if (r_type == RENORM_QUASI && prec > 16)
        fatal("Error: When using semi-quasi-renormalization, the precision has to be <= 16 bits");
        
    if (r_min == 0 && co == CO_BS) 
        fatal("Error: The CACM renorm method cannot be used with the BS procedure");
    if (r_min == 0 && end_bits == 1) 
        fatal("Error: The CACM renorm method needs at least 2 bits to disambiguate the last symbol");        
    if (ooc < OOC_FBAC && (rt_idx1 > 0 || rt_idx2 > 0 || rt_name != NULL || ni_name != NULL))
        error("Error: An FBAC param has been set for an IBAC");

    /* If there are more than 1 col in the RTable, then the col is selected based on the current R value.
     * We assume that the first fixed_msb_r MSBs of R does not change. We also assume that rt_idx2 is 
     * a power of 2, so that an efficient bit-shifting operation can be used to obtain the col index.
     * Refer to the paper regarding the M coder for more information about this.
     */
    if (rt_idx2 > 1) {
        int i = 0;
        for (; r_min>>i > 0; ++i) ; 
        if (r_min != (unsigned)1<<(i-1)) 
            fatal("Error: If the RTable has more than 1 col, then the min R value should be a power of 2");        
            /* For example, if R >= QTR, then the first 2 bits are always 01, assuming r_nb=1.
             * However, if, e.g., R >= QTR - 5, then the first 2 bits can be either 00 or 01.
             */
        for (fixed_msb_r=prec; fixed_msb_r > 0; --fixed_msb_r) { 
            if ((unsigned)(1 << (prec-fixed_msb_r)) >= r_min) break;
        }
        /* m_bits is the largest int such that 2^m_bits <= rt_idx2 */
        for (m_bits=prec; m_bits > 1; --m_bits) {
            if ((1 << m_bits) <= rt_idx2) break; 
        }          
    }
}

/* generate BAC definition */
void bac_decl(symbol_t *sp, list_t *stmts)
{ 
    FILE *tmp;

    if (sp == NULL) return;

    /* get the params for the BAC */
    c_bac_core(stmts);

    if (cpp())
        c_out("\nclass %s {\n", sp->name);
    else if (java()) 
        c_out("\npublic class %s {\n", sp->name); 
    else if (xsd()) {
        fatal("Error: The current translator cannot generate XML schemas for BAC descriptions");
        /*
        c_out("\n<xsd:complexType name=\"%s\">\n", sp->name);
        c_identlevel++;
        c_outi("<xsd:sequence>\n");
        */
    }
    else
        fatal("Internal error CB%d: Unsupported code generation option", __LINE__);
		
    c_identlevel++;

    /* if doing C++, declare member methods and switch output files */
    if (cpp()) {
        if (gen_get || gen_put) {
            c_outi("private:\n");
            c_outi("// constants\n");
            if (gen_get && r_type == RENORM_QUASI) {
                c_outi("// tables for semi-quasi-renormalization\n");
                c_outi("static const unsigned int tblOne[2];\n");
                c_outi("static const unsigned int tblLSB[%d];\n", prec+1);
                c_outi("static const char tblNumBits[%d];\n", 1<<(prec-1));
                c_outi("static const char tblF[%d];\n\n", 1<<(prec-1));
            }
            c_outi("static const unsigned int PREC_BITS[33];\n");

            if (co == CO_BS || r_min > qtr_plus_2) {
                c_outi("static const unsigned int BLEN; // length of the output buffer (BU)\n");
                c_outi("static const unsigned int BU_ONES; // the buffer BU with all 1-bits\n");
                if ((co == CO_BS && bs_n1+bs_ns+prec >= 32) || (co == CO_FO && prec > 27)) {
                    c_outi("static const unsigned int BS_ONE; // 1 << (precision+spacer bits)\n"); 
                    c_outi("static const unsigned int BS_HALF;\n");
                }
            }
            c_outi("static const unsigned int HALF;\n");                    
            c_outi("static const unsigned int QTR;\n\n");

            c_outi("static unsigned int %sT; // total bits read/written\n", prefix);
            c_outi("static unsigned int %sL; // current interval lower bound\n", prefix);
            c_outi("static unsigned int %sR; // current interval length\n", prefix);
        }
        if (gen_get) {
            c_outi("static unsigned int %sV; // current code value\n", prefix);
        }
        if (gen_put) {
            c_out("\n");
            c_outi("// variables needed to take care of carry-overs\n");
            if (co == CO_FO && r_min <= qtr_plus_2)  
                c_outi("static int %sF; // follow-bits counter\n", prefix);
            else {
                if (co == CO_FO)
                    c_outi("static int %sSC; // stack counter; counts the number of BU_ONES held back\n", prefix);
                c_outi("static int %sCT; // shift counter; if 0 --> output the next BLEN bits\n", prefix);
                c_outi("static int %sBU; // output BLEN bits are first buffered just in case a carry occurs\n", prefix);
                c_outi("static int %sCBU; // currently working BLEN bits\n", prefix);
                c_outi("static int %sFirst; // the very first %sBU data is never output\n", prefix, prefix);
            }

            /* we may need to skip some bits in the beginning */
            if (prec > v_init) {
                c_out("\n");
                c_outi("static int %sSkip; // bits that are omitted in the beginning of encoding\n\n", prefix); 
            }
        }

        /* the next_istate method can be called right after the ad_code()/ae_code() method to obtain the next state */
        if (ooc == OOC_FBAC) {
            c_out("\n");
            c_outi("// the next_istate method can be called right after the ad_code()/ae_code() method\n");
            c_outi("static int %siNext; // the next prob. state\n", prefix);
            c_outi("public: static int next_istate() { return %siNext; };\n\n", prefix);
            c_outi("private:\n");
        }

        if (end_bits == 0) {
            c_out("\n");
            if (gen_get) {
                c_outi("// read enough bits for the last symbol\n"); 
                c_outi("static int read_end_bits(%s &%sbs);\n", bitstream_class, prefix);
            }
            if (gen_put) {
                c_outi("// write enough bits for the last symbol\n"); 
                c_outi("static int write_end_bits(%s &%sbs);\n", bitstream_class, prefix);
            }
        }
   
        if (gen_get) {
            c_out("\n");
            c_outi("// read next n bits\n"); 
            c_outi("static int nextbits(%s &%sbs, int n) {\n", bitstream_class, prefix);
            c_identlevel++;
            c_outi("%sbs.skipbits(n);\n", prefix);
            if (co == CO_BS && bs_n1 > v_init) 
                c_outi("return (%sbs.nextbits(%d) & PREC_BITS[n]);\n", prefix, bs_n1);
            else
                c_outi("return (%sbs.nextbits(%d) & PREC_BITS[n]);\n", prefix, v_init);

            c_identlevel--;
            c_outi("};\n");

            c_out("\n");
            c_outi("// methods for decoding\n");
            c_outi("public: static int ad_init(%s &%sbs);\n", bitstream_class, prefix);
            c_outi("public: static int ad_end(%s &%sbs);\n", bitstream_class, prefix);
       
            if (ooc < OOC_FBAC) {   /* multiplicative operations are needed */
                c_outi("public: static int ad_code(%s &%sbs, unsigned int c_0, unsigned int c_1);\n", bitstream_class, prefix);
                if (soc == SOC_LM || soc == SOC_ML) 
                    c_outi("public: static int ad_code(%s &%sbs, unsigned int c_lps, unsigned int c_mps, int mps);\n", bitstream_class, prefix);
            }
            else {                  /* fast arithmetic coding using table lookups */
                if (soc == SOC_LM || soc == SOC_ML) 
                    c_outi("public: static int ad_code(%s &%sbs, int st, int mps);\n", bitstream_class, prefix);
                else
                    c_outi("public: static int ad_code(%s &%sbs, int st);\n", bitstream_class, prefix);
            }
        }
   
        if (gen_put) {
            c_out("\n");
            c_outi("// methods for encoding\n");
            c_outi("public: static int ae_init(%s &%sbs);\n", bitstream_class, prefix);
            c_outi("public: static int ae_end(%s &%sbs);\n", bitstream_class, prefix);

            if (ooc < OOC_FBAC) {   /* multiplicative operations are needed */
                c_outi("public: static int ae_code(%s &%sbs, int x, unsigned int c_0, unsigned int c_1);\n", bitstream_class, prefix);
                if (soc == SOC_LM || soc == SOC_ML) 
                    c_outi("public: static int ae_code(%s &%sbs, int x, unsigned int c_lps, unsigned int c_mps, int mps);\n", bitstream_class, prefix);
            }
            else {                  /* fast arithmetic coding using table lookups */
                if (soc == SOC_LM || soc == SOC_ML)
                    c_outi("public: static int ae_code(%s &%sbs, int x, int st, int mps);\n", bitstream_class, prefix);
                else
                    c_outi("public: static int ae_code(%s &%sbs, int x, int st);\n", bitstream_class, prefix); 
            }
        }
        /* putxml() */
        if (gen_putxml) {
            fatal("Error: The current translator cannot generate XML document gneration code BAC descriptions");

            /*
            c_out("\n");
            c_outi("// methods for xml doc generation\n");
            c_outi("public: static int putxml(%s &%sbs, short int bAttr);\n", bitstream_class, prefix);
            */
        }

        c_identlevel = 0;

        /* switch to .cpp file */
        tmp = ofp;
        ofp = ofp2;
 
        /* the static data members must be initialized at the file scope */
        if (gen_get || gen_put) {
            c_outi("\n");

            if (gen_get && r_type == RENORM_QUASI) {
                quasi_table_gen(sp);
            }

            c_outi("const unsigned int %s::PREC_BITS[33] = {0,0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F,0xFF,0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF,0xFFFF,0x1FFFF,0x3FFFF,0x7FFFF,0xFFFFF,0x1FFFFF,0x3FFFFF,0x7FFFFF,0xFFFFFF,0x1FFFFFF,0x3FFFFFF,0x7FFFFFF,0xFFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF};\n\n", sp->name);

            if (co == CO_FO && r_min > qtr_plus_2) {
                if (prec <= 27) {
                    c_outi("const unsigned int %s::BLEN    = %d;\n", sp->name, 31-prec);
                    c_outi("const unsigned int %s::BU_ONES = 0x%X;\n", sp->name, PREC_BITS[31-prec]);
                }
                else {
                    c_outi("const unsigned int %s::BLEN    = %d;\n", sp->name, 8);
                    c_outi("const unsigned int %s::BU_ONES = 0x%X;\n", sp->name, PREC_BITS[8]);
                    c_outi("const unsigned int %s::BS_ONE  = 0x%X;\n", sp->name, 1<<prec);
                    c_outi("const unsigned int %s::BS_HALF = 0x%X;\n", sp->name, 1<<(prec-1));
                }
            }
            else if (co == CO_BS) {
                c_outi("const unsigned int %s::BLEN    = %d;\n", sp->name, bs_n1);
                c_outi("const unsigned int %s::BU_ONES = 0x%X;\n", sp->name, PREC_BITS[bs_n1]);
                if (bs_n1+bs_ns+prec >= 32) {
                    c_outi("const unsigned int %s::BS_ONE   = 0x%X;\n", sp->name, 1<<(bs_ns+prec));
                    c_outi("const unsigned int %s::BS_HALF  = 0x%X;\n", sp->name, 1<<(bs_ns+prec-1));
                }
            }
            c_outi("const unsigned int %s::HALF     = 0x%X;\n", sp->name, 1<<(prec-1));
            c_outi("const unsigned int %s::QTR      = 0x%X;\n\n", sp->name, 1<<(prec-2));

            c_outi("unsigned int %s::%sT = 0;\n", sp->name, prefix);
            c_outi("unsigned int %s::%sL = 0;\n", sp->name, prefix);
            c_outi("unsigned int %s::%sR = 0;\n", sp->name, prefix);
        }
        if (gen_get) {
            c_outi("unsigned int %s::%sV = 0;\n", sp->name, prefix);
        }
        if (gen_put) {
            c_out("\n");
            if (co == CO_FO && r_min <= qtr_plus_2)
                c_outi("int %s::%sF = 0;\n", sp->name, prefix);
            else { 
                if (co == CO_FO)
                    c_outi("int %s::%sSC = 0;\n", sp->name, prefix);
                c_outi("int %s::%sCT = 0;\n", sp->name, prefix);
                c_outi("int %s::%sBU = 0;\n", sp->name, prefix);
                c_outi("int %s::%sCBU = 0;\n", sp->name, prefix);
                c_outi("int %s::%sFirst = 0;\n", sp->name, prefix);
            }

            if (prec > v_init) {
                c_out("\n");
                c_outi("int %s::%sSkip = 0;\n", sp->name, prefix); 
            }
        }

        if (ooc == OOC_FBAC)
            c_outi("int %s::%siNext = 0;\n", sp->name, prefix);
        
        /* generate helper methods that reads/writes bits for disambiguating the last symbol */
        if (end_bits == 0) {
            if (gen_get)
                c_in_end_bits(sp);
            if (gen_put)
                c_out_end_bits(sp);
        }

        /* generate ad_init(), ad_end(), and ad_code() methods */
        if (gen_get) {
            c_outi("\n");
            c_outi("//////////////\n");
            c_outi("// Decoding //\n");
            c_outi("//////////////\n");
            c_bac_get(sp);
        }
        /* generate ae_init(), ae_end(), and ae_code() methods */
        if (gen_put) {
            c_outi("\n");
            c_outi("//////////////\n");
            c_outi("// Encoding //\n");
            c_outi("//////////////\n");
            c_bac_put(sp);
        }
        /* generate putxml function */
        if (gen_putxml) {
            fatal("Error: The current translator cannot generate XML document gneration code BAC descriptions");
            /*
            c_outi("\n");
            c_outi("///////////////////////\n");
            c_outi("// Converting to XML //\n");
            c_outi("///////////////////////\n");
            c_bac_putxml(sp);
            */
        }
    }

    else if (java()) {
        if (gen_get || gen_put) {
            c_outi("// constants\n");

            if (gen_get && r_type == RENORM_QUASI) {
                c_outi("// tables for semi-quasi-renormalization\n");
                quasi_table_gen(sp);
            }

            c_outi("static final int PREC_BITS[] = {0,0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F,0xFF,0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF,0xFFFF,0x1FFFF,0x3FFFF,0x7FFFF,0xFFFFF,0x1FFFFF,0x3FFFFF,0x7FFFFF,0xFFFFFF,0x1FFFFFF,0x3FFFFFF,0x7FFFFFF,0xFFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF};\n\n");

            if (co == CO_FO && r_min > qtr_plus_2) {
                if (prec <= 27) {
                    c_outi("static final int BLEN     = %d;\n", 31-prec);
                    c_outi("static final int BU_ONES  = 0x%X;\n", (1<<(31-prec))-1);
                }
                else {
                    c_outi("static final int BLEN     = %d;\n", 8);
                    c_outi("static final int BU_ONES  = 0x%X;\n", PREC_BITS[8]);
                    c_outi("static final long BS_ONE  = 0x%XL;\n", 1<<prec);
                    c_outi("static final long BS_HALF = 0x%XL;\n", 1<<(prec-1));
                }
            }
            else if (co == CO_BS) {
                c_outi("static final int BLEN     = %d;\n", bs_n1);
                c_outi("static final int BU_ONES  = 0x%X;\n", PREC_BITS[bs_n1]);
                if (bs_n1+bs_ns+prec >= 32) {
                    c_outi("static final long BS_ONE  = 0x%XL;\n", 1<<(bs_ns+prec));
                    c_outi("static final long BS_HALF = 0x%XL;\n", 1<<(bs_ns+prec-1));
                }
            }
            c_outi("static final long HALF    = 0x%XL;\n", 1<<(prec-1));
            c_outi("static final long QTR     = 0x%XL;\n\n", 1<<(prec-2));

            c_outi("private static long %sT; // total bits read/written\n", prefix);
            c_outi("private static long %sL; // current interval lower bound\n", prefix);
            c_outi("private static long %sR; // current interval length\n", prefix);
        }
        if (gen_get) {
            c_outi("private static long %sV; // current code value\n", prefix);
        }
        if (gen_put) { 
            c_out("\n");
            c_outi("// variables needed to take care of carry-overs\n");
            if (co == CO_FO && r_min <= qtr_plus_2) 
                c_outi("private static int %sF; // follow-bits counter\n", prefix);
            else { 
                if (co == CO_FO)
                    c_outi("private static int %sSC; // stack counter; counts the number of BU_ONES held back\n", prefix);
                c_outi("private static int %sCT; // shift counter; if 0 --> output the next BLEN bits\n", prefix);
                c_outi("private static int %sBU; // output BLEN bits are first buffered just in case a carry occurs\n", prefix);
                c_outi("private static int %sCBU; // currently working BLEN bits\n", prefix);
                c_outi("private static int %sFirst; // the very first %sBU data is never output\n", prefix, prefix);
            }

            /* we may need to skip some bits in the beginning */
            if (prec > v_init) {
                c_out("\n");
                c_outi("private static int %sSkip; // bits that are omitted in the beginning of encoding\n", prefix); 
            }
        }

        /* the next_istate method can be called right after the ad_code()/ae_code() method to obtain the next state */
        if (ooc == OOC_FBAC) {
            c_out("\n");
            c_outi("// the next_istate method can be called right after the ad_code()/ae_code() method\n");
            c_outi("private static int %siNext; // the next state\n", prefix);
            c_outi("public static int next_istate() { return %siNext; };\n\n", prefix);
        }
    
        /* generate helper methods that reads/writes bits for disambiguating the last symbol */
        if (end_bits == 0) {
            c_outi("\n");
            if (gen_get)
                c_in_end_bits(sp);
            if (gen_put)
                c_out_end_bits(sp);
        }
 
        /* generate ad_init(), ad_end(), and ad_code() methods */
        if (gen_get) {
            c_outi("\n");
            c_outi("// read next n bits\n"); 
            c_outi("private static int nextbits(%s %sbs, int n) throws IOException {\n", bitstream_class, prefix);
            c_identlevel++;
            c_outi("%sbs.skipbits(n);\n", prefix);
            if (co == CO_BS && bs_n1 > v_init) 
                c_outi("return (%sbs.nextbits(%d) & PREC_BITS[n]);\n", prefix, bs_n1);
            else
                c_outi("return (%sbs.nextbits(%d) & PREC_BITS[n]);\n", prefix, v_init);
            
            c_identlevel--;
            c_outi("}\n");

            c_outi("\n");
            c_outi("//////////////\n");
            c_outi("// Decoding //\n");
            c_outi("//////////////\n");
            c_bac_get(sp);
        }
        /* generate ae_init(), ae_end(), and ae_code() functions */
        if (gen_put) {
            c_outi("\n");
            c_outi("//////////////\n");
            c_outi("// Encoding //\n");
            c_outi("//////////////\n");
            c_bac_put(sp);
        }
        /* generate putxml function */
        if (gen_putxml) {
            fatal("Error: The current translator cannot generate XML document gneration code BAC descriptions");
            /*
            c_outi("\n");
            c_outi("///////////////////////\n");
            c_outi("// Converting to XML //\n");
            c_outi("///////////////////////\n");
            c_bac_putxml(sp);
            */
        }
    }
    else if (xsd()) {
        fatal("Error: The current translator cannot generate XML schemas for BAC descriptions");
        /*
        c_bac_xsd(sp);
        */
    }

    /* switch back to .h file */
    if (cpp()) {
        c_identlevel = 1; /* that's where we left off */
        ofp = tmp;
    } 

    /* no trailing ';' in class definitions for Java */
    c_identlevel--;

    if (cpp())
        c_out("};\n");
    else if (java()) 
        c_out("}\n");
    else if (xsd()) {
        fatal("Error: The current translator cannot generate XML schemas for BAC descriptions");
        /*
        c_outi("</xsd:sequence>\n");
        c_identlevel--;
        c_out("</xsd:complexType>\n");
        */
    }
}

