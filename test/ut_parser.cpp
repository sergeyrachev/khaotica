//
// Created by user on 25.08.17.
//

#include "gtest/gtest.h"

#include "lexer.h"
#include "parser.hpp"

#include <sstream>

namespace {
    struct IO : public ::testing::Test{


        std::tuple<int, flavor::document_t, flavor::symbols_t> parse(const std::string& text){

            std::ostringstream serr;
            std::istringstream in(text);
            flavor::lexer_t _scanner(in, serr);

            flavor::symbols_t symbols;
            flavor::document_t doc;
            flavor::parser_t _parser(_scanner, symbols, doc);

            return {_parser.parse(), doc, symbols};
        }
    };
}


TEST_F(IO, VariableDeclaration){
    auto [ret, doc, symbols] = parse("N");
    ASSERT_EQ(1, ret);
}

TEST_F(IO, VariableDefinitionValue){
    auto [ret, doc, symbols] = parse("N=1");
    ASSERT_EQ(0, ret);
}

TEST_F(IO, VariableDefinitionExpression){
    auto [ret, doc, symbols] = parse("N=1+2");
    ASSERT_EQ(0, ret);
}

TEST_F(IO, EmptyCompound){
    auto [ret, doc, symbols] = parse("name(){}");
    ASSERT_EQ(0, ret);
}
