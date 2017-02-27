// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// #include "llvm/ExecutionEngine/JIT.h"
// #include "llvm/Support/TargetSelect.h"

#include "khaotica.h"

#include "codegen/renderer.h"
#include "parsing/tree.h"

#include <cctype>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iostream>
#include <sstream>
#include <string>


bool is_args_valid( int argc, char* argv[], const boost::program_options::options_description& opt_desc, const boost::program_options::positional_options_description& pos_opt_desc, boost::program_options::variables_map& varmap ) {
    namespace po = boost::program_options;
    try {
        po::store( po::command_line_parser( argc, argv )
            .style(po::command_line_style::unix_style)
            .options(opt_desc )
            .positional( pos_opt_desc )
            .run( ), varmap );
        po::notify( varmap );
    } catch( const po::error &err ) {
        std::cerr << "Error parsing command line: " << err.what( ) << std::endl << std::endl;
        std::cerr << opt_desc << std::endl;
        return false;
    }

    if( varmap.count( "help" ) || varmap.empty() ) {
        std::cout << opt_desc << std::endl;
        return false;
    }

    return true;
}

int main( int argc, char* argv[] ) {
    namespace po = boost::program_options;

    po::options_description opt_desc( "Options" );
    opt_desc.add_options( )
        ( "help,h", "Produce this message" )
        ( "input-definition,i", po::value<std::string>( )->required(), "Flavor definition for input bitstream" )
       ;

    po::positional_options_description pos_opt_desc;
    pos_opt_desc.add( "input", 1 );

    po::variables_map varmap;
    if( !is_args_valid(argc, argv, opt_desc, pos_opt_desc, varmap ) ) {
        return 1;
    }

    const std::string& input_definition_filename = varmap.at("input-definition").as<std::string>();
    std::cout << input_definition_filename << std::endl;

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    IRRenderer *renderer = new IRRenderer();

    STree *tree = new STree();

    std::string input;
    fprintf(stderr, "ready> ");
    std::ifstream f(input_definition_filename);

    while( std::getline(f, input, ';') ) {
        std::istringstream iss(input);

        tree->parse(iss);
        if( tree->root != 0 ) {
            if( Function *func = static_cast<Function*>(tree->root->codegen(renderer)) ) {
                if( func->getName() == "" ) {
         
                    double(*func_pointer)() = (double(*)())(intptr_t)(renderer->engine()->getFunctionAddress(func->getName()));
                    break;

                    //fprintf(stderr, "Evaluated to: %f\n", func_pointer());
                }
            }
        }
        fprintf(stderr, "ready> ");
    }

    //renderer->module->dump();


    return 0;

}
