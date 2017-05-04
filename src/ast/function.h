#pragma once

#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "node.h"
#include "prototype.h"
#include "codegen/renderer.h"


class FunctionNode : public ASTNode {
  std::shared_ptr<PrototypeNode> proto;
  std::shared_ptr<ASTNode> body;

public:
  FunctionNode(std::shared_ptr<PrototypeNode> proto, std::shared_ptr<ASTNode> body);
  llvm::Value *codegen(IRRenderer& renderer);
};
