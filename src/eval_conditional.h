#pragma once

#include "mpeg2_types.h"

#include <cstdint>
#include <cassert>
#include <vector>
#include <algorithm>

namespace khaotica{
    namespace eval{
        using khaotica::syntax::mpeg2::bitstring_v;

        struct conditional_t {
            bool operator()(const int64_t &operand) {
                return operand != 0;
            }

            bool operator()(const uint64_t &operand) {
                return operand != 0;
            }

            bool operator()(const bool &operand) {
                return operand;
            }

            bool operator()(const bitstring_v &operand) {
                return std::any_of(operand.value.begin(), operand.value.end(), [](const auto& b){
                    return b;
                });
            }

            template<typename T>
            bool operator()(const T &operand) {
                assert(false && "WAT?!");
                return false;
            }
        };
    }
}
