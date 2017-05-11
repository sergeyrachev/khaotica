// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>
#include <vector>

#include "node.h"
#include "var.h"


VarNode::VarNode(const std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> > &var_names,
                 std::shared_ptr<ASTNode>body)
    : var_names(var_names), body(body) {}

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"

#include "node.h"
#include "var.h"
#include "renderer.h"

#include <cstddef>
#include <string>
#include <vector>

using ::llvm::AllocaInst;
using ::llvm::APFloat;
using ::llvm::ConstantFP;
using ::llvm::Function;
using ::llvm::Value;


void VarNode::codegen(IRRenderer& renderer) {
   
}
