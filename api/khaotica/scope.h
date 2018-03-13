#pragma once

#include <memory>
#include <list>
#include <map>

namespace khaotica::syntax {
    template<typename Key, typename Value>
    class scope_tt {
    public:
        std::weak_ptr<scope_tt> parent{};
        std::list<std::shared_ptr<scope_tt>> children{};
        std::map<Key, Value> definitions{};

        static std::shared_ptr<scope_tt> open(const std::shared_ptr<scope_tt> &parent) {
            parent->children.push_back(std::make_shared<scope_tt>(scope_tt{parent, {}, {}}));
            return parent->children.back();
        }

        std::shared_ptr<scope_tt> close() {
            return parent.lock();
        }

        Value lookup(const std::string& name) {

            auto it = definitions.find(name);
            if(it != definitions.end()){
                return it->second;
            }

            if(!parent.expired()){
                return parent.lock()->lookup(name);
            }

            return {};
        }
    };
}
