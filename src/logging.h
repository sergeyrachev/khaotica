//
// Created by sergey.rachev on 3/31/17.
//
#pragma once

#ifndef KHAOTICA_LOGGING_H
#define KHAOTICA_LOGGING_H

#include <cstdint>
#include <string>

namespace logging{
    class debug {
    public:
        debug(){}
    };
}

const logging::debug &operator<<(const logging::debug &i, const std::string&);
const logging::debug &operator<<(const logging::debug &i, const int32_t&);
const logging::debug &operator<<(const logging::debug &i, const int64_t&);
const logging::debug &operator<<(const logging::debug &i, const uint32_t &);
const logging::debug &operator<<(const logging::debug &i, const uint64_t &);
const logging::debug &operator<<(const logging::debug &i, const double&);

#endif //KHAOTICA_LOGGING_H
