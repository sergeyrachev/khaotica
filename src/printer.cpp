#include "printer.h"

class khaotica::printer_t::impl_t{
public:
    impl_t(std::ostream &out){

    }

    void operator()(const flavor::compound_t&){

    }

    void operator()(const flavor::variable_t&){

    }
};


void khaotica::printer_t::print(std::ostream &out, const flavor::document_t &doc, const flavor::symbols_t &symbols, const flavor::values_t &values) {
    impl_t impl(out);
    for(auto&& entry : doc){
        std::visit(impl, entry);
    }

    out << std::endl;
}
