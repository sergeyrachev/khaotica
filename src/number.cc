// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "number.h"

NumberNode::NumberNode(double val) : val(val) {}

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"

#include "number.h"
#include "renderer.h"


void NumberNode::codegen(IRRenderer& renderer) {

}
