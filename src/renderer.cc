// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include <fstream>
#include "renderer.h"

IRRenderer::IRRenderer(std::ifstream& in) :_bitstream(in), cache(0), pos(8)
{
        uint8_t t;
        _bitstream >> t;
        cache = decltype(cache)(t);
        pos = 8;

}


IRRenderer::~IRRenderer() {

}
