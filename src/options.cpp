// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "options.h"

bool options::is_args_valid(int argc, char *argv[], const boost::program_options::options_description &opt_desc,
                   const boost::program_options::positional_options_description &pos_opt_desc,
                   boost::program_options::variables_map &varmap, std::ostream& cerr,std::ostream& cout ) {
   using namespace boost::program_options;
    try {
        store(command_line_parser(argc, argv)
                          .style(command_line_style::unix_style)
                          .options(opt_desc)
                          .positional(pos_opt_desc)
                          .run(), varmap);
        notify(varmap);
    } catch (const error &err) {
        cerr << "Error parsing command line: " << err.what() << std::endl << std::endl;
        cerr << opt_desc << std::endl;
        return false;
    }

    if (varmap.count("help") || varmap.empty()) {
        cout << opt_desc << std::endl;
        return false;
    }

    return true;
}