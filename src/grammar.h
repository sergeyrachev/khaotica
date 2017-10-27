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

    struct node_t : std::enable_shared_from_this<node_t>{
        virtual ~node_t() = default;
        virtual void process(traversal_t& traversal) = 0;
    };

    template<typename T>
    struct mixin_t : node_t{
        virtual void process(traversal_t& traversal) final {
            traversal.on(static_cast<T&>(*this), shared_from_this());
        }
    };

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
        if_t(const std::shared_ptr<node_t> &condition, const std::shared_ptr<node_t> &_then, const std::optional<std::shared_ptr<node_t>> &_else)
            : condition(condition), _then(_then), _else(_else) {}

        std::shared_ptr<node_t> condition;
        std::shared_ptr<node_t> _then;
        std::optional<std::shared_ptr<node_t>> _else;
    };

    struct for_t : mixin_t<for_t>{
        for_t(const std::optional<std::shared_ptr<node_t>> &initializer, const std::optional<std::shared_ptr<node_t>> &condition,
              const std::optional<std::shared_ptr<node_t>> &modifier, const std::shared_ptr<node_t> &body)
            : initializer(initializer), condition(condition), modifier(modifier), body(body) {}

        std::optional<std::shared_ptr<node_t>> initializer;
        std::optional<std::shared_ptr<node_t>> condition;
        std::optional<std::shared_ptr<node_t>> modifier;
        std::shared_ptr<node_t> body;
    };

    struct compound_t : mixin_t<compound_t>{
        compound_t(const std::string &name, const std::shared_ptr<node_t> &body = nullptr)
            : name(name), body(body) {}

        std::string name;
        std::shared_ptr<node_t> body;
    };

    struct block_t : mixin_t<block_t>{
        block_t(const std::list<std::shared_ptr<node_t>> &entries = {})
            : entries(entries) {}

        std::list<std::shared_ptr<node_t>> entries;
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
        virtual void on(bslbf_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(uimsbf_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(bitstring_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(integer_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(identifier_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(if_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(for_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(compound_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(block_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(assignment_t& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(preincrement_t<std::plus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(preincrement_t<std::minus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(postincrement_t<std::plus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(postincrement_t<std::minus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(unary_expression_t<std::bit_not<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(unary_expression_t<std::minus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(unary_expression_t<std::logical_not<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::plus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::minus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::multiplies<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::divides<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::modulus<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::less<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::greater<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::less_equal<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::greater_equal<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::equal_to<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::not_equal_to<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::logical_and<>>& node, std::shared_ptr<node_t> knot) = 0;
        virtual void on(binary_expression_t<std::logical_or<>>& node, std::shared_ptr<node_t> knot) = 0;
    };
}

#endif //KHAOTICA_GRAMMAR_H


