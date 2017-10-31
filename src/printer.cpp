#include "printer.h"

namespace {
    class print_t{
    public:
        explicit print_t(const flavor::document_t &doc):indentation(0), doc(doc){

        }

        std::string on(const flavor::node_t& node){
            return std::visit(*this, node.payload);
        }

        std::string operator()(const flavor::bslbf_t& node )  {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "bslbf"
                << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::uimsbf_t& node  )  {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << node.length
                << " "
                << "uimsbf"
                << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::bitstring_t& node  )  {
            std::ostringstream out;
            out << "'" << node.value << "'";
            return out.str();
        };
        std::string operator()(const flavor::integer_t& node  )  {
            std::ostringstream out;
            out << node.value;
            return out.str();
        };
        std::string operator()(const flavor::identifier_t& node  )  {
            std::ostringstream out;
            out << node.name;
            return out.str();
        };
        std::string operator()(const flavor::reference_t& node  )  {
            std::ostringstream out;

            out << std::string(indentation, ' ') << node.name << "() -> { " << std::endl;
            indentation++;

            auto& compound = std::get<flavor::compound_t>(doc.definitions.at(node.name)->payload);

            for (auto &&item : compound.body) {
                out << on(*item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::if_t& node  )  {
            std::ostringstream out;

            out << std::string(indentation, ' ') << "if" << "( ";

            out << on(*node.condition);

            out << " ) {" << std::endl;

            indentation++;
            for (auto &&item : node._then) {
                out << on(*item);
            }

            out << (node._else.empty()?"":"}else{");

            for (auto &&item : node._else) {
                out << on(*item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::for_t& node  )  {
            std::ostringstream out;
            out << std::string(indentation, ' ') << "for" << "(";

            if(node.initializer){
                out << on(**node.initializer);
            }

            out << ";";

            if(node.condition){
                out << on(**node.condition);
            }

            out << ";";

            if(node.modifier){
                out << on(**node.modifier);
            }

            out << ") {" << std::endl;
            indentation++;

            for (auto &&item : node.body) {
                out << on(*item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::compound_t& node  )  {
            std::ostringstream out;
            out<< std::string(indentation, ' ') << node.name << "() {" << std::endl;
            indentation++;
            for (auto &&item :node.body) {
                out << on(*item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" <<std::endl;
            return out.str();
        };


        std::string operator()(const flavor::assignment_t& node  )  {
            std::ostringstream out;
            out << node.symbol << "=";
            out << on(*node.expression);
            return out.str();
        };
        std::string operator()(const flavor::preincrement_t& node  )  {
            std::ostringstream out;
            out << "( " << "??" << node.operand << " )";
            return out.str();
        };

        std::string operator()(const flavor::postincrement_t& node  )  {
            std::ostringstream out;
            out << "( " << node.operand << "++" << " )";
            return out.str();
        };

        std::string operator()(const flavor::unary_expression_t& node  )  {
            std::ostringstream out;
            out << "( " << "~";
            out << on(*node.operand);
            out << " )";
            return out.str();
        };

        std::string operator()(const flavor::binary_expression_t& node  )  {
            std::ostringstream out;
            out << "( ";
            out << on(*node.left_operand);
            out << "+";
            out << on(*node.right_operand);
            out << " )";
            return out.str();
        };

    private:
        size_t indentation;
        const flavor::document_t &doc;
    };
}

void khaotica::printer_t::print(const flavor::document_t &doc, std::ostream &out) {

    print_t print(doc);

    for(auto&& entry : doc.structure){
        out << print.on(*entry);
    }
}
