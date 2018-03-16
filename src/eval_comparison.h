#pragma once

#include "mpeg2_types.h"

#include <cstdint>
#include <cassert>
#include <vector>
#include <functional>

namespace khaotica {
    namespace eval{
        using khaotica::syntax::mpeg2::bitstring_v;

        template<typename F>
        struct comparison_t {
            bool operator()(const int64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const int64_t &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const uint64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const uint64_t &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const uint64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const int64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const int64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const uint64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bitstring_v &left, const bitstring_v &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const auto &left, const auto &right) {
                assert(false && "WAT?!");
                return {false};
            }
        };

        typedef comparison_t<std::less<>> less_t;
        typedef comparison_t<std::greater<>> greater_t;
        typedef comparison_t<std::less_equal<>> less_equal_t;
        typedef comparison_t<std::greater_equal<>> greater_equal_t;
    }
}
