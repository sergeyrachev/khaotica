#include "printer.h"

namespace {

    class dump_t{
    public:
        explicit dump_t():indentation(0){

        }

        std::string on( std::shared_ptr<khaotica::value_t> node ){
            return std::visit(*this, node->payload);
        }

        std::string operator()(const std::pair<khaotica::bslbf_t, bslbf_v>& node )  {
            std::ostringstream out;

            const auto& field = node.first;
            const auto& value = node.second.value;

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

        std::string operator()(const std::pair<khaotica::uimsbf_t, khaotica::uimsbf_v>&node){
            std::ostringstream out;
            const auto& field = node.first;
            const auto& value = node.second.value;
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

        std::string operator()(const std::pair<khaotica::bslbf_ranged_t, std::pair<khaotica::bslbf_v, khaotica::bslbf_ranged_v>>&node){
            std::ostringstream out;
            const auto& field = node.first;
            const auto& value = node.second.first;
            const auto& mask = node.second.second.mask;
            const auto& sequence = node.second.second.value;
            out << std::string(indentation, ' ')
                << field.bits.name
                << "["
                << field.range.first
                << ".."
                << field.range.second
                << "]"
                << " "
                << field.bits.length
                << " "
                << "bslbf"
                << " -> "
                << "(" << khaotica::algorithm::to_hex(value.value) << ")"
                << " => (0b"
                << [](const auto& v, const auto& mask){
                    std::ostringstream out;
                    for (int i = mask.size() - 1 ; i >= 0; --i) {
                        if(mask[i]){
                            out << v[i];
                        } else {
                            out << "x";
                        }
                    }
                    return out.str();
                }(sequence, mask) << ")"
                << std::endl;
            return out.str();
        }

        std::string operator()(const std::pair<khaotica::compound_t, std::list<std::shared_ptr<khaotica::value_t>>>&node){

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
        std::string operator()(const std::pair<khaotica::if_t, khaotica::if_v> &node){
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
        std::string operator()(const std::pair<khaotica::do_t, khaotica::loop_v> &node){
            return "do{}while()";
        }
        std::string operator()(const std::pair<khaotica::while_t, khaotica::loop_v> &node){
            return "while(){}";
        }
        std::string operator()(const std::pair<khaotica::for_t, khaotica::loop_v> &node){
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
        std::string operator()(const std::pair<khaotica::expression_t, khaotica::expression_v> &node){
            return "'expr'";
        }

    private:
        size_t indentation;
    };


}

void khaotica::printer_t::print(const khaotica::document_t &doc, std::ostream &out) {

    print_t print(doc);

    for(auto&& entry : doc.structure){
        out << print.on(entry);
    }
}

void khaotica::printer_t::dump(const khaotica::document_t &doc, const khaotica::snapshot_t& snapshot, std::ostream &out) {

    dump_t print;

    for(auto&& entry : snapshot){
        out << print.on(entry);
    }
}
