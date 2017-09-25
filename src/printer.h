//
// Created by user on 25.08.17.
//

#ifndef KHAOTICA_PRINTER_H
#define KHAOTICA_PRINTER_H

#include "syntax.h"
#include "bit.h"
#include "eval.h"

#include <iostream>

namespace khaotica{
    class printer_t{
        class impl_t{

            template<size_t indent = 2>
            struct indent_t{
                explicit indent_t(size_t& alignment):alignment(alignment){
                    alignment += indent;
                }
                ~indent_t(){
                    alignment -= indent;
                }
                size_t& alignment;
            };

            class echo_t {
            public:
                echo_t(std::ostream& out):out(out){}

                void operator()(const flavor::variable_t& node ){
                    out << node.name;
                }
                void operator()(const flavor::integer_t& node ){
                    out << node.value;
                }
                void operator()(const flavor::bitstring_t& node ){
                    out << "'" << node.value << "'";
                }
                void operator()(const flavor::unary_expression_t& node ){
                    out << "( OP ";
                    (*this)(node.operand);
                }
                void operator()(const flavor::postincrement_t& node ){
                    out << node.operand.name << " -< " << node.operand.name << " OP " << 1;
                }
                void operator()(const flavor::preincrement_t& node ){
                    out << node.operand.name << " >- " << node.operand.name << " OP " << 1;
                }
                void operator()(const flavor::binary_expression_t& node ){
                    out << " ( ";
                    (*this)(node.left_operand);
                    out << " OP ";
                    (*this)(node.right_operand);
                    out << " ) ";
                }
                void operator()(const std::shared_ptr<const flavor::expression_t>& node ){
                    std::visit(*this, node->sentence);
                }

                std::ostream& out;
            };
        public:
            explicit impl_t(std::ostream& out, const flavor::symbols_t& symbols, const flavor::values_t& values = {})
                : out(out)
                , symbols(symbols)
                , values(values){
            }

            void operator( )(const flavor::bslbf_t& node) {
                // FIXME: Check absence, be robust, be flexible to print syntax tree only
                const auto& value = std::get<std::vector<bool>>(values.at(node.name).value);

                out << std::string(alignment, ' ')
                    << node.name << "(" << node.length << ")"
                    << " -> " << khaotica::algorithm::to_hex(value) << "(" << khaotica::algorithm::to_string(value) << ")"
                    << std::endl;
            }

            void operator( )(const flavor::uimsbf_t& node) {
                const auto& value = std::get<uint64_t>(values.at(node.name).value);
                out << std::string(alignment, ' ')
                    << node.name << "(" << node.length << ")"
                    << " -> " << khaotica::algorithm::to_hex(value) << "(" << value << ")"
                    << std::endl;
            }

            void operator( )(const flavor::tcimsbf_t& node) {
                out << std::string(alignment, ' ')
                    << node.name
                    << "(" << node.length << ")"
                    << std::endl;
            }

            void operator( )(const flavor::compound_t& node) {
                out << std::string(alignment, ' ') << node.name << "()";

                auto it = symbols.find(node.name);
                if(it != symbols.end()){

                    out << "{" << std::endl;
                    auto&& definition = it->second;
                    {
                        indent_t<> indent(alignment);
                        std::visit(*this, definition);
                    }
                    out << std::string(alignment, ' ') << "}" << std::endl;
                } else {
                    out << " <------!!! Not defined symbol" << std::endl;
                }
            }

            void operator( )(const flavor::variable_t& node) {
                out << node.name;

                auto it = symbols.find(node.name);
                if( it != symbols.end()){
                    out << " = ";
                    std::visit(*this, it->second);
                } else {
                    out << "(!!!!)";
                }
            }

            void operator( )(const flavor::if_t& node) {
                out << std::string(alignment, ' ') << "if( " ;
                (*this)(*node.condition);
                out << ")";

                out << " -> if( ";
                auto value = flavor::to_boolean(eval_t(symbols, values)(node.condition));
                out << value;
                out << " )";
                out << "{" << std::endl;

                if(value){
                    indent_t<> indent(alignment);
                    (*this)(*node._then);
                } else if( node._else ){
                    out << "} else {" << std::endl;
                    indent_t<> indent(alignment);
                    (*this)(**node._else);
                }

                out << std::string(alignment, ' ') << "}" << std::endl;
            }

            void operator( )(const flavor::for_t& node) {
                out << std::string(alignment, ' ') << "for( " ;
                if(node.counter){
                    (*this)(*node.counter);
                }
                out << "; ";
                if(node.condition){
                    (*this)(**node.condition);
                }
                out << "; ";
                if(node.modifier){
                    (*this)(**node.modifier);
                }
                out << ")";

                out << "{" << std::endl;

                {
                    indent_t<> indent(alignment);
                    (*this)(*node.body);
                }
                out << std::string(alignment, ' ') << "}" << std::endl;
            }

            void operator( )(const flavor::compound_definition_t& node) {
                for (auto &&entry : node.entries) {
                    std::visit(*this, entry);
                }
            }

            void operator( )(const flavor::expression_t& node) {
                std::visit(echo_t(out), node.sentence);
            }

        private:
            std::ostream& out;
            const flavor::symbols_t& symbols;
            const flavor::values_t& values;
            size_t alignment{0};
        };
    public:
        static void print(std::ostream& out, const flavor::document_t& doc, const flavor::symbols_t& symbols, const flavor::values_t& values){
            impl_t impl(out, symbols, values);
            for(auto&& entry : doc){
                std::visit(impl, entry);
            }

            out << std::endl;
        }
    };
}
#endif //KHAOTICA_PRINTER_H
