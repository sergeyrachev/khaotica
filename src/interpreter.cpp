// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "interpreter.h"

#include "scanner.h"
#include "parser.hpp"
#include "logging.h"

std::list<flavor::symbol_t> flavor::Interpreter::parse(std::istream &in){

    std::ostringstream serr;
    flavor::Scanner _scanner(in, serr);
    std::list<flavor::symbol_t> symbols;
    flavor::Parser _parser(_scanner, symbols);

    _scanner.set_debug(1);
    _parser.set_debug_level(1);
    _parser.set_debug_stream(std::cerr);

    int ret = _parser.parse();
    logging::debug() << "Parsing has ended with code: " << ret;

    return symbols;

}
