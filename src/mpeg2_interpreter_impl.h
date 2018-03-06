#ifndef KHAOTICA_MPEG2_INTERPERETER_IMPL_H
#define KHAOTICA_MPEG2_INTERPERETER_IMPL_H

#include "mpeg2_types.h"
#include <istream>

namespace khaotica::syntax::mpeg2::impl{
    using khaotica::syntax::mpeg2::document_t;
    class interpreter_t {
    public:
        static document_t parse(std::istream &in);
    };
}

#endif //KHAOTICA_MPEG2_INTERPERETER_IMPL_H
