#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include "khaotica/mpeg2_types.h"

#include <istream>
namespace khaotica::syntax::mpeg2 {
    class interpreter_t {
    public:
        static document_t parse(std::istream &in);
    };
}

#endif //KHAOTICA_INTERPRETER_H
