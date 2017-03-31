//
// Created by sergey.rachev on 3/31/17.
//

#include "logging.h"

#include <boost/log/trivial.hpp>

namespace {
    template<typename T>
    const logging::debug &impl(const logging::debug &i, const T& v) {
        BOOST_LOG_TRIVIAL(debug) << v;
        return i;
    }
}

const logging::debug& operator<<(const logging::debug& i, const std::string & v) {
    return impl(i, v);
}

const logging::debug &operator<<(const logging::debug &i, const int32_t& v){
    return impl(i, v);
}

const logging::debug &operator<<(const logging::debug &i, const int64_t &v) {
    return impl(i, v);
}

const logging::debug &operator<<(const logging::debug &i, const uint32_t &v) {
    return impl(i, v);
}

const logging::debug &operator<<(const logging::debug &i, const uint64_t &v) {
    return impl(i, v);
}

const logging::debug &operator<<(const logging::debug &i, const double& v) {
    return impl(i, v);
}