#pragma once

#include <string>

#include "llvm/IR/Value.h"

#include "node.h"
#include "renderer.h"


class VariableNode : public ASTNode {
  std::string name;
  double length;

public:
  VariableNode(const std::string &name, double length);
  const std::string getName() const;
  virtual void codegen(IRRenderer& renderer) override final;
};
