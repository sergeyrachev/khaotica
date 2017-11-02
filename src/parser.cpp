// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include <variant>
#include <stack>

using namespace khaotica;

namespace {

    struct default_t {
        flavor::expression_v operator()(const flavor::bslbf_t &node) {
            return std::vector<bool>();
        }


        flavor::expression_v operator()(const flavor::uimsbf_t &node) {
            return uint64_t{0};

        }

        flavor::expression_v operator()(const flavor::bitstring_t &node) {
            assert(false && "Must never be there");
            return std::vector<bool>();

        }

        flavor::expression_v operator()(const flavor::integer_t &node) {
            assert(false && "Must never be there");
            return int64_t{0};
        }

        flavor::expression_v operator()(const flavor::reference_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::compound_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::identifier_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::if_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::for_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::unary_expression_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::binary_expression_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::postincrement_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::preincrement_t &node) {
            assert(false && "Must never be there");
            return false;
        }

        flavor::expression_v operator()(const flavor::assignment_t &node) {
            assert(false && "Must never be there");
            return false;
        }
    };

    struct conditional_t {
        bool operator()(const int64_t &left) {
            return left != 0;
        }

        bool operator()(const uint64_t &left) {
            return left != 0;
        }

        bool operator()(const bool &left) {
            return left;
        }

        bool operator()(const std::vector<bool> &left) {
            assert(false && "WAT?!");
            return left.empty();
        }
    };

    struct equality_t {

        bool operator()(const uint64_t &left, const uint64_t &right) {
            return left == right;
        }

        bool operator()(const int64_t &left, const int64_t &right) {
            return left == right;
        }

        bool operator()(const bool &left, const bool &right) {
            return left == right;
        }

        bool operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
            return left == right;
        }

        bool operator()(const int64_t &left, const uint64_t &right) {
            return left == static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const bool &right) {
            return left == static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const int64_t &right) {
            return static_cast<int64_t>(left) == right;
        }

        bool operator()(const uint64_t &left, const bool &right) {
            return static_cast<int64_t>(left) == right;
        }

        bool operator()(const uint64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const uint64_t &right) {
            return static_cast<uint64_t>(left) == right;
        }

        bool operator()(const bool &left, const int64_t &right) {
            return static_cast<int64_t>(left) == right;
        }

