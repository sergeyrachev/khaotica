//
// Created by user on 25.08.17.
//

#include "gtest/gtest.h"

#include "lexer.h"
#include "parser.hpp"

#include <sstream>

namespace {
    std::string bslbf("identifier 8 bslbf");
    std::string uimsbf("identifier 8 uimsbf");
    std::string tcimsbf("identifier 8 tcimsbf");

    std::string string_literal("name");
    std::string numeric_literal("12");
    std::string bitstring("'0011'");

    std::string if_( const std::string& condition ) {
        return "if(" + condition + "){}";
    }
    std::string unary_expression(const std::string& arg1, const std::string& op){
        return op + arg1;
    }

    std::string binary_expression(const std::string& arg1, const std::string& op, const std::string& arg2){
        return arg1 + op + arg2;
    }

    std::string for_(const std::string& initializer, const std::string& condition, const std::string& modifier){
        return "for(" + initializer + ";" + condition + ";" + modifier + "){}";
    }

    std::string assignment(const std::string& identifier, const std::string& value){
        return identifier + "=" + value;
    }

    std::vector<std::string> additive_operators{"+", "-", "||"};
    std::vector<std::string> multiplicative_operators{"*", "/", "&&"};
    std::vector<std::string> compare_operators{"<", ">", "==", "<=", ">=", "!="};
    std::vector<std::string> unary_operators{"-", "~", "!"};

    struct interpreter_t : public ::testing::Test{

        std::tuple<int, flavor::document_t, flavor::symbols_t> parse(const std::string& text){

            std::ostringstream serr;
            std::istringstream in(text);
            flavor::lexer_t _scanner(in, serr);

            flavor::symbols_t symbols;
            flavor::document_t doc;
            flavor::parser_t _parser(_scanner, symbols, doc);
            //_parser.set_debug_level(1);
            return {_parser.parse(), doc, symbols};
        }
    };
}

TEST_F(interpreter_t, variable_definition){

    for (auto &&arg1 : {numeric_literal, string_literal}) {
        for (auto &&arg2 : {numeric_literal, string_literal}) {
            for (auto &&arg3 : {numeric_literal, string_literal}) {
                for (auto &&add : additive_operators) {
                    for (auto &&mult : multiplicative_operators) {
                        for (auto &&comp: compare_operators ) {
                            auto input = binary_expression(binary_expression(arg1, add, arg2), mult, arg3);
                            input = assignment( string_literal, binary_expression(input, comp, input));
                            std::cout << input << std::endl;
                            auto [ret, doc, symbols] = parse(input);
                            EXPECT_EQ(0, ret) << input;
                        }
                    }
                }
            }
        }
    }
}