// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "node.h"
#include "unary.h"

UnaryNode::UnaryNode(char opcode, ASTNode *operand)
    : opcode(opcode), operand(operand) {}

#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

#include "unary.h"
#include "renderer.h"
#include "errors.h"

using ::llvm::Value;
using ::llvm::Function;


void UnaryNode::codegen(IRRenderer& renderer) {
  
}
