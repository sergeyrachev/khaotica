//
// Created by user on 04.08.17.
//
#pragma once

#ifndef KHAOTICA_BIT_H
#define KHAOTICA_BIT_H

#include <vector>
#include <cstdint>
#include <sstream>

namespace khaotica {
    namespace algorithm {

        inline std::vector<bool> unpack(uint8_t v) {
            std::vector<bool> ret(8);
            for (auto &&entry : ret) {
                entry = static_cast<uint8_t>(v & 0x1);
                v >>= 1;
            }
            return ret;
        }

        inline uint64_t to_ull_msbf(const std::vector<bool> &v) {
            uint64_t u(0);
            for (auto it = v.rbegin(); it != v.rend(); ++it) {
                u |= *it;
                u <<= 1;
            }

            return u >> 1;
        }

        inline uint64_t to_ull_lsbf(const std::vector<bool> &v) {
            uint64_t u(0);
            for (auto it = v.begin(); it != v.end(); ++it) {
                u |= *it;
                u <<= 1;
            }

            return u >> 1;
        }

        inline std::string to_string(const std::vector<bool>& v) {
            std::ostringstream ss;
            for (const auto &&b : v) {
                ss << b ? 1 : 0;
            }
            return ss.str();
        }
    }
}
#endif //KHAOTICA_BIT_H
