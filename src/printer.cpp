#include "printer.h"

namespace {

    class dump_t{
    public:
        explicit dump_t(const flavor::document_t &doc, const flavor::snapshot_t& snapshot):indentation(0), doc(doc), snapshot(snapshot){

        }

        std::string on( std::shared_ptr<flavor::node_t> node ){
            auto value = snapshot.at(node);
            return std::visit(*this, value->payload);
        }

        std::string operator()(const std::pair<flavor::bslbf_t, std::vector<bool>>& node )  {
            std::ostringstream out;

            const auto& field = node.first;
            const auto& value = node.second;

            out << std::string(indentation, ' ')
                << field.name
                << " "
                << field.length
                << " "
                << "bslbf"
                << " -> "
                << khaotica::algorithm::to_string(value)
                << "(" << khaotica::algorithm::to_hex(value) << ")"
                << std::endl;
            return out.str();
        };

        std::string operator()(const std::pair<flavor::uimsbf_t, uint64_t>&node){
            std::ostringstream out;
            const auto& field = node.first;
            const auto& value = node.second;
            out << std::string(indentation, ' ')
                << field.name
                << " "
                << field.length
                << " "
                << "uimsbf"
                << " -> "
                << value
                << "(0x" << khaotica::algorithm::to_hex(value) << ")"
                << std::endl;
            return out.str();
        }

        std::string operator()(const std::pair<flavor::compound_t, std::list<std::shared_ptr<flavor::value_t>>>&node){

            const auto& field = node.first;
            const auto& value = node.second;

            std::ostringstream out;
            out<< std::string(indentation, ' ') << field.name << "() {" << std::endl;
            indentation++;
            for (auto &&item : value) {
                out << std::visit(*this, item->payload);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" <<std::endl;
            return out.str();
        }
        std::string operator()(const std::pair<flavor::if_t, flavor::if_v> &node){
            const auto& field = node.first;
            const auto& value = node.second;

            std::ostringstream out;

            out << std::string(indentation, ' ') << "if" << "( " << value.condition << " ) -> " << (value.condition ? "then" : "else");
            out << "{" << std::endl;

            indentation++;
            for (auto &&item : value.value) {
                out << std::visit(*this, item->payload);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        }
        std::string operator()(const std::pair<flavor::for_t, flavor::for_v> &node){
            const auto& field = node.first;
            const auto& value = node.second;

            std::ostringstream out;
            out << std::string(indentation, ' ') << "for" << "(" << value.value.size() << " iterations";

            out << ") {" << std::endl;
            indentation++;

            for (auto &&iteration : value.value) {
                out << std::string(indentation, ' ') << "[" << std::endl;
                indentation++;
                for (auto &&item : iteration) {
                    out << std::visit(*this, item->payload);
                }
                indentation--;
                out << std::string(indentation, ' ') << "] " << std::endl;
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        }
        std::string operator()(const std::pair<flavor::expression_t, flavor::expression_v> &node){
            return "'expr'";
        }

    private:
        size_t indentation;
        const flavor::document_t &doc;
        const flavor::snapshot_t &snapshot;
    };

    class print_t{
    public:
        explicit print_t(const flavor::document_t &doc):indentation(0), doc(doc){

        }

        std::string on( std::shared_ptr<flavor::node_t> node){
            return std::visit(*this, node->payload);
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
                out << on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::if_t& node  )  {
            std::ostringstream out;

            out << std::string(indentation, ' ') << "if" << "( ";

            out << on(node.condition);

            out << " ) {" << std::endl;

            indentation++;
            for (auto &&item : node._then) {
                out << on(item);
            }

            out << (node._else.empty()?"":"}else{");

            for (auto &&item : node._else) {
                out << on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };
        std::string operator()(const flavor::for_t& node  )  {
            std::ostringstream out;
            out << std::string(indentation, ' ') << "for" << "(";

            if(node.initializer){
                out << on(*node.initializer);
            }

            out << ";";

            if(node.condition){
                out << on(*node.condition);
            }

            out << ";";

            if(node.modifier){
                out << on(*node.modifier);
            }

            out << ") {" << std::endl;
            indentation++;

            for (auto &&item : node.body) {
                out << on(item);
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
                out << on(item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" <<std::endl;
            return out.str();
        };


        std::string operator()(const flavor::assignment_t& node  )  {
            std::ostringstream out;
            out << node.symbol << "=";
            out << on(node.expression);
            return out.str();
        };
        std::string operator()(const flavor::preincrement_t& node  )  {
            std::ostringstream out;
            out << "( " << node.operation << node.operation << node.operand << " )";
            return out.str();
        };

        std::string operator()(const flavor::postincrement_t& node  )  {
            std::ostringstream out;
            out << "( " << node.operand << node.operation << node.operation << " )";
            return out.str();
        };

        std::string operator()(const flavor::unary_expression_t& node  )  {
            std::ostringstream out;
            out << "( " << node.operation;
            out << on(node.operand);
            out << " )";
            return out.str();
        };

        std::string operator()(const flavor::binary_expression_t& node  )  {
            std::ostringstream out;
            out << "( ";
            out << on(node.left_operand);
            out << node.operation;
            out << on(node.right_operand);
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
        out << print.on(entry);
    }
}

void khaotica::printer_t::dump(const flavor::document_t &doc, const flavor::snapshot_t snapshot, std::ostream &out) {

    dump_t print(doc, snapshot);

    for(auto&& entry : doc.structure){
        out << print.on(entry);
    }
}