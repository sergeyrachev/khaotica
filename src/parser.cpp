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
            return {F()(v)};
        }
    };

    template<>
    class eval_unary_expression_t<std::negate<>>{
    public:
        flavor::value_t operator()(const std::vector<bool> &v) {
            assert(false && "It is really bad to apply negate operation to bitstring");
            return {false};
        }
        flavor::value_t operator()(const bool &v) {
            assert(false && "It is really bad to apply negate operation to bool");
            return {false};
        }
        flavor::value_t operator()(const std::pair<bool, std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply negate operation to if expression value");
            return {false};
        }
        flavor::value_t operator()(const std::list<std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply negate operation to compound value");
            return {false};
        }
    };

    template<>
    class eval_unary_expression_t<std::logical_not<>>{
    public:
        flavor::value_t operator()(const std::vector<bool> &v) {
            assert(false && "It is really bad to apply logical_not operation to bitstring");
            return {false};
        }
        flavor::value_t operator()(const bool &v) {
            assert(false && "It is really bad to apply logical_not operation to bool");
            return {false};
        }
        flavor::value_t operator()(const std::pair<bool, std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply logical_not operation to if expression value");
            return {false};
        }
        flavor::value_t operator()(const std::list<std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply logical_not operation to compound value");
            return {false};
        }
    };

    template<>
    class eval_unary_expression_t<std::bit_not<>>{
    public:
        flavor::value_t operator()(const std::vector<bool> &v) {
            assert(false && "It is really bad to apply bit_not operation to bitstring");
            return {false};
        }
        flavor::value_t operator()(const bool &v) {
            assert(false && "It is really bad to apply bit_not operation to bool");
            return {false};
        }
        flavor::value_t operator()(const std::pair<bool, std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply bit_not operation to if expression value");
            return {false};
        }
        flavor::value_t operator()(const std::list<std::shared_ptr<flavor::value_t>> &v) {
            assert(false && "It is really bad to apply bit_not operation to compound value");
            return {false};
        }
    };

    template<typename F>
    class eval_binary_expression_t{
    public:
        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return F()(u, v);
        }
    };

    template<typename... Args>
    template<Args...args>
    class eval_binary_expression_t{
    public:
        template<typename U, typename V>
        flavor::value_t operator()(const U &u, const V& v) {
            return args()(u, v);
        }
    };

    class evaluation_t{
    public:
        template<typename F, typename T>
        flavor::value_t operator()(const F &op, const T &v) {
            return {};
        }

        template<typename L, typename F, typename R>
        flavor::value_t operator()(const L &left_operand, const F &operation, const R &right_operand) {
            return {};
        }

//        flavor::value_t operator()(const std::negate<> &op, const int64_t &v) {
//            return {int64_t(op(v))};
//        }
//
//        flavor::value_t operator()(const std::logical_not<> &op, const int64_t &v) {
//            return {int64_t(op(v))};
//        }
//
//        flavor::value_t operator()(const std::logical_not<> &op, const uint64_t &v) {
//            return {uint64_t(op(v))};
//        }
//
//        flavor::value_t operator()(const std::bit_not<> &op, const std::vector<bool> &v) {
//            auto ret = v;
//            ret.flip();
//            return {ret};
//        }
//
//        flavor::value_t operator()(const std::bit_not<> &op, const uint64_t &v) {
//            return {op(v)};
//        }
//
//        flavor::value_t operator()(const std::bit_not<> &op, const int64_t &v) {
//            return {op(v)};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::less<> &operation, const int64_t &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::less<> &operation, const uint64_t &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const int64_t &right_operand) {
//            return {operation(left_operand, right_operand)};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const bool &right_operand) {
//            return {operation(left_operand, (int64_t) right_operand)};
//        }
//
//        flavor::value_t operator()(const uint64_t &left_operand, const std::minus<> &operation, const bool &right_operand) {
//            return {operation(left_operand, (uint64_t) right_operand)};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const uint64_t &right_operand) {
//            return {operation(left_operand, right_operand)};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::plus<> &operation, const int64_t &right_operand) {
//            return {operation(left_operand, right_operand)};
//        }
//
//        flavor::value_t operator()(const int64_t &left_operand, const std::greater<> &operation, const int64_t &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const uint64_t &left_operand, const std::greater<> &operation, const int64_t &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const uint64_t &left_operand, const std::logical_or<> &operation, const uint64_t &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const std::vector<bool> &left_operand, const std::equal_to<> &operation, const std::vector<bool> &right_operand) {
//            auto l = khaotica::algorithm::to_string(left_operand);
//            auto r = khaotica::algorithm::to_string(right_operand);
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const std::vector<bool> &left_operand, const std::less<> &operation, const std::vector<bool> &right_operand) {
//            auto l = khaotica::algorithm::to_string(left_operand);
//            auto r = khaotica::algorithm::to_string(right_operand);
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const std::vector<bool> &left_operand, const std::greater<> &operation, const std::vector<bool> &right_operand) {
//            auto l = khaotica::algorithm::to_string(left_operand);
//            auto r = khaotica::algorithm::to_string(right_operand);
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
//
//        flavor::value_t operator()(const bool &left_operand, const std::logical_or<> &operation, const bool &right_operand) {
//            return {uint64_t(operation(left_operand, right_operand))};
//        }
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
            return {false};
        }

        flavor::value_t on( const flavor::preincrement_t& node, traversal_t& traversal){
            return {false};
        }

        flavor::value_t on( const flavor::assignment_t& node, traversal_t& traversal){
            return traversal(*node.expression);
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::bslbf_t& definition, traversal_t& traversal){
            return {false};
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::uimsbf_t& definition, traversal_t& traversal){
            return {false};
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::tcimsbf_t& definition, traversal_t& traversal){
            return {false};
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::compound_definition_t& definition, traversal_t& traversal){
            return on_(definition, traversal);
        }

        flavor::value_t on( const flavor::symbol_t& node, const flavor::assignment_t& definition, traversal_t& traversal){
            return {false};
        }

        flavor::values_t values;

    protected:
        flavor::value_t on_( const flavor::compound_definition_t& node, traversal_t& traversal){

            std::list<std::shared_ptr<flavor::value_t>> sequence;
            for (auto &&entry : node.entries) {
                sequence.push_back( std::make_shared<flavor::value_t>(std::visit(traversal, entry)));
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
