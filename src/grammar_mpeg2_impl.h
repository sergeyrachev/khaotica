#pragma once

#include "mpeg2_types.h"

namespace khaotica::syntax::mpeg2 {
    class impl_t {
    public:
        void add(const compound_t &compound) {
            auto node = std::make_shared<node_t>(node_t{compound});
            auto[slot, is_inserted] = document.global->definitions.emplace(compound.name, node);
            if (is_inserted) {
                document.sequence.push_back(node);
            } else {
                slot->second = node;
            }
        }

        std::shared_ptr<node_t> add(const reference_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const assignment_t &node) {
            return add_symbol(node.symbol, node);
        }

        std::shared_ptr<node_t> add(const bslbf_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const uimsbf_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const simsbf_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const vlclbf_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const tcimsbf_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const collection_t &node) {
            return {};
        }

        std::shared_ptr<node_t> add(const slot_t &node) {
            return {};
        }

        std::shared_ptr<node_t> add(const sparsed_t &node) {
            return document.global->definitions[node.name] = add_symbol(node.name, node);
        }

        std::shared_ptr<node_t> add(const auto &node) {
            return std::make_shared<node_t>(node_t{node});
        }

        void open() {
            scope = scope_t::open(scope);
        }

        void close() {
            scope = scope->close();
        }

        document_t document{{}, std::make_shared<scope_t>()};
    private:
        template<class T>
        std::shared_ptr<node_t> add_symbol(const std::string &name, const T &node) {
            return scope->definitions[name] = std::make_shared<node_t>(node_t{node});
        }

    private:
        std::shared_ptr<scope_t> scope{document.global};
    };
}
