#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <map>
#include <list>
#include <memory>

namespace flavor{
    struct directive_include_t{};
    struct directive_import_t{};

    struct bslbf_t{
        std::string name;
        uint64_t length;
    };

    struct uimsbf_t{
        std::string name;
        uint64_t length;
    };

    struct tcimsbf_t{
        std::string name;
        uint64_t length;
    };

    struct variable_t;
    struct compound_t;
    typedef std::variant<
        bslbf_t,
        uimsbf_t,
        tcimsbf_t,
        compound_t,
        variable_t
    >  symbol_t;
    typedef std::list<symbol_t> symbols_t;

    struct bitstring_t{
        std::string value;
    };

    typedef uint64_t numeric_t;
    typedef std::variant<symbol_t, numeric_t, bitstring_t> entity_t;

    struct compound_t{
        std::string name;
        symbols_t body;
    };

    typedef std::variant<
        std::vector<bool>,
        uint64_t,
        int64_t,
        double
    > value_t;

    typedef std::variant<
        std::logical_not<value_t>,
        std::negate<value_t>
    > action_t;

    typedef std::pair<action_t, entity_t>  unary_expression;
    typedef std::tuple<entity_t, action_t, entity_t>  binary_expression;
    typedef std::variant<entity_t, unary_expression, binary_expression > expression_t;

    struct variable_t{
        std::string name;
        std::unique_ptr<expression_t> expression;
    };

    struct if_t{
        std::unique_ptr<expression_t> expression;
        symbols_t _then;
        symbols_t _else;
    };

    struct for_t {

    };

    typedef std::list<value_t> values_t;
    typedef std::map<std::string, entity_t> symbols_table_t;
}

#endif //KHAOTICA_GRAMMAR_H


