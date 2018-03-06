// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "mpeg2_interpreter.h"
#include "mpeg2_interpreter_impl.h"

#include "grammar_mpeg2_parser.hpp"
#include "grammar_mpeg2_lexer.h"
#include "grammar_mpeg2_parser_impl.h"

#include "logging.h"

using namespace khaotica::syntax::mpeg2;

document_t interpreter_t::parse(std::istream &in) {

    std::ostringstream serr;
    lexer_t _scanner(in, serr);
    _scanner.set_debug(0);

    auto impl = std::make_shared<impl_t>();
    parser_t _parser(_scanner, impl);

    std::ostringstream perr;
    _parser.set_debug_stream(perr);
    _parser.set_debug_level(false);

    int ret = _parser.parse();

    logging::debug() << "Parsing has ended with code " << ret << ": ";
    logging::debug() << perr.str();


    return impl->document;
}
