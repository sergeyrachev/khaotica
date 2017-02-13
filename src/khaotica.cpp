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
//     using namespace clang;
//     using namespace llvm;
// 
//     auto compiler(std::make_shared<CompilerInstance>());
//     compiler->createDiagnostics(); // to stdout
//     assert(compiler->hasDiagnostics());
// 
//     compiler->getFrontendOpts().Inputs.push_back({ "microuniverse.cpp", IK_CXX });
//     compiler->getFrontendOpts().OutputFile = "ast.xml";
//     compiler->getTargetOpts().Triple = "i686"; // x86, alpha, ppc, ppc64, ...
//     compiler->getLangOpts().CPlusPlus = 1;
//     
//     std::shared_ptr<FrontendAction> action(std::make_shared<ASTDeclListAction>());
// 
//     bool actionSuccessful = compiler->ExecuteAction(*action);
//     assert(actionSuccessful);   
// 
// 
// 
//     llvm::InitializeNativeTarget();
//     llvm::InitializeNativeTargetAsmPrinter();
//     llvm::InitializeNativeTargetAsmParser();
// 
//     llvm::LLVMContext Context;
// 
//     // Create the "module" or "program" or "translation unit" to hold the
//     // function
//     auto M = std::make_unique<llvm::Module>("test", Context);
// 
//     Function *F = cast<Function>(M->getOrInsertFunction("main", llvm::Type::getInt32Ty(Context), llvm::Type::getInt32Ty(Context), nullptr));
//     // Add a basic block to the function... again, it automatically inserts
//     // because of the last argument.
//     llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context, "EntryBlock", F);
// 
//     // Get pointers to the constant integers...
//     llvm::Value *Two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 2);
//     llvm::Value *Three = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 3);
// 
//     // Create the add instruction... does not insert...
//     llvm::Instruction *Add = llvm::BinaryOperator::Create(llvm::Instruction::Add, Two, Three, "addresult", BB);
// 
// 
//     // Create the return instruction and add it to the basic block
//     BB->getInstList().push_back(ReturnInst::Create(Context, Add));
// 
//     M->dump();
//     
//         
//     auto execution_engine = llvm::EngineBuilder(std::move(M)).create();
//     auto ret = execution_engine->runFunctionAsMain(F, std::vector<std::string>(), nullptr);

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    IRRenderer *renderer = new IRRenderer();

    STree *tree = new STree();

    std::string input;
    fprintf(stderr, "ready> ");
    while( std::getline(std::cin, input, ';') ) {
        std::istringstream iss(input);

        tree->parse(iss);
        if( tree->root != 0 ) {
            if( Function *func = static_cast<Function*>(tree->root->codegen(renderer)) ) {
                if( func->getName() == "" ) {
                    void *func_ptr = renderer->engine->getPointerToFunction(func);
                    double(*func_pointer)() = (double(*)())(intptr_t)func_ptr;
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
