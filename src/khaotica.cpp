#include <boost/uuid/uuid_io.hpp>
#include <boost/make_shared.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>
#include <boost/rational.hpp>
#include <boost/limits.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>

#include <boost/uuid/string_generator.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/chrono.hpp>

bool process_arguments( int argc, char* argv[], const boost::program_options::options_description& _opt_desc, const boost::program_options::positional_options_description& _pos_opt_desc, boost::program_options::variables_map& varmap ) {
    namespace po = boost::program_options;
    try {
        po::store( po::command_line_parser( argc, argv ).options( _opt_desc ).positional( _pos_opt_desc ).run( ), varmap );
        po::notify( varmap );
    } catch( const po::error &err ) {
        std::cerr << "Error parsing command line: " << err.what( ) << std::endl << std::endl;
        std::cout << _opt_desc << std::endl;
        return false;
    }

    if( varmap.count( "help" ) ) {
        std::cout << _opt_desc << std::endl;
        return false;
    }
}

int main( int argc, char* argv[] ) {
    namespace po = boost::program_options;

    po::options_description _opt_desc( "Allowed options" );
    _opt_desc.add_options( )
        ( "help,h", "Produce this message" )
        ( "input,i", po::value<std::string>( ), "Bitstream" )
        ( "flavor,f", po::value<std::string>( ), "Flavor description" )
       ;

    po::positional_options_description _pos_opt_desc;
    _pos_opt_desc.add( "input", 1 );
    _pos_opt_desc.add( "flavor", std::numeric_limits<int>::max() );

    po::variables_map varmap;
    if( !process_arguments( argc, argv, _opt_desc, _pos_opt_desc, varmap ) ) {
        return 1;
    }


}