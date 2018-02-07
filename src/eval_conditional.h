#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

namespace khaotica{
    namespace eval{
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

            bool operator()(const std::vector<bool> &operand) {
                assert(false && "WAT?!");
                return operand.empty();
            }
        };
    }
}

#endif //KHAOTICA_EVAL_CONDITIONAL_H
