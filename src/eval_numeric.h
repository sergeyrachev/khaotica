#pragma once

namespace khaotica {
    namespace eval {
        struct indexation_t {
            template<typename T, typename = std::enable_if <std::is_arithmetic_v<T>>>
            uint64_t operator()(const T &v) {
                return static_cast<uint64_t>(v);
            }

            uint64_t operator()(const bitstring_v &v) {
                return khaotica::algorithm::to_integer_msbf<uint64_t>(v.value);
            }

            uint64_t operator()(const std::shared_ptr <nextbits_v> &v) {
                assert(false && "WAT??????");
                return 0;
            }
        };
    }
}
