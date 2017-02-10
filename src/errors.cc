// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstdio>

#include "llvm/IR/Value.h"

#include "errors.h"

ASTNode *Error(const char *message) {
    fprintf(stderr, "Error, %s\n", message);
    return 0;
}
PrototypeNode *ErrorP(const char *message) {
    Error(message);
    return 0;
}
FunctionNode *ErrorF(const char *message) {
    Error(message);
    return 0;
}
llvm::Value *ErrorV(const char *message) {
    Error(message);
    return 0;
}
