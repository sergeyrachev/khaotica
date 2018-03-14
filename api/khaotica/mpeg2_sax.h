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

        virtual void on(const bslbf_t&, const bitstring_v&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const uimsbf_t&, const uimsbf_v&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const simsbf_t&, const simsbf_v&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const tcimsbf_t&, const tcimsbf_v&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const collection_t&, const collection_v&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const sparsed_t&, const std::pair<bitstring_v, sparsed_v>&node, const uint64_t position, const uint64_t length) = 0;
        virtual void on(const expression_t&, const expression_v&node, const uint64_t position, const uint64_t length) = 0;
    };
}
