#pragma once

#include "type_conversion.h"
#include "mpeg2_types.h"

#include <cstdint>
#include <cassert>
#include <vector>
#include <functional>

namespace khaotica {
    namespace eval{
        using khaotica::syntax::mpeg2::bitstring_v;

        template<typename F>
        struct equality_t {

            bool operator()(const uint64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const bool &left, const bool &right) {
                return F()(left, right);
            }

            bool operator()(const bitstring_v &left, const bitstring_v &right) {
                return F()(left.value, right.value) && F()(left.mask, right.mask);
            }

            bool operator()(const int64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const bool &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const uint64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const bool &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const bool &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const bool &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const int64_t &right) {
                return F()(left.value, khaotica::algorithm::unpack(right)) && F()(left.mask, std::vector<bool>(left.value.size(), true));
            }

            bool operator()(const bitstring_v &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const uint64_t &right) {
                return F()(left.value, khaotica::algorithm::unpack(right)) && F()(left.mask, std::vector<bool>(left.value.size(), true));
            }
        };

        typedef equality_t<std::equal_to<>> equal_t;
        typedef equality_t<std::not_equal_to<>> not_equal_t;
    }
}
