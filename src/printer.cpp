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


void khaotica::printer_t::print(const flavor::document_t &doc, std::ostream &out) {

    impl_t impl(out);
    for(auto&& entry : doc.hierarchy){
        impl(entry);
    }
}
