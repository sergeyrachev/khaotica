#include "bitreader.h"

#include <cstddef>

using namespace khaotica::bitstream::mpeg2;

bitreader_t::bitreader_t(std::istream &in) : in(in) {

}

std::vector<bool> bitreader_t::peek(uint64_t n) {
    using khaotica::algorithm::unpack;
    auto bytes = (n / 8) + 1;
    std::vector<int> buffer;
    for (int i = 0; i < bytes; ++i) {
        buffer.push_back(in.get());
    }

    std::for_each(buffer.rbegin(), buffer.rend(), [this](const auto& v){
        in.putback(v);
    });

    std::vector<bool> bits(_cache);
    for (auto &&item : buffer) {
        auto entry = unpack(item);
        std::copy(entry.rbegin(), entry.rend(), std::back_inserter(bits));
    }

    bits.resize(n);
    return bits;
}

std::vector<bool> bitreader_t::read(uint64_t n) {
    std::vector<bool> ret;
    for (auto i = 0; i < n; i++) {
        if(_cache.empty()){
            _cache = cache(in);
        }
        ret.push_back(_cache.back());
        _cache.pop_back();
    }
    return ret;
}

uint64_t bitreader_t::position() {
    uint64_t pos = in.tellg();
    return pos * 8 - _cache.size();
}

std::vector<bool> bitreader_t::cache(std::istream &in) {
    uint8_t t;
    if (in.read(reinterpret_cast<char *>(&t), 1)) {
        return khaotica::algorithm::unpack(t);
    }
    return {};
}
