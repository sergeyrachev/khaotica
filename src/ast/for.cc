// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "node.h"
#include "for.h"


ForNode::ForNode(const std::string &var_name,
                 ASTNode *start, ASTNode *end,
                 ASTNode *step,
                 ASTNode *body)
    : var_name(var_name), start(start), end(end), step(step), body(body) {}

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

#include "ast/for.h"
#include "renderer.h"

using ::llvm::AllocaInst;
using ::llvm::APFloat;
using ::llvm::BasicBlock;
using ::llvm::Constant;
using ::llvm::ConstantFP;
using ::llvm::Function;
using ::llvm::Value;
using ::llvm::Type;


llvm::Value * ForNode::codegen(IRRenderer& renderer) {
    Function *func = renderer.builder->GetInsertBlock()->getParent();

    AllocaInst *alloca = renderer.create_entry_block_alloca(func, var_name);

    Value *start_value = start->codegen(renderer);
    if( start_value == 0 ) {
        return 0;
    }

    renderer.builder->CreateStore(start_value, alloca);

    BasicBlock *loop_block = BasicBlock::Create(renderer.llvm_context(), "loop", func);

    renderer.builder->CreateBr(loop_block);
    renderer.builder->SetInsertPoint(loop_block);

    AllocaInst *old_value = renderer.get_named_value(var_name);
    renderer.set_named_value(var_name, alloca);

    if( body->codegen(renderer) == 0 ) {
        return 0;
    }

    Value *step_value;
    if( step ) {
        step_value = step->codegen(renderer);
        if( step_value == 0 ) {
            return 0;
        }
    } else {
        step_value = ConstantFP::get(renderer.llvm_context(), APFloat(1.0));
    }

    Value *end_condition = end->codegen(renderer);
    if( end_condition == 0 ) {
        return 0;
    }

    Value *current_var = renderer.builder->CreateLoad(alloca, var_name.c_str());
    Value *next_var = renderer.builder->CreateFAdd(current_var, step_value, "nextvar");
    renderer.builder->CreateStore(next_var, alloca);

    end_condition = renderer.builder->CreateFCmpONE(
        end_condition,
        ConstantFP::get(renderer.llvm_context(), APFloat(0.0)),
        "loopcond"
    );

    BasicBlock *after_block = BasicBlock::Create(renderer.llvm_context(), "afterloop", func);

    renderer.builder->CreateCondBr(end_condition, loop_block, after_block);
    renderer.builder->SetInsertPoint(after_block);

    if( old_value ) {
        renderer.set_named_value(var_name, old_value);
    } else {
        renderer.clear_named_value(var_name);
    }

    return Constant::getNullValue(Type::getDoubleTy(renderer.llvm_context()));
}
