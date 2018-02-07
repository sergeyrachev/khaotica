#pragma once

#include <cstdint>
#include <cassert>
#include <vector>

namespace khaotica {
    namespace eval{
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

            bool operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const bool &right) {
                return F()(left, right);
            }

            bool operator()(const int64_t &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const uint64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const bool &right) {
                return F()(left, right);
            }

            bool operator()(const uint64_t &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const bool &left, const uint64_t &right) {
                return F()(left, right);
            }

            bool operator()(const bool &left, const int64_t &right) {
                return F()(left, right);
            }

            bool operator()(const bool &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const std::vector<bool> &left, const int64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const std::vector<bool> &left, const bool &right) {
                assert(false && "WAT?!");
                return {false};
            }

            bool operator()(const std::vector<bool> &left, const uint64_t &right) {
                assert(false && "WAT?!");
                return {false};
            }
        };

        typedef equality_t<std::equal_to<>> equal_t;
        typedef equality_t<std::not_equal_to<>> not_equal_t;
    }
}