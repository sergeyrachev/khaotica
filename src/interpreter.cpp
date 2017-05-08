// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "interpreter.h"

#include "scanner.h"
#include "parser.hpp"
#include "logging.h"

flavor::Interpreter::Interpreter(){}

void flavor::Interpreter::parse(std::istream &in, std::vector<std::shared_ptr<ASTNode> >& asts) const{

    std::ostringstream serr;
    flavor::Scanner _scanner(in, serr);
    flavor::Parser _parser(_scanner, asts);
    _parser.set_debug_level(1);
    _parser.set_debug_stream(serr);

    int ret = _parser.parse();
    logging::debug() << "Parsing has ended with code: " << ret;
    logging::debug() << "Parsing log: " << serr.str();

}