#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>
#include "grammar.h"
#include "logging.h"

class IRRenderer {
public:
    typedef std::variant<std::bitset<8>, uint64_t> value_t;
public:
    IRRenderer(std::ifstream&, const std::list<flavor::symbol_t>&);
    value_t operator( ) (const flavor::bitstring_t& bs) {
        std::bitset<8> ret(0);

        if (pos == 0) {
            uint8_t t;
            _bitstream >> t;
            cache = decltype(cache)(t);
            pos = 8;
        }

        for (int i = 1; i <= bs.length; i++)
        {
            ret[bs.length - i] = cache[pos-1];
            pos--;

            if (pos == 0) {
                uint8_t t;
                _bitstream >> t;
                cache = decltype(cache)(t);
                pos = 8;
            }
        }

        return ret;
    }

    value_t operator( ) (const flavor::uint_t&) {
        return uint64_t{};
    }

    void operator( ) (const flavor::bitstring_t& s, const std::bitset<8>& v) {
        logging::debug() << s.name << "(" << s.length << "): " << v.to_string() << "(" << std::hex << v.to_ullong() << ")" << static_cast<uint8_t>(v.to_ullong());
    }

    void operator( ) (const flavor::uint_t& s, const uint64_t& v) {
        logging::debug() << s.name << "(" << s.length << "): " << v << "(" << std::hex << v << ")";
    }

    void operator()(auto&&, auto&&){

    }

private:
    std::ifstream& _bitstream;
    std::bitset<8> cache;
    uint8_t pos{0};
};
