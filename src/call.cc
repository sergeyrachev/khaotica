// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "call.h"

#include <string>
#include <vector>

CallNode::CallNode(const std::string &callee) : callee(callee) {

}

void CallNode::codegen(IRRenderer& renderer) {
 
}
