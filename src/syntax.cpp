#include "syntax.h"

#include <stack>
#include <cassert>
#include <iostream>

namespace {
    class validation_t : public flavor::traversal_t{
    public:
        explicit validation_t(const flavor::definitions_t& definitions, std::ostream& out):out(out){
            scopes.push(definitions);
        }

        void on(flavor::bslbf_t& node ) final {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(flavor::uimsbf_t& node ) final {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(flavor::bitstring_t& node ) final {

        };
        void on(flavor::integer_t& node ) final {

        };
        void on(flavor::identifier_t& node ) final {
            on_(node.name);
        };

        void on(flavor::if_t& node ) final {
            node.condition->process(*this);

            node._then->process(*this);

            if(node._else){
                (*node._else)->process(*this);
            }
        };
        void on(flavor::for_t& node ) final {

            scopes.push(scopes.top());

            if(node.initializer){
                (*node.initializer)->process(*this);
            }
            if(node.condition){
                (*node.condition)->process(*this);
            }
            if(node.modifier){
                (*node.modifier)->process(*this);
            }

            node.body->process(*this);

            scopes.pop();
        };
        void on(flavor::compound_t& node ) final {

            if(!node.body ){
                auto& scope = scopes.top();
                auto it = scope.find(node.name);
                if( it != scope.end()){
                    node.body = it->second;
                }
            }

            if(node.body){
                node.body->process(*this);
            } else {
                not_defined.push_back(node.name);
            }
        };

        void on(flavor::block_t& node) final{
            scopes.push(scopes.top());

            for (auto &&entry : node.entries) {
                entry->process(*this);
            }

            scopes.pop();
        }

        void on(flavor::assignment_t& node ) final {
            node.expression->process(*this);
            auto& scope = scopes.top();
            scope[node.symbol] = {};
        };

        void on(flavor::preincrement_t<std::plus<>>& node ) final {
            on_(node.operand);
        };
        void on(flavor::preincrement_t<std::minus<>>& node ) final {
            on_(node.operand);
        };
        void on(flavor::postincrement_t<std::plus<>>& node ) final {
            on_(node.operand);
        };
        void on(flavor::postincrement_t<std::minus<>>& node ) final {
            on_(node.operand);
        };
        void on(flavor::unary_expression_t<std::bit_not<>>& node ) final {
            node.operand->process(*this);
        };
        void on(flavor::unary_expression_t<std::minus<>>& node ) final {
            node.operand->process(*this);
        };
        void on(flavor::unary_expression_t<std::logical_not<>>& node ) final {
            node.operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::plus<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::minus<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::multiplies<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::divides<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::modulus<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::less<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::greater<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::less_equal<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::greater_equal<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::equal_to<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::not_equal_to<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::logical_and<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };
        void on(flavor::binary_expression_t<std::logical_or<>>& node ) final {
            node.left_operand->process(*this);
            node.right_operand->process(*this);
        };

        std::list<std::string> not_defined;
    private:
        void on_(const std::string& name){
            const auto& scope = scopes.top();
            auto it = scope.find(name);
            if(  it == scope.end() ){
                not_defined.push_back(name);
            }
        }

    private:
        std::ostream& out;
        std::stack<flavor::definitions_t> scopes;
    };
}

bool khaotica::syntax_t::is_valid(const flavor::document_t &doc, std::ostream &out) {

    validation_t valid(doc.definitions, out);

    for(auto&& entry : doc.hierarchy){
        entry->process(valid);
    }

    return !valid.not_defined.empty();
}