        bool operator()(const bool &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct logicar_or_t {
        bool operator()(const int64_t &left, const int64_t &right) {
            return left || static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const uint64_t &right) {
            return left || static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const bool &right) {
            return left || static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const int64_t &right) {
            return static_cast<int64_t>(left) || right;
        }

        bool operator()(const uint64_t &left, const uint64_t &right) {
            return left || static_cast<int64_t>(right);
        }

        bool operator()(const uint64_t &left, const bool &right) {
            return static_cast<int64_t>(left) || right;
        }

        bool operator()(const uint64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const uint64_t &right) {
            return static_cast<uint64_t>(left) || right;
        }

        bool operator()(const bool &left, const int64_t &right) {
            return static_cast<int64_t>(left) || right;
        }

        bool operator()(const bool &left, const bool &right) {
            return left || static_cast<int64_t>(right);
        }

        bool operator()(const bool &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct less_t {
        bool operator()(const int64_t &left, const int64_t &right) {
            return left < static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const uint64_t &right) {
            return left < static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const int64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const int64_t &right) {
            return static_cast<int64_t>(left) < right;
        }

        bool operator()(const uint64_t &left, const uint64_t &right) {
            return left < right;
        }

        bool operator()(const uint64_t &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }
    };

    struct greater_t {
        bool operator()(const int64_t &left, const int64_t &right) {
            return left > static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const uint64_t &right) {
            return left > static_cast<int64_t>(right);
        }

        bool operator()(const int64_t &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const int64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const int64_t &right) {
            return static_cast<int64_t>(left) > right;
        }

        bool operator()(const uint64_t &left, const uint64_t &right) {
            return left > right;
        }

        bool operator()(const uint64_t &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const uint64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const bool &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const int64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const bool &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return {false};
        }

        bool operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return {false};
        }
    };

    template<typename F>
    struct arithmetical_t{
        flavor::expression_v operator()(const int64_t &left, const int64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const int64_t &left, const uint64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const int64_t &left, const bool &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const int64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const uint64_t &left, const int64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const uint64_t &left, const uint64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const uint64_t &left, const bool &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const uint64_t &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const bool &left, const uint64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const bool &left, const int64_t &right) {
            return F()(left, right);
        }

        flavor::expression_v operator()(const bool &left, const bool &right) {
            return F()(static_cast<uint64_t>(left), static_cast<uint64_t>(right));
        }

        flavor::expression_v operator()(const bool &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const std::vector<bool> &left, const int64_t &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const std::vector<bool> &left, const bool &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const std::vector<bool> &left, const uint64_t &right) {
            assert(false && "WAT?!");
            return false;
        }

        flavor::expression_v operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
            assert(false && "WAT?!");
            return false;
        }
    };

    typedef arithmetical_t<std::minus<>> minus_t;
    typedef arithmetical_t<std::plus<>> plus_t;

    auto find_functor(const std::string &op) {
        static const std::map<std::string, std::function<flavor::expression_v(const flavor::expression_v &, const flavor::expression_v &)>> functors{
            {"||", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(logicar_or_t(), left, right); }},
            {"==", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(equality_t(), left, right); }},
            {"<", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(less_t(), left, right); }},
            {">", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(greater_t(), left, right); }},
            {"-", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(minus_t(), left, right); }},
            {"+", [](const flavor::expression_v &left, const flavor::expression_v &right) { return std::visit(plus_t(), left, right); }},
        };

        return functors.at(op);
    }

    class parse_t {
    public:
        explicit parse_t(bitreader_t &bitreader, const flavor::document_t &doc)
            : bitreader(bitreader), doc(doc) {
        }


        std::shared_ptr<flavor::value_t> on(std::shared_ptr<flavor::node_t> node) {
            return std::visit(*this, node->payload);
        }

        std::shared_ptr<flavor::value_t> operator()(const flavor::bslbf_t &node) {
            auto value = bitreader.read(node.length);
            auto v = std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(node, value)});
            symbols[node.name] = v;
            return v;
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::uimsbf_t &node) {
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
            auto v = std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(node, value)});
            symbols[node.name] = v;
            return v;
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::bitstring_t &node) {
            auto value = khaotica::algorithm::unpack(node.value);
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::integer_t &node) {
            auto value = node.value;
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::identifier_t &node) {

            auto symbol = symbols.find(node.name);
            if (symbol != symbols.end()) {
                flavor::expression_v value = extract(*symbol->second);
                auto v = std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, value)});
                return v;
            }

            auto definition = lookup(node.name, scope);
            if (definition) {
                auto value = on(definition);
                return value;
            }

            definition = deep_lookup(node.name, scope);
            if(definition){
                auto value = std::visit(default_t(), definition->payload);
                return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, value)});
            }

            assert(false && "It is REALLY bad to be there and try to use unresolved symbol");
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::reference_t &node) {
            std::list<std::shared_ptr<flavor::value_t>> value;

            auto it = doc.definitions.find(node.name);
            assert(it != doc.definitions.end());
            return on(it->second);
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::if_t &node) {
            scope = node.scope;

            auto conditional_expression = on(node.condition);
            auto conditional_expression_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(conditional_expression->payload);
            auto conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);

            std::list<std::shared_ptr<flavor::value_t>> value;
            if (conditional_expression_value) {
                for (auto &&item : node._then) {
                    value.push_back(on(item));
                }
            } else {
                for (auto &&item : node._else) {
                    value.push_back(on(item));
                }
            }

            scope = node.scope->parent;
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(node, flavor::if_v{ conditional_expression_value, value} )});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::for_t &node) {

            scope = node.scope;

            if(node.initializer){
                auto initializer = on(*node.initializer);
            }

            bool conditional_expression_value(true);
            if(node.condition){
                auto conditional_expression = on(*node.condition);
                auto conditional_expression_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(conditional_expression->payload);
                conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);
            }

            std::vector<flavor::for_v::iteration_t> value;
            while(conditional_expression_value){

                flavor::for_v::iteration_t iteration;
                for (auto &&item : node.body) {
                    iteration.push_back(on(item));
                }
                value.emplace_back(std::move(iteration));

                if(node.modifier){
                    auto modifier = on(*node.modifier);
                }

                if(node.condition){
                    auto conditional_expression = on(*node.condition);
                    auto conditional_expression_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(conditional_expression->payload);
                    conditional_expression_value = std::visit(conditional_t(), conditional_expression_payload.second);
                }
            }
            scope = node.scope->parent;
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(node, flavor::for_v{value} )});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::compound_t &node) {
            scope = node.scope;

            std::list<std::shared_ptr<flavor::value_t>> value;
            for (auto &&item : node.body) {
                value.push_back(on(item));
            }
            scope = node.scope->parent;
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::assignment_t &node) {
            auto initializer = on(node.expression);
            symbols[node.symbol] = initializer;
            auto initializer_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(initializer->payload);
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, flavor::expression_v{initializer_payload.second})});
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::preincrement_t &node) {
            return nullptr;
        };


        std::shared_ptr<flavor::value_t> operator()(const flavor::postincrement_t &node) {
            auto previous_value = symbols.find(node.operand)->second;
            auto& previous_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(previous_value->payload);
            auto value = std::visit(find_functor(node.operation), previous_payload.second, flavor::expression_v{uint64_t{1}});

            previous_payload.second = value;
            return previous_value;
        };


        std::shared_ptr<flavor::value_t> operator()(const flavor::unary_expression_t &node) {
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const flavor::binary_expression_t &node) {
            auto left = on(node.left_operand);
            auto right = on(node.right_operand);

            auto left_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(left->payload);
            auto right_payload = std::get<std::pair<flavor::expression_t, flavor::expression_v>>(right->payload);

            auto value = std::visit(find_functor(node.operation), left_payload.second, right_payload.second);
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair(flavor::expression_t{node}, flavor::expression_v{value})});
        };

    private:
        flavor::expression_v extract(const flavor::value_t &value) {
            if( const auto&& val = std::get_if<std::pair<flavor::bslbf_t, std::vector<bool>>>(&value.payload)){
                return val->second;
            } else if(const auto&& val = std::get_if<std::pair<flavor::uimsbf_t, uint64_t>>(&value.payload)) {
                return val->second;
            } else if(const auto&& val = std::get_if<std::pair<flavor::expression_t, flavor::expression_v>>(&value.payload)){
                return val->second;
            }
            assert(false && "No way to be there");
            return false;
        }

        std::shared_ptr<flavor::node_t> lookup(const std::string& name, flavor::scope_t* scope) {

            auto it = scope->definitions.find(name);
            if(it != scope->definitions.end()){
                return it->second;
            }

            if( scope->parent){
                return lookup(name, scope->parent);
            }

            return nullptr;
        }

        std::shared_ptr<flavor::node_t> deep_lookup(const std::string& name, flavor::scope_t* scope) {

            auto it = scope->definitions.find(name);
            if(it != scope->definitions.end()){
                return it->second;
            }

            if( scope->parent){
                for (auto &&sibling : scope->parent->childs) {
                    it = sibling->definitions.find(name);
                    if(it != sibling->definitions.end()){
                        return it->second;
                    }
                }

                return deep_lookup(name, scope->parent);
            }

            return nullptr;
        }

    private:
        bitreader_t &bitreader;
        const flavor::document_t &doc;
        std::map<std::string, std::shared_ptr<flavor::value_t>> symbols;
        flavor::scope_t* scope{nullptr};
    };
}

void parser_t::parse(bitreader_t &in, const flavor::document_t &doc) {
    parse_t parse(in, doc);
    flavor::snapshot_t snapshot;
    for (auto &&entry : doc.structure) {
        snapshot[entry] = parse.on(entry);
    }

    int i = 0;
}
