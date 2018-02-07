#pragma once

#include "grammar_mpeg2_types.h"

#include <cassert>

namespace khaotica{
    namespace eval {
        struct default_t {
            khaotica::expression_v operator()(const khaotica::bslbf_t &node) {
                return std::vector<bool>();
            }

            khaotica::expression_v operator()(const khaotica::bslbf_ranged_t &node) {
                return std::vector<bool>();
            }

            khaotica::expression_v operator()(const std::vector<khaotica::uimsbf_t> &node) {
                return std::vector<bool>(node.size());
            }

            khaotica::expression_v operator()(const khaotica::uimsbf_t &node) {
                return uint64_t{0};

            }

            khaotica::expression_v operator()(const khaotica::bitstring_t &node) {
                assert(false && "Must never be there");
                return std::vector<bool>();

            }

            khaotica::expression_v operator()(const khaotica::integer_t &node) {
                assert(false && "Must never be there");
                return int64_t{0};
            }

            khaotica::expression_v operator()(const khaotica::reference_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::compound_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::identifier_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::if_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::for_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::do_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::while_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::unary_expression_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::binary_expression_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::postincrement_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::preincrement_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::assignment_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::position_t &node) {
                assert(false && "Must never be there");
                return false;
            }

            khaotica::expression_v operator()(const khaotica::nextbits_t &node) {
                assert(false && "Must never be there");
                return false;
            }
        };
    }
}

#endif //KHAOTICA_EVAL_DEFAULT_H
