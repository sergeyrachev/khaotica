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
        typedef std::variant< std::pair<flavor::bslbf_t, std::vector<bool>> , std::pair< flavor::uimsbf_t, uint64_t>> value_t;
        typedef std::list<value_t> values_t;
    public:
        parser_t(std::ifstream& in, const flavor::symbols_t& symbols);

    public:
        values_t operator( ) (const flavor::bslbf_t& bs) {
            values_t v;
            v.push_back(std::make_pair(bs, bitreader.read(bs.length)));
            return v;
        }

        values_t operator( ) (const flavor::uimsbf_t& bs) {
            values_t v;
            v.push_back(std::make_pair(bs, khaotica::algorithm::to_ull_msbf(bitreader.read(bs.length))));
            return v;
        }

        values_t operator( ) (const flavor::compound_t& bs) {
            values_t values;
            for (const auto & symbol : bs.body) {
                const auto v = std::visit(*this, symbol);
                values.insert(values.end(), v.begin(), v.end());
            }
            return values;
        }

        values_t operator()(auto&&){

        }

        void operator( ) (const flavor::bslbf_t& s, const std::vector<bool>& v) {
            logging::debug()
                << s.name << "(" << s.length << "): " << khaotica::algorithm::to_hex(v)
                //<< "(" << std::hex << "0x" << khaotica::algorithm::to_ull_lsbf(v) << ")"
                ;
        }

        void operator( ) (const flavor::uimsbf_t& s, const uint64_t& v) {
            logging::debug() << s.name << "(" << s.length << "): " << v << "(" << std::hex << "0x" << v << ")";
        }

        void operator( ) (const flavor::compound_t& s, auto&& v) {
            logging::debug() << s.name << "(" << "): ";
            this->operator()(v.first, v.second);
        }

        void operator()(auto&&, auto&&){

        }



    private:
        bitreader_t bitreader;
    };

}
