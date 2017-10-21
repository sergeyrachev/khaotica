#include "printer.h"

#include <boost/core/demangle.hpp>

template<typename P>
class traversal_t{
public:
    traversal_t(const flavor::symbols_t& symbols, P& p):symbols(symbols), p(p){

    }

    void operator()(const flavor::bslbf_t& node){
        p.on(node, *this);
    }

    void operator()(const flavor::uimsbf_t& node){
        p.on(node, *this);
    }

    void operator()(const flavor::tcimsbf_t& node){
        p.on(node, *this);
    }

    void operator()(const flavor::if_t& node){
        p.on(node, *this);
    }

    void operator()(const flavor::for_t& node){
        p.on(node, *this);
    }

    void operator()(const flavor::compound_t& node){
        auto && definition = std::get<flavor::compound_definition_t>(symbols.at(node.name));
        p.on(node, definition, *this);
    }

    void operator()(const flavor::compound_definition_t& node){
        for (auto &&entry : node.entries) {
            std::visit(*this, entry);
        }
    }

    void operator()(const flavor::expression_t& node){
        std::visit(*this, node.sentence);
    }

    void operator()(const flavor::variable_t& node){
        assert( false && "Should never be here. AST should not contain free variables");
    }

    void operator()(const flavor::field_t& node){
        std::visit([this, &node](const auto& definition){
            p.on(node, definition, *this);
        }, symbols.at(node.name));
    }

    void operator()( const flavor::integer_t& node){
        p.on(node, *this);
    }
    void operator()( const flavor::bitstring_t& node){
        p.on(node, *this);
    }
    void operator()( const flavor::unary_expression_t& node){
        std::visit( [this, &node](const auto& operation){
            p.on(operation, *node.operand, *this);
        }, node.operation);

    }
    void operator()( const flavor::binary_expression_t& node){
        std::visit( [this, &node](const auto& operation){
            p.on(*node.left_operand, operation, *node.right_operand, *this);
        }, node.operation);
    }
    void operator()( const flavor::postincrement_t& node){
        p.on(node, *this);
    }
    void operator()( const flavor::preincrement_t& node){
        p.on(node, *this);
    }
    void operator()( const flavor::assignment_t& node){
        p.on(node, *this);
    }

    const flavor::symbols_t& symbols;
    P& p;
};

class print_t{
public:
    explicit print_t(std::ostream& out):out(out), indentation(0){

    }

    void on(const flavor::bslbf_t &node, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "bslbf"
            << std::endl;
    }

    void on(const flavor::uimsbf_t &node, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "uimsbf"
            << std::endl;
    }

    void on(const flavor::tcimsbf_t &node, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ')
            << node.name
            << " "
            << node.length
            << " "
            << "tcimsbf"
            << std::endl;
    }

    void on(const flavor::if_t& node, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ') << "if" << "( ";

        traversal(*node.condition);

        out << " )";

        on_(*node._then, traversal);

        out << "else";

        on_(*node._else, traversal);

        out << std::endl;
    }

    void on(const flavor::for_t& node, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ') << "for" << "(";

        if(node.initializer){
            traversal(**node.initializer);
        }

        out << ";";

        if(node.condition){
            traversal(**node.condition);
        }

        out << ";";

        if(node.modifier){
            traversal(**node.modifier);
        }

        out << ")";

        on_(*node.body, traversal);

        out << std::endl;
    }

    void on(const flavor::compound_t& node, const flavor::compound_definition_t& body, traversal_t<print_t>& traversal){
        out << std::string(indentation, ' ') << node.name << "()";

        on_(body, traversal);

        out << std::endl;
    }

    void on( const flavor::field_t& node, const flavor::bslbf_t& definition, traversal_t<print_t>& traversal){
        out << node.name << "<" <<  "bslbf("<< definition.length << ")>";
    }

    void on( const flavor::field_t& node, const flavor::uimsbf_t& definition, traversal_t<print_t>& traversal){
        out << node.name << "<" <<  "uimsbf("<< definition.length << ")>";
    }

    void on( const flavor::field_t& node, const flavor::tcimsbf_t& definition, traversal_t<print_t>& traversal){
        out << node.name << "<" <<  "tcimsbf("<< definition.length << ")>";
    }

    void on( const flavor::field_t& node, const flavor::compound_definition_t& definition, traversal_t<print_t>& traversal){
        assert( false && "Should never be here until sizeof is not implemented");
    }

    void on( const flavor::field_t& node, const flavor::assignment_t& definition, traversal_t<print_t>& traversal){
        out << node.name << "<";
        traversal(*definition.expression);
        out << ">";
    }

    void on( const flavor::integer_t& node, traversal_t<print_t>& traversal){
        out << node.value;
    }
    void on( const flavor::bitstring_t& node, traversal_t<print_t>& traversal){
        out << "'" << node.value << "'";
    }

    void on( const auto& operation, const flavor::expression_t& operand, traversal_t<print_t>& traversal){
        out << "( ";
        out << boost::core::demangle(typeid(operation).name()) << "(";
        traversal(operand);
        out << ") )";
    }

    void on( const flavor::expression_t& left, const auto& operation, const flavor::expression_t& right, traversal_t<print_t>& traversal){
        out << "( ";
        traversal(left);
        out << " " << boost::core::demangle(typeid(operation).name()) << " ";
        traversal(right);
        out << " )";
    }

    void on( const flavor::postincrement_t& node, traversal_t<print_t>& traversal){
        out << "(" << node.operand.name << "??" <<")";
    }

    void on( const flavor::preincrement_t& node, traversal_t<print_t>& traversal){
        out << "( ??" << node.operand.name <<")";
    }

    void on( const flavor::assignment_t& node, traversal_t<print_t>& traversal){
        out << node.variable.name << "=";
        traversal(*node.expression);
    }

private:
    void on_(const flavor::compound_definition_t& body, traversal_t<print_t>& traversal){
        out << "{" << std::endl;
        indentation++;

        traversal(body);

        indentation--;
        out << std::string(indentation, ' ') << "}";
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
