#include "bitreader.h"

using namespace khaotica::bitstream::mpeg2;

bitreader_t::bitreader_t(std::istream &in) : in(in) {

}

std::vector<bool> bitreader_t::peek() {
    using khaotica::algorithm::unpack;
    return unpack(in.peek());
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
    return pos;
}

std::vector<bool> bitreader_t::cache(std::istream &in) {
    uint8_t t;
    if (in.read(reinterpret_cast<char *>(&t), 1)) {
        return khaotica::algorithm::unpack(t);
    }
    return {};
}
