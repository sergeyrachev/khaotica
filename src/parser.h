#pragma once

#include "syntax.h"
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
        class impl_t;

    public:
        static flavor::values_t parse(std::ifstream& in, const flavor::document_t& doc);
    };
}
