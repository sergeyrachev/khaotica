#pragma once

#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "node.h"
#include "prototype.h"
#include "renderer.h"


class FunctionNode : public ASTNode {
  std::shared_ptr<PrototypeNode> proto;
  std::shared_ptr<ASTNode> body;

public:
  FunctionNode(std::shared_ptr<PrototypeNode> proto, std::shared_ptr<ASTNode> body);
  void codegen(IRRenderer& renderer);
};
