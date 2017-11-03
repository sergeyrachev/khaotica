#pragma once

#include "grammar.h"
#include "logging.h"
#include "bit.h"
#include "bitreader.h"
#include "eval.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>
#include <cassert>

namespace khaotica {
    class parser_t {
    public:
        static flavor::snapshot_t parse(bitreader_t& in, const flavor::document_t& doc);
    };
}
