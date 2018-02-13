// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "grammar_mpeg2_reader.h"
#include "grammar_mpeg2_types.h"

#include "eval_logical.h"
#include "eval_arithmetic.h"
#include "eval_bitwise.h"
#include "eval_comparison.h"
#include "eval_equality.h"
#include "eval_default.h"

#include <variant>
#include <stack>

using namespace khaotica;
using namespace khaotica::details;

namespace {

    auto find_binary_functor(const std::string &op) {
        using namespace eval;
        static const std::map<std::string, std::function<expression_v(const expression_v &, const expression_v &)>> functors{
            {"||", [](const expression_v &left, const expression_v &right) { return std::visit(logical_or_t(), left, right); }},
            {"&&", [](const expression_v &left, const expression_v &right) { return std::visit(logical_and_t(), left, right); }},
            {"==", [](const expression_v &left, const expression_v &right) { return std::visit(equal_t(), left, right); }},
            {"!=", [](const expression_v &left, const expression_v &right) { return std::visit(not_equal_t(), left, right); }},
            {"<", [](const expression_v &left, const expression_v &right) { return std::visit(less_t(), left, right); }},
            {">", [](const expression_v &left, const expression_v &right) { return std::visit(greater_t(), left, right); }},
            {"<=", [](const expression_v &left, const expression_v &right) { return std::visit(less_equal_t(), left, right); }},
            {">=", [](const expression_v &left, const expression_v &right) { return std::visit(greater_equal_t(), left, right); }},
            {"-", [](const expression_v &left, const expression_v &right) { return std::visit(minus_t(), left, right); }},
            {"+", [](const expression_v &left, const expression_v &right) { return std::visit(plus_t(), left, right); }},
            {"*", [](const expression_v &left, const expression_v &right) { return std::visit(multiplies_t(), left, right); }},
            {"/", [](const expression_v &left, const expression_v &right) { return std::visit(divides_t(), left, right); }},
            {"%", [](const expression_v &left, const expression_v &right) { return std::visit(modulus_t(), left, right); }},
        };
        return functors.at(op);
    }

    auto find_unary_functor(const std::string &op){
        using namespace eval;
        static const std::map<std::string, std::function<expression_v(const expression_v &)>> functors{
            {"!", [](const expression_v &operand) { return std::visit(logical_not_t(), operand); }},
            {"~", [](const expression_v &operand) { return std::visit(bitwise_not_t(), operand); }},
        };

        return functors.at(op);
    }

