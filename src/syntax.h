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

    struct compound_t{
        std::string name;
    };

    struct bitstring_t{
        std::string value;
    };

    struct integer_t{
        int64_t value;
    };

    struct variable_t{
        std::string name;
    };

    struct if_t{
        std::shared_ptr<expression_t> condition;
        std::shared_ptr<compound_definition_t> _then;
        std::optional<std::shared_ptr<compound_definition_t>> _else;
    };

    struct for_t {
        std::optional<variable_t> counter;
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
        compound_t
    > entry_t;

    typedef std::list<entry_t> entries_t;
    struct compound_definition_t{
        entries_t entries;
    };

    struct unary_expression_t {
        std::variant<
            std::negate<>,
            std::logical_not<>,
            std::bit_not<>
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
            std::greater<>
        > operation;
        std::shared_ptr<expression_t> right_operand;
    };

    struct expression_t {
        std::variant<
            variable_t,
            integer_t,
            bitstring_t,
            unary_expression_t,
            binary_expression_t,
            std::shared_ptr<const expression_t>
        > sentence;
    };

    typedef std::list<
        std::variant<
            variable_t,
            compound_t
        >
    > document_t;

    typedef std::variant<
        bslbf_t,
        uimsbf_t,
        tcimsbf_t,
        compound_definition_t,
        expression_t
    > symbol_t;
    typedef std::map< const std::string, symbol_t > symbols_t;

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

    // TODO: It is a temporary solution; there is a plan to print if\for evaluation steps also.
    inline bool to_boolean(const value_t& value){
        return std::visit([](auto&& value)->bool {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::vector<bool>> ) {
                return {!value.empty()};
            } else if constexpr (std::is_same_v<T, std::list<std::shared_ptr<value_t>>> ) {
                return {!value.empty()};
            } else {
                return value;
            }
        }, value.value);
    }
}

#endif //KHAOTICA_GRAMMAR_H


