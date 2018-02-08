#pragma once

#include "grammar_mpeg2_types.h"

#include <variant>
#include <iostream>
#include <sstream>

namespace khaotica {

    class print_t {
    public:
        explicit print_t(const sequence_t &doc) : indentation(0), doc(doc) {

        }

        std::string on(std::shared_ptr<node_t> node) {
            return std::visit(*this, node->payload);
        }

        std::string operator()(const fixed_length_t &length) {
            std::ostringstream out;
            out << length.value;
            return out.str();
        }

        std::string operator()(const variable_length_t &length) {
            std::ostringstream out;
            out << length.from << "-" << length.to;
            return out.str();
        }

        std::string operator()(const bslbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << (*this)(node.length)
                << " "
                << "bslbf"
                << std::endl;
            return out.str();
        };

        std::string operator()(const uimsbf_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.name
                << " "
                << std::visit(*this, node.length)
                << " "
                << "uimsbf"
                << std::endl;
            return out.str();
        };

        std::string operator()(const sparsed_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ')
                << node.bits.name
                << "["
                << node.range.first
                << ".."
                << node.range.second
                << "]"
                << " "
                << node.bits.length
                << " "
                << "bslbf"
                << std::endl;
            return out.str();
        };

        std::string operator()(const bitstring_t &node) {
            std::ostringstream out;
            out << "'" << node.value << "'";
            return out.str();
        };

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

        std::string operator()(const reference_t &node) {
            std::ostringstream out;

            out << std::string(indentation, ' ') << node.name << "() -> { " << std::endl;
            indentation++;

            auto &compound = std::get<compound_t>(doc.definitions.at(node.name)->payload);

            for (auto &&item : compound.body) {
                out << on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };

        std::string operator()(const if_t &node) {
            std::ostringstream out;

            out << std::string(indentation, ' ') << "if" << "( ";

            out << on(node.condition);

            out << " ) {" << std::endl;

            indentation++;
            for (auto &&item : node._then) {
                out << on(item);
            }

            out << (node._else.empty() ? "" : "}else{");

            for (auto &&item : node._else) {
                out << on(item);
            }

            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };

        std::string operator()(const for_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ') << "for" << "(";

            if (node.initializer) {
                out << on(*node.initializer);
            }

            out << ";";

            if (node.condition) {
                out << on(*node.condition);
            }

            out << ";";

            if (node.modifier) {
                out << on(*node.modifier);
            }

            out << ") {" << std::endl;
            indentation++;

            for (auto &&item : node.body) {
                out << on(item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };

        std::string operator()(const do_t &node) {
            return "do{}while()";
        }

        std::string operator()(const while_t &node) {
            return "while(){}";
        }

        std::string operator()(const compound_t &node) {
            std::ostringstream out;
            out << std::string(indentation, ' ') << node.name << "() {" << std::endl;
            indentation++;
            for (auto &&item :node.body) {
                out << on(item);
            }
            indentation--;
            out << std::string(indentation, ' ') << "}" << std::endl;
            return out.str();
        };


        std::string operator()(const assignment_t &node) {
            std::ostringstream out;
            out << node.symbol << "=";
            out << on(node.expression);
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
            out << on(node.operand);
            out << " )";
            return out.str();
        };

        std::string operator()(const binary_expression_t &node) {
            std::ostringstream out;
            out << "( ";
            out << on(node.left_operand);
            out << node.operation;
            out << on(node.right_operand);
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
        const sequence_t &doc;
    };
}