#pragma once

#include "llvm/IR/Value.h"

#include "node.h"
#include "prototype.h"
#include "function.h"


ASTNode *Error(const char *message);
PrototypeNode *ErrorP(const char *message);
FunctionNode *ErrorF(const char *message);
llvm::Value *ErrorV(const char *message);
