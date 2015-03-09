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
 * Alexandros Eleftheriadis <eleft@ee.columbia.edu>
 * Danny Hong <danny@ee.columbia.edu>
 * Yuntai Kyong <yuntaikyong@ieee.org>
 *
 */

/* main.c */


#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "port.h"
#include "globals.h"
#include "symbol.h"
#include "ptree.h"
#include "compile.h"
#include "error.h"
#include "util.h"


/* our name */
const char program[] = "flavorc";

/* our major version number */
const char version[] = "5.2";

/* our state (alpha, beta, etc., or release) */
const char state[] = "beta";

/* currently processed file */
char *filename;

/* current line in current file */
int lineno;

/* language to use for code generation; default is C++ */
lang_t lang = L_CPP;

/* the access type for the class members; default is public */
access_t access_type = A_PUBLIC;  

/* suppress line number info from verbatim sections */
int supp_lineno = 0;

/* generate put method */
int gen_put = 1;

/* generate get method */
int gen_get = 1;

/* generate tracing code */
int gen_trace = 0;

/* do we need to declare tracing functions; this is set when we hit a 'trace' pragma */
int decl_trace = 0;

/* parse only (no code generation) */
int parse_only = 0;

/* initial maximum size of arrays */
int init_max_array_size = 64;

/* maximum size of arrays -- may change dynamically via pragma statements */
int max_array_size;

/* generate MSVC-style error messages */
int msvc = 0;

/* number of errors encountered */
int nerrors = 0;

/* option settings */
int w_abstract = 0;
int w_cyclic = 0;
int w_includes = 0;

/* prefix to use for Flavor variables */
char *prefix = NULL;

/* default prefix */
char default_prefix[] = "_F_";

/* function to use for reporting errors on the bitstream */
char *err_func = NULL;

/* default error function */
char default_err_func[] = "flerror";

/* function to use for tracing */
char *trace_func = NULL;

/* default tracing function */
char default_trace_func[] = "trace";

/* class to use for bitstream I/O */
char *bitstream_class = NULL;

/* default bitstream class */
char default_bitstream_class[] = "IBitstream";

/* generate null-terminated strings for arrays */
int null_strings = 0;

/* output file name and pointer used by the code generator (.h file, .java file, or .xsd file) */
char ofile[MAX_LEN];
FILE* ofp;

/* secondary file name and pointer used by the code generator (.cpp, or .java file); the .java file contains global contants */
char ofile2[MAX_LEN];
FILE* ofp2;


/* C++ specific */

/* generate include directives */
int gen_includes = 1;


/* Java specific */

/* class to use for global constants in Java */
char *global_class = NULL;

/* default global constant class */
char default_global_class[] = "Global";

/* name of the Flavor library package */
char *flavor_pkg = NULL;

/* default Flavor library package */
char default_flavor_pkg[] = "flavor";

/* the global constant class is defined only if there are global constants */
int first_global = 1;

/* util class (used to anchor global functions in Java) */
char util_class[] = "Util";

/* map result class (used to return the result of map processing in Java) */
char map_result[] = "MapResult";

/* output package for Java code */
char opkg[MAX_LEN];


/* XML specific */

/* generate the putxml() method */
int gen_putxml = 1;

/* functions to use for xml */
char default_xml_func1[] = "CreateXmlHeader";
char default_xml_func2[] = "EndXml";
char default_xml_func3[] = "WriteXmlElement";
char default_xml_func4[] = "IntoAClass";
char default_xml_func5[]= "OutOfClass";
char *xml_func1 = default_xml_func1;
char *xml_func2 = default_xml_func2;
char *xml_func3 = default_xml_func3;
char *xml_func4 = default_xml_func4;
char *xml_func5 = default_xml_func5;

/* the root xsd name used for XML Schema */
char *root_xsd_name = NULL;


/* parser entry point */
int yyparse();

/* input file pointer for the lexical analyzer */
extern FILE* yyin;

/* this is the list of parsed statements; the entire input file ends up in this list */
list_t *parsed_code = NULL;


char *filenameproper(char* filename);


