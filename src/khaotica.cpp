// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "khaotica.h"
#include "renderer.h"

#include "logging.h"
#include "options.h"

#include "interpreter.h"

int main( int argc, char* argv[] ) {
    namespace po = boost::program_options;

    po::options_description opt_desc( "Options" );
    opt_desc.add_options( )
        ( "help,h", "Produce this message" )
        ( "input-definition,i", po::value<std::string>( )->required(), "Flavor definition for input bitstream" )
        ( "input-bitstream,I", po::value<std::string>()->required(), "input bitstream")
       ;

    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add( "input-definition", 1 );
    pos_opt_desc.add("input-bitstream", 1);

    po::variables_map varmap;
    if( !options::is_args_valid(argc, argv, opt_desc, pos_opt_desc, varmap, std::cerr, std::cout ) ) {
        return 1;
    }

    const std::string& input_definition_filename = varmap.at("input-definition").as<std::string>();
    const std::string& input_bitstream_filename = varmap.at("input-bitstream").as<std::string>();
    logging::debug() << "Flavor Definition file: " << input_definition_filename;
    logging::debug() << "Bitstream file: " << input_bitstream_filename;

    std::ifstream f(input_definition_filename);

    flavor::Interpreter driver;
    auto symbols = driver.parse(f, true);

    std::ifstream bitstream(input_bitstream_filename, std::ios_base::binary);
    //khaotica::parser_t renderer(bitstream, symbols);

    return 0;
}
