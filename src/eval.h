#pragma once

#include "grammar.h"
#include "logging.h"
#include "bit.h"
#include "bitreader.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>
#include <cassert>

namespace khaotica{
//    struct operation_t{
//        flavor::value_t operator()(const auto& operation, const auto& operand){
//            assert(false && "It is unlikely to call this operation on this value");
//            return {};
//        }
//

//    };
//
//    class eval_t{
//    public:
//        eval_t(const flavor::symbols_t& symbols, const flavor::values_t& values ):symbols(symbols), values(values){
//
//        }
//

//
//        flavor::value_t operator()(const std::shared_ptr<const flavor::expression_t>& node) {
//            return std::visit(*this, node->sentence);
//        }
//
//    private:
//        const flavor::symbols_t& symbols;
//        const flavor::values_t& values;
//    };
}

