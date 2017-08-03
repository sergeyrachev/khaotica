// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include <fstream>
#include "renderer.h"
#include <variant>
#include "grammar.h"
#include "logging.h"

IRRenderer::IRRenderer(std::ifstream& bitstream, const std::list<flavor::symbol_t>& symbols) :_bitstream(bitstream)
{
    while(!_bitstream.eof() && !_bitstream.bad()){
        for(const auto& symbol : symbols){
            const auto v = std::visit(*this, symbol);
            std::visit(*this, symbol, v);
        }
    }
}
