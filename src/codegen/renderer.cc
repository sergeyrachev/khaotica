// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/ObjectCache.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"

#include <string>

#include "renderer.h"

using ::llvm::AllocaInst;
using ::llvm::DataLayout;
using ::llvm::ExecutionEngine;
using ::llvm::EngineBuilder;
using ::llvm::Function;
using ::llvm::IRBuilder;
using ::llvm::LLVMContext;
using ::llvm::Module;
using ::llvm::Type;


IRRenderer::IRRenderer()
{
    auto M = std::make_unique<Module>("my cool jit", context);
    module = M.get();
    builder = std::make_unique<IRBuilder<>>(context);
    eng.reset(EngineBuilder(std::move(M)).create());
}


IRRenderer::~IRRenderer() {

}

unique_ptr<ExecutionEngine>& IRRenderer::engine()
{
    return eng; 
}

llvm::LLVMContext & IRRenderer::llvm_context() {
    return context;
}

llvm::AllocaInst *
IRRenderer::get_named_value(const std::string &name) {
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

    return tmp_builder.CreateAlloca(Type::getDoubleTy(context),
        0,
        name.c_str());
}
