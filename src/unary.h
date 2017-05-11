#pragma once

#include "llvm/IR/Value.h"

#include "node.h"
#include "renderer.h"


class UnaryNode : public ASTNode {
  char opcode;
  ASTNode *operand;

public:
  UnaryNode(char opcode, ASTNode *operand);
  virtual void codegen(IRRenderer& renderer) override final;
};
