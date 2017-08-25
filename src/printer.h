//
// Created by user on 25.08.17.
//

#ifndef KHAOTICA_PRINTER_H
#define KHAOTICA_PRINTER_H

#include "syntax.h"

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

        public:
            explicit impl_t(std::ostream& out, const flavor::symbols_t& symbols)
                : out(out)
                , symbols(symbols){

            }

            void operator( )(const flavor::bslbf_t& value) {
                out << std::string(alignment, ' ')
                    << value.name
                    << "(" << value.length << ")"
                    << std::endl;
            }

            void operator( )(const flavor::uimsbf_t& value) {
                out << std::string(alignment, ' ')
                    << value.name
                    << "(" << value.length << ")"
                    << std::endl;
            }

            void operator( )(const flavor::tcimsbf_t& value) {
                out << std::string(alignment, ' ')
                    << value.name
                    << "(" << value.length << ")"
                    << std::endl;
            }

            void operator( )(const flavor::compound_t& value) {
                out << std::string(alignment, ' ') << value.name << "()";

                auto it = symbols.find(value.name);
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

            void operator( )(const flavor::bitstring_t& value) {
                out << "'" << value.value << "'";
            }

            void operator( )(const flavor::integer_t& value) {
                out << value.value;
            }

            void operator( )(const flavor::variable_t& value) {
                out << value.name;

                auto it = symbols.find(value.name);
                if( it != symbols.end()){
                    out << " = ";
                    std::visit(*this, it->second);
                } else {
                    out << "(!!!!)";
                }
            }

            void operator( )(const flavor::if_t& value) {
                out << std::string(alignment, ' ') << "if( " ;
                (*this)(*value.condition);
                out << ")";

                {
                    out << "{" << std::endl;
                    indent_t<> indent(alignment);
                    (*this)(*value._then);

                }

                if( value._else){
                    out << "} else {" << std::endl;
                    indent_t<> indent(alignment);
                    (*this)(**value._else);
                }
                out << std::string(alignment, ' ') << "}" << std::endl;
            }

            void operator( )(const flavor::for_t& value) {
                out << std::string(alignment, ' ') << "for( " ;
                if(value.counter){
                    (*this)(*value.counter);
                }
                out << "; ";
                if(value.condition){
                    (*this)(**value.condition);
                }
                out << "; ";
                if(value.modifier){
                    (*this)(**value.modifier);
                }
                out << ")";

                out << "{" << std::endl;
                {
                    indent_t<> indent(alignment);
                    (*this)(*value.body);
                }
                out << std::string(alignment, ' ') << "}" << std::endl;
            }

            void operator( )(const flavor::compound_definition_t& value) {
                for (auto &&entry : value.entries) {
                    std::visit(*this, entry);
                }
            }

            void operator( )(const flavor::expression_t& value) {
                std::visit(*this, value.sentence);
            }

            void operator( )(const flavor::unary_expression_t& value) {
                out << "( OP ";
                (*this)(*value.operand);
                out << " )";
            }

            void operator( )(const flavor::binary_expression_t& value) {

                out << "( ";
                (*this)(*value.left_operand);
                out << " OP ";
                (*this)(*value.right_operand);
                out << " )";
            }

            void operator( )(const std::shared_ptr<flavor::expression_t>& value) {
                (*this)(*value);
            }

            void operator( )(const std::string& value) {
                out << value;
            }

        private:
            std::ostream& out;
            const flavor::symbols_t& symbols;
            size_t alignment{0};

        };
    public:
        printer_t(std::ostream& out, const flavor::document_t& doc, const flavor::symbols_t& symbols){
            impl_t impl(out, symbols);
            for(auto&& entry : doc){
                std::visit(impl, entry);
            }
        }

    private:

    };
}
#endif //KHAOTICA_PRINTER_H
