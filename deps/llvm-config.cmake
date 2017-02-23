set(LLVM_ROOT "" CACHE PATH "")

find_package(LLVM 3.9.0 PATHS ${LLVM_ROOT})

if(LLVM_FOUND)
    #all works bad in Windows-> LTO-NOTFOUND 
    llvm_map_components_to_libnames(llvm_libs all)

    set(llvm_INCLUDE_DIRECTORIES ${LLVM_INCLUDE_DIRS})
    set(llvm_LIBRARIES ${llvm_libs})
    set(llvm_COMPILE_DEFINITIONS ${LLVM_DEFINITIONS} )
endif()
