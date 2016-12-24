#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Scalar.h"

#include <string>

#include "renderer.h"

using ::llvm::AllocaInst;
using ::llvm::CloneModule;
using ::llvm::DataLayout;
using ::llvm::ExecutionEngine;
using ::llvm::EngineBuilder;
using ::llvm::Function;
using ::llvm::FunctionPassManager;
using ::llvm::IRBuilder;
using ::llvm::LLVMContext;
using ::llvm::Module;
using ::llvm::Type;


IRRenderer::IRRenderer() : IRRenderer(new Module("my cool jit", llvm::LLVMContext())) {

}

IRRenderer::IRRenderer(const IRRenderer &other)
    : IRRenderer(CloneModule(other.module.get()).get()) {}

IRRenderer::IRRenderer(Module *m)
    : module(unique_ptr<Module>(m)),
      engine(EngineBuilder().create()),
      builder(unique_ptr<IRBuilder<> >(new IRBuilder<>(module->getContext())))
{

    module->setDataLayout(engine->getTargetMachine()->createDataLayout());
}

IRRenderer::IRRenderer(IRRenderer &&other) {
    module = std::move(other.module);
    engine = std::move(other.engine);
    builder = std::move(other.builder);
 
    other.module = nullptr;
    other.engine = nullptr;
    other.builder = nullptr;
    
}

IRRenderer &
IRRenderer::operator =(IRRenderer other) {
    std::swap(module, other.module);
    std::swap(engine, other.engine);
    std::swap(builder, other.builder);
   
    return *this;
}

IRRenderer::~IRRenderer() {
    
    builder.reset();
    engine.reset();
    module.reset();
}

llvm::LLVMContext &
IRRenderer::llvm_context() { return module->getContext(); }

llvm::AllocaInst *
IRRenderer::get_named_value (const std::string &name){
    return named_values[name];
}

void
IRRenderer::set_named_value(const std::string &name, llvm::AllocaInst *value) {
    named_values[name] = value;
}

void
IRRenderer::clear_named_value(const std::string &name) {
    named_values.erase(name);
}

void
IRRenderer::clear_all_named_values() {
    named_values.clear();
}

AllocaInst *
IRRenderer::create_entry_block_alloca(Function *func, const std::string &name) {
    IRBuilder<> tmp_builder(&func->getEntryBlock(),
                            func->getEntryBlock().begin());

    return tmp_builder.CreateAlloca(Type::getDoubleTy(module->getContext()),
                                    0,
                                    name.c_str());
}
