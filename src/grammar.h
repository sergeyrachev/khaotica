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

namespace khaotica{

    struct node_t;
    struct value_t;
    struct scope_t;

    struct bslbf_t {
        std::string name;
        uint64_t length;
    };

    struct bslbf_ranged_t {
        bslbf_t bits;
        std::pair<uint64_t, uint64_t> range;
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

    struct identifier_t {
        std::string name;
    };

    struct reference_t{
        std::string name;
        std::list<std::shared_ptr<node_t>> args;
    };

    struct compound_t{
        std::string name;
        std::list<std::shared_ptr<node_t>> body;
        std::list<std::string> args;
        scope_t* scope;
    };

    struct if_t {
        std::shared_ptr<node_t> condition;
        std::list<std::shared_ptr<node_t>> _then;
        std::list<std::shared_ptr<node_t>> _else;
        scope_t* scope;
    };

    struct for_t {
        std::optional<std::shared_ptr<node_t>> initializer;
        std::optional<std::shared_ptr<node_t>> condition;
        std::optional<std::shared_ptr<node_t>> modifier;
        std::list<std::shared_ptr<node_t>> body;
        scope_t* scope;
    };

    struct do_t{
        std::optional<std::shared_ptr<node_t>> condition;
        std::list<std::shared_ptr<node_t>> body;
        scope_t* scope;
    };

    struct while_t{
        std::optional<std::shared_ptr<node_t>> condition;
        std::list<std::shared_ptr<node_t>> body;
        scope_t* scope;
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

    struct position_t{
        std::optional<std::string> name;
    };

    struct nextbits_t{
        std::optional<uint64_t> length;
    };

    struct node_t{
        std::variant<
            bslbf_t,
            bslbf_ranged_t,
            uimsbf_t,
            bitstring_t,
            integer_t,
            identifier_t,
            reference_t,
            compound_t,
            if_t,
            for_t,
            do_t,
            while_t,
            unary_expression_t,
            binary_expression_t,
            postincrement_t,
            preincrement_t,
            assignment_t,
            position_t,
            nextbits_t
        > payload;
    };

    typedef std::variant<
        integer_t,
        bitstring_t,
        identifier_t,
        unary_expression_t,
        binary_expression_t,
        postincrement_t,
        preincrement_t,
        assignment_t,
        position_t
    > expression_t;

    typedef std::list<std::shared_ptr<node_t>> structure_t;
    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

    struct scope_t{
        scope_t* parent{nullptr};
        std::list<std::shared_ptr<scope_t>> childs;
        definitions_t definitions;
    };

    struct document_t{
        structure_t structure;
        definitions_t definitions;
        scope_t global;
    };

    struct bslbf_v{
        std::vector<bool> value;
        uint64_t location;
    };

    struct bslbf_ranged_v{
        std::vector<bool> value;
        std::vector<bool> mask;
    };

    struct uimsbf_v {
        uint64_t value;
        uint64_t location;
    };

    struct if_v{
        bool condition;
        std::list<std::shared_ptr<value_t>> value;
    };

    struct loop_v{
        typedef std::list<std::shared_ptr<value_t>> iteration_t;
        std::vector<iteration_t> value;
    };

    typedef std::variant<
        std::vector<bool>,
        uint64_t,
        int64_t,
        bool
    > expression_v;

    struct value_t{
        std::variant<
            std::pair<bslbf_t, bslbf_v>,
            std::pair<bslbf_ranged_t, std::pair<bslbf_v, bslbf_ranged_v>>,
            std::pair<uimsbf_t, uimsbf_v>,
            std::pair<compound_t, std::list<std::shared_ptr<value_t>>>,
            std::pair<if_t, if_v>,
            std::pair<for_t, loop_v>,
            std::pair<do_t, loop_v>,
            std::pair<while_t, loop_v>,
            std::pair<expression_t, expression_v>
        > payload;
    };

    typedef std::list<std::shared_ptr<value_t>> snapshot_t;
}

#endif //KHAOTICA_GRAMMAR_H


