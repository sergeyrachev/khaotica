// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// #include "llvm/ExecutionEngine/JIT.h"
// #include "llvm/Support/TargetSelect.h"

#include "khaotica.h"

#include "renderer.h"

#include "logging.h"
#include "options.h"

#include "export.h"
#include "repository.h"

#include "interpreter.h"
#include "ast.h"

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

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();

    std::ifstream f(input_definition_filename);

    std::vector<ASTNode*> asts;
    flavor::Interpreter driver;
    driver.parse(f, asts);
    std::shared_ptr<repository> r(create_repository(input_bitstream_filename.c_str()), [](repository* p){destroy_repository(p);});
    IRRenderer renderer((std::ifstream(input_bitstream_filename)));
    std::for_each(asts.rbegin(), asts.rend(), [&renderer](ASTNode* node ){
        node->codegen(renderer);
    });
    return 0;

}
