#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "grammar.h"


#include <istream>
#include <memory>
#include <vector>
#include <list>

namespace flavor{
    class Interpreter {
    public:
        static std::list<symbol_t> parse(std::istream &in, bool enable_verbosity = true);
    };
}

#endif //KHAOTICA_INTERPRETER_H
