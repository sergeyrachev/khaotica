// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include <fstream>
#include "parser.h"
#include <variant>

#include "syntax.h"
#include "logging.h"

using namespace khaotica;

bool cast_to_bool(const flavor::value_t& value){
    return std::visit( [](auto&& v) -> bool{
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::vector<bool> > ) {
            return {!v.empty()};
        } else if constexpr (std::is_same_v<T, std::list<std::shared_ptr<flavor::value_t>>> ) {
            return {!v.empty()};
        } else {
            return !!v;
        }
    }, value.value);
}

class evaluation_t{
    const flavor::symbols_t &symbols;
    flavor::values_t &values;
public:
    evaluation_t( const flavor::symbols_t &symbols, flavor::values_t &values ):symbols(symbols), values(values){

    }

    template<typename F, typename T>
    flavor::value_t operator()(const F &op, const T &v) {
        return {};
    }

    template<typename L, typename F, typename R>
    flavor::value_t operator()(const L &left_operand, const F &operation, const R &right_operand) {
        return {};
    }

    flavor::value_t operator()(const std::negate<> &op, const int64_t &v) {
        return {int64_t(op(v))};
    }

    flavor::value_t operator()(const std::logical_not<> &op, const int64_t &v) {
        return {int64_t(op(v))};
    }

    flavor::value_t operator()(const std::logical_not<> &op, const uint64_t &v) {
        return {uint64_t(op(v))};
    }

    flavor::value_t operator()(const std::bit_not<> &op, const std::vector<bool> &v) {
        auto ret = v;
        ret.flip();
        return {ret};
    }

    flavor::value_t operator()(const std::bit_not<> &op, const uint64_t &v) {
        return {op(v)};
    }

    flavor::value_t operator()(const std::bit_not<> &op, const int64_t &v) {
        return {op(v)};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::less<> &operation, const int64_t &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::less<> &operation, const uint64_t &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const int64_t &right_operand) {
        return {operation(left_operand, right_operand)};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const bool &right_operand) {
        return {operation(left_operand, (int64_t) right_operand)};
    }

    flavor::value_t operator()(const uint64_t &left_operand, const std::minus<> &operation, const bool &right_operand) {
        return {operation(left_operand, (uint64_t) right_operand)};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::minus<> &operation, const uint64_t &right_operand) {
        return {operation(left_operand, right_operand)};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::plus<> &operation, const int64_t &right_operand) {
        return {operation(left_operand, right_operand)};
    }

    flavor::value_t operator()(const int64_t &left_operand, const std::greater<> &operation, const int64_t &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const uint64_t &left_operand, const std::greater<> &operation, const int64_t &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const uint64_t &left_operand, const std::logical_or<> &operation, const uint64_t &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const std::vector<bool> &left_operand, const std::equal_to<> &operation, const std::vector<bool> &right_operand) {
        auto l = khaotica::algorithm::to_string(left_operand);
        auto r = khaotica::algorithm::to_string(right_operand);
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const std::vector<bool> &left_operand, const std::less<> &operation, const std::vector<bool> &right_operand) {
        auto l = khaotica::algorithm::to_string(left_operand);
        auto r = khaotica::algorithm::to_string(right_operand);
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const std::vector<bool> &left_operand, const std::greater<> &operation, const std::vector<bool> &right_operand) {
        auto l = khaotica::algorithm::to_string(left_operand);
        auto r = khaotica::algorithm::to_string(right_operand);
        return {uint64_t(operation(left_operand, right_operand))};
    }

    flavor::value_t operator()(const bool &left_operand, const std::logical_or<> &operation, const bool &right_operand) {
        return {uint64_t(operation(left_operand, right_operand))};
    }


    flavor::value_t operator() ( const std::shared_ptr<flavor::expression_t>& expression){
       return std::visit(*this, expression->sentence);
    }

    flavor::value_t operator()(const flavor::bitstring_t &node) {
        return {khaotica::algorithm::unpack(node.value)};
    }

    flavor::value_t operator()(const flavor::integer_t &node) {
        return {node.value};
    }

    flavor::value_t operator()(const flavor::assignment_t &node) {
        auto value = (*this)(node.expression);
        values[node.variable.name] = value;
        return value;
    }

    flavor::value_t operator()(const flavor::variable_t &node) {

        auto it = values.find(node.name);
        if(it != values.end()){
            return it->second;
        }

        return {int64_t(0)};
    }

    flavor::value_t operator()(const flavor::unary_expression_t &node) {
        auto operand = (*this)(node.operand);
        return std::visit(*this, node.operation, operand.value);
    }

    flavor::value_t operator()(const flavor::binary_expression_t &node) {
        auto left_operand = (*this)(node.left_operand);
        auto right_operand = (*this)(node.right_operand);
        return std::visit(*this, left_operand.value, node.operation, right_operand.value);
    }

    flavor::value_t operator()(const flavor::postincrement_t &node) {
        auto operand = (*this)(node.operand);
        flavor::value_t inc;
        inc.value = static_cast<int64_t>(1);
        return std::visit(*this, operand.value, node.operation, inc.value);
    }

    flavor::value_t operator()(const flavor::preincrement_t &node) {
        auto operand = (*this)(node.operand);
        flavor::value_t inc;
        inc.value = static_cast<int64_t>(1);
        return std::visit(*this, operand.value, node.operation, inc.value);
    }
};

class khaotica::parser_t::impl_t {
public:
    explicit impl_t(bitreader_t &bitreader, const flavor::symbols_t &symbols, flavor::values_t &values)
        : bitreader(bitreader), symbols(symbols), values(values) {
    }

    void operator( )(const flavor::bslbf_t &bs) {
        auto value = bitreader.read(bs.length);
        values.emplace(bs.name, flavor::value_t{value});
    }

    void operator( )(const flavor::uimsbf_t &bs) {
        auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(bs.length));
        values.emplace(bs.name, flavor::value_t{value});
    }

    void operator( )(const flavor::if_t &node) {
        auto value = cast_to_bool(evaluation_t(symbols, values)(node.condition));
        if (value) {
            auto& definition = *node._then;
            for (const auto &entry : definition.entries) {
                std::visit(*this, entry);
            }

        } else if (node._else) {
            auto& definition = **node._else;
            for (const auto &entry : definition.entries) {
                std::visit(*this, entry);
            }
        }
    }

    void operator( )(const flavor::for_t &node) {

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
    }

    void operator()(const auto &) {
        flavor::values_t values;
    }

    void operator()(const flavor::compound_t &compound) {
        auto it = symbols.find(compound.name);
        assert(it != symbols.end());

        const auto& definition = std::get<flavor::compound_definition_t>(it->second);
        for (const auto &entry : definition.entries) {
            std::visit(*this, entry);
        }
    }

    void operator()(const flavor::variable_t & variable) {
        auto it = symbols.find(variable.name);
        assert(it != symbols.end());
    }

private:
    bitreader_t &bitreader;
    const flavor::symbols_t &symbols;
    flavor::values_t &values;
};

flavor::values_t parser_t::parse(std::ifstream &in, const flavor::document_t &doc, const flavor::symbols_t &symbols) {
    bitreader_t bitreader(in);
    flavor::values_t values;
    impl_t impl(bitreader, symbols, values);
    for (auto &&entry : doc) {
        std::visit(impl, entry);
    }
    return values;
}
