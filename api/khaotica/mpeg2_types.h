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
        std::optional<std::vector<bool>> mask; // TODO: Use properly on grammar parsing
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

    struct tcimsbf_v {
        int64_t value;
    };


    template<typename T>
    struct tag_tt{};
    using bslbf_tag = tag_tt<bslbf_t>;
    using uimsbf_tag = tag_tt<uimsbf_t>;
    using simsbf_tag = tag_tt<simsbf_t>;
    using vlclbf_tag = tag_tt<vlclbf_t>;
    using tcimsbf_tag = tag_tt<tcimsbf_t>;

    typedef std::variant<
        bslbf_tag,
        uimsbf_tag,
        simsbf_tag,
        vlclbf_tag,
        tcimsbf_tag
    > tag_t;

    struct collection_t {
        std::string name;
        length_t length;
        size_t size;
        tag_t tag;
    };

    struct collection_v {
        std::vector<std::vector<bool>> value;
    };

    typedef std::variant<
        identifier_t,
        integer_t
    > dimension_t;

    struct slot_t {
        std::string name;
        length_t length;
        std::vector<dimension_t> indices;
        tag_t tag;
    };

    struct slot_v {
        std::vector<bool> field;
        std::vector<uint64_t> indices;
    };

    struct range_t {
        size_t front;
        size_t back;
    };

    struct sparsed_t {
        std::string name;
        length_t length;
        range_t range;
        tag_t tag;
    };

    struct sparsed_v {
        std::vector<bool> field;
        std::vector<bool> value;
        std::vector<bool> mask;
    };

    struct node_t;
    struct value_t;

    typedef std::vector<std::shared_ptr<node_t>> sequence_t;

    struct reference_t {
        std::string name;
        std::vector<std::shared_ptr<node_t>> args;
    };

    struct compound_t {
        std::string name;
        std::vector<std::string> args;
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

    };

    struct bytealigned_t {

    };

    struct nextbits_v{
        virtual std::vector<bool> get(size_t n) = 0;
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
        nextbits_t,
        bytealigned_t
    > expression_t;

    typedef std::variant<
        std::shared_ptr<nextbits_v>,
        bitstring_v,
        uint64_t,
        int64_t,
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
