#pragma once

#include "mpeg2_types.h"

#include <cassert>

namespace khaotica {
    namespace eval {
        using khaotica::syntax::mpeg2::expression_v;
        using khaotica::syntax::mpeg2::bitstring_v;
        
        struct bitwise_not_t {
            expression_v operator()(const int64_t &operand) {
                return ~operand;
            }

            expression_v operator()(const uint64_t &operand) {
                return ~operand;
            }

            expression_v operator()(const bool &operand) {
                assert(false && "WAT????");
                return static_cast<int64_t>(~operand);
            }

            expression_v operator()(const bitstring_v &operand) {
                auto ret = operand.value;
                ret.flip();
                return bitstring_v{ret, operand.mask};
            }

            template<typename T>
            expression_v operator()(const T &operand) {
                assert(false && "WAT????");
                return {false};
            }
        };
    }
}
