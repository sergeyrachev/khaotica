#pragma once

#include "node.h"
#include "renderer.h"

#include <string>
#include <vector>

class CallNode : public ASTNode {
  std::string callee;
  std::vector<std::shared_ptr<ASTNode>> args;

public:
  CallNode(const std::string &callee);
  virtual void codegen(IRRenderer& renderer) override final;
};