void Usage(void)
{
    fprintf(stderr,
            "Usage: %s [options] input.fl\n"
            "Available options are:\n"
            "\t-h or -?   print this message\n"
            "\t-V         print version information\n"
            "\t-gc        generate C++ code (default)\n"
            "\t-gj        generate Java code\n"
            "\t-gx        generate XML schema\n"
            "\t-n         parse only; no code will be generated\n"
            "\t-g         do not generate a get() method\n"
            "\t-p         do not generate a put() method\n"
            "\t-x         do not generate a putxml() method\n"
            "\t-t         generate tracing code\n"
            "\t-l         suppress output of line number information for verbatim code\n"
            "\t-oh file   set the name of the output include file (default 'input.h')\n"
            "\t-oc file   set the name of the output C++ file (default 'input.cpp')\n"
            "\t-oj file   set the name of the output Java package (default 'input')\n"
            "\t-og file   set the name of the Java class that will contain all global constants\n"
            "\t           (default 'Global')\n"
            "\t-op file   set the name of the Flavor library package\n"
            "\t           (default 'flavor')\n"
			"\t-ox file   set the name of the output XML schema file (default 'input.xsd')\n"
            "\t-a size    set maximum array size (default %d)\n"
            "\t-s         use null-terminated strings\n"
            "\t-msvc      generate Microsoft Visual C++-style error messages\n"
            "\t-i         don't generate include (import) directives when generating C++ (Java) code\n"
            "\t-w option  enable optional checks; use '-w help' for more information\n"
            "\t           (may be provided multiple times)\n"
            "\t-B string  use string as the bitstream I/O class (default %s)\n"
            "\t-F string  use string as a prefix for internal Flavor-generated\n"
            "\t           variables (default '%s')\n"
            "\t-E string  use function 'string' to report bitstream syntax errors\n"
            "\t           (default '%s')\n"
            "\t-T string  use function 'string' to create trace reports (default\n"
            "\t           '%s')\n"
            "\t--         disable further options processing\n",
            program, init_max_array_size, default_bitstream_class, default_prefix,
            default_err_func, default_trace_func);
}


void WarnUsage(void)
{
    fprintf(stderr, 
            "The following optional checks can be enabled via the '-w check' option:\n"
            "\thelp       print this message\n"
            "\tabstract   warn when pure abstract classes are used\n"
            "\tcyclic     warn when cyclic references are detected\n"
            "\tincludes   warn when cyclic includes/imports are detected\n");
}


/* generate and print version information */
void Version(void)
{
    static char vss_revision[] = "$Revision: 0 $";
    static char vss_date[] = "$JustDate:  3/10/2005 $";
    char program_caps[64], version_info[128];
    int i, len;
    
    /* process VSS info */
    vss_revision[strlen(vss_revision)-2] = '\0';
    strcpy(version_info, version);
    strcat(version_info, ".");
    strcat(version_info, vss_revision+11);

    if (strcmp("release", state) != 0) {
        strcat(version_info, " [");
        strcat(version_info, state);
        strcat(version_info, "]");
    }

    vss_date[strlen(vss_date)-2] = '\0';

    /* single-digit months have extra space */
    if (vss_date[11] == ' ') 
        strcat(version_info, vss_date+11);
    else 
        strcat(version_info, vss_date+10);
    
    /* get progname in caps -- ASCII */
    strcpy(program_caps, program);
    len = strlen(program_caps);
    for (i=0; i<len; i++) {
        if (program_caps[i] >= 'a' && program_caps[i] <= 'z')
            program_caps[i]-='a'-'A';
    }
    printf("%s Version %s\n"
           "Copyright (C) 1997-20%s, Columbia University.\n"
           "For the latest information, visit the Flavor Web site at:\n"
           "\t http://flavor.sourceforge.net\n"
           "\n",
           program_caps, version_info, version_info+strlen(version_info)-2);
}


/********
 * main *
 ********/


