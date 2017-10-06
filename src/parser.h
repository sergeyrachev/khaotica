#pragma once

#include "syntax.h"
#include "logging.h"
#include "bit.h"
#include "bitreader.h"
#include "eval.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>
#include <cassert>

namespace khaotica {
    class parser_t {
//        class impl_t{
//            flavor::value_t eval(const flavor::expression_t& def){
//                return std::visit(eval_t(symbols, values), def.sentence);
//            }
//        public:
//            explicit impl_t(bitreader_t& bitreader, const flavor::symbols_t& symbols, flavor::values_t& values)
//                : bitreader(bitreader), symbols(symbols), values(values) {
//
//            }
//
//            void operator( )(const flavor::bslbf_t &bs) {
//                auto value = bitreader.read(bs.length);
//                values.emplace(bs.name, flavor::value_t{value});
//            }
//
//            void operator( )(const flavor::uimsbf_t &bs) {
//                auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(bs.length));
//                values.emplace(bs.name, flavor::value_t{value});
//            }
//
//            void operator( )(const flavor::compound_t &bs) {
//                auto it = symbols.find(bs.name);
//                assert( it != symbols.end() );
//                const auto& compound_definition = it->second;
//                std::visit(*this, compound_definition);
//
//            }
//
//            void operator()(const flavor::compound_definition_t& bs){
//                for (const auto &entry : bs.entries) {
//                    std::visit(*this, entry);
//                }
//            }
//
//            void operator( )(const flavor::variable_t& node) {
//                auto it = symbols.find(node.name);
//                assert( it != symbols.end());
//                values.emplace(node.name, eval(std::get<flavor::expression_t>(it->second)));
//            }
//
//            void operator( )(const flavor::if_t& node) {
//                auto value = flavor::to_boolean(eval_t(symbols, values)(node.condition));
//                if(value){
//                    (*this)(*node._then);
//                } else if(node._else){
//                    (*this)(**node._else);
//                }
//            }
//
//            void operator( )(const flavor::for_t& node) {
//
//                values.emplace(node.counter->name, eval(**node.initializer));
//
//                while(auto condition = flavor::to_boolean(eval(**node.condition))){
//
//                    (*this)(*node.body);
//
//                    values[node.counter->name] = eval(**node.modifier);
//                }
//
//                values.erase(node.counter->name);
//            }
//
//            void operator()(const auto &) {
//                flavor::values_t values;
//            }
//
//        private:
//            bitreader_t& bitreader;
//            const flavor::symbols_t& symbols;
//            flavor::values_t& values;
//        };

    public:
        static flavor::values_t parse(std::ifstream& in, const flavor::document_t& doc, const flavor::symbols_t& symbols){
            bitreader_t bitreader(in);
            flavor::values_t values;
            //impl_t impl(bitreader, symbols, values);
            for (auto &&entry : doc) {
               // std::visit(impl, entry);
            }

            return values;
        }
    };
}
