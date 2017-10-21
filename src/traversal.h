#pragma once

#include "syntax.h"

#include <cassert>

namespace khaotica {

    template<typename P>
    class traversal_t{
    public:
        traversal_t(const flavor::definitions_t& symbols, P& p):symbols(symbols), p(p){

        }

        void operator()(const flavor::bslbf_t& node){
            p.on(node, *this);
        }

        void operator()(const flavor::uimsbf_t& node){
            p.on(node, *this);
        }

        void operator()(const flavor::tcimsbf_t& node){
            p.on(node, *this);
        }

        void operator()(const flavor::if_t& node){
            p.on(node, *this);
        }

        void operator()(const flavor::for_t& node){
            p.on(node, *this);
        }

        void operator()(const flavor::compound_definition_t& node){
            for (auto &&entry : node.entries) {
                std::visit(*this, entry);
            }
        }

        void operator()(const flavor::expression_t& node){
            std::visit(*this, node.sentence);
        }

        void operator()(const flavor::symbol_t& node){
            std::visit([this, &node](const auto& definition){
                p.on(node, definition, *this);
            }, symbols.at(node.name));
        }

        void operator()( const flavor::integer_t& node){
            p.on(node, *this);
        }
        void operator()( const flavor::bitstring_t& node){
            p.on(node, *this);
        }
        void operator()( const flavor::unary_expression_t& node){
            std::visit( [this, &node](const auto& operation){
                p.on(operation, *node.operand, *this);
            }, node.operation);

        }
        void operator()( const flavor::binary_expression_t& node){
            std::visit( [this, &node](const auto& operation){
                p.on(*node.left_operand, operation, *node.right_operand, *this);
            }, node.operation);
        }
        void operator()( const flavor::postincrement_t& node){
            p.on(node, *this);
        }
        void operator()( const flavor::preincrement_t& node){
            p.on(node, *this);
        }
        void operator()( const flavor::assignment_t& node){
            p.on(node, *this);
        }

        const flavor::definitions_t& symbols;
        P& p;
    };
}
