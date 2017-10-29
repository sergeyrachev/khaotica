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

         void on(flavor::bslbf_t& node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = bitreader.read(node.length);


        };
         void on(flavor::uimsbf_t& node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));


        };
         void on(flavor::bitstring_t& node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = khaotica::algorithm::unpack(node.value);

        };
         void on(flavor::integer_t& node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = node.value;

        };
         void on(flavor::identifier_t& node, std::shared_ptr<flavor::node_t> knot) final {
            auto value = node.name;

        };
         void on(flavor::if_t& node, std::shared_ptr<flavor::node_t> knot) final {


        };
         void on(flavor::for_t& node, std::shared_ptr<flavor::node_t> knot) final {

        };
         void on(flavor::compound_t& node, std::shared_ptr<flavor::node_t> knot) final {
            node.body->process(*this);
        };
         void on(flavor::block_t& node, std::shared_ptr<flavor::node_t> knot) final {
            for (auto &&entry : node.entries) {
                entry->process(*this);
            }
        };
         void on(flavor::assignment_t& node, std::shared_ptr<flavor::node_t> knot) final {

        };
         void on(flavor::preincrement_t<std::plus<>>& node, std::shared_ptr<flavor::node_t> knot) final {

        };
         void on(flavor::preincrement_t<std::minus<>>& node, std::shared_ptr<flavor::node_t> knot) final {

        };
         void on(flavor::postincrement_t<std::plus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::postincrement_t<std::minus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::unary_expression_t<std::bit_not<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::unary_expression_t<std::minus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::unary_expression_t<std::logical_not<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::plus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::minus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::multiplies<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::divides<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::modulus<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::less<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::greater<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::less_equal<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::greater_equal<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::equal_to<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::not_equal_to<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::logical_and<>>&, std::shared_ptr<flavor::node_t> knot)  final {

        };
         void on(flavor::binary_expression_t<std::logical_or<>>&, std::shared_ptr<flavor::node_t> knot)  final {

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
        entry->process(parse);
    }
}
