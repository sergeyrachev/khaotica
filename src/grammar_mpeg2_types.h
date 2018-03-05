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

    struct uimsbf_t {
        std::string name;
        length_t length;
    };

    struct simsbf_t {
        std::string name;
        length_t length;
    };

    struct vlclbf_t {
        std::string name;
        length_t length;
    };
    struct tcimsbf_t {
        std::string name;
        length_t length;
    };

    typedef std::variant<bslbf_t, uimsbf_t, simsbf_t, vlclbf_t> entry_t;

    struct collection_t {
        entry_t entry;
        size_t size;
    };

    typedef std::variant<identifier_t, integer_t> dimension_t;
    struct slot_t {
        entry_t entry;
        std::vector<dimension_t> indices;
    };

    struct range_t {
        size_t front;
        size_t back;
    };

    struct sparsed_t {
        entry_t entry;
        range_t range;
    };

    struct node_t;
    struct scope_t;

    typedef std::list<std::shared_ptr<node_t>> sequence_t;
    typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

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

    struct bytealigned_t{

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

    struct scope_t {
        std::weak_ptr<scope_t> parent;
        std::list<std::shared_ptr<scope_t>> children;
        definitions_t definitions;

        static std::shared_ptr<scope_t> open(const std::shared_ptr<scope_t>& parent) {
            parent->children.push_back( std::make_shared<scope_t>(scope_t{parent, {}, {}}));
            return parent->children.back();
        }

        std::shared_ptr<scope_t> close() {
            return parent.lock();
        }
    };

    struct document_t{
        sequence_t sequence;
        std::shared_ptr<scope_t> global;
    };

    struct value_t;
    typedef std::list<std::shared_ptr<value_t>> block_v;

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
    > entry_v;

    struct collection_v {
        std::vector<entry_v> value;
    };

    struct slot_v {
        std::map<
            std::vector<size_t>,
            entry_v
        > value;
    };

    struct sparsed_v {
        std::vector<bool> value;
        std::vector<bool> mask;
    };

    struct if_v {
        bool condition;
        block_v value;
    };

    struct loop_v {
        std::vector<block_v> value;
    };

    typedef std::variant<
        std::vector<bool>,
        uint64_t,
        int64_t,
        //sparsed_v,
        bool
    > expression_v;

    struct value_t {
        std::variant<
            std::pair<bslbf_t, bitstring_v>,
            std::pair<uimsbf_t, uimsbf_v>,
            std::pair<simsbf_t, simsbf_v>,
            std::pair<vlclbf_t, vlclbf_v>,
            std::pair<collection_t, collection_v>,
            std::pair<slot_t, std::pair<entry_v, slot_v>>,
            std::pair<sparsed_t, std::pair<bitstring_v, sparsed_v>>,
            std::pair<compound_t, block_v>,
            std::pair<if_t, if_v>,
            std::pair<for_t, loop_v>,
            std::pair<do_t, loop_v>,
            std::pair<while_t, loop_v>,
            std::pair<expression_t, expression_v>
        > payload;
        uint64_t offset;
        uint64_t length;
    };
}

#endif //KHAOTICA_GRAMMAR_H
