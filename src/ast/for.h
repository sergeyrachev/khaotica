#pragma once

#include <string>

#include "llvm/IR/Value.h"

#include "node.h"
#include "codegen/renderer.h"


class ForNode: public ASTNode {
  std::string var_name;
  std::shared_ptr<ASTNode>start, end, step, body;

public:
  ForNode(const std::string &var_name,
          std::shared_ptr<ASTNode> start, std::shared_ptr<ASTNode> end, std::shared_ptr<ASTNode> step,
          std::shared_ptr<ASTNode> body);
  virtual llvm::Value *codegen(IRRenderer& renderer) override final;
};
