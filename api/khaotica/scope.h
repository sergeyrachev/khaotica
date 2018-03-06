#pragma once

#include <memory>
#include <list>

namespace khaotica::syntax {
    template<typename Collection>
    class scope_t {
    public:
        std::weak_ptr<scope_t> parent;
        std::list<std::shared_ptr<scope_t>> children;
        Collection definitions;

        static std::shared_ptr<scope_t> open(const std::shared_ptr<scope_t> &parent) {
            parent->children.push_back(std::make_shared<scope_t>(scope_t{parent, {}, {}}));
            return parent->children.back();
        }

        std::shared_ptr<scope_t> close() {
            return parent.lock();
        }
    };
}
