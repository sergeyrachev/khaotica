/* lex -l -P etc\yylex.c -o lex.yy.c D:\flavor\src\flavorc\lexer.l */
#define YYNEWLINE 10
#define INITIAL 0
#define yy_endst 177
#define yy_nxtmax 895
#define YY_LA_SIZE 23

static unsigned short yy_la_act[] = {
 49, 70, 49, 70, 18, 70, 18, 70, 49, 70, 49, 70, 49, 70, 49, 70,
 49, 70, 49, 70, 49, 70, 49, 70, 49, 70, 49, 70, 49, 70, 49, 70,
 48, 70, 48, 70, 49, 70, 50, 70, 51, 70, 70, 70, 68, 69, 70, 70,
 61, 53, 61, 67, 66, 65, 67, 65, 64, 67, 63, 67, 60, 67, 59, 67,
 58, 67, 57, 67, 56, 67, 55, 67, 54, 67, 62, 67, 53, 52, 48, 42,
 44, 42, 43, 42, 42, 43, 47, 48, 46, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 47, 48, 40, 32, 30, 31, 43, 43, 29, 28, 41, 27, 39,
 26, 25, 22, 24, 38, 21, 23, 37, 20, 31, 36, 31, 19, 35, 18, 18,
 18, 2, 34, 17, 16, 11, 6, 7, 5, 14, 10, 4, 15, 12, 8, 3,
 13, 9, 0, 1, 33, 0
};

static unsigned char yy_look[] = {
 0
};

static short yy_final[] = {
 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
 30, 32, 34, 36, 38, 40, 42, 43, 44, 45, 47, 48, 48, 49, 49, 51,
 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 52, 52, 53, 53,
 55, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 77, 77, 77, 77,
 78, 79, 80, 80, 80, 81, 83, 83, 83, 84, 84, 84, 86, 88, 88, 88,
 88, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 97, 98, 99, 101, 102,
 103, 104, 105, 106, 106, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 130, 130,
 130, 130, 130, 131, 131, 131, 131, 131, 131, 132, 132, 132, 132, 132, 133, 133,
 133, 134, 135, 135, 135, 135, 135, 136, 137, 137, 137, 137, 138, 139, 140, 140,
 140, 140, 140, 140, 141, 141, 141, 142, 143, 144, 144, 144, 144, 145, 146, 147,
 148, 149
};
#ifndef yy_state_t
#define yy_state_t unsigned char
#endif

static yy_state_t yy_begin[] = {
 0, 0, 0
};

static yy_state_t yy_next[] = {
 26, 26, 26, 26, 26, 26, 26, 26, 26, 25, 24, 25, 25, 26, 26, 26,
 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
 25, 10, 22, 26, 26, 2, 11, 23, 19, 19, 15, 5, 19, 6, 14, 1,
 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 13, 19, 7, 9, 8, 19,
 26, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 19, 26, 19, 16, 3,
 26, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 20, 12, 21, 19, 26,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 43, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 29, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 45, 42, 42, 42, 42, 42,
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
 42, 42, 38, 42, 42, 42, 42, 30, 42, 42, 42, 42, 42, 42, 42, 42,
 40, 40, 40, 40, 40, 40, 40, 40, 42, 42, 42, 42, 42, 42, 42, 42,
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 39, 42, 42, 42,
 42, 31, 32, 42, 42, 42, 33, 42, 42, 42, 42, 42, 42, 42, 34, 42,
 42, 42, 35, 42, 36, 42, 37, 42, 41, 42, 42, 42, 42, 42, 42, 42,
 43, 48, 49, 50, 51, 52, 53, 54, 55, 44, 44, 44, 44, 44, 44, 44,
 44, 44, 44, 56, 57, 58, 59, 47, 82, 83, 44, 44, 44, 44, 44, 44,
 46, 46, 46, 46, 46, 46, 46, 46, 67, 177, 67, 177, 177, 68, 68, 68,
 68, 68, 68, 68, 68, 68, 68, 177, 177, 177, 44, 44, 44, 44, 44, 44,
 62, 63, 62, 62, 62, 62, 62, 84, 85, 62, 62, 62, 94, 62, 62, 61,
 61, 61, 61, 61, 61, 61, 61, 61, 61, 62, 62, 177, 62, 177, 95, 62,
 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 62, 62, 62, 62, 62, 102,
 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 86, 87, 65, 62, 64, 64,
 64, 64, 64, 64, 64, 64, 64, 64, 69, 69, 69, 69, 69, 69, 69, 69,
 69, 69, 66, 66, 177, 107, 177, 73, 73, 71, 104, 71, 70, 70, 72, 72,
 72, 72, 72, 72, 72, 72, 72, 72, 177, 177, 177, 177, 88, 89, 90, 108,
 91, 92, 66, 66, 177, 111, 177, 73, 73, 74, 114, 74, 70, 70, 75, 75,
 75, 75, 75, 75, 75, 75, 75, 75, 76, 76, 76, 76, 76, 76, 76, 76,
 76, 76, 177, 105, 177, 110, 109, 112, 113, 103, 78, 115, 118, 119, 132, 131,
 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 106, 117, 116, 133, 134, 120,
 80, 81, 81, 81, 81, 81, 81, 135, 136, 137, 77, 93, 93, 93, 93, 93,
 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 138,
 79, 81, 81, 81, 81, 81, 81, 97, 177, 139, 140, 96, 177, 98, 98, 98,
 98, 98, 98, 98, 98, 98, 98, 141, 144, 142, 147, 148, 149, 150, 155, 100,
 143, 100, 99, 99, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 156, 160,
 161, 162, 146, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 152, 163, 158,
 166, 167, 99, 99, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 169, 172,
 173, 177, 123, 177, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121,
 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 127, 145,
 153, 164, 128, 159, 170, 177, 177, 154, 165, 174, 151, 171, 157, 177, 175, 177,
 177, 130, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 176, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 168, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 125, 177, 129, 177, 177,
 177, 177, 177, 177, 126, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 124,
 0
};

