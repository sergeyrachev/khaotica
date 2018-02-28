%{
#include "grammar_mpeg2_lexer.h"
#include "logging.h"

#define YY_USER_ACTION { _location.step(); _location.columns(yyleng); }
#define yyterminate() parser_t::make_END(_location);

using namespace khaotica::core::mpeg2;

%}

%option yylineno
%option debug
%option nodefault
%option noyywrap
%option c++
%option nounistd
%option yyclass="lexer_t"

blank [[:blank:]]
newline    \r?\n
identifier [[:alpha:]_][[:alnum:]_-]*
uinteger_dec    [0-9]+[0-9]*
uinteger_hex    0x[0-9a-fA-F]+[0-9a-fA-F]*
uinteger_oct    0[0-7]+[0-7]*
uinteger_bin    0b[01]+[01]*
bits       [01][01[[:blank:]]*]*
text [[:print:][:space:]]
line [[:print:][:blank:]]

%x COMMENT

%%
"," return parser_t::make_COMMA(_location);
"[" return parser_t::make_BRACKET_OPEN(_location);
"]" return parser_t::make_BRACKET_CLOSE(_location);
"{" return parser_t::make_BRACE_OPEN(_location);
"}" return parser_t::make_BRACE_CLOSE(_location);
"(" return parser_t::make_PARENTHESIS_OPEN(_location);
")" return parser_t::make_PARENTHESIS_CLOSE(_location);
"do" return parser_t::make_DO(_location);
"while" return parser_t::make_WHILE(_location);
"for" return parser_t::make_FOR(_location);
"if" return parser_t::make_IF(_location);
"else" return parser_t::make_ELSE(_location);
"nextbits" return parser_t::make_FUNCTION_NEXTBITS(_location);
"lengthof" return parser_t::make_FUNCTION_LENGTHOF(_location);
"__position" return parser_t::make_FUNCTION_POSITION(_location);
"bytealigned" return parser_t::make_FUNCTION_BYTEALIGNED(_location);
"==" return parser_t::make_EQUAL(_location);
"!=" return parser_t::make_NOTEQUAL(_location);
"bslbf" return parser_t::make_BSLBF(_location);
"uimsbf" return parser_t::make_UIMSBF(_location);
"tcimsbf" return parser_t::make_TCIMSBF(_location);
"simsbf" return parser_t::make_SIMSBF(_location);
"vlclbf" return parser_t::make_VLCLBF(_location);
".." return parser_t::make_RANGE(_location);
"=" return parser_t::make_ASSIGN(_location);
";" return parser_t::make_SEMICOLON(_location);
"++" return parser_t::make_INCREMENT(_location);
"&&" return parser_t::make_AND(_location);
"||" return parser_t::make_OR(_location);
"--" return parser_t::make_DECREMENT(_location);
"-" return parser_t::make_MINUS(_location);
"+" return parser_t::make_PLUS(_location);

"!" return parser_t::make_LOGICAL_NOT(_location);
"~" return parser_t::make_BITWISE_NOT(_location);

"*" return parser_t::make_MULTIPLY(_location);
"/" return parser_t::make_DIVIDE(_location);
"<" return parser_t::make_LESSTHAN(_location);
">" return parser_t::make_GREATERTHAN(_location);
"<=" return parser_t::make_LESSTHAN_EQUAL(_location);
">=" return parser_t::make_GREATERTHAN_EQUAL(_location);

"//"{line}* { ECHO; }

"/*" { BEGIN(COMMENT); }

<COMMENT>"*/" {
    BEGIN(INITIAL);
}
<COMMENT>{text} {
    ECHO; yymore();
}

\"[^"]*\" {
    return parser_t::make_TEXT( {yytext}, _location);
}

\'{bits}\' {
    return parser_t::make_BITSTRING( {yytext}, _location);
}

{identifier} {
    return parser_t::make_IDENTIFIER( {yytext}, _location);
}

{uinteger_dec}|{uinteger_hex}|{uinteger_oct} {
    auto number = std::stoll(yytext, 0, 0);
    return parser_t::make_INTEGER( {number}, _location);
}

{uinteger_bin} {
    auto number = std::stoll(yytext, 0, 2);
    return parser_t::make_INTEGER( {number}, _location);
}

{blank}* {}

{newline} { _location.initialize(YY_NULLPTR, yylineno, 1); }

<<EOF>> { return yyterminate(); }

%%
