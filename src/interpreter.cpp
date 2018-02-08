// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "interpreter.h"

#include "grammar_mpeg2_parser.hpp"
#include "grammar_mpeg2_lexer.h"
#include "logging.h"

khaotica::sequence_t khaotica::interpreter_t::parse(std::istream &in, bool enable_verbosity) {

    std::ostringstream serr;
    khaotica::core::mpeg2::lexer_t _scanner(in, serr);

    khaotica::sequence_t document;
    khaotica::core::mpeg2::parser_t _parser(_scanner, document.body, document.scope, document.scope);

    _scanner.set_debug(enable_verbosity);
    _parser.set_debug_level(enable_verbosity);

    std::ostringstream perr;
    _parser.set_debug_stream(perr);

    int ret = _parser.parse();
    logging::debug() << "Parsing has ended with code " << ret << ": ";
    logging::debug() << perr.str();

    return document;
}
