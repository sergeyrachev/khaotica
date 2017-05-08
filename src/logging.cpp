// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "logging.h"

#include <boost/log/trivial.hpp>

logging::debug::~debug(){
    BOOST_LOG_TRIVIAL(debug) << get();
}
logging::error::~error(){
    BOOST_LOG_TRIVIAL(error) << get();
}