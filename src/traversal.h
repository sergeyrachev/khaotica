#pragma once

#include "syntax.h"

#include <cassert>

namespace khaotica {

    template<typename P>
    class traversal_t{
    public:
        traversal_t(const flavor::symbols_t& symbols, P& p):symbols(symbols), p(p){

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

        void operator()(const flavor::compound_t& node){
            auto && definition = std::get<flavor::compound_definition_t>(symbols.at(node.name));
            p.on(node, definition, *this);
        }

        void operator()(const flavor::compound_definition_t& node){
            for (auto &&entry : node.entries) {
                std::visit(*this, entry);
            }
        }

        void operator()(const flavor::expression_t& node){
            std::visit(*this, node.sentence);
        }

        void operator()(const flavor::variable_t& node){
            assert( false && "Should never be here. AST should not contain free variables");
        }

        void operator()(const flavor::field_t& node){
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

        const flavor::symbols_t& symbols;
        P& p;
    };
}
