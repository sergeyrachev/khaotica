#pragma once

#include "renderer.h"

#include <iostream>
#include <memory>

class ASTNode {
public:
  virtual ~ASTNode() { };
  virtual void codegen(IRRenderer& renderer) = 0;
};
