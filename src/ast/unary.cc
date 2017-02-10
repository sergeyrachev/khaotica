// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "node.h"
#include "unary.h"

UnaryNode::UnaryNode(char opcode, ASTNode *operand)
    : opcode(opcode), operand(operand) {}

#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

#include "ast/unary.h"
#include "renderer.h"
#include "errors.h"

using ::llvm::Value;
using ::llvm::Function;


Value *
UnaryNode::codegen(IRRenderer *renderer) {
    Value *operand_value = operand->codegen(renderer);
    if( operand_value == 0 ) {
        return 0;
    }

    Function *func = renderer->module->getFunction(std::string("unary") + opcode);
    if( func == 0 ) {
        return ErrorV("Unknown unary operator");
    }

    return renderer->builder->CreateCall(func, operand_value, "unop");
}
