// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "number.h"

NumberNode::NumberNode(double val) : val(val) {}

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"

#include "ast/number.h"
#include "renderer.h"


llvm::Value * NumberNode::codegen(IRRenderer& renderer) {
    return llvm::ConstantFP::get(renderer.llvm_context(), llvm::APFloat(val));
}
