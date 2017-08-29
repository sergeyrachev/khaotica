#pragma once

#include "syntax.h"
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
    struct operation_t{
        flavor::value_t operator()(const auto& operation, const auto& operand){
            assert((false, "It is unlikely to call this operation on this value"));
            return {};
        }

        flavor::value_t operator()(const std::negate<>& op, const int64_t& v){
            return op(v);
        }

        flavor::value_t operator()(const std::logical_not<>& op, const int64_t& v){
            return op(v);
        }

        flavor::value_t operator()(const std::logical_not<>& op, const uint64_t& v){
            return op(v);
        }

        flavor::value_t operator()(const std::bit_not<>& op, const std::vector<bool>& v){
            auto ret = v;
            ret.flip();
            return ret;
        }

        flavor::value_t operator()(const std::bit_not<>& op, const uint64_t& v){
            return op(v);
        }

        flavor::value_t operator()(const std::bit_not<>& op, const int64_t& v){
            return op(v);
        }

        flavor::value_t operator()(const auto& left_operand, const auto& operation, const auto& right_operand){
            assert((false, "It is unlikely to call operation with these values"));
            return {};
        }

        flavor::value_t operator()(const std::vector<bool>& left_operand, const std::equal_to<>& operation, const std::vector<bool>& right_operand){
            auto l = khaotica::algorithm::to_string(left_operand);
            auto r = khaotica::algorithm::to_string(right_operand);
            return operation(left_operand, right_operand);
        }

        flavor::value_t operator()(const bool& left_operand, const std::logical_or<>& operation, const bool& right_operand){
            return operation(left_operand, right_operand);
        }
    };

    class eval_t{
    public:
        eval_t(const flavor::symbols_t& symbols, const flavor::values_t& values ):symbols(symbols), values(values){

        }

        flavor::value_t operator()(const flavor::bitstring_t& node) {
            return khaotica::algorithm::unpack(node.value);
        }

        flavor::value_t operator()(const flavor::integer_t& node) {
            return node.value;
        }

        flavor::value_t operator()(const flavor::variable_t& node) {

            auto prev_value = values.find(node.name);
            if( prev_value != values.end()){
                return prev_value->second;
            }

            auto it = symbols.find(node.name);
            assert( it != symbols.end());
            auto&& def = std::get<flavor::expression_t>(it->second);
            auto value = std::visit(*this, def.sentence);
            return value;
        }

        flavor::value_t operator()(const flavor::unary_expression_t& node) {
            auto operand = (*this)(node.operand);
            return std::visit(operation_t(), node.operation, operand);
        }

        flavor::value_t operator()(const flavor::binary_expression_t& node) {
            auto left_operand = (*this)(node.left_operand);
            auto right_operand = (*this)(node.right_operand);
            return std::visit(operation_t(), left_operand, node.operation, right_operand);
        }

        flavor::value_t operator()(const std::shared_ptr<const flavor::expression_t>& node) {
            return std::visit(*this, node->sentence);
        }

    private:
        const flavor::symbols_t& symbols;
        const flavor::values_t& values;
    };
}

