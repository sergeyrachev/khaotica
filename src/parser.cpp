// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include "traversal.h"
#include <fstream>

#include <boost/mpl/for_each.hpp>

using namespace khaotica;

namespace {
    template<typename F>
    class eval_unary_expression_t{
    public:
        template<typename T>
        flavor::value_t operator()(const T &v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_unary_expression_t<std::negate<>>{
    public:
        flavor::value_t operator()(const uint64_t &v) {
            return {-v};
        }
        flavor::value_t operator()(const int64_t &v) {
            return {-v};
        }

        template<typename T>
        flavor::value_t operator()(const T &v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_unary_expression_t<std::logical_not<>>{
    public:
        flavor::value_t operator()(const uint64_t &v) {
            return {!v};
        }
        flavor::value_t operator()(const int64_t &v) {
            return {!v};
        }
        flavor::value_t operator()(const bool &v) {
            return {!v};
        }
        template<typename T>
        flavor::value_t operator()(const T &v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_unary_expression_t<std::bit_not<>>{
    public:
        flavor::value_t operator()(const uint64_t &v) {
            return {~v};
        }
        flavor::value_t operator()(const int64_t &v) {
            return {~v};
        }
        flavor::value_t operator()(const bool &v) {
            return {bool{~v}};
        }
        template<typename T>
        flavor::value_t operator()(const T &v) {
            return {bool{false}};
        }
    };

    template<typename F>
    class eval_binary_expression_t{
    public:
        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_binary_expression_t<std::minus<>>{
    public:
        flavor::value_t operator()(const int64_t &u, const int64_t& v) {
            return {u - v};
        }

        flavor::value_t operator()(const uint64_t &u, const uint64_t& v) {
            return {u - v};
        }

        flavor::value_t operator()(const uint64_t &u, const bool& v) {
            return {u - uint64_t(v)};
        }

        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_binary_expression_t<std::greater<>>{
    public:
        flavor::value_t operator()(const int64_t &u, const int64_t& v) {
            return {u > v};
        }

        flavor::value_t operator()(const uint64_t &u, const uint64_t& v) {
            return {u > v};
        }

        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_binary_expression_t<std::less<>>{
    public:
        flavor::value_t operator()(const int64_t &u, const int64_t& v) {
            return {u < v};
        }

        flavor::value_t operator()(const uint64_t &u, const uint64_t& v) {
            return {u < v};
        }

        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_binary_expression_t<std::logical_or<>>{
    public:
        flavor::value_t operator()(const bool &u, const bool& v) {
            return {u || v};
        }
        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    template<>
    class eval_binary_expression_t<std::equal_to<>>{
    public:
        template<typename U>
        flavor::value_t operator()(const U &u, const U& v) {
            return {u == v};
        }

        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return {bool{false}};
        }
    };

    class parse_t {
    public:
        typedef khaotica::traversal_t<parse_t, flavor::value_t> traversal_t;

        explicit parse_t(bitreader_t& bitreader, const flavor::document_t &doc):bitreader(bitreader), doc(doc){

        }

        flavor::value_t on(const flavor::bslbf_t &node, traversal_t& traversal){
            auto value = bitreader.read(node.length);
            values.emplace(node.name, flavor::value_t{value});
            return {value};
        }

        flavor::value_t on(const flavor::uimsbf_t &node, traversal_t& traversal){
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
            values.emplace(node.name, flavor::value_t{value});
            return {value};
        }

        flavor::value_t on(const flavor::tcimsbf_t &node, traversal_t& traversal){
            logging::debug() << "tcimsbf_t is not implemented yet";
            return {false};
        }

        flavor::value_t on(const flavor::if_t& node, traversal_t& traversal){

            std::list<std::shared_ptr<flavor::value_t>> empty;

            flavor::value_t value{std::make_pair(false, std::make_shared<flavor::value_t>(flavor::value_t{empty}))};

            auto condition = traversal(*node.condition);
            if(std::get<bool>(condition.value)){
                value = flavor::value_t{std::make_pair(true, std::make_shared<flavor::value_t>(on_(*node._then, traversal)))};
            } else if(node._else){
                value = flavor::value_t{std::make_pair(false, std::make_shared<flavor::value_t>(on_(**node._else, traversal)))};
            }
            values.emplace("if", value);
            return value;
        }

        flavor::value_t on(const flavor::for_t& node, traversal_t& traversal){

            if(node.initializer){
                auto initializer = traversal(**node.initializer);
                initializer;;
            }

            flavor::value_t condition{true};
            if(node.condition){
                condition = traversal(**node.condition);
                condition;;
            }

            std::list<std::shared_ptr<flavor::value_t>> sequence;

            while(std::get<bool>(condition.value)){
                auto value = on_(*node.body, traversal);
                sequence.push_back(std::make_shared<flavor::value_t>(value));

                if(node.modifier){
                    auto modifier = traversal(**node.modifier);
                    modifier;;
                }

                if(node.condition){
                    condition = traversal(**node.condition);
                    condition;;
                }
            }

            values.emplace("for", flavor::value_t{sequence});
        }

        flavor::value_t on( const flavor::integer_t& node, traversal_t& traversal){
            return {node.value};
        }

        flavor::value_t on( const flavor::bitstring_t& node, traversal_t& traversal){
            return {khaotica::algorithm::unpack(node.value)};
        }

        template<typename F>
        flavor::value_t on( const F& operation, const flavor::expression_t& operand, traversal_t& traversal){
            auto operand_value = traversal(operand);
            return std::visit(eval_unary_expression_t<F>(), operand_value.value);
        }

        template<typename F>
        flavor::value_t on( const flavor::expression_t& left, const F& operation, const flavor::expression_t& right, traversal_t& traversal){
            auto left_value = traversal(left);
            auto right_value = traversal(right);
            return std::visit(eval_binary_expression_t<F>(), left_value.value, right_value.value);
        }

        flavor::value_t on( const flavor::postincrement_t& node, traversal_t& traversal){
            auto range = values.equal_range(node.operand.name);
            auto value = range.second->second;
            auto ref = std::get<uint64_t>(value.value);

            value = flavor::value_t{ref + 1};
            values.emplace(node.operand.name, value);
            return value;
        }

        flavor::value_t on( const flavor::preincrement_t& node, traversal_t& traversal){
            return {false};
        }

        flavor::value_t on( const flavor::assignment_t& node, traversal_t& traversal){
            auto value = traversal(*node.expression);
            values.emplace(node.symbol.name, value);
            return value;
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::bslbf_t& definition, traversal_t& traversal){
            return values.equal_range(node.name).first->second;
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::uimsbf_t& definition, traversal_t& traversal){
            auto range = values.equal_range(node.name);
            if(range.first == range.second){
                return {uint64_t{0}};
            }
            return range.second->second;
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::tcimsbf_t& definition, traversal_t& traversal){
            return values.equal_range(node.name).first->second;
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::compound_definition_t& definition, traversal_t& traversal){
            return on_(definition, traversal);
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::assignment_t& definition, traversal_t& traversal){
            auto range = values.equal_range(node.name);
            if(range.first == range.second){
                auto value = traversal(*definition.expression);
                values.emplace(node.name, value);
                return value;
            }
            return range.second->second;
        }

        flavor::values_t values;

    protected:
        flavor::value_t on_( const flavor::compound_definition_t& node, traversal_t& traversal){

            std::list<std::shared_ptr<flavor::value_t>> sequence;
            for (auto &&entry : node.entries) {
                auto value = std::visit(traversal, entry);
                sequence.push_back( std::make_shared<flavor::value_t>(value));
            }

            return {sequence};
        }

    private:
        bitreader_t& bitreader;
        const flavor::document_t &doc;
    };
}

//bool cast_to_bool(const flavor::value_t& value){
//    return std::visit( [](auto&& v) -> bool {
//        using T = std::decay_t<decltype(v)>;
//        if constexpr (std::is_same_v<T, std::vector<bool> > ) {
//            return {!v.empty()};
//        } else if constexpr (std::is_same_v<T, std::list<std::shared_ptr<flavor::value_t>>> ) {
//            return {!v.empty()};
//        } else {
//            return !!v;
//        }
//    }, value.value);
//}


flavor::values_t parser_t::parse(std::ifstream &in, const flavor::document_t &doc) {
    bitreader_t bitreader(in);

    parse_t parse(bitreader, doc);
    parse_t::traversal_t traversal(doc.symbols, parse);
    for(auto&& entry : doc.hierarchy){
        traversal(entry);
    }

    return parse.values;
}

//class khaotica::parser_t::impl_t {
//public:
//    explicit impl_t(bitreader_t &bitreader, const flavor::symbols_t &symbols, flavor::values_t &values)
//        : bitreader(bitreader), symbols(symbols), values(values) {
//    }
//
//
//
//    void operator( )(const flavor::if_t &node) {
//        auto value = cast_to_bool(evaluation_t(symbols, values)(node.condition));
//        if (value) {
//            auto& definition = *node._then;
//            for (const auto &entry : definition.entries) {
//                std::visit(*this, entry);
//            }
//
//        } else if (node._else) {
//            auto& definition = **node._else;
//            for (const auto &entry : definition.entries) {
//                std::visit(*this, entry);
//            }
//        }
//    }
//
//    void operator( )(const flavor::for_t &node) {
//
//        values.emplace(node.counter->name, eval(**node.initializer));
//
//        while (auto condition = flavor::to_boolean(eval(**node.condition))) {
//
//            (*this)(*node.body);
//
//            values[node.counter->name] = eval(**node.modifier);
//        }
//
//        values.erase(node.counter->name);
//    }
//
//    void operator()(const auto &) {
//        flavor::values_t values;
//    }
//
//    void operator()(const flavor::compound_t &compound) {
//        auto it = symbols.find(compound.name);
//        assert(it != symbols.end());
//
//        const auto& definition = std::get<flavor::compound_definition_t>(it->second);
//        for (const auto &entry : definition.entries) {
//            std::visit(*this, entry);
//        }
//    }
//
//    void operator()(const flavor::variable_t & variable) {
//        auto it = symbols.find(variable.name);
//        assert(it != symbols.end());
//    }
//
//private:
//    bitreader_t &bitreader;
//    const flavor::symbols_t &symbols;
//    flavor::values_t &values;
//};
