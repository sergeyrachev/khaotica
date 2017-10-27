// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include <fstream>

#include <boost/mpl/for_each.hpp>

using namespace khaotica;

namespace {
    using namespace flavor;

    class parse_t : public flavor::traversal_t{
    public:
        explicit parse_t(bitreader_t& bitreader, const flavor::document_t &doc):bitreader(bitreader), doc(doc){

        }

         void on(flavor::bslbf_t& node) final {
            auto value = bitreader.read(node.length);
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::uimsbf_t& node) final {
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::bitstring_t& node) final {
            auto value = khaotica::algorithm::unpack(node.value);
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::integer_t& node) final {
            auto value = node.value;
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::identifier_t& node) final {
            auto value = node.name;
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::if_t& node) final {


        };
         void on(flavor::for_t& node) final {

        };
         void on(flavor::compound_t& node) final {
            auto value = node.body->process(*this);
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
        };
         void on(flavor::block_t& node) final {
            std::list<std::shared_ptr<node_t>> value;
            for (auto &&entry : node.entries) {
                value.push_back(entry->process(*this));
            }
            auto p = std::make_shared<value_t<decltype(node), decltype(value)>>(node, value);
            return p;
        };
         void on(flavor::assignment_t& node) final {

        };
         void on(flavor::preincrement_t<std::plus<>>& node) final {

        };
         void on(flavor::preincrement_t<std::minus<>>& node) final {

        };
         void on(flavor::postincrement_t<std::plus<>>& node) final {

        };
         void on(flavor::postincrement_t<std::minus<>>& node) final {

        };
         void on(flavor::unary_expression_t<std::bit_not<>>& node) final {

        };
         void on(flavor::unary_expression_t<std::minus<>>& node) final {

        };
         void on(flavor::unary_expression_t<std::logical_not<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::plus<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::minus<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::multiplies<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::divides<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::modulus<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::less<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::greater<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::less_equal<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::greater_equal<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::equal_to<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::not_equal_to<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::logical_and<>>& node) final {

        };
         void on(flavor::binary_expression_t<std::logical_or<>>& node) final {

        };

    private:
        bitreader_t& bitreader;
        const flavor::document_t &doc;
    };
}

void parser_t::parse(std::ifstream &in, const flavor::document_t &doc) {
    bitreader_t bitreader(in);

    parse_t parse(bitreader, doc);
    for(auto&& entry : doc.hierarchy){
        auto value = entry->process(parse);
    }
}
