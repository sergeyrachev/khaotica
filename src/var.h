#pragma once

#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "node.h"
#include "renderer.h"


class VarNode : public ASTNode {
  std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> > var_names;
    std::shared_ptr<ASTNode> body;

public:
  VarNode(const std::vector<std::pair<std::string, std::shared_ptr<ASTNode>> > &var_names,
          std::shared_ptr<ASTNode> body);
  virtual llvm::Value *codegen(IRRenderer& renderer) override final;
};
