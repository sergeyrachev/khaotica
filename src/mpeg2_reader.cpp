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
#include "eval_numeric.h"

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

    class nextbits_impl : public nextbits_v {
    public:
        nextbits_impl(bitreader_t& bitreader):bitreader(bitreader){

        }

        virtual std::vector<bool> get(size_t n) final {
            return bitreader.peek(n);
        }

    private:
        bitreader_t &bitreader;
    };

    class parse_t {
    public:
        explicit parse_t(bitreader_t &bitreader, const document_t &doc, sax_t& handler)
            : bitreader(bitreader), doc(doc), handler(handler) {
        }

        void parse() {
            (*this)(doc.sequence);
        }

        std::vector<bool> read(const length_t &length) {
            return bitreader.read(length.from);
        }
        std::vector<bool> read_vlc(const length_t &length) {
            //assert(false && "Not implemented");
            return bitreader.read(length.from);
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
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{ bitstring_v{value, std::vector<bool>(value.size(), true)}}});
        };

        std::shared_ptr<value_t> operator()(const tcimsbf_t &node) {
            auto position = bitreader.position();
            tcimsbf_v field{khaotica::algorithm::to_integer_msbf<int64_t>(read(node.length))};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const bslbf_t &node) {
            auto position = bitreader.position();
            bitstring_v field{read(node.length)};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{
                expression_t{
                    identifier_t{
                        node.name
                    }
                },
                expression_v{
                    bitstring_v{
                        field.value,
                        std::vector<bool>(field.value.size(), true)
                    }
                },
                position,
                bitreader.position() - position
            });
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const uimsbf_t &node) {
            auto position = bitreader.position();
            uimsbf_v field{khaotica::algorithm::to_integer_msbf<uint64_t>(read(node.length))};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const uilsbf_t &node) {
            auto position = bitreader.position();
            uimsbf_v field{khaotica::algorithm::to_integer_lsbf<uint64_t>(read(node.length))};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const simsbf_t &node) {
            auto position = bitreader.position();
            simsbf_v field{khaotica::algorithm::to_integer_msbf<int64_t>(read(node.length))};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{expression_t{identifier_t{node.name}}, expression_v{field.value}, position, bitreader.position() - position});
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const vlclbf_t &node) {
            auto position = bitreader.position();
            vlclbf_v field{read_vlc(node.length)};

            handler.on(node, field, position, bitreader.position() - position);

            auto node_value = std::make_shared<value_t>(value_t{
                expression_t{
                    identifier_t{
                        node.name
                    }
                },
                expression_v{
                    bitstring_v{
                        field.value,
                        std::vector<bool>(field.value.size(), true)
                    }
                },
                position,
                bitreader.position() - position
            });
            scope->definitions[node.name] = node_value;
            return node_value;
        };

        std::shared_ptr<value_t> operator()(const collection_t &node) {
            return std::visit([&node, this](const auto& tag){
                auto position = bitreader.position();

                collection_v collection_value;
                for (auto i = 0; i < node.size; ++i) {
                    auto field = read(node.length);
                    collection_value.value.push_back(field);
                }

                handler.on(node, collection_value, position, bitreader.position() - position, tag);

                auto node_value = std::make_shared<value_t>(value_t{
                    expression_t{
                        identifier_t{
                            node.name
                        }
                    },
                    expression_v{
                        true
                    }, position, bitreader.position() - position});
                scope->definitions[node.name] = node_value;
                return node_value;
            }, node.tag);
        };



        std::shared_ptr<value_t> operator()(const slot_t &node) {
            return std::visit([&node, this](const auto& tag){
                auto position = bitreader.position();

                auto field = read(node.length);

                slot_v slot_value{field, {}};

                for(auto&& dim : node.indices){
                    auto idx_value = std::visit(*this, dim);
                    uint64_t idx = std::visit(indexation_t(), idx_value->payload);
                    slot_value.indices.push_back(idx);
                }

                handler.on(node, slot_value, position, bitreader.position() - position, tag);

                auto node_value = std::make_shared<value_t>(value_t{
                    expression_t{
                        identifier_t{
                            node.name
                        }
                    },
                    expression_v{
                        true
                    }, position, bitreader.position() - position});
                scope->definitions[node.name] = node_value;
                return node_value;
            }, node.tag);
        };


        std::shared_ptr<value_t> operator()(const sparsed_t &node) {
            return std::visit([&node, this](const auto& tag){
                auto position = bitreader.position();
                auto field = read(node.length);

                auto [value, mask] = update({}, {}, field, node.range.front, node.range.back);

                sparsed_v sparsed_value{field, value, mask};

                if(auto it = scope->definitions.find(node.name); it != scope->definitions.end()){
                    auto accumulator = std::get<bitstring_v>(it->second->payload);

                    auto [value, mask] = update(accumulator.value, *accumulator.mask, field, node.range.front, node.range.back);
                    accumulator.value = value;
                    *accumulator.mask = mask;

                    sparsed_value.value = value;
                    sparsed_value.mask = mask;
                }

                handler.on(node, sparsed_value, position, bitreader.position() - position, tag);

                auto node_value = std::make_shared<value_t>(value_t{
                    expression_t{
                        identifier_t{
                            node.name
                        }
                    },
                    expression_v{
                        bitstring_v{
                            sparsed_value.value,
                            sparsed_value.mask
                        }
                    }, position, bitreader.position() - position});
                scope->definitions[node.name] = node_value;
                return node_value;
            }, node.tag);
        }

        std::shared_ptr<value_t> operator()(const reference_t &node) {
            auto it = doc.global->definitions.find(node.name);

            scope = scope->open(scope);

            auto& compound = std::get<compound_t>(it->second->payload);
            assert(compound.args.size() == node.args.size());
            handler.open(compound);
            for (int i = 0; i < node.args.size(); ++i) {
                scope->definitions[compound.args[i]] = std::visit(*this, node.args[i]->payload);
            }

            std::visit(*this, it->second->payload);

            handler.close(compound);
            scope = scope->close();
            return {};
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
            scope = scope->open(scope);
            handler.open(node);

            auto conditional_expression = std::visit(*this, node.condition->payload);
            bool conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);
            while(conditional_expression_value) {
                std::visit(*this, node.body->payload);

                conditional_expression = std::visit(*this, node.condition->payload);
                conditional_expression_value = std::visit(conditional_t(), conditional_expression->payload);
            }

            handler.close(node);
            scope = scope->close();
            return {};
        }

        std::shared_ptr<value_t> operator()(const compound_t &node) {

            std::visit(*this, node.body->payload);

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
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{std::make_shared<nextbits_impl>(bitreader)}});
        };

        std::shared_ptr<value_t> operator()(const bytealigned_t &node) {
            bool value = (bitreader.position() % 8) == 0;
            return std::make_shared<value_t>(value_t{expression_t{node}, expression_v{value}});
        };

        std::shared_ptr<value_t> operator()(const skip_t &node) {
            bitreader.read(node.amount);
            return {};
        };

        std::tuple<std::vector<bool>, std::vector<bool>>
        update(const std::vector<bool> &initial_value,
               const std::vector<bool> &initial_mask,
               const std::vector<bool> &slice_value,
               const uint64_t low, const uint64_t high) {

            // TODO: Bidirectional ranges: 30..32 != 32..30
            auto right = std::min(low, high);
            auto left = std::max(low, high) + 1;

            auto upper_bound = std::max(initial_mask.size(), left);
            std::vector<bool> mask = initial_mask;
            mask.resize(upper_bound);
            std::fill(mask.begin() + right, mask.begin() + left, true);

            std::vector<bool> value = initial_value;
            value.resize(upper_bound);

            auto it = value.begin();
            std::advance(it, right);
            std::copy(slice_value.rbegin(), slice_value.rend(), it);

            return {value, mask};
        }

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

