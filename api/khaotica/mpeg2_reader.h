#pragma once

#include "khaotica/mpeg2_types.h"
#include "khaotica/mpeg2_sax.h"
#include "khaotica/bitreader.h"

namespace khaotica::bitstream::mpeg2 {
    using khaotica::syntax::mpeg2::document_t;
    using khaotica::syntax::mpeg2::sax_t;

    class reader_t {
    public:
        static void parse(bitreader_t& in, const document_t& doc, sax_t& handler);
    };
}
