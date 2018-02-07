//
// Created by user on 25.08.17.
//

#include "gtest/gtest.h"

#include "grammar_mpeg2_lexer.h"
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
        return "root(){ if(" + condition + "){} }";
    }
    std::string unary_expression(const std::string& arg1, const std::string& op){
        return op + arg1;
    }

    std::string binary_expression(const std::string& arg1, const std::string& op, const std::string& arg2){
        return arg1 + op + arg2;
    }

    std::string for_(const std::string& initializer, const std::string& condition, const std::string& modifier){
        return "root() { for(" + initializer + ";" + condition + ";" + modifier + "){} }";
    }

    std::string assignment(const std::string& identifier, const std::string& value){
        return identifier + "=" + value;
    }

    std::vector<std::string> additive_operators{"+", "-", "||"};
    std::vector<std::string> multiplicative_operators{"*", "/", "&&"};
    std::vector<std::string> compare_operators{"<", ">", "==", "<=", ">=", "!="};
    std::vector<std::string> unary_operators{"-", "~", "!"};
    std::vector<std::string> increment{"i++", "i--"};

    struct interpreter_t : public ::testing::Test{
        std::tuple<int, khaotica::document_t> parse(const std::string& text){

            std::ostringstream serr;
            std::istringstream in(text);
            khaotica::lexer_t _scanner(in, serr);

            khaotica::document_t doc;

            khaotica::parser_t _parser(_scanner, doc, &doc.global);
            //_parser.set_debug_level(1);
            return {_parser.parse(), doc};
        }
    };
}

TEST_F(interpreter_t, variable_definition){
    std::vector<std::string> expressions;
    for (auto &&arg1 : {numeric_literal, string_literal}) {
        for (auto &&arg2 : {numeric_literal, string_literal}) {
            for (auto &&arg3 : {numeric_literal, string_literal}) {
                for (auto &&add : additive_operators) {
                    for (auto &&mult : multiplicative_operators) {
                        for (auto &&comp: compare_operators ) {
                            auto input = binary_expression(binary_expression(arg1, add, arg2), mult, arg3);
                            expressions.push_back(input);

                            auto definition = assignment( string_literal, binary_expression(input, comp, input));
                            auto [ret, doc] = parse(definition);
                            EXPECT_EQ(0, ret) << definition;
                        }
                    }
                }
            }
        }
    }

    for (auto &&expression : expressions) {
        auto expr_if = if_(expression);
        auto [ret, doc] = parse(expr_if);
        EXPECT_EQ(0, ret) << expr_if;
    }

    for (auto &&init_expr : expressions) {
        for (auto &&modifier_expr : increment) {
            auto expr_for = for_(assignment("i", init_expr), init_expr, modifier_expr);
            auto [ret, doc] = parse(expr_for);
            EXPECT_EQ(0, ret) << expr_for;
        }

        auto expr_for = for_(assignment("i", init_expr), init_expr, init_expr);
        auto [ret, doc] = parse(expr_for);
        EXPECT_EQ(0, ret) << expr_for;
    }
}

TEST_F(interpreter_t, conditions){

    auto expr_if = R"'(
    root(){
        if(i) {
            if(j) {
            } else {
            }
        } else {
            if (k) {}
        }
    }
    )'";
    auto [ret, doc] = parse(expr_if);
    EXPECT_EQ(0, ret) << expr_if;
}
