#pragma once

#include "grammar_mpeg2_types.h"
#include "grammar_mpeg2_sax.h"
#include "logging.h"
#include "type_conversion.h"
#include "bitreader.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>
#include <cassert>

namespace khaotica {
    class bsparser_t {
    public:
        bsparser_t() = default;
        void parse(khaotica::details::bitreader_t& in, const khaotica::document_t& doc, std::shared_ptr<khaotica::mpeg2::sax_t> handler);
    };
}
