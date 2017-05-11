// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>
#include <vector>

#include "prototype.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "prototype.h"
#include "renderer.h"
#include "errors.h"

#include <vector>

PrototypeNode::PrototypeNode(const std::string &name,
                             const std::vector<std::string> &args)
    : name(name), args(args) {}

using ::llvm::AllocaInst;
using ::llvm::Function;
using ::llvm::FunctionType;
using ::llvm::Type;
using ::llvm::Value;

void PrototypeNode::codegen(IRRenderer& renderer) {
   
}
