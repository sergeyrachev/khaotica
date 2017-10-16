#include "printer.h"

template<typename P>
class traversal_t{
public:
    traversal_t(const flavor::symbols_t& symbols, P& p):symbols(symbols), p(p){

    }

    void operator()(const flavor::bslbf_t& node){
        p.enter(node);

//      p.on(node);

        p.exit(node);
    }

    void operator()(const flavor::uimsbf_t& node){
        p.enter(node);

        p.exit(node);
    }

    void operator()(const flavor::tcimsbf_t& node){
        p.enter(node);

        p.exit(node);
    }

    void operator()(const flavor::if_t& node){
        p.enter(node);

        p.on(node, *this);

        p.exit(node);
    }

    void operator()(const flavor::for_t& node){
        p.enter(node);

        p.on(node, *this);

        p.exit(node);
    }

    void operator()(const flavor::compound_t& node){

        p.enter(node);

        auto&& definition = std::get<flavor::compound_definition_t>(symbols.at(node.name));
        for (auto &&entry : definition.entries) {
            std::visit(*this, entry);
        }

        p.exit(node);
    }

    void operator()(const flavor::variable_t& node){
        p.enter(node);

        p.exit(node);
    }

    const flavor::symbols_t& symbols;
    P& p;
};

class print_t{
public:
    explicit print_t(std::ostream& out):out(out), indentation(0){

    }

    void enter(const flavor::bslbf_t &node){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "bslbf"
            << std::endl;
    }
    void enter(const flavor::uimsbf_t &node){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "uimsbf"
            << std::endl;
    }

    void enter(const flavor::tcimsbf_t &node){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "tcimsbf"
            << std::endl;
    }

    void enter(const flavor::if_t &node){
        out << std::string(indentation, ' ') << "if" << "(" << ")" << "{" << std::endl;
        indentation++;
    }
    void enter(const flavor::for_t &node){
        out << std::string(indentation, ' ') << "for" << "(" << ")" << "{" << std::endl;
        indentation++;
    }

    void enter(const flavor::compound_t& node){
        out << std::string(indentation, ' ') << node.name << "(){" << std::endl;
        indentation++;
    }

    void exit(const flavor::bslbf_t &node){

    }
    void exit(const flavor::uimsbf_t &node){

    }
    void exit(const flavor::tcimsbf_t &node){

    }
    void exit(const flavor::if_t &node){
        indentation--;
        out << std::string(indentation, ' ') << "}" << std::endl;
    }
    void exit(const flavor::for_t &node){
        indentation--;
        out << std::string(indentation, ' ') << "}" << std::endl;
    }

    void exit(const flavor::compound_t& node){
        indentation--;
        out << std::string(indentation, ' ') << "}" << std::endl;;
    }


    void on(const flavor::if_t& node, traversal_t<print_t>& traversal){

    }

    void on(const flavor::for_t& node, traversal_t<print_t>& traversal){

    }

private:
    std::ostream& out;
    size_t indentation;
};

void khaotica::printer_t::print(const flavor::document_t &doc, std::ostream &out) {

    print_t print(out);

    traversal_t<print_t> traversal(doc.symbols, print);
    for(auto&& entry : doc.hierarchy){
        traversal(entry);
    }
}
