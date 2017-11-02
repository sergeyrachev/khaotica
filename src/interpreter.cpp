// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "interpreter.h"

#include "parser.hpp"
#include "lexer.h"
#include "logging.h"

flavor::document_t flavor::interpreter_t::parse(std::istream &in, bool enable_verbosity) {

    std::ostringstream serr;
    flavor::lexer_t _scanner(in, serr);

    flavor::document_t document;
    flavor::parser_t _parser(_scanner, document, &document.global);

    _scanner.set_debug(enable_verbosity);
    _parser.set_debug_level(enable_verbosity);

    std::ostringstream perr;
    _parser.set_debug_stream(perr);

    int ret = _parser.parse();
    logging::debug() << "Parsing has ended with code " << ret << ": ";
    logging::debug() << perr.str();

    return document;
}
