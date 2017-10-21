#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include <type_traits>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <map>
#include <list>
#include <memory>
#include <cstddef>

namespace flavor{

    struct compound_definition_t;
    struct expression_t;
    struct value_t;

    struct bslbf_t{
        std::string name;
        uint64_t length;
    };

    struct uimsbf_t{
        std::string name;
        uint64_t length;
    };

    struct tcimsbf_t{
        std::string name;
        uint64_t length;
    };

    struct symbol_t{
        std::string name;
    };

    struct bitstring_t{
        std::string value;
    };

    struct integer_t{
        uint64_t value;
    };

    struct if_t{
        std::shared_ptr<expression_t> condition;
        std::shared_ptr<compound_definition_t> _then;
        std::optional<std::shared_ptr<compound_definition_t>> _else;
    };

    struct for_t {
        std::optional<std::shared_ptr<expression_t>> initializer;
        std::optional<std::shared_ptr<expression_t>> condition;
        std::optional<std::shared_ptr<expression_t>> modifier;
        std::shared_ptr<compound_definition_t> body;
    };

    typedef std::variant<
        bslbf_t,
        uimsbf_t,
        tcimsbf_t,
        if_t,
        for_t,
        symbol_t
    > entry_t;

    typedef std::list<entry_t> entries_t;
    struct compound_definition_t{
        entries_t entries;
    };

    template<typename T>
    struct cast_t{
        template<typename U>
        T operator()(const U& u){
            return static_cast<T>(std::decay(u));
        }
    };

    struct unary_expression_t {
        std::variant<
            std::negate<>,
            std::logical_not<>,
            std::bit_not<>,
            cast_t<bool>
        > operation;
        std::shared_ptr<expression_t> operand;
    };

    struct binary_expression_t {
        std::shared_ptr<expression_t> left_operand;
        std::variant<
            std::plus<>,
            std::minus<>,
            std::multiplies<>,
            std::divides<>,
            std::modulus<>,
            std::logical_and<>,
            std::logical_or<>,
            std::not_equal_to<>,
            std::equal_to<>,
            std::less<>,
            std::greater<>,
            std::less_equal<>,
            std::greater_equal<>
        > operation;
        std::shared_ptr<expression_t> right_operand;
    };

    struct assignment_t {
        symbol_t symbol;
        std::shared_ptr<expression_t> expression;
    };

    struct postincrement_t{
        symbol_t operand;
        std::variant<
            std::plus<>,
            std::minus<>
        > operation;
    };

    struct preincrement_t{
        symbol_t operand;
        std::variant<
            std::plus<>,
            std::minus<>
        > operation;
    };

    struct expression_t {
        std::variant<
            symbol_t,
            integer_t,
            bitstring_t,
            unary_expression_t,
            binary_expression_t,
            postincrement_t,
            preincrement_t,
            assignment_t
        > sentence;
    };

    typedef std::map<
        const std::string, std::variant<
            bslbf_t,
            uimsbf_t,
            tcimsbf_t,
            compound_definition_t,
            assignment_t>
    > definitions_t;

    typedef std::list<symbol_t> ast_t;

    struct document_t{
        ast_t hierarchy;
        definitions_t symbols;
    };

    struct value_t{
        std::variant<
            std::vector<bool>,
            uint64_t,
            int64_t,
            bool,
            std::list<std::shared_ptr<value_t>>
        > value;
    };
    typedef std::map< const std::string, value_t > values_t;
}

#endif //KHAOTICA_GRAMMAR_H


