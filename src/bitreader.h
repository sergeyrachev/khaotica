//
// Created by user on 04.08.17.
//

#pragma once

#ifndef KHAOTICA_BITREADER_H
#define KHAOTICA_BITREADER_H

#include <bit.h>

#include <vector>
#include <fstream>
#include <cstdint>

namespace khaotica{
    class bitreader_t {

    public:
        explicit bitreader_t(std::istream& in) :in(in){

        }

        std::vector<bool> read(uint64_t n){
            std::vector<bool> ret;
            for (auto i = 0; i < n; i++) {
                cache(in);
                if (!_cache.empty()){
                    ret.push_back(_cache.back());
                    _cache.pop_back();
                }
            }
            return ret;
        }

    private:
        void cache(std::istream& in){
            using khaotica::algorithm::unpack;

            if (_cache.empty()) {
                uint8_t t;
                if (in.read(reinterpret_cast<char*>(&t), 1)){
                    _cache = unpack(t);
                }
            }
        }

    private:
        std::istream& in;
        std::vector<bool> _cache;
    };
}

#endif