// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include <fstream>
#include "parser.h"
#include <variant>

#include "syntax.h"
#include "logging.h"

using namespace khaotica;

parser_t::parser_t(std::ifstream& bitstream, const flavor::document_t& doc)
    : bitreader(bitstream)
{
//    while(!bitstream.eof() && !bitstream.bad()){
//        for(const auto& symbol : symbols){
//            const auto v = std::visit(*this, symbol);
//            for (auto &&item : v) {
//                std::visit(*this, symbol, item);
//            }
//        }
//
//        logging::debug() << "";
//    }
}
