#pragma once

#include <string>
#include <vector>

#include "llvm/IR/Module.h"

#include "node.h"
#include "renderer.h"


class PrototypeNode : public ASTNode {
  std::string name;
  std::vector<std::string> args;

public:
  PrototypeNode(const std::string &name,
                const std::vector<std::string> &args);

  void codegen(IRRenderer& renderer);

};
