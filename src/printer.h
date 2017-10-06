//
// Created by user on 25.08.17.
//

#ifndef KHAOTICA_PRINTER_H
#define KHAOTICA_PRINTER_H

#include "syntax.h"
#include "bit.h"
#include "eval.h"

#include <iostream>

namespace khaotica{
    class printer_t{
        class impl_t;
    public:
        static void print(std::ostream& out, const flavor::document_t& doc, const flavor::symbols_t& symbols, const flavor::values_t& values);
    };
}
#endif //KHAOTICA_PRINTER_H
