#include "printer.h"

namespace {
    class print_t : public flavor::traversal_t{
    public:
        explicit print_t(std::ostream& out):out(out), indentation(0){

        }

        void on(flavor::bslbf_t& node ) final {
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "bslbf"
                << std::endl;
        };
        void on(flavor::uimsbf_t& node )final {
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "uimsbf"
                << std::endl;
        };
        void on(flavor::bitstring_t& node )final {
            out << "'" << node.value << "'";
        };
        void on(flavor::integer_t& node )final {
            out << node.value;
        };
        void on(flavor::identifier_t& node )final {
            out << node.name;
        };
        void on(flavor::if_t& node )final {
            out << std::string(indentation, ' ') << "if" << "( ";

            node.condition->process(*this);

            out << " )";

            node._then->process(*this);

            if(node._else){
                out << "else";
                (*node._else)->process(*this);
            }

            out << std::endl;
        };
        void on(flavor::for_t& node )final {
            out << std::string(indentation, ' ') << "for" << "(";

            if(node.initializer){
                (*node.initializer)->process(*this);
            }

            out << ";";

            if(node.condition){
                (*node.condition)->process(*this);
            }

            out << ";";

            if(node.modifier){
                (*node.modifier)->process(*this);
            }

            out << ")";

            node.body->process(*this);

            out << std::endl;
        };
        void on(flavor::compound_t& node )final {
            out<< std::string(indentation, ' ') << node.name << "()";
            if(node.body){
                node.body->process(*this);
            }
            out << std::endl;
        };

        void on(flavor::block_t& node) final {
            out << "{" << std::endl;
            indentation++;

            for (auto &&entry : node.entries) {
                entry->process(*this);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}";
        };

        void on(flavor::assignment_t& node )final {
            out << node.symbol << "=";
            node.expression->process(*this);
        };
        void on(flavor::preincrement_t<std::plus<>>& node )final {
            out << "( " << "++" << node.operand << " )";
        };
        void on(flavor::preincrement_t<std::minus<>>& node )final {
            out << "( " << "--" << node.operand << " )";
        };
        void on(flavor::postincrement_t<std::plus<>>& node )final {
            out << "( " << node.operand << "++" << " )";
        };
        void on(flavor::postincrement_t<std::minus<>>& node )final {
            out << "( " << node.operand << "--" << " )";
        };
        void on(flavor::unary_expression_t<std::bit_not<>>& node )final {
            out << "( " << "~";
            node.operand->process(*this);
            out << " )";
        };
        void on(flavor::unary_expression_t<std::minus<>>& node )final {
            out << "( " << "-";
            node.operand->process(*this);
            out << " )";
        };
        void on(flavor::unary_expression_t<std::logical_not<>>& node )final {
            out << "( " << "!";
            node.operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::plus<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "+";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::minus<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "-";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::multiplies<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "*";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::divides<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "/";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::modulus<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "%";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::less<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "<";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::greater<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << ">";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::less_equal<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "<=";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::greater_equal<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << ">=";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::equal_to<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "==";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::not_equal_to<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "!=";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::logical_and<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "&&";
            node.right_operand->process(*this);
            out << " )";
        };
        void on(flavor::binary_expression_t<std::logical_or<>>& node )final {
            out << "( ";
            node.left_operand->process(*this);
            out << "||";
            node.right_operand->process(*this);
            out << " )";
        };
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
