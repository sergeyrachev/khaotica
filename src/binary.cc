// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Value.h"

#include "binary.h"
#include "variable.h"
#include "renderer.h"
#include "errors.h"

#include "node.h"
#include "binary.h"

using ::llvm::Value;
using ::llvm::Type;

BinaryNode::BinaryNode(char op, std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
    : op(op), lhs(lhs), rhs(rhs) {}
    

void BinaryNode::codegen(IRRenderer& renderer) {
   
}