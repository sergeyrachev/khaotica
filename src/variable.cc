// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "variable.h"

#include "logging.h"

#include <string>

VariableNode::VariableNode(const std::string &name, double length)
    : name(name), length(length){}

const std::string
VariableNode::getName() const {
  return name;
}

void VariableNode::codegen(IRRenderer& renderer) {
    uint32_t value = renderer.read_bits(length);
    logging::debug() << "Variable: " << name << " Length: " << length << " Value: " << value;
}
