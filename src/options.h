//
// Created by sergey.rachev on 3/31/17.
//
#pragma once

#ifndef KHAOTICA_OPTIONS_H
#define KHAOTICA_OPTIONS_H

#include <boost/program_options.hpp>
#include <iostream>

namespace options{
    bool is_args_valid(int argc, char *argv[], const boost::program_options::options_description &opt_desc,
                       const boost::program_options::positional_options_description &pos_opt_desc,
                       boost::program_options::variables_map &varmap, std::ostream &cerr, std::ostream &cout);    
}



#endif //KHAOTICA_OPTIONS_H
