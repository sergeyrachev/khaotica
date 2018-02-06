#pragma once

#include "grammar_mpeg2.h"

namespace khaotica {
    class syntax_t{
    public:
        static bool is_valid(const khaotica::document_t& doc, std::ostream& out);
    };
}