static yy_state_t yy_check[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 42, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 44, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
 41, 46, 39, 38, 37, 36, 35, 34, 33, 41, 41, 41, 41, 41, 41, 41,
 41, 41, 41, 32, 31, 30, 28, 40, 78, 78, 41, 41, 41, 41, 41, 41,
 40, 40, 40, 40, 40, 40, 40, 40, 66, 68, 66, 68, 92, 66, 66, 66,
 66, 66, 66, 66, 66, 66, 66, 67, 91, 67, 41, 41, 41, 41, 41, 41,
 22, 22, 22, 22, 22, 22, 22, 83, 83, 22, 22, 22, 16, 22, 22, 22,
 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 72, 22, 72, 15, 22,
 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 13,
 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 85, 85, 18, 22, 18, 18,
 18, 18, 18, 18, 18, 18, 18, 18, 65, 65, 65, 65, 65, 65, 65, 65,
 65, 65, 18, 18, 71, 10, 71, 69, 69, 70, 12, 70, 65, 65, 70, 70,
 70, 70, 70, 70, 70, 70, 70, 70, 75, 74, 75, 74, 87, 87, 89, 9,
 89, 89, 18, 18, 101, 109, 101, 69, 69, 73, 112, 73, 65, 65, 73, 73,
 73, 73, 73, 73, 73, 73, 73, 73, 17, 17, 17, 17, 17, 17, 17, 17,
 17, 17, 100, 11, 100, 8, 8, 7, 7, 12, 17, 6, 116, 5, 129, 129,
 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 11, 6, 6, 132, 133, 5,
 17, 80, 80, 80, 80, 80, 80, 134, 135, 131, 17, 76, 76, 76, 76, 76,
 76, 76, 76, 76, 76, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 137,
 17, 80, 80, 80, 80, 80, 80, 14, 98, 138, 139, 14, 98, 14, 14, 14,
 14, 14, 14, 14, 14, 14, 14, 140, 143, 128, 146, 147, 148, 149, 154, 99,
 128, 99, 98, 98, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 153, 159,
 160, 161, 142, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 127, 162, 126,
 165, 164, 98, 98, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 125, 171,
 170, ~0, 4, ~0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 142,
 152, 158, 2, 126, 169, ~0, ~0, 152, 158, 1, 127, 169, 126, ~0, 1, ~0,
 ~0, 2, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, 1, ~0, ~0, ~0,
 ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, 125, ~0, ~0, ~0, ~0,
 ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, 2, ~0, 2, ~0, ~0,
 ~0, ~0, ~0, ~0, 2, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, ~0, 2,
 0
};

static yy_state_t yy_default[] = {
 177, 177, 177, 4, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
 177, 18, 177, 177, 177, 177, 177, 177, 177, 177, 177, 28, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 41, 177, 40, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 22, 22, 22, 177,
 18, 177, 177, 66, 66, 65, 177, 70, 70, 177, 73, 73, 18, 78, 177, 80,
 177, 80, 83, 177, 85, 177, 87, 177, 89, 177, 78, 89, 89, 18, 177, 177,
 177, 177, 14, 177, 99, 99, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 4, 4, 4, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177, 177,
 177, 0
};

static short yy_base[] = {
 0, 783, 772, 896, 691, 594, 590, 571, 568, 530, 504, 589, 509, 453, 653, 417,
 399, 568, 494, 896, 896, 896, 415, 128, 896, 896, 896, 896, 367, 256, 366, 365,
 364, 353, 352, 351, 350, 349, 348, 347, 368, 345, 99, 896, 227, 896, 346, 896,
 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
 896, 504, 381, 396, 382, 499, 526, 521, 432, 558, 542, 541, 619, 896, 360, 896,
 592, 896, 896, 407, 896, 490, 896, 540, 896, 544, 896, 394, 382, 629, 896, 896,
 896, 896, 654, 676, 583, 553, 896, 896, 896, 896, 896, 896, 896, 536, 896, 896,
 541, 896, 896, 896, 594, 896, 896, 896, 896, 896, 896, 896, 896, 736, 705, 703,
 614, 529, 896, 566, 541, 543, 549, 548, 896, 579, 580, 598, 610, 896, 692, 589,
 896, 896, 615, 603, 604, 594, 896, 896, 717, 611, 595, 896, 896, 896, 718, 638,
 633, 628, 653, 896, 630, 629, 896, 896, 896, 721, 661, 660, 896, 896, 896, 896,
 896, 896
};


/*
 * Copyright 1988, 1992 by Mortice Kern Systems Inc.  All rights reserved.
 * All rights reserved.
 *
 * $Id: YYLEX.C,v 1.2 2001/03/10 21:46:07 yuntai Exp $
 *
 */
#include <stdlib.h>
#include <stdio.h>
#if	__STDC__
#define YY_ARGS(args)	args
#else
#define YY_ARGS(args)	()
#endif

#ifdef LEX_WINDOWS
#include <windows.h>

/*
 * define, if not already defined
 * the flag YYEXIT, which will allow
 * graceful exits from yylex()
 * without resorting to calling exit();
 */

#ifndef YYEXIT
#define YYEXIT	1
#endif

/*
 * the following is the handle to the current
 * instance of a windows program. The user
 * program calling yylex must supply this!
 */

extern HANDLE hInst;	

#endif	/* LEX_WINDOWS */

