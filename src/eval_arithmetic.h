#pragma once

#include "grammar_mpeg2_types.h"

#include <cassert>
#include <functional>

namespace khaotica {
    namespace eval {
        template<typename F>
        struct arithmetical_t {
            khaotica::expression_v operator()(const int64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const int64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const int64_t &left, const bool &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const int64_t &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const uint64_t &left, const int64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const uint64_t &left, const uint64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const uint64_t &left, const bool &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const uint64_t &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const bool &left, const uint64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const bool &left, const int64_t &right) {
                return F()(left, right);
            }

            khaotica::expression_v operator()(const bool &left, const bool &right) {
                return F()(static_cast<uint64_t>(left), static_cast<uint64_t>(right));
            }

            khaotica::expression_v operator()(const bool &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const std::vector<bool> &left, const int64_t &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const std::vector<bool> &left, const bool &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const std::vector<bool> &left, const uint64_t &right) {
                assert(false && "WAT?!");
                return false;
            }

            khaotica::expression_v operator()(const std::vector<bool> &left, const std::vector<bool> &right) {
                assert(false && "WAT?!");
                return false;
            }
        };

        typedef arithmetical_t<std::minus<>> minus_t;
        typedef arithmetical_t<std::plus<>> plus_t;
        typedef arithmetical_t<std::multiplies<>> multiplies_t;
        typedef arithmetical_t<std::divides<>> divides_t;
        typedef arithmetical_t<std::modulus<>> modulus_t;
    }
}