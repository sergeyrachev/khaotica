#pragma once

#include <string>

#include "llvm/IR/Value.h"

#include "node.h"
#include "codegen/renderer.h"


class BinaryNode : public ASTNode {
  char op;
  std::shared_ptr<ASTNode> lhs, rhs;

public:
  BinaryNode(char op, std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs);
  virtual llvm::Value *codegen(IRRenderer& renderer) override final;
};
