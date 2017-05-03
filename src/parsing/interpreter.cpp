//
// Created by sergey.rachev on 4/21/17.
//
//
//#include "interpreter.h"
//
//flavor::Interpreter::Interpreter() :_scanner(*this), _parser(*this, _scanner), _out(std::cout){}
//
//ASTNode *flavor::Interpreter::parse(std::istream &in) {
//    _scanner.switch_streams(in, _out);
//    _parser.parse();
//    return nullptr;
//}
//
//void flavor::Interpreter::ast(ASTNode * ast) {
//    _ast = ast;
//}


// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ast.h"

#include "scanner.h"
#include "parser.hpp"


Interpreter::Interpreter() : root(nullptr) {}

Interpreter::~Interpreter() {
    root.reset();
}

void
Interpreter::parse(std::istream &input) {
    root.release();

    Lexer lexer(&input);
    bison::Parser *parser = new bison::Parser(lexer, *this);

    parser->parse();
}

void
Interpreter::set_root(FunctionNode *node) {
    root = unique_ptr<ASTNode>(node);
}

void
Interpreter::set_root(PrototypeNode *node) {
    root = unique_ptr<ASTNode>(node);
}