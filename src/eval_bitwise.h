#pragma once

#include "grammar_mpeg2_types.h"

#include <cassert>

namespace khaotica {
    namespace eval {
        struct bitwise_not_t {
            khaotica::expression_v operator()(const int64_t &operand) {
                return ~operand;
            }

            khaotica::expression_v operator()(const uint64_t &operand) {
                return ~operand;
            }

            khaotica::expression_v operator()(const bool &operand) {
                assert(false && "WAT????");
                return static_cast<int64_t>(~operand);
            }

            khaotica::expression_v operator()(const std::vector<bool> &operand) {
                auto ret = operand;
                ret.flip();
                return ret;
            }
        };
    }
}