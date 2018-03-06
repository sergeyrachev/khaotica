//
// Created by user on 04.08.17.
//

#include "gtest/gtest.h"

#include "bitreader.h"

using namespace khaotica::bitstream::mpeg2;

TEST(Bitreader, FirstBit){
    std::stringstream ss;
    ss.str("\x80");

    bitreader_t bs(ss);
    auto bits = bs.read(1);

    ASSERT_EQ(1, bits.size());
    ASSERT_EQ(1, bits[0]);
}

TEST(Bitreader, TSHeader){
    std::stringstream ss;
    ss.str("\x47\x1f\xff");

    bitreader_t bs(ss);
    auto bits = bs.read(8);

    ASSERT_EQ(8, bits.size());
    ASSERT_EQ(0, bits[0]);
    ASSERT_EQ(1, bits[1]);
    ASSERT_EQ(0, bits[2]);
    ASSERT_EQ(0, bits[3]);

    bits = bs.read( 1);
    ASSERT_EQ(1, bits.size());
    ASSERT_EQ(0, bits[0]);

    bits = bs.read(1);
    ASSERT_EQ(1, bits.size());
    ASSERT_EQ(0, bits[0]);

    bits = bs.read(1);
    ASSERT_EQ(1, bits.size());
    ASSERT_EQ(0, bits[0]);

    bits = bs.read(13);
    ASSERT_EQ(13, bits.size());
    ASSERT_EQ(1, bits[0]);
    ASSERT_EQ(1, bits[1]);
    ASSERT_EQ(1, bits[2]);
    ASSERT_EQ(1, bits[3]);
}
