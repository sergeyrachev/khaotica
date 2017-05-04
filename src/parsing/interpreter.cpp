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



//#include "ast.h"
//
//#include "scanner.h"
//#include "parser.hpp"
//
//using namespace flavor;
//
//Interpreter::Interpreter() : root(nullptr) {}
//
//Interpreter::~Interpreter() {
//    root.reset();
//}
//
//void
//Interpreter::parse(std::istream &input) {
//    root.release();
//
//    Lexer lexer(&input);
//    bison::Parser *parser = new bison::Parser(lexer, *this);
//
//    parser->parse();
//}
//
//void
//Interpreter::set_root(FunctionNode *node) {
//    root = unique_ptr<ASTNode>(node);
//}
//
//void
//Interpreter::set_root(PrototypeNode *node) {
//    root = unique_ptr<ASTNode>(node);
//}