#pragma once

#include "grammar_mpeg2_types.h"

namespace khaotica{
    namespace mpeg2{
        class sax_t {
        public:
            virtual ~sax_t() = default;
            virtual void on(const value_t &) = 0;
        };
    }
}