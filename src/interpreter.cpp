// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "interpreter.h"

#include "parser.hpp"
#include "scanner.h"
#include "logging.h"

flavor::symbols_t flavor::Interpreter::parse(std::istream &in, bool enable_verbosity) {

    std::ostringstream serr;
    flavor::Scanner _scanner(in, serr);

    flavor::symbols_t symbols;
    flavor::symbols_table_t table;
    flavor::Parser _parser(_scanner, symbols, table);

    _scanner.set_debug(enable_verbosity);
    _parser.set_debug_level(enable_verbosity);

    std::ostringstream perr;
    _parser.set_debug_stream(perr);

    int ret = _parser.parse();
    logging::debug() << "Parsing has ended with code " << ret << ": " << perr.str();

    return symbols;
}
