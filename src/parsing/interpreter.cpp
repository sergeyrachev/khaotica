//
// Created by sergey.rachev on 4/21/17.
//

#include "interpreter.h"

flavor::Interpreter::Interpreter() :_scanner(*this), _parser(*this, _scanner), _out(std::cout){}

ASTNode *flavor::Interpreter::parse(std::istream &in) {
    _scanner.switch_streams(in, _out);
    _parser.parse();
    return nullptr;
}

void flavor::Interpreter::ast(ASTNode * ast) {
    _ast = ast;
}
