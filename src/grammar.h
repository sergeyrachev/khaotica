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

    struct node_t;

    struct bslbf_t {
        std::string name;
        uint64_t length;
    };

    struct uimsbf_t {
        std::string name;
        uint64_t length;
    };

    struct bitstring_t {
        std::string value;
    };

    struct integer_t {
        int64_t value;
    };

    struct reference_t{
        std::string name;
    };

    struct compound_t{
        std::string name;
        std::list<std::shared_ptr<node_t>> body;
    };

    struct identifier_t {
        std::string name;
    };

    struct if_t {
        std::shared_ptr<node_t> condition;
        std::list<std::shared_ptr<node_t>> _then;
        std::list<std::shared_ptr<node_t>> _else;
    };

    struct for_t {
        std::optional<std::shared_ptr<node_t>> initializer;
        std::optional<std::shared_ptr<node_t>> condition;
        std::optional<std::shared_ptr<node_t>> modifier;
        std::list<std::shared_ptr<node_t>> body;
    };

    struct unary_expression_t {
        std::shared_ptr<node_t> operand;
        std::string operation;
    };

    struct binary_expression_t {
        std::shared_ptr<node_t> left_operand;
        std::string operation;
        std::shared_ptr<node_t> right_operand;
    };

    struct postincrement_t {
        std::string operand;
        std::string operation;
    };

    struct preincrement_t {
        std::string operand;
        std::string operation;
    };

    struct assignment_t {
        std::string symbol;
        std::shared_ptr<node_t> expression;
    };

    struct node_t{
        std::variant<
            bslbf_t,
            uimsbf_t,
            bitstring_t,
            integer_t,
            reference_t,
            compound_t,
            identifier_t,
            if_t,
            for_t,
            unary_expression_t,
            binary_expression_t,
            postincrement_t,
            preincrement_t,
            assignment_t
        > payload;
    };

    typedef std::list<std::shared_ptr<node_t>> structure_t;
    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;
    typedef std::list<std::string> scope_t;
    typedef std::map<std::shared_ptr<node_t>, scope_t> scopes_t;

    struct document_t{
        structure_t structure;
        definitions_t definitions;
        scopes_t scopes;
    };
}

#endif //KHAOTICA_GRAMMAR_H


