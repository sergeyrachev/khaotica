#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "syntax.h"

#include <istream>
#include <memory>
#include <vector>
#include <list>

namespace flavor{
    class interpreter_t {
    public:
        static std::tuple<flavor::document_t, flavor::symbols_t> parse(std::istream &in, bool enable_verbosity = true);
    };
}

#endif //KHAOTICA_INTERPRETER_H
