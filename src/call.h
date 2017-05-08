#pragma once

#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "node.h"
#include "renderer.h"


class CallNode : public ASTNode {
  std::string callee;
  std::vector<std::shared_ptr<ASTNode>> args;

public:
  CallNode(const std::string &callee, std::vector<std::shared_ptr<ASTNode>> &args);
  virtual llvm::Value *codegen(IRRenderer& renderer) override final;
};