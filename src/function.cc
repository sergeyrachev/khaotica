// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "node.h"
#include "prototype.h"
#include "function.h"
#include <llvm/IR/Verifier.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include "function.h"
#include "renderer.h"

using ::llvm::BasicBlock;
using ::llvm::Function;
using ::llvm::Value;


FunctionNode::FunctionNode(std::shared_ptr<PrototypeNode> proto, std::shared_ptr<ASTNode> body)
    : proto(proto), body(body) {}



void FunctionNode::codegen(IRRenderer& renderer) {
   
}