/*
 * Define m_textmsg() to an appropriate function for internationalized messages
 * or custom processing.
 */
#ifndef I18N
#define	m_textmsg(id, str, cls)	(str)
#else /*I18N*/
extern	char* m_textmsg YY_ARGS((int id, const char* str, char* cls));
#endif/*I18N*/

/*
 * Include string.h to get definition of memmove() and size_t.
 * If you do not have string.h or it does not declare memmove
 * or size_t, you will have to declare them here.
 */
#include <string.h>
/* Uncomment next line if memmove() is not declared in string.h */
/*extern char * memmove();*/
/* Uncomment next line if size_t is not available in stdio.h or string.h */
/*typedef unsigned size_t;*/
/* Drop this when LATTICE provides memmove */
#ifdef LATTICE
#define memmove	memcopy
#endif

/*
 * YY_STATIC determines the scope of variables and functions
 * declared by the lex scanner. It must be set with a -DYY_STATIC
 * option to the compiler (it cannot be defined in the lex program).
 */
#ifdef	YY_STATIC
/* define all variables as static to allow more than one lex scanner */
#define	YY_DECL	static
#else
/* define all variables as global to allow other modules to access them */
#define	YY_DECL	
#endif

/*
 * You can redefine yygetc. For YACC Tracing, compile this code
 * with -DYYTRACE to get input from yt_getc
 */
#ifdef YYTRACE
extern int	yt_getc YY_ARGS((void));
#define yygetc()	yt_getc()
#else
#define	yygetc()	getc(yyin) 	/* yylex input source */
#endif

/*
 * the following can be redefined by the user.
 */
#ifdef YYEXIT
#define	YY_FATAL(msg)	{ fprintf(yyout, "yylex: %s\n", msg); yyLexFatal = 1; }
#else /* YYEXIT */
#define	YY_FATAL(msg)	{ fprintf(stderr, "yylex: %s\n", msg); exit(1); }
#endif /* YYEXIT */

#define	ECHO		fputs(yytext, yyout)
#define	output(c)	putc((c), yyout) /* yylex sink for unmatched chars */
#define	YY_INTERACTIVE	1		/* save micro-seconds if 0 */
#define	YYLMAX		100		/* token and pushback buffer size */

/*
 * If %array is used (or defaulted), yytext[] contains the token.
 * If %pointer is used, yytext is a pointer to yy_tbuf[].
 */
YY_DECL char	yytext[YYLMAX+1];

#define	BEGIN		yy_start =
#define	REJECT		goto yy_reject
#define	NLSTATE		(yy_lastc = YYNEWLINE)
#define	YY_INIT \
	(yy_start = yyleng = yy_end = 0, yy_lastc = YYNEWLINE)
#define	yymore()	goto yy_more
#define	yyless(n)	if ((n) < 0 || (n) > yy_end) ; \
			else { YY_SCANNER; yyleng = (n); YY_USER; }

YY_DECL	void	yy_reset YY_ARGS((void));
YY_DECL	int	input	YY_ARGS((void));
YY_DECL	int	unput	YY_ARGS((int c));

/* functions defined in libl.lib */
extern	int	yywrap	YY_ARGS((void));
extern	void	yyerror	YY_ARGS((char *fmt, ...));
extern	void	yycomment	YY_ARGS((char *term));
extern	int	yymapch	YY_ARGS((int delim, int escape));

 

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

/*
 * Lexical analyzer for the Flavor translator
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "port.h"
#include "globals.h"
#include "symbol.h"
#include "ptree.h"
#include "util.h"
#include "error.h"
#include "compile.h"

#include "y.tab.h"

/* symbol lookup */
static int getsymbol(void);

/* error reporting */
void yyerror(char *fmt, ...);

/* comment processing */
void yycomment(char *end);

/* transparent user-code processing */
verbatim_t *yyverbatim(char *end);

/* sometimes yywrap is defined as a macro */
#undef yywrap

/* disable 'unreferenced label' warning from Visual C++ */
#pragma warning(disable : 4102)

/* Set when processing pragma statements.
 * Suppresses regular symbol lookup -- see getsymbol() below.
 */
static int pragma_active = 0;

/* parser entry point */
int yyparse();




#ifdef	YY_DEBUG
#undef	YY_DEBUG
#define	YY_DEBUG(fmt, a1, a2)	fprintf(stderr, fmt, a1, a2)
#else
#define	YY_DEBUG(fmt, a1, a2)
#endif

/*
 * The declaration for the lex scanner can be changed by
 * redefining YYLEX or YYDECL. This must be done if you have
 * more than one scanner in a program.
 */
#ifndef	YYLEX
#define	YYLEX yylex			/* name of lex scanner */
#endif

#ifndef YYDECL
#define	YYDECL	int YYLEX YY_ARGS((void))	/* declaration for lex scanner */
#endif

/*
 * stdin and stdout may not neccessarily be constants.
 * If stdin and stdout are constant, and you want to save a few cycles, then
 * #define YY_STATIC_STDIO 1 in this file or on the commandline when
 * compiling this file
 */
#ifndef YY_STATIC_STDIO
#define YY_STATIC_STDIO	0
#endif

#if YY_STATIC_STDIO
YY_DECL	FILE   *yyin = stdin;
YY_DECL	FILE   *yyout = stdout;
#else
YY_DECL	FILE   *yyin = (FILE *)0;
YY_DECL	FILE   *yyout = (FILE *)0;
#endif
YY_DECL	int	yylineno = 1;		/* line number */

/* yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
 * yytext[0:yyleng-1] contains the current token.
 * yytext[yyleng:yy_end-1] contains pushed-back characters.
 * When the user action routine is active,
 * yy_save contains yytext[yyleng], which is set to '\0'.
 * Things are different when YY_PRESERVE is defined. 
 */
