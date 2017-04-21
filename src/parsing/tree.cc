// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ast.h"
#include "tree.h"

#include "lexer.h"
#include "bison_parser.hpp"


STree::STree() : root(nullptr) {}

STree::~STree() {
    root.reset();
}

void
STree::parse(std::istream &input) {
    root.release();

    Lexer lexer(&input);
    bison::Parser *parser = new bison::Parser(lexer,
                                              *this);

    parser->parse();
}

void
STree::set_root(FunctionNode *node) {
    root = unique_ptr<ASTNode>(node);
}

void
STree::set_root(PrototypeNode *node) {
    root = unique_ptr<ASTNode>(node);
}
