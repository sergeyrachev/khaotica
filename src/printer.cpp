#include "printer.h"

#include <boost/core/demangle.hpp>

namespace {
    class print_t{
    public:
        explicit print_t(std::ostream& out):out(out), indentation(0){

        }

        template<typename T>
        void traversal(T& v){

        }

        void on(const flavor::bslbf_t &node){
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "bslbf"
                << std::endl;
        }

        void on(const flavor::uimsbf_t &node){
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "uimsbf"
                << std::endl;
        }
        
        void on(const flavor::if_t& node){
            out << std::string(indentation, ' ') << "if" << "( ";

            traversal(*node.condition);

            out << " )";

            on_(*node._then);

            if(node._else){
                out << "else";
                on_(**node._else);
            }

            out << std::endl;
        }

        void on(const flavor::for_t& node){
            out << std::string(indentation, ' ') << "for" << "(";

            if(node.initializer){
                traversal(**node.initializer);
            }

            out << ";";

            if(node.condition){
                traversal(**node.condition);
            }

            out << ";";

            if(node.modifier){
                traversal(**node.modifier);
            }

            out << ")";

            on_(*node.body);

            out << std::endl;
        }

        void on( const flavor::integer_t& node){
            out << node.value;
        }

        void on( const flavor::bitstring_t& node){
            out << "'" << node.value << "'";
        }

        void on( const auto& operation, const flavor::expression_t& operand){
            out << "( ";
            out << boost::core::demangle(typeid(operation).name()) << "(";
            traversal(operand);
            out << ") )";
        }

        void on( const flavor::expression_t& left, const auto& operation, const flavor::expression_t& right){
            out << "( ";
            traversal(left);
            out << " " << boost::core::demangle(typeid(operation).name()) << " ";
            traversal(right);
            out << " )";
        }

        void on( const flavor::postincrement_t& node){
            out << "(" << node.operand << "??" <<")";
        }

        void on( const flavor::preincrement_t& node){
            out << "( ??" << node.operand <<")";
        }

        void on( const flavor::assignment_t& node){
            out << node.symbol << "=";
            traversal(*node.expression);
        }

        void on( const flavor::identifier_t& node, const flavor::bslbf_t& definition){
            out << node.name << "<" <<  "bslbf("<< definition.length << ")>";
        }

        void on( const flavor::symbol_t& node, const flavor::uimsbf_t& definition){
            out << node.name << "<" <<  "uimsbf("<< definition.length << ")>";
        }

        void on( const flavor::symbol_t& node, const flavor::tcimsbf_t& definition){
            out << node.name << "<" <<  "tcimsbf("<< definition.length << ")>";
        }

        void on( const flavor::symbol_t& node, const flavor::compound_definition_t& definition){
            out << std::string(indentation, ' ') << node.name << "()<";

            on_(definition, traversal);

            out << ">" << std::endl;
        }

        void on( const flavor::symbol_t& node, const flavor::assignment_t& definition){
            out << node.name << "<";
            traversal(*definition.expression);
            out << ">";
        }
    private:
        void on_(const flavor::compound_definition_t& body){
            out << "{" << std::endl;
            indentation++;

            for (auto &&entry : body.entries) {
                //std::visit(traversal, entry);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}";
        }

    private:
        std::ostream& out;
        size_t indentation;
    };
}

void khaotica::printer_t::print(const flavor::document_t &doc, std::ostream &out) {

    print_t print(out);


    for(auto&& entry : doc.hierarchy){
        entry->process(print);
    }
}
