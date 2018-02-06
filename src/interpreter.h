#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "grammar_mpeg2.h"

#include <istream>
#include <memory>
#include <vector>
#include <list>

namespace khaotica{
    class interpreter_t {
    public:
        static khaotica::document_t parse(std::istream &in, bool enable_verbosity = true);
    };
}

#endif //KHAOTICA_INTERPRETER_H
