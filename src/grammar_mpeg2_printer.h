#pragma once

#include "grammar_mpeg2_types.h"

#include <variant>
#include <iostream>
#include <sstream>
#include <cassert>

namespace khaotica {

    class printer_t{
    public:
        explicit printer_t(std::shared_ptr<scope_t> global) : indentation(0), global(global) {

        }

        std::string operator()(const sequence_t& node){
            std::ostringstream out;
            for(const auto& entry : node){
                out << std::visit(*this, entry->payload);
                out << std::endl;
            }
            return out.str();
        }

        std::string operator()(const auto& node) {
            std::ostringstream out;

            return out.str();
        }

        std::string operator()(const length_t &length) {
            std::ostringstream out;

            if(length.from == length.to){
                out << length.from;
            }else {
                out << length.from << "-" << length.to;
            }
            return out.str();
        }

        std::string operator()(const integer_t &node) {
            std::ostringstream out;
            out << node.value;
            return out.str();
        };

        std::string operator()(const identifier_t &node) {
            std::ostringstream out;
            out << node.name;
            return out.str();
        };

        std::string operator()(const bitstring_t &node) {
            std::ostringstream out;
            out << "'" << node.value << "'";
            return out.str();
        };

        std::string operator()(const bslbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << (*this)(node.length)
                << " "
                << "bslbf";
            return out.str();
        };

        std::string operator()(const uimsbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << (*this)(node.length)
                << " "
                << "uimsbf";
            return out.str();
        };

        std::string operator()(const simsbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << (*this)(node.length)
                << " "
                << "simsbf";
            return out.str();
        };

        std::string operator()(const vlclbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << (*this)(node.length)
                << " "
                << "vlclbf";
            return out.str();
        };

        std::string operator()(const collection_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << std::visit(*this, node.entry)
                << " * " << node.size;
            return out.str();
        };

        std::string operator()(const slot_t &node) {
            std::ostringstream out;
            out << std::visit(*this, node.entry);

            for(const auto& i : node.indices) {
                out << "[" << std::visit(*this, i) << "]";
            }
            return out.str();
        };

        std::string operator()(const sparsed_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << std::visit(*this, node.entry)
                << "[" << node.range.front << ".." << node.range.back <<"]";
            return out.str();
        };

        std::string operator()(const reference_t &node) {
            std::ostringstream out;

            auto& compound = std::get<compound_t>(global->definitions.at(node.name)->payload);

            out << std::string(indentation, ' ')
                << node.name << "(";

            assert( node.args.size() == compound.args.size() );

            for (auto [arg, param] = std::pair{node.args.begin(), compound.args.begin()}; arg != node.args.end() || param != compound.args.end(); ++arg, ++param) {
                out << *param << " = " << std::visit(*this, (*arg)->payload) << ", ";
            }
            out << "){" << std::endl;

            indentation++;

            out << std::visit(*this, compound.body->payload);

            indentation--;
            out << std::string(indentation, ' ') << "}";
            return out.str();
        };

        std::string operator()(const if_t &node) {
            std::ostringstream out;

            out << std::string(indentation, ' ') << "if" << "( ";

            out << std::visit(*this, node.condition->payload );

            out << " ) {" << std::endl;

            indentation++;
            out << std::visit(*this, node._then->payload );
            if(node._else ){

                indentation--;
                out << std::string(indentation, ' ') << "} else {" << std::endl;
                indentation++;
                out << std::visit(*this, (*node._else)->payload );
            }

            indentation--;
            out << std::string(indentation, ' ') << "}";
            return out.str();
        };

        std::string operator()(const for_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ') << "for" << "(";

            if (node.initializer) {
                //out << on(*node.initializer);
            }

            out << ";";

            if (node.condition) {
                //out << on(*node.condition);
            }

            out << ";";

            if (node.modifier) {
               // out << on(*node.modifier);
            }

            out << ") {" << std::endl;
            indentation++;

//            for (auto &&item : node.body.body) {
//                out << on(item);
//            }
            indentation--;

            out << std::string(indentation, ' ') << "}";
            return out.str();
        };

        std::string operator()(const do_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ') << "do {" << std::endl;

            indentation++;

            out << std::visit(*this, node.body->payload);

            indentation--;
            out << std::string(indentation, ' ') << "} while()";
            return  out.str();
        }

        std::string operator()(const while_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << "while(){" << std::endl;
            indentation++;

            out << std::visit(*this, node.body->payload);

            indentation--;
            out << std::string(indentation, ' ') << "}";
            return  out.str();
        }

        std::string operator()(const compound_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ') << node.name << "() {" << std::endl;
            indentation++;

            out << std::visit(*this, node.body->payload);

            indentation--;
            out << std::string(indentation, ' ') << "}";
            return out.str();
        };


        std::string operator()(const assignment_t &node) {
            std::ostringstream out;
            out << node.symbol << "=";
            out << std::visit(*this, node.expression->payload);
            return out.str();
        };

        std::string operator()(const preincrement_t &node) {
            std::ostringstream out;
            out << "( " << node.operation << node.operation << node.operand << " )";
            return out.str();
        };

        std::string operator()(const postincrement_t &node) {
            std::ostringstream out;
            out << "( " << node.operand << node.operation << node.operation << " )";
            return out.str();
        };

        std::string operator()(const unary_expression_t &node) {
            std::ostringstream out;
            out << "( " << node.operation;
            out << std::visit(*this, node.operand->payload);
            out << " )";
            return out.str();
        };

        std::string operator()(const binary_expression_t &node) {
            std::ostringstream out;
            out << "( ";
            out << std::visit(*this, node.left_operand->payload);
            out << node.operation;
            out << std::visit(*this, node.right_operand->payload);
            out << " )";
            return out.str();
        };

        std::string operator()(const position_t &node) {
            std::ostringstream out;
            out << "__position(";
            out << (node.name ? *node.name : ".");
            out << " )";
            return out.str();
        };

        std::string operator()(const nextbits_t &node) {
            std::ostringstream out;
            out << "nextbits(";
            out << " )";
            return out.str();
        };

    private:
        size_t indentation;
        std::shared_ptr<scope_t> global;
    };
}