    //class parse_t {
//    public:
//        explicit parse_t(bitreader_t &bitreader, const sequence_t &doc)
//            : bitreader(bitreader), doc(doc) {
//        }
//
//
//        std::shared_ptr<value_t> on(std::shared_ptr<node_t> node) {
//            return std::visit(*this, node->payload);
//        }
//
//        std::shared_ptr<value_t> operator()(const bslbf_t &node) {
//            auto position = bitreader.position();
//            auto value = bitreader.read(node.length);
//            auto v = std::make_shared<value_t>(value_t{std::make_pair(node, bslbf_v{value, position})});
//            symbols[node.name] = v;
//            return v;
//        };
//
//        std::shared_ptr<value_t>operator()(const std::vector<uimsbf_t>& node )  {
//            return nullptr;
//        }
//
//        std::shared_ptr<value_t> operator()(const sparsed_t &node) {
//
//            auto value = bitreader.read(node.bits.length);
//
//            auto it = symbols.find(node.bits.name);
//            if( it == symbols.end()){
//                auto [bits, mask] = update({}, {}, value, node.range);
//
//                auto v = std::make_shared<value_t>(value_t{std::make_pair(node, std::make_pair(bslbf_v{value, position}, bslbf_ranged_v{bits, mask}))});
//                symbols[node.bits.name] = v;
//                return v;
//            } else {
//                auto previous = std::get<std::pair<bslbf_ranged_t, std::pair<bslbf_v, bslbf_ranged_v>>>(it->second->payload);
//                auto previous_value = previous.second.second;
//                auto [bits, mask] = update(previous_value.value, previous_value.mask, value, node.range);
//
//                auto v = std::make_shared<value_t>(value_t{std::make_pair(node, std::make_pair(bslbf_v{value, position}, bslbf_ranged_v{bits, mask}))});
//                symbols[node.bits.name] = v;
//                return v;
//            }
//        };
//
//        std::shared_ptr<value_t> operator()(const uimsbf_t &node) {
//            auto position = bitreader.position();
//            auto value = algorithm::to_ull_msbf(bitreader.read(node.length));
//            auto v = std::make_shared<value_t>(value_t{std::make_pair(node, uimsbf_v{value, position})});
//            symbols[node.name] = v;
//            return v;
//        };
//
//        std::shared_ptr<value_t> operator()(const bitstring_t &node) {
//            auto value = algorithm::unpack(node.value);
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, value)});
//        };
//
//        std::shared_ptr<value_t> operator()(const integer_t &node) {
//            auto value = node.value;
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, value)});
//        };
//
//        std::shared_ptr<value_t> operator()(const identifier_t &node) {
//
//            auto symbol = symbols.find(node.name);
//            if (symbol != symbols.end()) {
//                expression_v value = extract(*symbol->second);
//                auto v = std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, value)});
//                return v;
//            }
//
//            auto definition = lookup(node.name, scope);
//            if (definition) {
//                auto value = on(definition);
//                return value;
//            }
//
//            // FIXME: Fix this shit with a correct scope handling. Note! WHAT is a scope in bitstream is not clear yet.
//            definition = deep_lookup(node.name, scope);
//            if(definition){
//                auto value = std::visit(default_t(), definition->payload);
//                return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, value)});
//            }
//
//            assert(false && "It is REALLY bad to be there and try to use unresolved symbol");
//            return nullptr;
//        };
//
//        std::shared_ptr<value_t> operator()(const reference_t &node) {
//            std::list<std::shared_ptr<value_t>> value;
//
//            auto it = doc.definitions.find(node.name);
//            assert(it != doc.definitions.end());
//            return on(it->second);
//        };
//
//        std::shared_ptr<value_t> operator()(const if_t &node) {
//            scope = node.scope;
//
//            auto conditional_expression = on(node.condition);
//            auto conditional_expression_payload = std::get<std::pair<expression_t, expression_v>>(conditional_expression->payload);
//            auto conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);
//
//            std::list<std::shared_ptr<value_t>> value;
//            if (conditional_expression_value) {
//                for (auto &&item : node._then) {
//                    value.push_back(on(item));
//                }
//            } else {
//                for (auto &&item : node._else) {
//                    value.push_back(on(item));
//                }
//            }
//
//            scope = node.scope->parent;
//            return std::make_shared<value_t>(value_t{std::make_pair(node, if_v{ conditional_expression_value, value} )});
//        };
//
//        std::shared_ptr<value_t> operator()(const for_t &node) {
//
//            scope = node.scope;
//
//            if(node.initializer){
//                auto initializer = on(*node.initializer);
//            }
//
//            bool conditional_expression_value(true);
//            if(node.condition){
//                auto conditional_expression = on(*node.condition);
//                auto conditional_expression_payload = std::get<std::pair<expression_t, expression_v>>(conditional_expression->payload);
//                conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);
//            }
//
//            std::vector<loop_v::iteration_t> value;
//            while(conditional_expression_value){
//
//                loop_v::iteration_t iteration;
//                for (auto &&item : node.body) {
//                    iteration.push_back(on(item));
//                }
//                value.emplace_back(std::move(iteration));
//
//                if(node.modifier){
//                    auto modifier = on(*node.modifier);
//                }
//
//                if(node.condition){
//                    auto conditional_expression = on(*node.condition);
//                    auto conditional_expression_payload = std::get<std::pair<expression_t, expression_v>>(conditional_expression->payload);
//                    conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);
//                }
//            }
//            scope = node.scope->parent;
//            return std::make_shared<value_t>(value_t{std::make_pair(node, loop_v{value} )});
//        };
//
//        std::shared_ptr<value_t> operator()(const do_t &node) {
//
//        }
//
//        std::shared_ptr<value_t> operator()(const while_t &node) {
//
//        }
//
//        std::shared_ptr<value_t> operator()(const compound_t &node) {
//            scope = node.scope;
//
//            std::list<std::shared_ptr<value_t>> value;
//            for (auto &&item : node.body) {
//                value.push_back(on(item));
//            }
//            scope = node.scope->parent;
//            return std::make_shared<value_t>(value_t{std::make_pair(node, value)});
//        };
//
//        std::shared_ptr<value_t> operator()(const assignment_t &node) {
//            auto initializer = on(node.expression);
//            symbols[node.symbol] = initializer;
//            auto initializer_payload = std::get<std::pair<expression_t, expression_v>>(initializer->payload);
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, expression_v{initializer_payload.second})});
//        };
//
//        std::shared_ptr<value_t> operator()(const preincrement_t &node) {
//            auto previous_value = symbols.find(node.operand)->second;
//            auto& previous_payload = std::get<std::pair<expression_t, expression_v>>(previous_value->payload);
//            auto value = std::visit(find_binary_functor(node.operation), previous_payload.second, expression_v{int64_t{1}});
//
//            previous_payload.second = value;
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, value)});
//        };
//
//        std::shared_ptr<value_t> operator()(const postincrement_t &node) {
//            auto previous_value = symbols.find(node.operand)->second;
//            auto& previous_payload = std::get<std::pair<expression_t, expression_v>>(previous_value->payload);
//            auto value = std::visit(find_binary_functor(node.operation), previous_payload.second, expression_v{int64_t{1}});
//
//            previous_payload.second = value;
//            return previous_value;
//        };
//
//
//        std::shared_ptr<value_t> operator()(const unary_expression_t &node) {
//            auto operand = on(node.operand);
//
//            auto operand_payload = std::get<std::pair<expression_t, expression_v>>(operand->payload);
//
//            auto value = std::visit(find_unary_functor(node.operation), operand_payload.second);
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, expression_v{value})});
//        };
//
//        std::shared_ptr<value_t> operator()(const binary_expression_t &node) {
//            auto left = on(node.left_operand);
//            auto right = on(node.right_operand);
//
//            auto left_payload = std::get<std::pair<expression_t, expression_v>>(left->payload);
//            auto right_payload = std::get<std::pair<expression_t, expression_v>>(right->payload);
//
//            auto value = std::visit(find_binary_functor(node.operation), left_payload.second, right_payload.second);
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, expression_v{value})});
//        };
//
//        std::shared_ptr<value_t> operator()(const position_t &node) {
//
//            uint64_t value(0);
//            if( node.name){
//                auto symbol = symbols.find(*node.name);
//                if (symbol != symbols.end()) {
//                    value = extract_location(*symbol->second);
//                }
//            } else {
//                value = bitreader.position();
//            }
//
//            return std::make_shared<value_t>(value_t{std::make_pair(expression_t{node}, expression_v{value})});
//        };
//
//        std::shared_ptr<value_t> operator()(const nextbits_t &node) {
//            return nullptr;
//        }
//    private:
//        expression_v extract(const value_t &value) {
//            if( const auto&& val = std::get_if<std::pair<bslbf_t, bslbf_v>>(&value.payload)){
//                return val->second.value;
//            } else if(const auto&& val = std::get_if<std::pair<uimsbf_t, uimsbf_v>>(&value.payload)) {
//                return val->second.value;
//            } else if(const auto&& val = std::get_if<std::pair<expression_t, expression_v>>(&value.payload)){
//                return val->second;
//            }
//            assert(false && "No way to be there");
//            return false;
//        }
//
//        uint64_t extract_location(const value_t &value) {
//            if( const auto&& val = std::get_if<std::pair<bslbf_t, bslbf_v>>(&value.payload)){
//                return val->second.location;
//            } else if(const auto&& val = std::get_if<std::pair<uimsbf_t, uimsbf_v>>(&value.payload)) {
//                return val->second.location;
//            }
//            assert(false && "No way to be there");
//            return false;
//        }
//
//        std::shared_ptr<node_t> lookup(const std::string& name, scope_t* scope) {
//
//            auto it = scope->definitions.find(name);
//            if(it != scope->definitions.end()){
//                return it->second;
//            }
//
//            if( scope->parent){
//                return lookup(name, scope->parent);
//            }
//
//            return nullptr;
//        }
//
//        std::shared_ptr<node_t> deep_lookup(const std::string& name, scope_t* scope) {
//
//            auto it = scope->definitions.find(name);
//            if(it != scope->definitions.end()){
//                return it->second;
//            }
//
//            if( scope->parent){
//                for (auto &&sibling : scope->parent->childs) {
//                    it = sibling->definitions.find(name);
//                    if(it != sibling->definitions.end()){
//                        return it->second;
//                    }
//                }
//
//                return deep_lookup(name, scope->parent);
//            }
//
//            return nullptr;
//        }
//
//        std::tuple<std::vector<bool>, std::vector<bool>>
//        update(const std::vector<bool> &initial_value, const std::vector<bool> &initial_mask, const std::vector<bool> &slice_value,
//               const std::pair<uint64_t, uint64_t> &range) {
//
//            auto right = std::min(range.first, range.second);
//            auto left = std::max(range.first, range.second) + 1;
//
//            auto upper_bound = std::max(initial_mask.size(), left);
//            std::vector<bool> mask = initial_mask;
//            mask.resize(upper_bound);
//            std::fill(mask.begin() + right, mask.begin() + left, true);
//
//            std::vector<bool> value = initial_value;
//            value.resize(upper_bound);
//
//            auto it = value.begin();
//            std::advance(it, right);
//            std::copy(slice_value.begin(), slice_value.end(), it);
//
//            return {value, mask};
//        }
//
//    private:
//        bitreader_t &bitreader;
//        const sequence_t &doc;
//        std::map<std::string, std::shared_ptr<value_t>> symbols;
//        scope_t* scope{nullptr};
//    };
}

void bsparser_t::parse(bitreader_t &in, const sequence_t &doc) {
//    parse_t parse(in, doc);
//    for (auto &&entry : doc.body) {
//        parse.on(entry);
//    }
}
