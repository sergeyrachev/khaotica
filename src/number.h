#pragma once

#include "llvm/IR/Value.h"

#include <string>

#include "node.h"
#include "renderer.h"


class NumberNode : public ASTNode {
    double val;

public:
    NumberNode(double val);
    virtual void codegen(IRRenderer& renderer) override final;
};
