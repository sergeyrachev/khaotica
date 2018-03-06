#pragma once

#include "mpeg2_types.h"

#include <cassert>

namespace khaotica{
    namespace eval {
        using khaotica::syntax::mpeg2::expression_v;
        struct default_t {
            expression_v operator()(const auto &node) {
                assert(false && "Must never be there");
                return uint64_t{0};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::bslbf_t &node) {
                return std::vector<bool>{};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::sparsed_t &node) {
                return std::vector<bool>{};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::uimsbf_t &node) {
                return uint64_t{0};
            }
            expression_v operator()(const khaotica::syntax::mpeg2::simsbf_t &node) {
                return int64_t{0};
            }
            expression_v operator()(const khaotica::syntax::mpeg2::tcimsbf_t &node) {
                return int64_t{0};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::bitstring_t &node) {
                assert(false && "Must never be there");
                return std::vector<bool>{};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::integer_t &node) {
                assert(false && "Must never be there");
                return int64_t{0};
            }

            expression_v operator()(const khaotica::syntax::mpeg2::reference_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::compound_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::identifier_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::if_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::for_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::do_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::while_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::unary_expression_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::binary_expression_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::postincrement_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::preincrement_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::assignment_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::position_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            expression_v operator()(const khaotica::syntax::mpeg2::nextbits_t &node) {
                assert(false && "Must never be there");
                return false;
            }
        };
    }
}
