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

    struct entry_t{
        std::string name;
        uint64_t length;
    };

    struct bslbf_t : entry_t{
    };

    struct uimsbf_t : entry_t{
    };

    struct tcimsbf_t : entry_t{
    };

    struct compound_t;
    typedef std::variant<bslbf_t, uimsbf_t, tcimsbf_t, compound_t>  symbol_t;
    typedef std::list<symbol_t> symbols_t;
    typedef std::map<std::string, symbol_t> symbol_table_t;

    struct functor_t{

    };

    struct operand_t{

    };

    struct expression_t{
        functor_t f;
        operand_t operand;
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




}

#endif //KHAOTICA_GRAMMAR_H


