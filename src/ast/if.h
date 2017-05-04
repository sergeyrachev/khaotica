#pragma once

#include "llvm/IR/Value.h"

#include "node.h"
#include "codegen/renderer.h"


class IfNode : public ASTNode {
    std::shared_ptr<ASTNode> condition, then, _else;

public:
  IfNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> then, std::shared_ptr<ASTNode> _else);
  virtual llvm::Value *codegen(IRRenderer& renderer) override final;
};
