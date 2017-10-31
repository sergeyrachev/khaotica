#include "syntax.h"

#include <stack>
#include <cassert>
#include <iostream>

namespace {
    class validation_t{
    public:
        explicit validation_t(const flavor::definitions_t& definitions, std::ostream& out):out(out){
            scopes.push(definitions);
        }

        void on(std::shared_ptr<flavor::node_t> node){

        }

        void on(flavor::bslbf_t& node )  {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(flavor::uimsbf_t& node )  {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(flavor::bitstring_t& node )  {

        };
        void on(flavor::integer_t& node )  {

        };
        void on(flavor::identifier_t& node )  {

        };

        void on(flavor::if_t& node )  {

            on(node.condition);

            for (auto &&item : node._then) {
                on(item);
            }

            for (auto &&item : node._else) {
                on(item);
            }
        };
        void on(flavor::for_t& node )  {

            scopes.push(scopes.top());

            if(node.initializer){
                on(*node.initializer);
            }
            if(node.condition){
                on(*node.condition);
            }
            if(node.modifier){
                on(*node.modifier);
            }

            for (auto &&item : node.body) {
                on(item);
            }

            scopes.pop();
        };
        void on(flavor::compound_t& node )  {
            for (auto &&item : node.body) {
                on(item);
            }
        };

        void on(flavor::assignment_t& node )  {

            on(node.expression);
        };

        void on(flavor::preincrement_t& node )  {

        };

        void on(flavor::postincrement_t& node )  {

        };

        void on(flavor::unary_expression_t& node )  {

        };


        void on(flavor::binary_expression_t& node )  {

        };

    private:
        std::ostream& out;
        std::stack<flavor::definitions_t> scopes;
    };
}

bool khaotica::syntax_t::is_valid(const flavor::document_t &doc, std::ostream &out) {

    validation_t valid(doc.definitions, out);
//
//    for(auto&& entry : doc.hierarchy){
//        entry->process(valid);
//    }

    return false;
}
