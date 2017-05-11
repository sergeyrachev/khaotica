// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string>

#include "variable.h"

#include "logging.h"

VariableNode::VariableNode(const std::string &name, double length)
    : name(name), length(length){}

const std::string
VariableNode::getName() const {
  return name;
}

#include "llvm/IR/Value.h"

#include "variable.h"
#include "renderer.h"
#include "errors.h"


void VariableNode::codegen(IRRenderer& renderer) {
    uint32_t value = renderer.read_bits(length);
    logging::debug() << "Variable: " << name << " Length: " << length << " Value: " << value;
}
