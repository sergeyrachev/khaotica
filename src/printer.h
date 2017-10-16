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
        static void print(const flavor::document_t& doc, std::ostream& out);
    };
}
#endif //KHAOTICA_PRINTER_H
