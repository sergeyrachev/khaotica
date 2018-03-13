#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include "scope.h"

#include <type_traits>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <map>
#include <list>
#include <memory>
#include <cstddef>

namespace khaotica::syntax::mpeg2 {

    struct length_t {
        uint64_t from;
        uint64_t to;
    };

    struct integer_t {
        int64_t value;
    };

    struct identifier_t {
        std::string name;
    };

    struct bitstring_t {
        std::string value;
    };

    struct bslbf_t {
        std::string name;
        length_t length;
    };

    struct bitstring_v {
        std::vector<bool> value;
    };

    struct uimsbf_t {
        std::string name;
        length_t length;
    };

    struct uimsbf_v {
        uint64_t value;
    };

    struct simsbf_t {
        std::string name;
        length_t length;
    };

    struct simsbf_v {
        int64_t value;
    };

    struct vlclbf_t {
        std::string name;
        length_t length;
    };

    struct vlclbf_v {
        std::vector<bool> value;
    };

    struct tcimsbf_t {
        std::string name;
        length_t length;
    };

    typedef std::variant<
        bslbf_t,
        uimsbf_t,
        simsbf_t,
        vlclbf_t
    > entry_t;

    typedef std::variant<
        bitstring_v,
        uimsbf_v,
        simsbf_v,
        vlclbf_v
    > entry_v;

    struct collection_t {
        entry_t entry;
        size_t size;
    };

    struct collection_v {
        std::vector<entry_v> value;
    };

    typedef std::variant<
        identifier_t,
        integer_t
    > dimension_t;

    struct slot_t {
        entry_t entry;
        std::vector<dimension_t> indices;
    };

    struct slot_v {
        std::map<
            std::vector<size_t>,
            entry_v
        > value;
    };

    struct range_t {
        size_t front;
        size_t back;
    };

    struct sparsed_t {
        entry_t entry;
        range_t range;
    };

    struct sparsed_v {
        std::vector<bool> value;
        std::vector<bool> mask;
    };

    struct node_t;
    struct value_t;

    typedef std::list<std::shared_ptr<node_t>> sequence_t;

    struct reference_t {
        std::string name;
        std::list<std::shared_ptr<node_t>> args;
    };

    struct compound_t {
        std::string name;
        std::list<std::string> args;
        std::shared_ptr<node_t> body;
    };

    struct if_t {
        std::shared_ptr<node_t> condition;
        std::shared_ptr<node_t> _then;
        std::optional<std::shared_ptr<node_t>> _else;
    };

    struct for_t {
        std::shared_ptr<node_t> initializer;
        std::shared_ptr<node_t> condition;
        std::shared_ptr<node_t> modifier;
        std::shared_ptr<node_t> body;
    };

    struct do_t {
        std::shared_ptr<node_t> condition;
        std::shared_ptr<node_t> body;
    };

    struct while_t {
        std::shared_ptr<node_t> condition;
        std::shared_ptr<node_t> body;
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

    struct bytealigned_t {

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

    typedef std::variant<
        std::vector<bool>,
        uint64_t,
        int64_t,
        //sparsed_v,
        bool
    > expression_v;

    typedef scope_tt<std::string, std::shared_ptr<node_t>> scope_t;

    struct document_t {
        sequence_t sequence;
        std::shared_ptr<scope_t> global;
    };

    struct node_t {
        std::variant<
            bslbf_t,
            uimsbf_t,
            simsbf_t,
            vlclbf_t,
            tcimsbf_t,

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
            sequence_t,

            unary_expression_t,
            binary_expression_t,
            postincrement_t,
            preincrement_t,
            assignment_t,
            position_t,
            nextbits_t,
            bytealigned_t
        > payload;
    };

    struct value_t {
        expression_t origin;
        expression_v payload;
        uint64_t position;
        uint64_t length;
    };
}

#endif //KHAOTICA_GRAMMAR_H