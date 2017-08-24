#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "syntax.h"

#include <istream>
#include <memory>
#include <vector>
#include <list>

namespace flavor{
    class Interpreter {
    public:
        static flavor::symbols_t parse(std::istream &in, bool enable_verbosity = true);
    };
}

#endif //KHAOTICA_INTERPRETER_H
