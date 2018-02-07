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

namespace khaotica {

    struct fixed_length_t {
        uint64_t value;
    };

    struct variable_length_t {
        uint64_t from;
        uint64_t to;
    };

    typedef std::variant<fixed_length_t, variable_length_t> entry_length_t;

    struct integer_t {
        int64_t value;
    };

    struct identifier_t {
        std::string name;
    };

    struct bitstring_t {
        std::optional<std::string> name;
        std::string value;
    };

    struct bslbf_t {
        std::string name;
        fixed_length_t length;
    };

    struct uimsbf_t {
        std::string name;
        entry_length_t length;
    };

    struct simsbf_t {
        std::string name;
        entry_length_t length;
    };

    struct vlclbf_t {
        std::string name;
        entry_length_t length;
    };

    struct collection_t {
        std::variant<bslbf_t, uimsbf_t, simsbf_t, vlclbf_t> entry;
        size_t size;
    };

    struct slot_t {
        std::variant<bslbf_t, uimsbf_t, simsbf_t, vlclbf_t> entry;
        std::vector<std::variant<identifier_t, integer_t>> indices;
    };

    struct sparsed_t {
        std::variant<bslbf_t, uimsbf_t, simsbf_t, vlclbf_t> entry;
        size_t front;
        size_t back;
    };

    struct node_t;
    struct scope_t;

    struct reference_t {
        std::string name;
        std::list<std::shared_ptr<node_t>> args;
    };

    struct compound_t {
        std::string name;
        std::list<std::string> args;
        std::list<std::shared_ptr<node_t>> body;
        std::shared_ptr<scope_t> scope;
    };

    struct if_t {
        std::shared_ptr<node_t> condition;
        std::list<std::shared_ptr<node_t>> _then;
        std::list<std::shared_ptr<node_t>> _else;
        std::shared_ptr<scope_t> scope;
    };

    struct for_t {
        std::optional<std::shared_ptr<node_t>> initializer;
        std::optional<std::shared_ptr<node_t>> condition;
        std::optional<std::shared_ptr<node_t>> modifier;
        std::list<std::shared_ptr<node_t>> body;
        std::shared_ptr<scope_t> scope;
    };

    struct do_t {
        std::optional<std::shared_ptr<node_t>> condition;
        std::list<std::shared_ptr<node_t>> body;
        std::shared_ptr<scope_t> scope;
    };

    struct while_t {
        std::optional<std::shared_ptr<node_t>> condition;
        std::list<std::shared_ptr<node_t>> body;
        std::shared_ptr<scope_t> scope;
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

    struct position_t {
        std::optional<std::string> name;
    };

    struct nextbits_t {
        std::optional<uint64_t> length;
    };

    struct node_t {
        std::variant<
            bslbf_t,
            uimsbf_t,
            simsbf_t,
            vlclbf_t,

            collection_t,
            slot_t,
            sparsed_t,

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
        position_t,
        nextbits_t
    > expression_t;

    typedef std::list<std::shared_ptr<node_t>> structure_t;
    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

    struct scope_t {
        std::weak_ptr<scope_t> parent;
        std::list<std::shared_ptr<scope_t>> childs;
        definitions_t definitions;
    };

    struct document_t {
        structure_t structure;
        std::shared_ptr<scope_t> global{std::make_shared<scope_t>()};
    };

    struct value_t;
    typedef std::list<std::shared_ptr<value_t>> block_t;

    struct bitstring_v {
        std::vector<bool> value;
    };
    struct uimsbf_v {
        uint64_t value;
    };
    struct simsbf_v {
        int64_t value;
    };
    struct vlclbf_v {
        std::vector<bool> value;
    };

    typedef std::variant<
        bitstring_v,
        uimsbf_v,
        simsbf_v,
        vlclbf_v
    > entry_t;

    struct collection_v {
        std::vector<entry_t> value;
    };

    struct slot_v {
        std::map<
            std::vector<size_t>,
            entry_t
        > value;
    };

    struct sparsed_v {
        std::vector<bool> value;
        std::vector<bool> mask;
    };

    struct if_v {
        bool condition;
        block_t value;
    };

    struct loop_v {
        std::vector<block_t> value;
    };

    typedef std::variant<
        bitstring_v,
        uimsbf_v,
        simsbf_v,
        vlclbf_v,
        sparsed_v,
        bool
    > expression_v;

    struct value_t {
        std::variant<
            std::pair<bslbf_t, bitstring_v>,
            std::pair<uimsbf_t, uimsbf_v>,
            std::pair<simsbf_t, simsbf_v>,
            std::pair<vlclbf_t, vlclbf_v>,
            std::pair<collection_t, collection_v>,
            std::pair<slot_t, std::pair<entry_t, slot_v>>,
            std::pair<sparsed_t, std::pair<bitstring_v, sparsed_v>>,
            std::pair<compound_t, block_t>,
            std::pair<if_t, if_v>,
            std::pair<for_t, loop_v>,
            std::pair<do_t, loop_v>,
            std::pair<while_t, loop_v>,
            std::pair<expression_t, expression_v>
        > payload;
    };
}

#endif //KHAOTICA_GRAMMAR_H
