#ifndef KHAOTICA_INTERPRETER_H
#define KHAOTICA_INTERPRETER_H

#include <istream>
#include <memory>
#include <vector>

class ASTNode;
namespace flavor{
    class Interpreter {
    public:
        Interpreter();
        void parse(std::istream &in, std::vector<std::shared_ptr<ASTNode>>& asts) const;
    };
}

#endif //KHAOTICA_INTERPRETER_H
