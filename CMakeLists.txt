cmake_minimum_required(VERSION 3.6)
add_executable(flavor
        src/flavorc/symbol.c
        src/flavorc/cclass.c
        src/flavorc/ctrace.c
        src/flavorc/cgetput.c
        src/flavorc/cbac.c
        src/flavorc/cout.c
        src/flavorc/cmap.c
        src/flavorc/cpragma.c
        src/flavorc/main.c
        src/flavorc/eval.c
        src/flavorc/ptree.c
        src/flavorc/cxml.c
        src/flavorc/cverbatim.c
        src/flavorc/cexpr.c
        src/flavorc/compile.c
        src/flavorc/cvar.c
        src/flavorc/check.c
        src/flavorc/cid.c
        src/flavorc/carray.c
        src/flavorc/lex.yy.c
        src/flavorc/error.c
        src/flavorc/util.c
        src/flavorc/y.tab.c
)

target_include_directories(flavor PUBLIC include)
