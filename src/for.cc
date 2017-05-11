// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "node.h"
#include "for.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

#include "for.h"
#include "renderer.h"

using ::llvm::AllocaInst;
using ::llvm::APFloat;
using ::llvm::BasicBlock;
using ::llvm::Constant;
using ::llvm::ConstantFP;
using ::llvm::Function;
using ::llvm::Value;
using ::llvm::Type;


ForNode::ForNode(const std::string &var_name,
                 std::shared_ptr<ASTNode> start, std::shared_ptr<ASTNode> end,
                 std::shared_ptr<ASTNode> step,
                 std::shared_ptr<ASTNode> body)
    : var_name(var_name), start(start), end(end), step(step), body(body) {}




void ForNode::codegen(IRRenderer& renderer) {
  
}
