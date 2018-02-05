#pragma once

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <list>
#include <map>

namespace khaotica{
    namespace mpeg2{

        struct node_t;
        struct scope_t;


        struct bslbf_t{};
        struct uimsbf_t{};
        struct simsbf_t{};
        struct vlclbf_t{};
        struct bitstring_t{};

        struct integer_t{};
        struct complex_t{};
        struct variable_t{};

        struct simple_t{
            std::string name;
        };
        struct array_t{
            std::string name;
            size_t size;
        };
        struct slot_t{
            std::string name;
            std::vector<std::shared_ptr<node_t>> indices;
        };

        struct entry_t{
            std::variant<
                simple_t,
                array_t,
                slot_t
            > identifier;

            std::variant<
                integer_t,
                complex_t,
                variable_t
            > size;

            std::variant<
                bslbf_t,
                uimsbf_t,
                bitstring_t
            > mnemonic;
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

        struct node_t{
            std::variant<
                entry_t,
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

        typedef std::list<std::shared_ptr<node_t>> structure_t;
        typedef std::map<std::string, std::shared_ptr<node_t>> definitions_t;

        struct scope_t{
            scope_t* parent{nullptr};
            std::list<std::shared_ptr<scope_t>> childs;
            definitions_t definitions;
        };
    }
}
