#include "syntax.h"

#include <stack>
#include <cassert>
#include <iostream>

namespace {
    class validation_t{
    public:
        explicit validation_t(const khaotica::definitions_t& definitions, std::ostream& out):out(out){
            scopes.push(definitions);
        }

        void on(std::shared_ptr<khaotica::node_t> node){

        }

        void on(khaotica::bslbf_t& node )  {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(khaotica::uimsbf_t& node )  {
            auto& scope = scopes.top();
            auto it = scope.find(node.name);
            assert( it == scope.end());
            scope[node.name] = {};
        };

        void on(khaotica::bitstring_t& node )  {

        };
        void on(khaotica::integer_t& node )  {

        };
        void on(khaotica::identifier_t& node )  {

        };

        void on(khaotica::if_t& node )  {

            on(node.condition);

            for (auto &&item : node._then) {
                on(item);
            }

            for (auto &&item : node._else) {
                on(item);
            }
        };
        void on(khaotica::for_t& node )  {

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
        void on(khaotica::compound_t& node )  {
            for (auto &&item : node.body) {
                on(item);
            }
        };

        void on(khaotica::assignment_t& node )  {

            on(node.expression);
        };

        void on(khaotica::preincrement_t& node )  {

        };

        void on(khaotica::postincrement_t& node )  {

        };

        void on(khaotica::unary_expression_t& node )  {

        };


        void on(khaotica::binary_expression_t& node )  {

        };

    private:
        std::ostream& out;
        std::stack<khaotica::definitions_t> scopes;
    };
}

bool khaotica::syntax_t::is_valid(const khaotica::document_t &doc, std::ostream &out) {

    validation_t valid(doc.definitions, out);
//
//    for(auto&& entry : doc.hierarchy){
//        entry->process(valid);
//    }

    return false;
}
