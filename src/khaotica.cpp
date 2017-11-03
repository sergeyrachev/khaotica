// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "khaotica.h"
#include "parser.h"

#include "logging.h"
#include "options.h"

#include "interpreter.h"
#include "printer.h"
#include "syntax.h"

int main( int argc, char* argv[] ) {
    std::string input_definition_filename;
    std::string input_bitstream_filename;

    namespace po = boost::program_options;
    po::options_description opt_desc( "Options" );
    opt_desc.add_options( )
        ( "help,h", "Produce this message" )
        ( "input-definition,i", po::value(&input_definition_filename)->required(), "Flavor definition for input bitstream" )
        ( "input-bitstream,I", po::value(&input_bitstream_filename)->required(), "input bitstream")
       ;

    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add( "input-definition", 1 );
    pos_opt_desc.add("input-bitstream", 1);
    po::variables_map varmap;
    if( !options::is_args_valid(argc, argv, opt_desc, pos_opt_desc, varmap, std::cerr, std::cout ) ) {
        return 1;
    }

    logging::debug() << "Flavor Definition file: " << input_definition_filename;
    logging::debug() << "Bitstream file: " << input_bitstream_filename;

    std::ifstream flavor_script(input_definition_filename);

    auto doc = flavor::interpreter_t::parse(flavor_script, false);
    bool is_valid = khaotica::syntax_t::is_valid(doc, std::cout);
    khaotica::printer_t::print(doc, std::cout);

    std::ifstream bitstream(input_bitstream_filename, std::ios_base::binary);
    khaotica::bitreader_t bitreader(bitstream);

    while(!bitstream.bad() && !bitstream.eof()){
        auto snapshot = khaotica::parser_t::parse(bitreader, doc);
        khaotica::printer_t::dump(doc, snapshot, std::cout);
    }

    return 0;
}
