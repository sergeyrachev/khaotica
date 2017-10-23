#pragma once

#include "syntax.h"

#include <cassert>

namespace khaotica {

    template<typename P, typename V = void>
    class traversal_t{
    public:
        traversal_t(const flavor::definitions_t& symbols, P& p):symbols(symbols), p(p){

        }

        V operator()(const flavor::bslbf_t& node){
            return p.on(node, *this);
        }

        V operator()(const flavor::uimsbf_t& node){
            return p.on(node, *this);
        }

        V operator()(const flavor::tcimsbf_t& node){
            return p.on(node, *this);
        }

        V operator()(const flavor::if_t& node){
            return p.on(node, *this);
        }

        V operator()(const flavor::for_t& node){
            return p.on(node, *this);
        }

        V operator()(const flavor::expression_t& node){
            return std::visit(*this, node.sentence);
        }

        V operator()(const flavor::symbol_t& node){
            return std::visit([this, &node](const auto& definition){
                return p.on(node, definition, *this);
            }, symbols.at(node.name));
        }

        V operator()( const flavor::integer_t& node){
            return p.on(node, *this);
        }

        V operator()( const flavor::bitstring_t& node){
            return p.on(node, *this);
        }

        V operator()( const flavor::unary_expression_t& node){
            return std::visit( [this, &node](const auto& operation){
                return p.on(operation, *node.operand, *this);
            }, node.operation);
        }

        V operator()( const flavor::binary_expression_t& node){
            return std::visit( [this, &node](const auto& operation){
                return p.on(*node.left_operand, operation, *node.right_operand, *this);
            }, node.operation);
        }

        V operator()( const flavor::postincrement_t& node){
            return p.on(node, *this);
        }

        V operator()( const flavor::preincrement_t& node){
            return p.on(node, *this);
        }

        V operator()( const flavor::assignment_t& node){
            return p.on(node, *this);
        }

    private:
        const flavor::definitions_t& symbols;
        P& p;
    };
}
