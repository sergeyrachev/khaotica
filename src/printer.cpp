#include "printer.h"

namespace {
    class print_t{
    public:
        explicit print_t(std::ostream& out, const flavor::document_t &doc):out(out), indentation(0), doc(doc){

        }

        void on(std::shared_ptr<flavor::node_t> node){
            std::visit(*this, node->payload);
        }

        void operator()(flavor::bslbf_t& node )  {
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "bslbf"
                << std::endl;
        };
        void operator()(flavor::uimsbf_t& node  )  {
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "uimsbf"
                << std::endl;
        };
        void operator()(flavor::bitstring_t& node  )  {
            out << "'" << node.value << "'";
        };
        void operator()(flavor::integer_t& node  )  {
            out << node.value;
        };
        void operator()(flavor::identifier_t& node  )  {
            out << node.name;
        };
        void operator()(flavor::reference_t& node  )  {
            out << std::string(indentation, ' ') << node.name << "() -> { " << std::endl;
            indentation++;

            auto& compound = std::get<flavor::compound_t>(doc.definitions.at(node.name)->payload);

            for (auto &&item : compound.body) {
                on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
        };
        void operator()(flavor::if_t& node  )  {
            out << std::string(indentation, ' ') << "if" << "( ";

            on(node.condition);

            out << " ) {" << std::endl;

            indentation++;
            for (auto &&item : node._then) {
                on(item);
            }

            out << (node._else.empty()?"":"}else{");

            for (auto &&item : node._else) {
                on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
        };
        void operator()(flavor::for_t& node  )  {
            out << std::string(indentation, ' ') << "for" << "(";

            if(node.initializer){
                on(*node.initializer);
            }

            out << ";";

            if(node.condition){
                on(*node.condition);
            }

            out << ";";

            if(node.modifier){
                on(*node.modifier);
            }

            out << ") {" << std::endl;
            indentation++;

            for (auto &&item : node.body) {
                on(item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
        };
        void operator()(flavor::compound_t& node  )  {
            out<< std::string(indentation, ' ') << node.name << "() {" << std::endl;
            indentation++;
            for (auto &&item :node.body) {
                on(item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" <<std::endl;
        };


        void operator()(flavor::assignment_t& node  )  {
            out << node.symbol << "=";
            on(node.expression);
        };
        void operator()(flavor::preincrement_t& node  )  {
            out << "( " << "??" << node.operand << " )";
        };

        void operator()(flavor::postincrement_t& node  )  {
            out << "( " << node.operand << "++" << " )";
        };

        void operator()(flavor::unary_expression_t& node  )  {
            out << "( " << "~";
            on(node.operand);
            out << " )";
        };

        void operator()(flavor::binary_expression_t& node  )  {
            out << "( ";
            on(node.left_operand);
            out << "+";
            on(node.right_operand);
            out << " )";
        };

    private:
        std::ostream& out;
        size_t indentation;
        const flavor::document_t &doc;
    };
}

void khaotica::printer_t::print(const flavor::document_t &doc, std::ostream &out) {

    print_t print(out, doc);

    for(auto&& entry : doc.structure){
        print.on(entry);
    }
}
