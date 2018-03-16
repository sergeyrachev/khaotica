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
        struct logical_t {
            bool operator()(const int64_t &left, const int64_t &right) {
                return F()(left, right);
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

            bool operator()(const uint64_t &left, const uint64_t &right) {
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

            bool operator()(const bool &left, const bool &right) {
                return F()(left, right);
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

        struct logical_not_t {
            bool operator()(const int64_t &operand) {
                return !operand;
            }

            bool operator()(const uint64_t &operand) {
                return !operand;
            }

            bool operator()(const bool &operand) {
                return !operand;
            }

            bool operator()(const auto& operand){
                assert(false && "WAT?!");
                return false;
            }
        };

        typedef logical_t<std::logical_or<>> logical_or_t;
        typedef logical_t<std::logical_and<>> logical_and_t;
    }
}
