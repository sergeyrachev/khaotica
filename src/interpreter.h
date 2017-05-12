#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include <istream>
#include <memory>
#include <vector>

namespace flavor{
    class Interpreter {
    public:
        Interpreter();
        void parse(std::istream &in) const;
    };
}

#endif //KHAOTICA_INTERPRETER_H
