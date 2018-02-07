//
// Created by user on 04.08.17.
//

#include "gtest/gtest.h"

#include "type_conversion.h"

using namespace khaotica::algorithm;
TEST(Bit, UnpackLeastSignificantFirst){

    auto v = unpack(0x82);
    ASSERT_EQ(8, v.size());
    ASSERT_EQ(0, v[0]);
    ASSERT_EQ(1, v[7]);
}

TEST(Bit, to_ull_msbf){
    auto u = to_ull_msbf({true, false, false, false, false, false, false, false});

    ASSERT_EQ(128, u);
}

TEST(Bit, to_ull_lsbf){
    auto u = to_ull_lsbf({true, false, false, false, false, false, false, false});

    ASSERT_EQ(1, u);
}

TEST(Bit, to_str){
    auto u = to_string({true, false, false, false, false, false, false, false});

    ASSERT_EQ("10000000", u);
}

