#pragma once

#include "grammar.h"
#include "logging.h"
#include "bit.h"
#include "bitreader.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <list>

namespace khaotica {
    class parser_t {
    public:
        typedef std::variant<std::vector<bool>, uint64_t> value_t;
    public:
        parser_t(std::ifstream& in, const std::list<flavor::symbol_t>& symbols);

    public:
        value_t operator( ) (const flavor::bitstring_t& bs) {
            return bitreader.read(bs.length);
        }

        value_t operator( ) (const flavor::uint_t& bs) {
            return khaotica::algorithm::to_ull_msbf(bitreader.read(bs.length));
        }

        void operator( ) (const flavor::bitstring_t& s, const std::vector<bool>& v) {
            logging::debug()
                << s.name << "(" << s.length << "): " << khaotica::algorithm::to_string(v)
                //<< "(" << std::hex << "0x" << khaotica::algorithm::to_ull_lsbf(v) << ")"
                ;
        }

        void operator( ) (const flavor::uint_t& s, const uint64_t& v) {
            logging::debug() << s.name << "(" << s.length << "): " << v << "(" << std::hex << "0x" << v << ")";
        }

        void operator()(auto&&, auto&&){

        }

    private:
        bitreader_t bitreader;
    };

}
