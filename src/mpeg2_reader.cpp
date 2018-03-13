// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "mpeg2_reader.h"
#include "mpeg2_types.h"

#include "eval_logical.h"
#include "eval_conditional.h"
#include "eval_arithmetic.h"
#include "eval_bitwise.h"
#include "eval_comparison.h"
#include "eval_equality.h"
#include "eval_default.h"

#include "scope.h"

#include <variant>
#include <stack>

using namespace khaotica::bitstream::mpeg2;
using namespace khaotica::syntax::mpeg2;
using namespace khaotica::syntax;

namespace {
    using namespace khaotica::eval;

    auto find_binary_functor(const std::string &op) {
        static const std::map<std::string, std::function<expression_v(const expression_v &, const expression_v &)>> functors{
            {"||", [](const expression_v &left, const expression_v &right) { return std::visit(logical_or_t(), left, right); }},
            {"&&", [](const expression_v &left, const expression_v &right) { return std::visit(logical_and_t(), left, right); }},
            {"==", [](const expression_v &left, const expression_v &right) { return std::visit(equal_t(), left, right); }},
            {"!=", [](const expression_v &left, const expression_v &right) { return std::visit(not_equal_t(), left, right); }},
            {"<",  [](const expression_v &left, const expression_v &right) { return std::visit(less_t(), left, right); }},
            {">",  [](const expression_v &left, const expression_v &right) { return std::visit(greater_t(), left, right); }},
            {"<=", [](const expression_v &left, const expression_v &right) { return std::visit(less_equal_t(), left, right); }},
            {">=", [](const expression_v &left, const expression_v &right) { return std::visit(greater_equal_t(), left, right); }},
            {"-",  [](const expression_v &left, const expression_v &right) { return std::visit(minus_t(), left, right); }},
            {"+",  [](const expression_v &left, const expression_v &right) { return std::visit(plus_t(), left, right); }},
            {"*",  [](const expression_v &left, const expression_v &right) { return std::visit(multiplies_t(), left, right); }},
            {"/",  [](const expression_v &left, const expression_v &right) { return std::visit(divides_t(), left, right); }},
            {"%",  [](const expression_v &left, const expression_v &right) { return std::visit(modulus_t(), left, right); }},
        };
        return functors.at(op);
    }

    auto find_unary_functor(const std::string &op) {
        static const std::map<std::string, std::function<expression_v(const expression_v &)>> functors{
            {"!", [](const expression_v &operand) { return std::visit(logical_not_t(), operand); }},
            {"~", [](const expression_v &operand) { return std::visit(bitwise_not_t(), operand); }},
        };

        return functors.at(op);
    }

    class parse_t {
    public:
        explicit parse_t(bitreader_t &bitreader, const document_t &doc, sax_t& handler)
            : bitreader(bitreader), doc(doc), handler(handler) {
        }

        void parse() {
            (*this)(doc.sequence);
        }

        std::vector<bool> read(const length_t &length) {
            if (length.from == length.to) {
                return bitreader.read(length.from);
            } else {
                assert(false && "Not implemented");
            }
            return {};
        }

        std::shared_ptr<value_t> operator()(const auto &node) {
            return {};
        }

        std::shared_ptr<value_t> operator()(const sequence_t &node) {
            std::shared_ptr<value_t> value;
            for (auto &&item : node) {
                value = std::visit(*this, item->payload);
            }
            return value;
        }

