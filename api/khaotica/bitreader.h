#pragma once

#ifndef KHAOTICA_BITREADER_H
#define KHAOTICA_BITREADER_H

#include <vector>
#include <fstream>
#include <type_conversion.h>

namespace khaotica::bitstream::mpeg2 {
    class bitreader_t {
    public:
        explicit bitreader_t(std::istream &in);

        std::vector<bool> peek();
        std::vector<bool> read(uint64_t n);
        uint64_t position();

    private:
        static std::vector<bool> cache(std::istream &in);

    private:
        std::istream &in;
        std::vector<bool> _cache;
    };
}

#endif
