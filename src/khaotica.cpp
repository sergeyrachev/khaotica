#include <iostream>

#include <boost/program_options.hpp>
//#include <boost/uuid/uuid_io.hpp>
//#include <boost/make_shared.hpp>
// #include <boost/date_time.hpp>
// #include <boost/rational.hpp>
// #include <boost/limits.hpp>
// #include <boost/foreach.hpp>
// #include <boost/uuid/string_generator.hpp>
// #include <boost/thread/thread.hpp>
// #include <boost/thread/locks.hpp>
// #include <boost/algorithm/hex.hpp>
// #include <boost/chrono.hpp>
// 
// #include <clang/AST/ASTConsumer.h>
// #include <clang/AST/ASTContext.h>
// #include <clang/AST/DeclVisitor.h>
// #include <clang/Basic/Builtins.h>
// #include <clang/Basic/Diagnostic.h>
// #include <clang/Basic/FileManager.h>
// #include <clang/Basic/IdentifierTable.h>
// #include <clang/Basic/SourceManager.h>
// #include <clang/Basic/TargetInfo.h>
// #include <clang/Frontend/CompilerInstance.h>
// #include <clang/Frontend/FrontendActions.h>
// #include <clang/Frontend/TextDiagnosticBuffer.h>
// #include <clang/Lex/HeaderSearch.h>
// #include <clang/Lex/Preprocessor.h>
// #include <clang/Parse/ParseAST.h>
// #include <clang/Parse/Parser.h>
// #include <clang/Sema/Sema.h>
// 
// #include <llvm/IR/LLVMContext.h>
// #include <llvm/Support/Casting.h>
// #include <llvm/IR/DerivedTypes.h>
// #include <llvm/IR/LLVMContext.h>
// #include <llvm/IR/Module.h>
// #include <llvm/IR/IRBuilder.h>
// #include <llvm/ExecutionEngine/ExecutionEngine.h>
// #include <llvm/ExecutionEngine/GenericValue.h>
// #include <llvm/ExecutionEngine/MCJIT.h>
// #include <llvm/Support/TargetSelect.h>
// #include "llvm/Bitcode/ReaderWriter.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/Support/raw_ostream.h"

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
        ( "output-definition,o", po::value<std::string>(), "Flavor definition for output bitstream")
        ( "input-bitstream,I", po::value<std::string>( ), "Input bitstream" )
        ( "output-bitstream,O", po::value<std::string>(), "Output bitstream")
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

   
    return 0;

}