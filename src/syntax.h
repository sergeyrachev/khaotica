#pragma once

#include "grammar.h"

namespace khaotica {
    class syntax_t{
    public:
        static bool is_valid(const khaotica::document_t& doc, std::ostream& out);
    };
}
