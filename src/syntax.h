#pragma once

#include "grammar.h"

namespace khaotica {
    class syntax_t{
    public:
        static bool is_valid(const flavor::document_t& doc, std::ostream& out);
    };
}
