// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "interpreter.h"

flavor::Interpreter::Interpreter() :_scanner(*this), _parser(*this, _scanner), _out(std::cerr){}

ASTNode *flavor::Interpreter::parse(std::istream &in) {
    _scanner.switch_streams(in, _out);

    _parser.set_debug_level(2);
    _parser.set_debug_stream(_out);
    _parser.parse();
    return nullptr;
}

void flavor::Interpreter::ast(std::shared_ptr<ASTNode> ast) {
    _ast = ast;
}
