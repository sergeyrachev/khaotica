// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>
#include <vector>

#include "node.h"
#include "call.h"

CallNode::CallNode(const std::string &callee, std::vector<std::shared_ptr<ASTNode>> &args)
    : callee(callee), args(args) {}


#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

#include "ast/call.h"
#include "renderer.h"
#include "errors.h"

#include <vector>

llvm::Value * CallNode::codegen(IRRenderer& renderer) {
    llvm::Function *callee_func = renderer.module->getFunction(callee);
    if( callee_func == 0 ) {
        return ErrorV("Unknown function referenced");
    }

    if( callee_func->arg_size() != args.size() + 1 ) {
        return ErrorV("Incorrect number of arguments passed");
    }

    std::vector<llvm::Value*> arg_values;
    for( auto &arg : args ) {
        arg_values.push_back(arg->codegen(renderer));
        if( arg_values.back() == 0 ) {
            return 0;
        }
    }

    arg_values.push_back(llvm::ConstantInt::get(renderer.llvm_context(), llvm::APInt(64, renderer.THIS)));

    return renderer.builder->CreateCall(callee_func, arg_values, "calltmp");
}