        std::shared_ptr<value_t> operator()(const integer_t &node) {
            auto value = node.value;
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{value}});
        };

        std::shared_ptr<value_t> operator()(const identifier_t &node) {
            auto symbol = scope->lookup(node.name);
            if (symbol) {
                return symbol;
            }

            auto definition = doc.global->lookup(node.name);
            if (definition) {
                auto value = std::visit(default_t(), definition->payload);
                return std::make_shared<value_t>(value_t{expression_t{node}, value});
            }

            assert(false && "It is REALLY bad to be there and try to use unresolved symbol");
            return nullptr;
        };

        std::shared_ptr<value_t> operator()(const bitstring_t &node) {
            auto value = khaotica::algorithm::unpack(node.value);
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{value}});
        };

        std::shared_ptr<value_t> operator()(const tcimsbf_t &node) {
            return {};
        };

        std::shared_ptr<value_t> operator()(const bslbf_t &node) {
            auto position = bitreader.position();
            bitstring_v field{read(node.length)};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const uimsbf_t &node) {
            auto position = bitreader.position();
            uimsbf_v field{khaotica::algorithm::to_ull_msbf(read(node.length))};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const simsbf_t &node) {
            return {};
        };

        std::shared_ptr<value_t> operator()(const vlclbf_t &node) {
            return {};
        };

        std::shared_ptr<value_t> operator()(const collection_t &node) {
            return {};
        };

        std::shared_ptr<value_t> operator()(const slot_t &node) {
            return {};
        };

        std::shared_ptr<value_t> operator()(const sparsed_t &node) {

//            auto value = bitreader.read(node.bits.length);
//            auto it = symbols.find(node.bits.name);
//            if (it == symbols.end()) {
//                auto[bits, mask] = update({}, {}, value, node.range);
//
//                auto v = std::make_shared<value_t>(value_t{std::make_pair(node, std::make_pair(bslbf_v{value, position}, bslbf_ranged_v{bits, mask}))});
//                symbols[node.bits.name] = v;
//                return v;
//            } else {
//                auto previous = std::get<std::pair<bslbf_ranged_t, std::pair<bslbf_v, bslbf_ranged_v>>>(it->second->payload);
//                auto previous_value = previous.second.second;
//                auto[bits, mask] = update(previous_value.value, previous_value.mask, value, node.range);
//
//                auto v = std::make_shared<value_t>(value_t{std::make_pair(node, std::make_pair(bslbf_v{value, position}, bslbf_ranged_v{bits, mask}))});
//                symbols[node.bits.name] = v;
//            }
            return {};
        }

        std::shared_ptr<value_t> operator()(const reference_t &node) {
            auto it = doc.global->definitions.find(node.name);
            assert(it != doc.global->definitions.end());
            return std::visit(*this, it->second->payload);
        };

        std::shared_ptr<value_t> operator()(const if_t &node) {
            scope = scope->open(scope);
            handler.open(node);

            auto conditional_expression = std::visit(*this, node.condition->payload);
            auto conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);

            if (conditional_expression_value) {
                std::visit(*this, node._then->payload);
            } else if(node._else){
                std::visit(*this, (*node._else)->payload);
            }

            handler.close(node);
            scope = scope->close();
            return {};
        };

        std::shared_ptr<value_t> operator()(const for_t &node) {
            scope = scope->open(scope);
            handler.open(node);

            auto initializer = std::visit(*this, node.initializer->payload);

            auto conditional_expression = std::visit(*this, node.condition->payload);
            bool conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);

            while (conditional_expression_value) {
                std::visit(*this, node.body->payload);
                auto modifier = std::visit(*this, node.modifier->payload);

                conditional_expression = std::visit(*this, node.condition->payload);
                conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);
            }

            handler.close(node);
            scope = scope->close();
            return {};
        };

        std::shared_ptr<value_t> operator()(const do_t &node) {
            scope = scope->open(scope);
            handler.open(node);

            bool conditional_expression_value(true);
            do {
                std::visit(*this, node.body->payload);

                auto conditional_expression = std::visit(*this, node.condition->payload);
                conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);
            } while (conditional_expression_value);

            handler.close(node);
            scope = scope->close();
            return {};
        }

        std::shared_ptr<value_t> operator()(const while_t &node) {
            handler.open(node);

            handler.close(node);
            return {};
        }

        std::shared_ptr<value_t> operator()(const compound_t &node) {
            scope = scope->open(scope);
            handler.open(node);

            std::visit(*this, node.body->payload);

            handler.close(node);
            scope = scope->close();
            return {};
        };

        std::shared_ptr<value_t> operator()(const assignment_t &node) {
            auto node_value = std::visit(*this, node.expression->payload);
            scope->definitions[node.symbol] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const preincrement_t &node) {
            auto previous_value = scope->lookup(node.operand);

            auto value = std::visit(find_binary_functor(node.operation), previous_value->payload, expression_v{int64_t{1}});
            previous_value->payload = value;

            return std::make_shared<value_t>(value_t{expression_t{node}, value});
        };

        std::shared_ptr<value_t> operator()(const postincrement_t &node) {
            auto previous_value = scope->lookup(node.operand);
            auto previous_payload = previous_value->payload;

            auto value = std::visit(find_binary_functor(node.operation), previous_payload, expression_v{int64_t{1}});
            previous_value->payload = value;

            return std::make_shared<value_t>(value_t{expression_t{node}, previous_payload});
        };

        std::shared_ptr<value_t> operator()(const unary_expression_t &node) {
            auto operand = std::visit(*this, node.operand->payload);
            auto value = std::visit(find_unary_functor(node.operation), operand->payload);
            return std::make_shared<value_t>(value_t{expression_t{node}, value});
        };

        std::shared_ptr<value_t> operator()(const binary_expression_t &node) {
            auto left = std::visit(*this, node.left_operand->payload);
            auto right = std::visit(*this, node.right_operand->payload);

            auto value = std::visit(find_binary_functor(node.operation), left->payload, right->payload);
            return std::make_shared<value_t>(value_t{expression_t{node}, value});
        };

        std::shared_ptr<value_t> operator()(const position_t &node) {
            uint64_t value(0);
            if (node.name) {
                auto symbol = scope->lookup(*node.name);
                if (symbol) {
                    value = symbol->position;
                }
            } else {
                value = bitreader.position();
            }

            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{value}});
        };

        std::shared_ptr<value_t> operator()(const nextbits_t &node) {
            auto value = bitreader.peek();
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{value}});
        };

        std::shared_ptr<value_t> operator()(const bytealigned_t &node) {
            return nullptr;
        };

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
    private:
        bitreader_t &bitreader;
        const document_t &doc;
        sax_t& handler;

        typedef scope_tt<std::string, std::shared_ptr<value_t>> symbols_t;
        std::shared_ptr<symbols_t> global{std::make_shared<symbols_t>()};
        std::shared_ptr<symbols_t> scope{global};
    };
}

void reader_t::parse(bitreader_t& in, const document_t& doc, sax_t& handler) {
    parse_t parse(in, doc, handler);
    parse.parse();
}
