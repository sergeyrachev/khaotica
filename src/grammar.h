#ifndef KHAOTICA_GRAMMAR_H
#define KHAOTICA_GRAMMAR_H

#include <string>
#include <vector>
#include <variant>

namespace flavor{
    struct directive_include_t{};
    struct directive_import_t{};

    struct bitstring_t{
        std::string name;
        int64_t length;
    };

    struct uint_t{
        std::string name;
        int64_t length;
    };

    typedef std::variant<bitstring_t, uint_t>  symbol_t;
}

#endif //KHAOTICA_GRAMMAR_H
