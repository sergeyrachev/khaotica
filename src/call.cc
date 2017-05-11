// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>
#include <vector>

#include "node.h"
#include "call.h"

CallNode::CallNode(const std::string &callee, std::vector<std::shared_ptr<ASTNode>> &args)
    : callee(callee), args(args) {}


CallNode::CallNode(const std::string &callee) : callee(callee) {

}

#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

#include "call.h"
#include "renderer.h"
#include "errors.h"

#include <vector>

void CallNode::codegen(IRRenderer& renderer) {
 
}
