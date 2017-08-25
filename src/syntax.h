#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <map>
#include <list>
#include <memory>
#include <cstddef>

namespace flavor{

    struct variable_definition_t;
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
        std::shared_ptr<bslbf_t>,
        std::shared_ptr<uimsbf_t>,
        std::shared_ptr<tcimsbf_t>,
        std::shared_ptr<if_t>,
        std::shared_ptr<for_t>,
        std::shared_ptr<compound_t>
    > entry_t;

    typedef std::list<entry_t> entries_t;
    struct compound_definition_t{
        entries_t entries;
    };

    struct  unary_expression_t {
        std::variant<
            std::negate<expression_t>,
            std::logical_not<expression_t>,
            std::bit_not<expression_t>
        > operation;
        std::shared_ptr<expression_t> operand;
    };

    struct  binary_expression_t {
        std::shared_ptr<expression_t> left_operand;
        std::variant<
            std::plus<expression_t>,
            std::minus<expression_t>,
            std::multiplies<expression_t>,
            std::divides<expression_t>,
            std::modulus<expression_t>,
            std::logical_and<expression_t>,
            std::logical_or<expression_t>,
            std::not_equal_to<expression_t>,
            std::equal_to<expression_t>,
            std::less<expression_t>,
            std::greater<expression_t>
        > operation;
        std::shared_ptr<expression_t> right_operand;
    };

    struct expression_t {
        std::variant<
            std::string,
            integer_t,
            bitstring_t,
            unary_expression_t,
            binary_expression_t,
            std::shared_ptr<expression_t>
        > sentence;
    };

    typedef std::variant<
        int64_t,
        uint64_t,
        std::byte,
        std::vector<bool>
    > value_t;

    typedef std::list<std::variant<std::shared_ptr<variable_t>, std::shared_ptr<compound_t>>> document_t;

    typedef std::map<
        std::string, std::variant<
            std::shared_ptr<bslbf_t>,
            std::shared_ptr<uimsbf_t>,
            std::shared_ptr<tcimsbf_t>,
            std::shared_ptr<compound_definition_t>,
            std::shared_ptr<expression_t>
        >
    > symbols_t;

    typedef std::map<std::string, value_t > values_t;
}

#endif //KHAOTICA_GRAMMAR_H