static	yy_state_t yy_sbuf [YYLMAX+1];	/* state buffer */
static	int	yy_end = 0;		/* end of pushback */
static	int	yy_start = 0;		/* start state */
static	int	yy_lastc = YYNEWLINE;	/* previous char */
YY_DECL	int	yyleng = 0;		/* yytext token length */
#ifdef YYEXIT
static	int yyLexFatal;
#endif /* YYEXIT */

#ifndef YY_PRESERVE	/* the efficient default push-back scheme */

static	char yy_save;	/* saved yytext[yyleng] */

#define	YY_USER	{ /* set up yytext for user */ \
		yy_save = yytext[yyleng]; \
		yytext[yyleng] = 0; \
	}
#define	YY_SCANNER { /* set up yytext for scanner */ \
		yytext[yyleng] = yy_save; \
	}

#else		/* not-so efficient push-back for yytext mungers */

static	char yy_save [YYLMAX];
static	char *yy_push = yy_save+YYLMAX;

#define	YY_USER { \
		size_t n = yy_end - yyleng; \
		yy_push = yy_save+YYLMAX - n; \
		if (n > 0) \
			memmove(yy_push, yytext+yyleng, n); \
		yytext[yyleng] = 0; \
	}
#define	YY_SCANNER { \
		size_t n = yy_save+YYLMAX - yy_push; \
		if (n > 0) \
			memmove(yytext+yyleng, yy_push, n); \
		yy_end = yyleng + n; \
	}

#endif


#ifdef LEX_WINDOWS

/*
 * When using the windows features of lex,
 * it is necessary to load in the resources being
 * used, and when done with them, the resources must
 * be freed up, otherwise we have a windows app that
 * is not following the rules. Thus, to make yylex()
 * behave in a windows environment, create a new
 * yylex() which will call the original yylex() as
 * another function call. Observe ...
 */

/*
 * The actual lex scanner (usually yylex(void)).
 * NOTE: you should invoke yy_init() if you are calling yylex()
 * with new input; otherwise old lookaside will get in your way
 * and yylex() will die horribly.
 */
static int win_yylex();			/* prototype for windows yylex handler */

YYDECL {
	int wReturnValue;
	HANDLE hRes_table;
	unsigned short *old_yy_la_act;	/* remember previous pointer values */
	short *old_yy_final;
	yy_state_t *old_yy_begin;
	yy_state_t *old_yy_next;
	yy_state_t *old_yy_check;
	yy_state_t *old_yy_default;
	short *old_yy_base;

	/*
	 * the following code will load the required
	 * resources for a Windows based parser.
	 */

	hRes_table = LoadResource (hInst,
		FindResource (hInst, "UD_RES_yyLEX", "yyLEXTBL"));
	
	/*
	 * return an error code if any
	 * of the resources did not load
	 */

	if (hRes_table == NULL)
		return (0);
	
	/*
	 * the following code will lock the resources
	 * into fixed memory locations for the scanner
	 * (and remember previous pointer locations)
	 */

	old_yy_la_act = yy_la_act;
	old_yy_final = yy_final;
	old_yy_begin = yy_begin;
	old_yy_next = yy_next;
	old_yy_check = yy_check;
	old_yy_default = yy_default;
	old_yy_base = yy_base;

	yy_la_act = (unsigned short *)LockResource (hRes_table);
	yy_final = (short *)(yy_la_act + sizeof(yy_la_act));
	yy_begin = (yy_state_t *)(yy_final + sizeof(yy_final));
	yy_next = (yy_state_t *)(yy_begin + sizeof(yy_begin));
	yy_check = (yy_state_t *)(yy_next + sizeof(yy_next));
	yy_default = (yy_state_t *)(yy_check + sizeof(yy_check));
	yy_base = (yy_state_t *)(yy_default + sizeof(yy_default));


	/*
	 * call the standard yylex() code
	 */

	wReturnValue = win_yylex();

	/*
	 * unlock the resources
	 */

	UnlockResource (hRes_table);

	/*
	 * and now free the resource
	 */

	FreeResource (hRes_table);

	/*
	 * restore previously saved pointers
	 */

	yy_la_act = old_yy_la_act;
	yy_final = old_yy_final;
	yy_begin = old_yy_begin;
	yy_next = old_yy_next;
	yy_check = old_yy_check;
	yy_default = old_yy_default;
	yy_base = old_yy_base;

	return (wReturnValue);
}	/* end function */

