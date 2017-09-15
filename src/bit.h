//
// Created by user on 04.08.17.
//
#pragma once

#ifndef KHAOTICA_BIT_H
#define KHAOTICA_BIT_H

#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cassert>

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

        inline std::vector<bool> unpack(const std::string& v) {
            std::vector<bool> ret;
            for (auto &&entry : v) {
                assert(entry == '0' || entry == '1');
                bool bit = entry == '1';
                ret.push_back(bit);
            }
            return ret;
        }

        inline uint64_t to_ull_msbf(const std::vector<bool> &v) {
            uint64_t u(0);
            for (auto it = v.begin(); it != v.end(); ++it) {
                u <<= 1;
                u |= *it;
            }

            return u;
        }

        inline uint64_t to_ull_lsbf(const std::vector<bool> &v) {
            uint64_t u(0);
            for (auto it = v.rbegin(); it != v.rend(); ++it) {
                u <<= 1;
                u |= *it;
            }

            return u;
        }

        inline std::string to_string(const std::vector<bool> &v) {
            std::ostringstream ss;
            for (const auto &&b : v) {
                ss << b ? 1 : 0;
            }
            return ss.str();
        }

        inline std::string to_hex(uint64_t v) {
            std::stringstream ss;
            ss << std::hex << v;
            return ss.str();
        }

        inline std::string to_hex(const std::vector<bool> &v) {
            std::stringstream ss;
            ss << "0x";
            uint16_t value(0);
            size_t idx(0);
            for (const auto &&b : v) {
                value <<= 1;
                value |= b;
                idx++;
                if (idx == 8) {
                    ss << std::setfill('0') << std::setw(2) << std::hex << value;
                    idx = 0;
                    value = 0;
                }

            }
            if (idx) {
                ss << std::setfill('0') << std::setw(2) << std::hex << value;
            }

            return ss.str();
        }
    }
}
#endif //KHAOTICA_BIT_H

