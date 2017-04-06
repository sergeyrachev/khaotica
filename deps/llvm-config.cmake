if(NOT LLVM_ROOT)
message(FATAL_ERROR "Set LLVM_ROOT to LLVM root folder")
endif()
find_package(LLVM 3.9 PATHS ${LLVM_ROOT})

if(LLVM_FOUND)
    #all works bad in Windows-> LTO-NOTFOUND 
    llvm_map_components_to_libnames(llvm_libs 
	analysis
	asmparser
	bitreader
	bitwriter
	codegen
	DebugInfoCodeView 
	DebugInfoDWARF 
	DebugInfoPDB
	Symbolize 
	executionengine
	irreader
	Interpreter 
	OrcJIT
	MCJIT 
	RuntimeDyld 
	Core  
	LibDriver 
	LineEditor 
	Linker
	MC 
	MCDisassembler 
	MCParser 
	Object 
	ObjectYAML 
	Option 
	Passes
	ProfileData  
	Support  
	TableGen 
	X86
	XCore 
)

    set(llvm_INCLUDE_DIRECTORIES ${LLVM_INCLUDE_DIRS})
    set(llvm_LIBRARIES ${llvm_libs})
    set(llvm_COMPILE_DEFINITIONS ${LLVM_DEFINITIONS} )
endif()
