#pragma once

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>

using ::std::map;
using ::std::string;
using ::std::unique_ptr;

using ::llvm::AllocaInst;
using ::llvm::ExecutionEngine;
using ::llvm::Function;
using ::llvm::IRBuilder;
using ::llvm::LLVMContext;
using ::llvm::Module;


class IRRenderer {

    std::ifstream& _bitstream;
    std::bitset<8> cache;
    uint8_t pos;
public:
    IRRenderer(std::ifstream&);
    ~IRRenderer();

    uint8_t read_bits(size_t count) {

        std::bitset<8> ret(0);
       
        for (int i = 1; i <= count; i++)
        {
            ret[count - i] = cache[pos-1];
            pos--;

            if (pos == 0) {
                uint8_t t;
                _bitstream >> t;
                cache = decltype(cache)(t);
                pos = 8;
            }
        }
               
        return ret.to_ulong();
    }
};
