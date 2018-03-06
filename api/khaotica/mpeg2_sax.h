#pragma once

#include "khaotica/mpeg2_types.h"

namespace khaotica::syntax::mpeg2 {
    class sax_t {
    public:
        virtual ~sax_t() = default;

        virtual void open(const compound_t &node) = 0;
        virtual void close(const compound_t &node) = 0;

        virtual void open(const if_t &node) = 0;
        virtual void close(const if_t &node) = 0;

        virtual void open(const for_t &node) = 0;
        virtual void close(const for_t &node) = 0;

        virtual void open(const while_t &node) = 0;
        virtual void close(const while_t &node) = 0;

        virtual void open(const do_t &node) = 0;
        virtual void close(const do_t &node) = 0;

        virtual void on(const std::pair<bslbf_t, bitstring_v> &node) = 0;
        virtual void on(const std::pair<uimsbf_t, uimsbf_v> &node) = 0;
        virtual void on(const std::pair<collection_t, collection_v> &node) = 0;
        virtual void on(const std::pair<sparsed_t, std::pair<bitstring_v, sparsed_v>> &node) = 0;
        virtual void on(const std::pair<expression_t, expression_v> &node) = 0;
    };
}