int main(int argc, char *argv[])
{
    int optind;
    char *opt;
    
    /* set output file names to empty before parsing command line */
    ofile[0] = ofile2[0] = opkg[0] = '\0';
    
    /* process command line arguments */
    optind = 1;
    while (optind < argc && *argv[optind] == '-') {
        /* skip option prefix char and prepare for next argument */
        opt = argv[optind++]+1;   
        
        /* check option */
        if (strcmp(opt, "h") == 0 || strcmp(opt, "?") == 0) {
            Usage();           /* print a usage message */
            exit(0);
        }
        else if (strcmp(opt, "V") == 0) {
            Version();          /* print version information */
            exit(1);
        }
        else if (strcmp(opt, "gc") == 0) 
            lang = L_CPP;       /* C++ code generation (default) */
        else if (strcmp(opt, "gj") == 0) 
            lang = L_JAVA;      /* Java code generation */
       	else if (strcmp(opt, "gx") == 0) 
            lang = L_XSD;       /* XML schema generation */
        else if (strcmp(opt, "n") == 0) 
            parse_only = 1;     /* parse only */
        else if (strcmp(opt, "g") == 0) 
            gen_get = 0;        /* no get method */
        else if (strcmp(opt, "p") == 0) 
            gen_put = 0;        /* no put method */
        else if (strcmp(opt, "x") == 0) 
            gen_putxml = 0;     /* no putxml method */
        else if (strcmp(opt, "t") == 0) 
            gen_trace = 1;      /* generate tracing code */
        else if (strcmp(opt, "l") == 0) 
            supp_lineno = 1;    /* no line number info in verbatim sections */
        else if (strcmp(opt, "s") == 0) 
            null_strings = 1;   /* generate null-terminated strings */
        else if (strcmp(opt, "msvc") == 0) 
            msvc = 1;           /* MSVC-style error messages */
        else if (strcmp(opt, "i") == 0) 
            gen_includes = 0;   /* no include directives */
        else if (strcmp(opt, "-") == 0) 
            break;              /* stop options processing */
        else {
            /* options with one argument */
            if (optind >= argc) fatal("Option -%s requires an argument", opt);
            
            if (strcmp(opt, "a") == 0) {
                /* maximum array size */
                init_max_array_size = atol(argv[optind]);
                if (init_max_array_size <= 0) 
                    fatal("Invalid maximum array size: %s", argv[optind]);
            }
            else if (strcmp(opt, "w") == 0) {
                if (strcmp(argv[optind], "help") == 0) {
                    WarnUsage();
                    exit(0);
                }
                else if (strcmp(argv[optind], "cyclic") == 0) 
                    w_cyclic = 1;
                else if (strcmp(argv[optind], "abstract") == 0) 
                    w_abstract = 1;
                else if (strcmp(argv[optind], "includes") == 0) 
                    w_includes = 1;
                else 
                    fatal("Unknown -w option: %s", argv[optind]);
            }
            else if (strcmp(opt, "B") == 0) 
                bitstream_class = argv[optind]; /* class name for bitstream I/O */
            else if (strcmp(opt, "F") == 0)
                prefix = argv[optind];          /* prefix for Flavor variables */
            else if (strcmp(opt, "E") == 0)
                err_func = argv[optind];        /* function name for bistream errors */
            else if (strcmp(opt, "T") == 0) 
                trace_func = argv[optind];      /* function name for tracing */
            else if (strcmp(opt, "oh") == 0) 
                strcpy(ofile, argv[optind]);
            else if (strcmp(opt, "oc") == 0) 
                strcpy(ofile2, argv[optind]);
            else if (strcmp(opt, "oj") == 0)
                strcpy(opkg, argv[optind]);
            else if (strcmp(opt, "og") == 0) 
                global_class = argv[optind];
            else if (strcmp(opt, "op") == 0) 
                flavor_pkg = argv[optind];
            else if (strcmp(opt, "ox") == 0)
                strcpy(ofile, argv[optind]);
            else if (strcmp(opt, "o") == 0) 
                fatal("option -o has been replaced by -oh, -oc, -oj, -og and -ox");
            else {
                fatal("Unknown option: -%s", opt);
                exit(1);
            }
            optind++;
        }
    }
    /* ensure that prefix is set */
    if (prefix == NULL) prefix = default_prefix;
    
    /* ensure that error function is set */
    if (err_func == NULL) {
        if (cpp()) 
            err_func = default_err_func;
        else if (java())  {
			err_func = (char *)getmem(strlen(util_class)+strlen(default_err_func)+2);
            sprintf(err_func, "%s.%s", util_class, default_err_func);
        }
    }

    /* ensure that trace function is set */
    if (trace_func == NULL) {
        if (cpp()) 
            trace_func = default_trace_func;
        else if (java())  {
			trace_func = (char *)getmem(strlen(util_class)+strlen(default_trace_func)+2);
            sprintf(trace_func, "%s.%s", util_class, default_trace_func);
        }
    }

    /* ensure that bitstream I/O class is set */
    if (bitstream_class == NULL) 
        bitstream_class = default_bitstream_class;
    
    /* ensure the global class name is set for java; all global constants are defined by this class */
    if (java() && global_class == NULL) 
        global_class = default_global_class;

    /* ensure the flavor library package name is set for java */
    if (java() && flavor_pkg == NULL) 
        flavor_pkg = default_flavor_pkg;

    /* initialize current maximum array size */
    max_array_size = init_max_array_size;
    
    /* check that we have a file to process */
    if (argc-optind != 1) {
        Usage();
        exit(1);
    }
    
    /* input filename must end with .fl */
    if (strcmp(&argv[optind][ strlen(argv[optind]) >= 3 ? strlen(argv[optind])-3 : 0 ], ".fl"))
        fatal("Input file does hot have '.fl' suffix: %s", argv[optind]);
    
    /* set 'filename' for use by the error reporting and output functions */
    filename = filenameproper(argv[optind]);
	
    /* open input file -- assign to yyin for use by the parser */
    if ((yyin = fopen(filename, "r")) == NULL) 
        fatal("Cannot open file: %s", filename);
    
    /* initialize symbol tables */
    s_init();
    
    /* run parser (will read the entire file) */
    lineno = 1;
    yyparse();
    
    /* done with input file */
    fclose(yyin);
    
    /* translate */
    compile(parsed_code);
    
    /* done -- return total number of errors found */
	free(filename);
    return nerrors;
}


char* filenameproper(char* filename)
{
	char* newfilename = malloc(1024);
	int i = 0, j = 0;
	while (filename[i] != '\0') {	
		newfilename[j] = filename[i++];
		if (newfilename[j++] == '\\' && filename[i] != '\\') 
            newfilename[j++] = '\\';
    }
	newfilename[j] = '\0';
	return newfilename;
}
