message(STATUS "Set LLVM_ROOT to LLVM root folder")

find_package(LLVM 4.0 REQUIRED HINTS ${LLVM_ROOT})

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

