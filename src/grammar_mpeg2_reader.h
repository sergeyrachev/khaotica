#pragma once

#include "grammar_mpeg2_types.h"
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
        static khaotica::snapshot_t parse(bitreader_t& in, const khaotica::document_t& doc);
    };
}
