// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include <variant>

using namespace khaotica;

namespace {
    using namespace flavor;

    class parse_t : public flavor::traversal_t {
    public:
        explicit parse_t(bitreader_t &bitreader, const flavor::document_t &doc)
            : bitreader(bitreader), doc(doc) {
        }

        void on(flavor::bslbf_t &node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = bitreader.read(node.length);
            values[knot] = std::make_shared<value_t>(value_t{value});
            definitions[node.name] = knot;
        };

        void on(flavor::uimsbf_t &node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
            values[knot] = std::make_shared<value_t>(value_t{value});
            definitions[node.name] = knot;
        };

        void on(flavor::bitstring_t &node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = khaotica::algorithm::unpack(node.value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::integer_t &node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = node.value;
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::identifier_t &node, std::shared_ptr<flavor::node_t> knot) final {
            values[knot] = values[definitions[node.name]];
        };

        void on(flavor::if_t &node, std::shared_ptr<flavor::node_t> knot) final {

            node.condition->process(*this);
            auto condition = *values[node.condition];
            bool condition_value = cast(condition.value);
            if(condition_value){
                node._then->process(*this);
                values[knot] = values[node._then];
            } else if( node._else){
                (*node._else)->process(*this);
                values[knot] = values[*node._else];
            }
        };

        void on(flavor::for_t &node, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::compound_t &node, std::shared_ptr<flavor::node_t> knot) final {
            node.body->process(*this);
            values[knot] = values[node.body];
        };

        void on(flavor::block_t &node, std::shared_ptr<flavor::node_t> knot) final {
            std::list<std::shared_ptr<value_t>> entries;
            for (auto &&entry : node.entries) {
                entry->process(*this);
                entries.push_back(values[entry]);
            }
            values[knot] = std::make_shared<value_t>(value_t{entries});
        };

        void on(flavor::assignment_t &node, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::preincrement_t<std::plus<>> &node, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::preincrement_t<std::minus<>> &node, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::postincrement_t<std::plus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::postincrement_t<std::minus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::unary_expression_t<std::bit_not<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::unary_expression_t<std::minus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::unary_expression_t<std::logical_not<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::plus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::minus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::multiplies<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::divides<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::modulus<>> &, std::shared_ptr<flavor::node_t> knot) final {

        };

        void on(flavor::binary_expression_t<std::less<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) < cast(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::greater<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) > cast(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::less_equal<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) <= cast(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::greater_equal<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) >= cast(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::equal_to<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = values[node.left_operand]->value == values[node.right_operand]->value;
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::not_equal_to<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) != cast(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::logical_and<>> &node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);

            auto value = cast(values[node.left_operand]->value) && cast(values[node.right_operand]->value);
            values[knot] =  std::make_shared<value_t>(value_t{value});
        };

        void on(flavor::binary_expression_t<std::logical_or<>> & node, std::shared_ptr<flavor::node_t> knot) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
            
            auto value = std::get<bool>(values[node.left_operand]->value) || std::get<bool>(values[node.right_operand]->value);
            values[knot] = std::make_shared<value_t>(value_t{value});
        };

        struct value_t{
            std::variant<int64_t, uint64_t, bool, std::vector<bool>, std::list<std::shared_ptr<value_t>>> value;
        };

        std::map< std::shared_ptr<flavor::node_t>, std::shared_ptr<value_t>> values;
        std::map<std::string, std::shared_ptr<flavor::node_t>> definitions;

    private:
        bool cast(const std::variant<int64_t, uint64_t, bool, std::vector<bool>, std::list<std::shared_ptr<value_t>>>& value) {
            bool condition_value(false);
            if( std::holds_alternative<bool>(value) ){
                condition_value  = std::get<bool>(value);
            } else if(std::holds_alternative<uint64_t>(value)) {
                condition_value  = std::get<uint64_t>(value);
            } else if(std::holds_alternative<int64_t>(value)) {
                condition_value  = std::get<int64_t>(value);
            } else {
                assert(false && "Wat?!");
            }
        }
    private:
        bitreader_t &bitreader;
        const flavor::document_t &doc;
    };
}

void parser_t::parse(std::ifstream &in, const flavor::document_t &doc) {
    bitreader_t bitreader(in);

    parse_t parse(bitreader, doc);
    for (auto &&entry : doc.hierarchy) {
        entry->process(parse);
    }
}
