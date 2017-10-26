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

    struct node_t{
        virtual ~node_t() = default;
    };
    typedef std::list<std::shared_ptr<node_t>> entries_t;

    struct expression_t : node_t {
        virtual ~expression_t() = default;
    };

    struct bslbf_t : expression_t{
        bslbf_t(const std::string &name, uint64_t length)
            : name(name), length(length) {}

        std::string name;
        uint64_t length;
    };

    struct uimsbf_t: expression_t{
        uimsbf_t(const std::string &name, uint64_t length)
            : name(name), length(length) {}

        std::string name;
        uint64_t length;
    };

    struct bitstring_t : expression_t{
        bitstring_t(const std::string &value)
            : value(value) {}

        std::string value;
    };

    struct integer_t : expression_t{
        integer_t(int64_t value)
            : value(value) {}

        int64_t value;
    };

    struct identifier_t : expression_t {
        identifier_t(const std::string &name)
            : name(name) {}

        std::string name;
    };

    struct if_t : node_t{
        if_t(const std::shared_ptr<expression_t> &condition, const entries_t &_then, const std::optional<entries_t> &_else)
            : condition(condition), _then(_then), _else(_else) {}

        std::shared_ptr<expression_t> condition;
        entries_t _then;
        std::optional<entries_t> _else;
    };

    struct for_t : node_t{
        for_t(const std::optional<std::shared_ptr<expression_t>> &initializer, const std::optional<std::shared_ptr<expression_t>> &condition,
              const std::optional<std::shared_ptr<expression_t>> &modifier, const entries_t &body)
            : initializer(initializer), condition(condition), modifier(modifier), body(body) {}

        std::optional<std::shared_ptr<expression_t>> initializer;
        std::optional<std::shared_ptr<expression_t>> condition;
        std::optional<std::shared_ptr<expression_t>> modifier;
        entries_t body;
    };

    struct compound_t : node_t{
        compound_t(const std::string &value, const entries_t &body = {})
            : value(value), body(body) {}

        std::string value;
        entries_t body;
    };

    template<typename Function>
    struct unary_expression_t : expression_t{
        unary_expression_t(const std::shared_ptr<expression_t> &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::shared_ptr<expression_t> operand;
        Function operation;
    };

    template<typename Function>
    struct binary_expression_t : expression_t {
        binary_expression_t(const std::shared_ptr<expression_t> &left_operand, const std::shared_ptr<expression_t> &right_operand, Function operation = {})
            : left_operand(left_operand), right_operand(right_operand), operation(operation) {}

        std::shared_ptr<expression_t> left_operand;
        std::shared_ptr<expression_t> right_operand;
        Function operation;
    };

    template<typename Function>
    struct postincrement_t : expression_t{
        postincrement_t(const std::string &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::string operand;
        Function operation;
    };

    template<typename Function>
    struct preincrement_t : expression_t{
        preincrement_t(const std::string &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::string operand;
        Function operation;
    };

    struct assignment_t : expression_t{
        assignment_t(const std::string &symbol, const std::shared_ptr<expression_t> &expression)
            : symbol(symbol), expression(expression) {}

        std::string symbol;
        std::shared_ptr<expression_t> expression;
    };

    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

    typedef std::list<std::shared_ptr<node_t>> ast_t;

    struct document_t{
        ast_t hierarchy;
        definitions_t definitions;
    };
}

#endif //KHAOTICA_GRAMMAR_H


