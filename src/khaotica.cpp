// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "khaotica.h"
#include "khaotica/mpeg2_interpreter.h"
#include "khaotica/mpeg2_reader.h"

#include "printer.h"
#include "logging.h"
#include "options.h"

#include "khaotica/mpeg2_syntax.h"
#include "scanner.h"

int main( int argc, char* argv[] ) {
    std::string input_definition_filename;
    std::string input_bitstream_filename;
    bool enable_verbose{false};
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

    auto document = khaotica::syntax::mpeg2::interpreter_t::parse(flavor_script);

    khaotica::printer_t printer(document.global);
    std::cout << printer(document.sequence);

    std::ifstream bitstream(input_bitstream_filename, std::ios_base::binary);
    khaotica::bitstream::mpeg2::bitreader_t bitreader(bitstream);
    khaotica::bitstream::mpeg2::reader_t::parse(bitreader, document, *std::make_shared<khaotica::scanner_t>());

    return 0;
}

