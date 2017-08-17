#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include <string>
#include <vector>
#include <variant>
#include <map>
#include <list>

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
    typedef std::variant<bslbf_t, uimsbf_t, tcimsbf_t, compound_t, variable_t>  symbol_t;
    typedef std::list<symbol_t> symbols_t;

    typedef std::map<std::string, symbol_t> symbols_table_t;

    struct action_t{

    };

    struct operand_t{
        std::variant<variable_t, value_t, >
    };

    struct expression_t {
        action_t action;
        operand_t operand;
    }

    struct variable_t{
        std::string name;
        expression_t declaration;
    };

    struct compound_t{
        std::string name;
        symbols_t body;
    };

    struct if_t{
        expression_t condition;
        symbols_t _then;
        symbols_t _else;
    };

    struct for_t {

    };

    typedef std::variant<uint64_t, int64_t, double > numeric_t;
    typedef std::variant<
        std::vector<bool>,
        uint64_t,
        numeric_t
    > value_t;
    typedef std::list<value_t> values_t;

}

#endif //KHAOTICA_GRAMMAR_H


