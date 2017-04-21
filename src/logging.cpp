// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

logging::debug::debug() {

}

logging::debug::~debug() {
    (*this) << "\n";
}
