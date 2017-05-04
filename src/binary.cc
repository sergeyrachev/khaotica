// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Value.h"

#include "binary.h"
#include "variable.h"
#include "renderer.h"
#include "errors.h"

#include "node.h"
#include "binary.h"

using ::llvm::Value;
using ::llvm::Type;

BinaryNode::BinaryNode(char op, std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
    : op(op), lhs(lhs), rhs(rhs) {}
    

llvm::Value * BinaryNode::codegen(IRRenderer& renderer) {
    if( op == '=' ) {
        const VariableNode *lhse = dynamic_cast<VariableNode*>(lhs.get());
        if( !lhse ) {
            return ErrorV("destination of '=' must be a variable");
        }

        Value *val = rhs->codegen(renderer);
        if( val == 0 ) {
            return 0;
        }

        Value *variable = renderer.get_named_value(lhse->getName());
        if( variable == 0 ) {
            return ErrorV("Unknown variable name");
        }

        renderer.builder->CreateStore(val, variable);

        return val;
    }

    Value *left = lhs->codegen(renderer);
    Value *right = rhs->codegen(renderer);

    if( left == 0 || right == 0 ) {
        return 0;
    }

    Type *llvm_double_type = Type::getDoubleTy(renderer.llvm_context());

    switch( op ) {
    case '+': return renderer.builder->CreateFAdd(left, right, "addtmp");
    case '-': return renderer.builder->CreateFSub(left, right, "subtmp");
    case '*': return renderer.builder->CreateFMul(left, right, "multmp");
    case '<':
        left = renderer.builder->CreateFCmpULT(left, right, "cmptmp");
        return renderer.builder->CreateUIToFP(left,
            llvm_double_type,
            "booltmp");
    case '>':
        right = renderer.builder->CreateFCmpULT(right, left, "cmptmp");
        return renderer.builder->CreateUIToFP(right,
            llvm_double_type,
            "booltmp");
    default: break;
    }

    return ErrorV("Unknown binary operator!");
}