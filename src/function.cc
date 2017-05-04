// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "node.h"
#include "prototype.h"
#include "function.h"

FunctionNode::FunctionNode(std::shared_ptr<PrototypeNode> proto, std::shared_ptr<ASTNode> body)
    : proto(proto), body(body) {}

#include <llvm/IR/Verifier.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>

#include "function.h"
#include "renderer.h"

using ::llvm::BasicBlock;
using ::llvm::Function;
using ::llvm::Value;


llvm::Value * FunctionNode::codegen(IRRenderer& renderer) {
    renderer.clear_all_named_values();

    Function *func = proto->codegen(renderer);
    if( func == 0 ) {
        return 0;
    }

    BasicBlock *block = BasicBlock::Create(renderer.llvm_context(),
        "entry",
        func);
    renderer.builder->SetInsertPoint(block);

    proto->create_argument_allocas(renderer, func);

    if( Value *retval = body->codegen(renderer) ) {
        renderer.builder->CreateRet(retval);
        llvm::verifyFunction(*func);

        

        return func;
    }

    func->eraseFromParent();

    return 0;
}