static int win_yylex() {

#else /* LEX_WINDOWS */

/*
 * The actual lex scanner (usually yylex(void)).
 * NOTE: you should invoke yy_init() if you are calling yylex()
 * with new input; otherwise old lookaside will get in your way
 * and yylex() will die horribly.
 */
YYDECL {

#endif /* LEX_WINDOWS */

	register int c, i, yybase;
	unsigned	yyst;	/* state */
	int yyfmin, yyfmax;	/* yy_la_act indices of final states */
	int yyoldi, yyoleng;	/* base i, yyleng before look-ahead */
	int yyeof;		/* 1 if eof has already been read */




#if !YY_STATIC_STDIO
	if (yyin == (FILE *)0)
		yyin = stdin;
	if (yyout == (FILE *)0)
		yyout = stdout;
#endif

#ifdef YYEXIT
	yyLexFatal = 0;
#endif /* YYEXIT */

	yyeof = 0;
	i = yyleng;
	YY_SCANNER;

  yy_again:
	yyleng = i;
	/* determine previous char. */
	if (i > 0)
		yy_lastc = yytext[i-1];
	/* scan previously accepted token adjusting yylineno */
	while (i > 0)
		if (yytext[--i] == YYNEWLINE)
			yylineno++;
	/* adjust pushback */
	yy_end -= yyleng;
	memmove(yytext, yytext+yyleng, (size_t) yy_end);
	i = 0;

  yy_contin:
	yyoldi = i;

	/* run the state machine until it jams */
	yyst = yy_begin[yy_start + (yy_lastc == YYNEWLINE)];
	yy_sbuf[i] = (yy_state_t) yyst;
	do {
		YY_DEBUG(m_textmsg(1547, "<state %d, i = %d>\n", "I num1 num2"), yyst, i);
		if (i >= YYLMAX) {
			YY_FATAL(m_textmsg(1548, "Token buffer overflow", "E"));
#ifdef YYEXIT
			if (yyLexFatal)
				return -2;
#endif /* YYEXIT */
		}	/* endif */

		/* get input char */
		if (i < yy_end)
			c = yytext[i];		/* get pushback char */
		else if (!yyeof && (c = yygetc()) != EOF) {
			yy_end = i+1;
			yytext[i] = (char) c;
		} else /* c == EOF */ {
			c = EOF;		/* just to make sure... */
			if (i == yyoldi) {	/* no token */
				yyeof = 0;
				if (yywrap())
					return 0;
				else
					goto yy_again;
			} else {
				yyeof = 1;	/* don't re-read EOF */
				break;
			}
		}
		YY_DEBUG(m_textmsg(1549, "<input %d = 0x%02x>\n", "I num hexnum"), c, c);

		/* look up next state */
		while ((yybase = yy_base[yyst]+(unsigned char)c) > yy_nxtmax
		    || yy_check[yybase] != (yy_state_t) yyst) {
			if (yyst == yy_endst)
				goto yy_jammed;
			yyst = yy_default[yyst];
		}
		yyst = yy_next[yybase];
	  yy_jammed: ;
	  yy_sbuf[++i] = (yy_state_t) yyst;
	} while (!(yyst == yy_endst || YY_INTERACTIVE && yy_base[yyst] > yy_nxtmax && yy_default[yyst] == yy_endst));
	YY_DEBUG(m_textmsg(1550, "<stopped %d, i = %d>\n", "I num1 num2"), yyst, i);
	if (yyst != yy_endst)
		++i;

  yy_search:
	/* search backward for a final state */
	while (--i > yyoldi) {
		yyst = yy_sbuf[i];
		if ((yyfmin = yy_final[yyst]) < (yyfmax = yy_final[yyst+1]))
			goto yy_found;	/* found final state(s) */
	}
	/* no match, default action */
	i = yyoldi + 1;
	output(yytext[yyoldi]);
	goto yy_again;

  yy_found:
	YY_DEBUG(m_textmsg(1551, "<final state %d, i = %d>\n", "I num1 num2"), yyst, i);
	yyoleng = i;		/* save length for REJECT */
	
	/* pushback look-ahead RHS */
	if ((c = (int)(yy_la_act[yyfmin]>>9) - 1) >= 0) { /* trailing context? */
		unsigned char *bv = yy_look + c*YY_LA_SIZE;
		static unsigned char bits [8] = {
			1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7
		};
		while (1) {
			if (--i < yyoldi) {	/* no / */
				i = yyoleng;
				break;
			}
			yyst = yy_sbuf[i];
			if (bv[(unsigned)yyst/8] & bits[(unsigned)yyst%8])
				break;
		}
	}

	/* perform action */
	yyleng = i;
	YY_USER;
	switch (yy_la_act[yyfmin] & 0777) {
	case 0:
	yycomment("*/");
	break;
	case 1:
	yycomment("\n");
	break;
	case 2:
	yylval.vp=yyverbatim("%}"); return VERBATIM;
	break;
	case 3:
	yylval.vp=yyverbatim("%g}"); return VERBATIM_GET;
	break;
	case 4:
	yylval.vp=yyverbatim("%p}"); return VERBATIM_PUT;
	break;
	case 5:
	yylval.vp=yyverbatim("%*}"); return VERBATIM_BOTH;
	break;
	case 6:
	yylval.vp=yyverbatim("%.c}"); return VERBATIM_C;
	break;
	case 7:
	yylval.vp=yyverbatim("%.c.cpp}"); return VERBATIM_C_CPP;
	break;
	case 8:
	yylval.vp=yyverbatim("%p.c}"); return VERBATIM_PUT_C;
	break;
	case 9:
	yylval.vp=yyverbatim("%g.c}"); return VERBATIM_GET_C;
	break;
	case 10:
	yylval.vp=yyverbatim("%*.c}"); return VERBATIM_BOTH_C;
	break;
	case 11:
	yylval.vp=yyverbatim("%.j}"); return VERBATIM_J;
	break;
	case 12:
	yylval.vp=yyverbatim("%p.j}"); return VERBATIM_PUT_J;
	break;
	case 13:
	yylval.vp=yyverbatim("%g.j}"); return VERBATIM_GET_J;
	break;
	case 14:
	yylval.vp=yyverbatim("%*.j}"); return VERBATIM_BOTH_J;
	break;
	case 15:
	pragma_active=1; return yylval.ival=PRAGMA;
	break;
	case 16:
	return yylval.ival=INCLUDE;
	break;
	case 17:
	return yylval.ival=IMPORT;
	break;
	case 18:
	{
    int c = getsymbol();
    if (c) return c;
}
	break;
	case 19:
	return yylval.ival=INC;
	break;
	case 20:
	return yylval.ival=DEC;
	break;
	case 21:
	return yylval.ival=LS;
	break;
	case 22:
	return yylval.ival=RS;
	break;
	case 23:
	return yylval.ival=LE;
	break;
	case 24:
	return yylval.ival=GE;
	break;
	case 25:
	return yylval.ival=EQ;
	break;
	case 26:
	return yylval.ival=NE;
	break;
	case 27:
	return yylval.ival=AND;
	break;
	case 28:
	return yylval.ival=OR;
	break;
	case 29:
	return yylval.ival=CLCL;
	break;
	case 30:
	return yylval.ival=RANGE;
	break;
	case 31:
	{
    yyerror("C++ operator '%s' not supported in Flavor", yytext);
}
	break;
	case 32:
	return yylval.ival=MUL_ASSIGN;
	break;
	case 33:
	return yylval.ival=DIV_ASSIGN;
	break;
	case 34:
	return yylval.ival=MOD_ASSIGN;
	break;
	case 35:
	return yylval.ival=ADD_ASSIGN;
	break;
	case 36:
	return yylval.ival=SUB_ASSIGN;
	break;
	case 37:
	return yylval.ival=LS_ASSIGN;
	break;
	case 38:
	return yylval.ival=RS_ASSIGN;
	break;
	case 39:
	return yylval.ival=AND_ASSIGN;
	break;
	case 40:
	return yylval.ival=XOR_ASSIGN;
	break;
	case 41:
	return yylval.ival=OR_ASSIGN;
	break;
	case 42:
	case 43:
	case 44:
	{
    /* double number */
    char *stop;
    yylval.dval = strtod(yytext, &stop);
    if (strlen(stop) > 0) yyerror("Incorrect double number");
    return DOUBLE_LITERAL;
}
	break;
	case 45:
	{
    /* bitstring */ 
    int i;
    unsigned int shift;
    
    yylval.bit = (bit_t *)getmem(sizeof(bit_t));

    /* compute value */
    yylval.bit->len = 0;
    yylval.bit->val = 0;
    shift = 1;
    for (i=yyleng-1; i>=2; i--) {
        if (yytext[i] == '.') continue;
        if (yytext[i] == '1') yylval.bit->val|=shift;
        shift = shift<<1;
        /* we only accept up to 32 bits */
        if (yylval.bit->len == 32) {
            yyerror("Bitstring value longer than 32 bits");
            return BIT_LITERAL;
        } 
        else yylval.bit->len++;
    }
    return BIT_LITERAL;
}
	break;
	case 46:
	{
    /* hexadecimal - treated as bitstring */

    yylval.bit = (bit_t *)getmem(sizeof(bit_t));

    /* we only accept up to 32 bits */
    if (yyleng-2 > 8) {
        yyerror("Hexadecimal value longer than 32 bits");
        yylval.bit->len = 32;
        yylval.bit->val = strtol(yytext+yyleng-8, (char **)NULL, 16);
    }
    else {
        /* every hex digit is taken as 4 bits */
        yylval.bit->len = (yyleng-2)*4;
        yylval.bit->val = strtoul(yytext+2, (char **)NULL, 16);
    }
    return BIT_LITERAL;
}
	break;
	case 47:
	{
    /* octal number - treated as bitstring */

    yylval.bit = (bit_t *)getmem(sizeof(bit_t));

    /* we only accept up to 32 bits */
    if (yyleng-2 > 10) {
        yyerror("Octal value longer than 32 bits");
        yylval.bit->len = 30;
        yylval.bit->val = strtol(yytext+yyleng-10, (char **)NULL, 8);
    }
    else {
        /* every octal digit is taken as 3 bits */
        yylval.bit->len = (yyleng-2)*3;
        yylval.bit->val = strtol(yytext+1, (char **)NULL, 8);
    }
    return BIT_LITERAL;
}
	break;
	case 48:
	{
    /* int number */
    char *stop;

    yylval.ival = strtol(yytext, &stop, 10);
    if (strlen(stop) > 0) yyerror("Incorrect integer number");
    return INT_LITERAL;
}
	break;
	case 49:
	return yylval.ival = *yytext;
	break;
	case 50:
	s_newcontext(0); return yylval.ival = *yytext;
	break;
	case 51:
	s_remcontext(); return yylval.ival = *yytext;
	break;
	case 52:
	{
    /* copy, trimming initial and final '"' */
    yylval.string = strdup(yytext+1);
    if (yylval.string == NULL) fatal("Could not allocate memory (%d bytes)", strlen(yytext)+1);
    yylval.string[strlen(yylval.string)-1] = '\0';
    return STRING_LITERAL;
}
	break;
	case 53:
	yylval.ival=yytext[1]; return INT_LITERAL;
	break;
	case 54:
	yylval.ival='\a'; return INT_LITERAL;
	break;
	case 55:
	yylval.ival='\b'; return INT_LITERAL;
	break;
	case 56:
	yylval.ival='\f'; return INT_LITERAL;
	break;
	case 57:
	yylval.ival='\n'; return INT_LITERAL;
	break;
	case 58:
	yylval.ival='\r'; return INT_LITERAL;
	break;
	case 59:
	yylval.ival='\t'; return INT_LITERAL;
	break;
	case 60:
	yylval.ival='\v'; return INT_LITERAL;
	break;
	case 61:
	yylval.ival='?'; return INT_LITERAL;
	break;
	case 62:
	yylval.ival='\''; return INT_LITERAL;
	break;
	case 63:
	yylval.ival='\"'; return INT_LITERAL;
	break;
	case 64:
	yylval.ival='\\'; return INT_LITERAL;
	break;
	case 65:
	{
    /* ascii code in octal */
    char *stop;

    yylval.ival = strtol(yytext+2, &stop, 8);
    if (strlen(stop) != 1 || yylval.ival > 255)
        yyerror("Incorrect ASCII code in char constant: %d", yylval.ival);
    return INT_LITERAL;
}
	break;
	case 66:
	{
    /* ascii code in hex */
    char *stop;

    yylval.ival = strtol(yytext+3, &stop, 16);
    if (strlen(stop) != 1 || yylval.ival > 255)
        yyerror("Incorrect ASCII code in char constant: %d", yylval.ival);
    return INT_LITERAL;
}
	break;
	case 67:
	yylval.ival=yytext[2]; return INT_LITERAL;
	break;
	case 68:
	{ lineno++; pragma_active = 0; }
	break;
	case 69:
	;
	break;
	case 70:
	{ return yytext[0]; };
	break;


	}
	YY_SCANNER;
	i = yyleng;
	goto yy_again;			/* action fell though */

  yy_reject:
	YY_SCANNER;
	i = yyoleng;			/* restore original yytext */
	if (++yyfmin < yyfmax)
		goto yy_found;		/* another final state, same length */
	else
		goto yy_search;		/* try shorter yytext */

  yy_more:
	YY_SCANNER;
	i = yyleng;
	if (i > 0)
		yy_lastc = yytext[i-1];
	goto yy_contin;
}
/*
 * Safely switch input stream underneath LEX
 */
typedef struct yy_save_block_tag {
	FILE	* oldfp;
	int	oldline;
	int	oldend;
	int	oldstart;
	int	oldlastc;
	int	oldleng;
	char	savetext[YYLMAX+1];
	yy_state_t	savestate[YYLMAX+1];
} YY_SAVED;

YY_SAVED *
yySaveScan(fp)
FILE * fp;
{
	YY_SAVED * p;

	if ((p = (YY_SAVED *) malloc(sizeof(*p))) == NULL)
		return p;

	p->oldfp = yyin;
	p->oldline = yylineno;
	p->oldend = yy_end;
	p->oldstart = yy_start;
	p->oldlastc = yy_lastc;
	p->oldleng = yyleng;
	(void) memcpy(p->savetext, yytext, sizeof yytext);
	(void) memcpy((char *) p->savestate, (char *) yy_sbuf,
		sizeof yy_sbuf);

	yyin = fp;
	yylineno = 1;
	YY_INIT;

	return p;
}
/*f
 * Restore previous LEX state
 */
void
yyRestoreScan(p)
YY_SAVED * p;
{
	if (p == NULL)
		return;
	yyin = p->oldfp;
	yylineno = p->oldline;
	yy_end = p->oldend;
	yy_start = p->oldstart;
	yy_lastc = p->oldlastc;
	yyleng = p->oldleng;

	(void) memcpy(yytext, p->savetext, sizeof yytext);
	(void) memcpy((char *) yy_sbuf, (char *) p->savestate,
		sizeof yy_sbuf);
	free(p);
}
/*
 * User-callable re-initialization of yylex()
 */
void
yy_reset()
{
	YY_INIT;
	yylineno = 1;		/* line number */
}
/* get input char with pushback */
YY_DECL int
input()
{
	int c;
#ifndef YY_PRESERVE
	if (yy_end > yyleng) {
		yy_end--;
		memmove(yytext+yyleng, yytext+yyleng+1,
			(size_t) (yy_end-yyleng));
		c = yy_save;
		YY_USER;
#else
	if (yy_push < yy_save+YYLMAX) {
		c = *yy_push++;
#endif
	} else
		c = yygetc();
	yy_lastc = c;
	if (c == YYNEWLINE)
		yylineno++;
	return c;
}

/*f
 * pushback char
 */
YY_DECL int
unput(c)
	int c;
{
#ifndef YY_PRESERVE
	if (yy_end >= YYLMAX) {
		YY_FATAL(m_textmsg(1552, "Push-back buffer overflow", "E"));
	} else {
		if (yy_end > yyleng) {
			yytext[yyleng] = yy_save;
			memmove(yytext+yyleng+1, yytext+yyleng,
				(size_t) (yy_end-yyleng));
			yytext[yyleng] = 0;
		}
		yy_end++;
		yy_save = (char) c;
#else
	if (yy_push <= yy_save) {
		YY_FATAL(m_textmsg(1552, "Push-back buffer overflow", "E"));
	} else {
		*--yy_push = c;
#endif
		if (c == YYNEWLINE)
			yylineno--;
	}	/* endif */
	return c;
}


/* lookup symbol */
static int getsymbol()
{
    symbol_t *sp;

    if (pragma_active) {
        sp = s_pragma_lookup(yytext);
        if (sp != NULL) {
            /* all pragma symbols are internal and not exported */
            yylval.ival = sp->ident;
            return sp->ident;
    	}
    	/* pragma lookup failed; disable and do a regular lookup */
    	pragma_active = 0;
    }
    sp = s_lookupadd(yytext, VARIABLE);

    if (sp->ident == 0) {
        yyerror("Reserved keyword '%s' cannot be used", yytext);
        return 0;
    }
    if (sp->internal && !sp->exported){
        yylval.ival = sp->ident;
    }
    else {
        yylval.sp = sp;
    }

    return sp->ident;
}

/* varargs version */
void yyerror(char *fmt, ...)
{
    va_list ap;
    char buf[256];

    /* keep track of how many errors we found */
    nerrors++;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    if (!msvc) {
        if (feof(yyin)) {
            /* in MKS Lex, lineno and yytext are wrong after EOF */
            fprintf(stderr, "%s:%d: %s\n", filename, lineno-1, buf);
    	}
        else {
            /* print next token if just 'Syntax error' */
            if (strcmp(buf, "Syntax error")==0)
    	        fprintf(stderr, "%s:%d: %s: '%s'\n", filename, lineno, buf, yytext);
            else
    	        fprintf(stderr, "%s:%d: %s\n", filename, lineno, buf);
        }
    }
    else {
        if (feof(yyin)) {
            /* in MKS Lex, lineno and yytext are wrong after EOF */
            fprintf(stderr, "%s(%d) : error: %s\n", filename, lineno-1, buf);
        }
        else {
	    /* print next token if just 'Syntax error' */
	    if (strcmp(buf, "Syntax error")==0)
	        fprintf(stderr, "%s(%d) : error: %s: '%s'\n", filename, lineno, buf, yytext);
	    else
	        fprintf(stderr, "%s(%d) : error: %s\n", filename, lineno, buf);
        }
    }
    return;
}

/* skip input until end of comment */
void yycomment(char *end)
{
    int c, startline;
    char *cp;

    startline = lineno;
    for (cp=end; *cp!='\0'; ) {
        c = input();
        if (c == EOF) {
            yyerror("End of file in comment started at line %d", startline);
            exit(1);
        }
        if (c == '\n') lineno++;
        if (c != *cp++) {
            cp = end;
            if (c == *cp) cp++;
        }
    }
}

/*
 * Transparently pass user to code to the output file.
 * Returns verbatim code.
 */
verbatim_t *yyverbatim(char *end)
{
    int c;
    char *cp, *sp;
    verbatim_t *vp;

    vp = (verbatim_t *)getmem(sizeof(verbatim_t));

    /* we need to hold off so that we don't print the 'end' characters */

    vp->lineno = lineno;
    vp->str = NULL;
    for (cp=end; *cp!='\0'; ) {
        c = input();
        if (c == EOF) {
            yyerror("End of file in verbatim section started at line %d", vp->lineno);
            exit(1);
        }
        if (c == '\n') lineno++;
        if (c != *cp++) {
            /* no match, output and restart */
            sp = end;
            while (sp < cp-1) vp->str = buildstr(vp->str, *sp++);
            cp = end;
            /* see if at least the first 'end' char matches */
            if (c == *cp) cp++;
            else vp->str = buildstr(vp->str, c);
        }
    }
    return vp;
}

/* state stack (reversed) for include file processing */
typedef struct _state_t {
    char filename[MAX_LEN];
    int lineno;
    FILE *yyin;
    int max_array_size;
    int null_strings;
    int import;     /* set to 1 if import, 0 if include */
    struct _state_t *next;
} state_t;

static state_t *state_top=NULL;

/* push; returns 0 if file with same base name has already been included */
static int state_push(char *newfname, FILE *newfp, int import)
{
    state_t *sp, *tp;

    /* sanity check: can't include ourselves */
    if (strcmp(basename(newfname), basename(filename)) == 0) return 0;

    sp = (state_t *)getmem(sizeof(state_t));

    /* add to stack */
    if (state_top == NULL) state_top = sp;
    else {
        tp = state_top;
        while (tp != NULL) {
            /* detect loops */
#if defined(WIN32) || defined(_WIN32_)
            if (stricmp(basename(newfname), basename(tp->filename))==0) {
#else
            if (strcmp(basename(newfname), basename(tp->filename))==0) {
#endif
                free(sp);
                return 0;
            }
            if (tp->next == NULL) break;
            tp = tp->next;
        }
        tp->next = sp;
    }
    strcpy(sp->filename, filename);
    strcpy(filename, newfname);
    sp->import = import;
    sp->max_array_size = max_array_size;
    sp->null_strings = null_strings;
    sp->lineno = lineno;
    sp->yyin = yyin;
    yyin = newfp;
    lineno = 1;
    sp->next = NULL;

    return 1;
}

static void state_pop(void)
{
    state_t *sp, *prevsp;

    if (state_top == NULL) fatal("Internal error L%d: State stack is empty", __LINE__);
    
    /* find entry at the bottom of stack */
    prevsp = NULL;
    sp = state_top;
    while (sp->next != NULL) {
        if (prevsp == NULL) prevsp = sp;
        else prevsp = prevsp->next;
        sp = sp->next;
    }
    /* restore state */
    strcpy(filename, sp->filename);
    lineno = sp->lineno;
    yyin = sp->yyin;
    /* pop max_array_size and null_strings *only* if we are including */
    if (including()) {
        max_array_size = sp->max_array_size;
        null_strings = sp->null_strings;
    }

    /* unlink entry */
    if (prevsp == NULL) state_top = NULL;
    else prevsp->next = NULL;

    free(sp);
}

/*
 * Are we currently processing an include file? 
 * An import is only treated as such if all previous includes where imports themselves.
 */
int including(void)
{
    state_t *sp;
   
    sp = state_top;
    while (sp != NULL) {
        if (!sp->import) return 1;
        sp = sp->next;
    }
    return 0;
}

/* file processing continuation (0 means there is more input) */
int yywrap(void) 
{
    if (state_top != NULL) {
        state_pop();
        return 0;
    }
    return 1;
}

/* process include/import file (returns 0 if a loop would occur) */
int include(char *file, int import)
{
    char path[MAX_LEN]; /* full path to included file */
    FILE *fp;

    /* input filename must end with .fl */
    if (strcmp(&file[ strlen(file) >= 3 ? strlen(file)-3 : 0 ], ".fl"))
        fatal("Included file does hot have '.fl' suffix: %s", file);

    if (!isabsolute(file)) {
        strcpy(path, dirname(filename));
        strcat(path, file);
    }
    else strcpy(path, file);

    /* open input file  */
    if ((fp = fopen(path, "r")) == NULL) fatal("Cannot open included file: %s", path);

    /* switch state */
    if (!state_push(path, fp, import)) {
        fclose(fp);
        return 0;
    }

    /* let the parser continue */
    return 1;
}

