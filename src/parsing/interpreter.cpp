//
// Created by sergey.rachev on 4/21/17.
//

#include "interpreter.h"

flavor::Interpreter::Interpreter() :_scanner(*this), _parser(*this, _scanner){}

ASTNode *flavor::Interpreter::parse(std::istream &in) {
    _parser.parse();
    return nullptr;
}
