#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "ast.h"
#include "scanner.h"
#include "parser.hpp"

#include <istream>

namespace flavor{
    class Interpreter {
        friend class Scanner;
        friend class Parser;
    public:
        Interpreter();

        ASTNode* parse(std::istream &in);

    private:
        void ast(std::shared_ptr<ASTNode> ast);
    private:
        Scanner _scanner;
        Parser _parser;

        std::ostream& _out;

        std::shared_ptr<ASTNode> _ast;


    };
}

#endif //KHAOTICA_INTERPRETER_H

//#pragma once
//
//#include "ast.h"
//
//class Interpreter {
//public:
//    Interpreter();
//    ~Interpreter();
//
//    unique_ptr<ASTNode> root;
//
//    void parse(std::istream &input);
//
//    void set_root(FunctionNode *node);
//    void set_root(PrototypeNode *node);
//
//
//};