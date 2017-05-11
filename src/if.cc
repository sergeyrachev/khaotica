// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "node.h"
#include "if.h"

IfNode::IfNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> then, std::shared_ptr<ASTNode> _else)
    : condition(cond), then(then), _else(_else) {}

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

#include "if.h"
#include "renderer.h"

using ::llvm::APFloat;
using ::llvm::BasicBlock;
using ::llvm::ConstantFP;
using ::llvm::Function;
using ::llvm::PHINode;
using ::llvm::Value;
using ::llvm::Type;


void IfNode::codegen(IRRenderer& renderer) {
   
}
