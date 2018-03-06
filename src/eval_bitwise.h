#pragma once

#include "mpeg2_types.h"

#include <cassert>

namespace khaotica {
    namespace eval {
        using khaotica::syntax::mpeg2::expression_v;
        
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

            expression_v operator()(const std::vector<bool> &operand) {
                auto ret = operand;
                ret.flip();
                return ret;
            }
        };
    }
}
