// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include <variant>

using namespace khaotica;

namespace {

    typedef std::variant<int64_t, uint64_t, bool, std::vector<bool>> value_t;

    struct conditional_t{
        value_t operator()(const int64_t& left){
            return !!left;
        }
        value_t operator()(const uint64_t& left){
            return !!left;
        }
        value_t operator()(const bool& left){
            return !!left;
        }
        value_t operator()(const std::vector<bool>& left){
            assert(false && "WAT?!");
            return false;
        }
    };
    struct equality_t{

        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left == right;
        }

        value_t operator()(const int64_t& left, const int64_t& right){
            return left == right;
        }

        value_t operator()(const bool& left, const bool& right){
            return left == right;
        }

        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            return left == right;
        }

        value_t operator()(const int64_t& left, const uint64_t& right){
            return left == static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            return left == static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) == right;
        }
        value_t operator()(const uint64_t& left, const bool& right){
            return static_cast<int64_t>(left) == right;
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            return static_cast<uint64_t>(left) == right;
        }
        value_t operator()(const bool& left, const int64_t& right){
            return static_cast<int64_t>(left) == right;
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
    };


    struct logicar_or_t{
        value_t operator()(const int64_t& left, const int64_t& right){
            return left || static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const uint64_t& right){
            return left || static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            return left || static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) || right;
        }
        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left || static_cast<int64_t>(right);
        }
        value_t operator()(const uint64_t& left, const bool& right){
            return static_cast<int64_t>(left) || right;
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            return static_cast<uint64_t>(left) || right;
        }
        value_t operator()(const bool& left, const int64_t& right){
            return static_cast<int64_t>(left) || right;
        }
        value_t operator()(const bool& left, const bool& right){
            return left || static_cast<int64_t>(right);
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
    };
    struct less_t{
        value_t operator()(const int64_t& left, const int64_t& right){
            return left < static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const uint64_t& right){
            return left < static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) < right;
        }
        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left < right;
        }
        value_t operator()(const uint64_t& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct greater_t{
        value_t operator()(const int64_t& left, const int64_t& right){
            return left > static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const uint64_t& right){
            return left > static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) > right;
        }
        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left > right;
        }
        value_t operator()(const uint64_t& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct minus_t{
        value_t operator()(const int64_t& left, const int64_t& right){
            return left - static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const uint64_t& right){
            return left - static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            return left - static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) - right;
        }
        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left - right;
        }
        value_t operator()(const uint64_t& left, const bool& right){
            return left - static_cast<uint64_t>(right);
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            return static_cast<uint64_t>(left) - right;
        }
        value_t operator()(const bool& left, const int64_t& right){
            return static_cast<int64_t>(left) - right;
        }
        value_t operator()(const bool& left, const bool& right){
            return static_cast<int64_t>(left) - static_cast<int64_t>(right);
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct plus_t{
        value_t operator()(const int64_t& left, const int64_t& right){
            return left + static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const uint64_t& right){
            return left + static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const bool& right){
            return left + static_cast<int64_t>(right);
        }
        value_t operator()(const int64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const uint64_t& left, const int64_t& right){
            return static_cast<int64_t>(left) + right;
        }
        value_t operator()(const uint64_t& left, const uint64_t& right){
            return left + right;
        }
        value_t operator()(const uint64_t& left, const bool& right){
            return left + static_cast<uint64_t>(right);
        }
        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const bool& left, const uint64_t& right){
            return static_cast<uint64_t>(left) + right;
        }
        value_t operator()(const bool& left, const int64_t& right){
            return static_cast<int64_t>(left) + right;
        }
        value_t operator()(const bool& left, const bool& right){
            return static_cast<int64_t>(left) + static_cast<int64_t>(right);
        }
        value_t operator()(const bool& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const int64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const bool& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
            assert(false && "WAT?!");
            return {false};
        }
        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
            assert(false && "WAT?!");
            return {false};
        }
    };
    using namespace flavor;

    class parse_t{
    public:
        explicit parse_t(bitreader_t &bitreader, const flavor::document_t &doc)
            : bitreader(bitreader), doc(doc) {
        }

//        void on(flavor::bslbf_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto value = bitreader.read(node.length);
//            definitions[node.name] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::uimsbf_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
//            definitions[node.name] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::bitstring_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto value = khaotica::algorithm::unpack(node.value);
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::integer_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto value = node.value;
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::identifier_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto it = definitions.find(node.name);
//            if( it == definitions.end()){
//                auto definition = doc.definitions.find(node.name);
//                if(definition != doc.definitions.end()){
//                    definition->second->process(*this);
//                }
//            }
//
//            it = definitions.find(node.name);
//            if(it == definitions.end()){
//                // TODO: Default value
//                definitions[node.name] = std::make_shared<value_t>(value_t{uint64_t{0}});
//            }
//            values[knot] = definitions[node.name];
//        };
//
//        void on(flavor::if_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.condition->process(*this);
//            auto condition = std::visit(conditional_t(), (*values[node.condition]));
//            values[node.condition] = std::make_shared<value_t>(condition);
//
//            if(std::get<bool>(condition)){
//                node._then->process(*this);
//            } else if(node._else){
//                (*node._else)->process(*this);
//            }
//        };
//
//        void on(flavor::for_t &node, std::shared_ptr<flavor::node_t> knot)  {
//
//            if(node.initializer){
//                (*node.initializer)->process(*this);
//            }
//
//            value_t condition{true};
//            if(node.condition){
//                (*node.condition)->process(*this);
//                condition = std::visit(conditional_t(), (*values[*node.condition]));
//            }
//
//            while(std::get<bool>(condition)){
//                node.body->process(*this);
//
//                (*node.modifier)->process(*this);
//                if(node.condition){
//                    (*node.condition)->process(*this);
//                    condition = std::visit(conditional_t(), (*values[*node.condition]));
//                }
//            }
//        };
//
//        void on(flavor::compound_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.body->process(*this);
//        };
//
//        void on(flavor::block_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            for (auto &&entry :node.entries) {
//                entry->process(*this);
//            }
//        };
//
//        void on(flavor::assignment_t &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.expression->process(*this);
//            definitions[node.symbol] = values[node.expression];
//        };
//
//        void on(flavor::preincrement_t<std::plus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//
//        };
//
//        void on(flavor::preincrement_t<std::minus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//
//        };
//
//        void on(flavor::postincrement_t<std::plus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            auto& operand = *definitions[node.operand];
//            auto value = std::visit(plus_t(), operand, value_t{uint64_t{1}});
//            definitions[node.operand] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::postincrement_t<std::minus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//
//        };
//
//        void on(flavor::unary_expression_t<std::bit_not<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.operand->process(*this);
//        };
//
//        void on(flavor::unary_expression_t<std::minus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.operand->process(*this);
//        };
//
//        void on(flavor::unary_expression_t<std::logical_not<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.operand->process(*this);
//        };
//
//        void on(flavor::binary_expression_t<std::plus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//        };
//
//        void on(flavor::binary_expression_t<std::minus<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//            auto& left = *(values[node.left_operand]);
//            auto& right = *(values[node.right_operand]);
//
//            auto value = std::visit(minus_t(), left, right);
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::binary_expression_t<std::multiplies<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//        };
//
//        void on(flavor::binary_expression_t<std::divides<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//        };
//
//        void on(flavor::binary_expression_t<std::modulus<>> & node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//        };
//
//        void on(flavor::binary_expression_t<std::less<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//            auto& left = *(values[node.left_operand]);
//            auto& right = *(values[node.right_operand]);
//
//            auto value = std::visit(less_t(), left, right);
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::binary_expression_t<std::greater<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//            auto& left = *(values[node.left_operand]);
//            auto& right = *(values[node.right_operand]);
//
//            auto value = std::visit(greater_t(), left, right);
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::binary_expression_t<std::less_equal<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//        };
//
//        void on(flavor::binary_expression_t<std::greater_equal<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//        };
//
//        void on(flavor::binary_expression_t<std::equal_to<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//            auto& left = *(values[node.left_operand]);
//            auto& right = *(values[node.right_operand]);
//
//            auto value = std::visit(equality_t(), left, right);
//            values[knot] = std::make_shared<value_t>(value);
//        };
//
//        void on(flavor::binary_expression_t<std::not_equal_to<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//        };
//
//        void on(flavor::binary_expression_t<std::logical_and<>> &node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//        };
//
//        void on(flavor::binary_expression_t<std::logical_or<>> & node, std::shared_ptr<flavor::node_t> knot)  {
//            node.left_operand->process(*this);
//            node.right_operand->process(*this);
//
//            auto& left = *(values[node.left_operand]);
//            auto& right = *(values[node.right_operand]);
//
//            auto value = std::visit(logicar_or_t(), left, right);
//            values[knot] = std::make_shared<value_t>(value);
//        };


        std::map<std::shared_ptr<node_t>, std::shared_ptr<value_t>> values;
        std::map<std::string, std::shared_ptr<value_t>> definitions;

    private:
        bitreader_t &bitreader;
        const flavor::document_t &doc;
    };
}

void parser_t::parse(bitreader_t &in, const flavor::document_t &doc) {
    parse_t parse(in, doc);
//    for (auto &&entry : doc.hierarchy) {
//        entry->process(parse);
//    }
}
