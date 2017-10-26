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

    struct traversal_t;

    struct node_t{
        virtual ~node_t() = default;
        virtual void process(traversal_t& traversal) = 0;
    };

    template<typename T>
    struct mixin_t : node_t{
        virtual void process(traversal_t& traversal) final {
            traversal.on(static_cast<T&>(*this));
        }
    };

    typedef std::list<std::shared_ptr<node_t>> entries_t;

    struct bslbf_t : mixin_t<bslbf_t>{
        bslbf_t(const std::string &name, uint64_t length)
            : name(name), length(length) {}

        std::string name;
        uint64_t length;
    };

    struct uimsbf_t : mixin_t<uimsbf_t> {
        uimsbf_t(const std::string &name, uint64_t length)
            : name(name), length(length) {}

        std::string name;
        uint64_t length;
    };

    struct bitstring_t : mixin_t<bitstring_t>{
        bitstring_t(const std::string &value)
            : value(value) {}

        std::string value;
    };

    struct integer_t : mixin_t<integer_t>{
        integer_t(int64_t value)
            : value(value) {}

        int64_t value;
    };

    struct identifier_t : mixin_t<identifier_t> {
        identifier_t(const std::string &name)
            : name(name) {}

        std::string name;
    };

    struct if_t : mixin_t<if_t>{
        if_t(const std::shared_ptr<node_t> &condition, const entries_t &_then, const std::optional<entries_t> &_else)
            : condition(condition), _then(_then), _else(_else) {}

        std::shared_ptr<node_t> condition;
        entries_t _then;
        std::optional<entries_t> _else;
    };

    struct for_t : mixin_t<for_t>{
        for_t(const std::optional<std::shared_ptr<node_t>> &initializer, const std::optional<std::shared_ptr<node_t>> &condition,
              const std::optional<std::shared_ptr<node_t>> &modifier, const entries_t &body)
            : initializer(initializer), condition(condition), modifier(modifier), body(body) {}

        std::optional<std::shared_ptr<node_t>> initializer;
        std::optional<std::shared_ptr<node_t>> condition;
        std::optional<std::shared_ptr<node_t>> modifier;
        entries_t body;
    };

    struct compound_t : mixin_t<compound_t>{
        compound_t(const std::string &name, const entries_t &body = {})
            : name(name), body(body) {}

        std::string name;
        entries_t body;
    };

    template<typename Function>
    struct unary_expression_t : mixin_t<unary_expression_t<Function>>{
        unary_expression_t(const std::shared_ptr<node_t> &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::shared_ptr<node_t> operand;
        Function operation;
    };

    template<typename Function>
    struct binary_expression_t : mixin_t<binary_expression_t<Function>> {
        binary_expression_t(const std::shared_ptr<node_t> &left_operand, const std::shared_ptr<node_t> &right_operand, Function operation = {})
            : left_operand(left_operand), right_operand(right_operand), operation(operation) {}

        std::shared_ptr<node_t> left_operand;
        std::shared_ptr<node_t> right_operand;
        Function operation;
    };

    template<typename Function>
    struct postincrement_t : mixin_t<postincrement_t<Function>>{
        postincrement_t(const std::string &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::string operand;
        Function operation;
    };

    template<typename Function>
    struct preincrement_t : mixin_t<preincrement_t<Function>>{
        preincrement_t(const std::string &operand, Function operation = {})
            : operand(operand), operation(operation) {}

        std::string operand;
        Function operation;
    };

    struct assignment_t : mixin_t<assignment_t>{
        assignment_t(const std::string &symbol, const std::shared_ptr<node_t> &expression)
            : symbol(symbol), expression(expression) {}

        std::string symbol;
        std::shared_ptr<node_t> expression;
    };

    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

    typedef std::list<std::shared_ptr<node_t>> ast_t;

    struct document_t{
        ast_t hierarchy;
        definitions_t definitions;
    };

    struct traversal_t{
        virtual void on(bslbf_t&) = 0;
        virtual void on(uimsbf_t&) = 0;
        virtual void on(bitstring_t&) = 0;
        virtual void on(integer_t&) = 0;
        virtual void on(identifier_t&) = 0;
        virtual void on(if_t&) = 0;
        virtual void on(for_t&) = 0;
        virtual void on(compound_t&) = 0;
        virtual void on(assignment_t&) = 0;
        virtual void on(preincrement_t<std::plus<>>&) = 0;
        virtual void on(preincrement_t<std::minus<>>&) = 0;
        virtual void on(postincrement_t<std::plus<>>&) = 0;
        virtual void on(postincrement_t<std::minus<>>&) = 0;
        virtual void on(unary_expression_t<std::bit_not<>>&) = 0;
        virtual void on(unary_expression_t<std::minus<>>&) = 0;
        virtual void on(unary_expression_t<std::logical_not<>>&) = 0;
        virtual void on(binary_expression_t<std::plus<>>&) = 0;
        virtual void on(binary_expression_t<std::minus<>>&) = 0;
        virtual void on(binary_expression_t<std::multiplies<>>&) = 0;
        virtual void on(binary_expression_t<std::divides<>>&) = 0;
        virtual void on(binary_expression_t<std::modulus<>>&) = 0;
        virtual void on(binary_expression_t<std::less<>>&) = 0;
        virtual void on(binary_expression_t<std::greater<>>&) = 0;
        virtual void on(binary_expression_t<std::less_equal<>>&) = 0;
        virtual void on(binary_expression_t<std::greater_equal<>>&) = 0;
        virtual void on(binary_expression_t<std::equal_to<>>&) = 0;
        virtual void on(binary_expression_t<std::not_equal_to<>>&) = 0;
        virtual void on(binary_expression_t<std::logical_and<>>&) = 0;
        virtual void on(binary_expression_t<std::logical_or<>>&) = 0;

    };
}

#endif //KHAOTICA_GRAMMAR_H


