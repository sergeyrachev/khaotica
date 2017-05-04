// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>
#include <vector>

#include "prototype.h"


PrototypeNode::PrototypeNode(const std::string &name,
                             const std::vector<std::string> &args)
    : name(name), args(args) {}

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "prototype.h"
#include "renderer.h"
#include "errors.h"

#include <vector>

using ::llvm::AllocaInst;
using ::llvm::Function;
using ::llvm::FunctionType;
using ::llvm::Type;
using ::llvm::Value;


void
PrototypeNode::create_argument_allocas(IRRenderer &renderer, Function *func) {
    Function::arg_iterator iterator = func->arg_begin();
    for( auto &arg : args ) {
        Value& val = *iterator++;
        AllocaInst *alloca = renderer.create_entry_block_alloca(func, arg);
        renderer.builder->CreateStore(&val, alloca);
        renderer.set_named_value(arg, alloca);
    }
}

llvm::Function * PrototypeNode::codegen(IRRenderer& renderer) {
    std::vector<Type*> doubles(args.size()+1,
        Type::getDoubleTy(renderer.llvm_context()));

    doubles[args.size()] = Type::getInt64Ty(renderer.llvm_context());

    FunctionType *func_type = FunctionType::get(
        Type::getDoubleTy(renderer.llvm_context()),
        doubles,
        false);

    Function *func = Function::Create(func_type,
        Function::ExternalLinkage,
        name,
        renderer.module);

    if( func->getName() != name ) {
        func->eraseFromParent();
        func = renderer.module->getFunction(name);

        if( !func->empty() ) {
            ErrorF("redefinition of function");
            return 0;
        }
        if( func->arg_size() != args.size() ) {
            ErrorF("redefintion of function with different number of args");
            return 0;
        }
    }

    Function::arg_iterator iterator = func->arg_begin();
    for( auto &arg : args ) {
        Value& val = *iterator++;
        val.setName(arg);
    }

    return func;
}
