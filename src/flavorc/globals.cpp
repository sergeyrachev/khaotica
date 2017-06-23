//
// Created by sergey.rachev on 6/23/17.
//

#include "globals.h"



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

#define MAX_LEN 1024

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
int gen_putxml = 0;

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
list_t *parsed_code = NULL;