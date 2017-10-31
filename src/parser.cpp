// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parser.h"

#include <variant>

using namespace khaotica;

namespace {

//    typedef std::variant<int64_t, uint64_t, bool, std::vector<bool>> value_t;
//
//    struct conditional_t{
//        value_t operator()(const int64_t& left){
//            return !!left;
//        }
//        value_t operator()(const uint64_t& left){
//            return !!left;
//        }
//        value_t operator()(const bool& left){
//            return !!left;
//        }
//        value_t operator()(const std::vector<bool>& left){
//            assert(false && "WAT?!");
//            return false;
//        }
//    };
//    struct equality_t{
//
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left == right;
//        }
//
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left == right;
//        }
//
//        value_t operator()(const bool& left, const bool& right){
//            return left == right;
//        }
//
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            return left == right;
//        }
//
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left == static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            return left == static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) == right;
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            return static_cast<int64_t>(left) == right;
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            return static_cast<uint64_t>(left) == right;
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            return static_cast<int64_t>(left) == right;
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//
//
//    struct logicar_or_t{
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left || static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left || static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            return left || static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) || right;
//        }
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left || static_cast<int64_t>(right);
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            return static_cast<int64_t>(left) || right;
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            return static_cast<uint64_t>(left) || right;
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            return static_cast<int64_t>(left) || right;
//        }
//        value_t operator()(const bool& left, const bool& right){
//            return left || static_cast<int64_t>(right);
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//    struct less_t{
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left < static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left < static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) < right;
//        }
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left < right;
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//
//    struct greater_t{
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left > static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left > static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) > right;
//        }
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left > right;
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//
//    struct minus_t{
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left - static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left - static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            return left - static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) - right;
//        }
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left - right;
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            return left - static_cast<uint64_t>(right);
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            return static_cast<uint64_t>(left) - right;
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            return static_cast<int64_t>(left) - right;
//        }
//        value_t operator()(const bool& left, const bool& right){
//            return static_cast<int64_t>(left) - static_cast<int64_t>(right);
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//
//    struct plus_t{
//        value_t operator()(const int64_t& left, const int64_t& right){
//            return left + static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const uint64_t& right){
//            return left + static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const bool& right){
//            return left + static_cast<int64_t>(right);
//        }
//        value_t operator()(const int64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const uint64_t& left, const int64_t& right){
//            return static_cast<int64_t>(left) + right;
//        }
//        value_t operator()(const uint64_t& left, const uint64_t& right){
//            return left + right;
//        }
//        value_t operator()(const uint64_t& left, const bool& right){
//            return left + static_cast<uint64_t>(right);
//        }
//        value_t operator()(const uint64_t& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const bool& left, const uint64_t& right){
//            return static_cast<uint64_t>(left) + right;
//        }
//        value_t operator()(const bool& left, const int64_t& right){
//            return static_cast<int64_t>(left) + right;
//        }
//        value_t operator()(const bool& left, const bool& right){
//            return static_cast<int64_t>(left) + static_cast<int64_t>(right);
//        }
//        value_t operator()(const bool& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const int64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const bool& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const uint64_t& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//        value_t operator()(const std::vector<bool>& left, const std::vector<bool>& right){
//            assert(false && "WAT?!");
//            return {false};
//        }
//    };
//    using namespace flavor;

    class parse_t{
    public:
        explicit parse_t(bitreader_t &bitreader, const flavor::document_t &doc)
            : bitreader(bitreader), doc(doc) {
        }


        std::shared_ptr<flavor::value_t> on(std::shared_ptr<flavor::node_t> node){
            return std::visit(*this, node->payload);
        }

        std::shared_ptr<flavor::value_t> operator()(const  flavor::bslbf_t &node)  {
            auto value = bitreader.read(node.length);
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair( node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::uimsbf_t &node)  {
            auto value = khaotica::algorithm::to_ull_msbf(bitreader.read(node.length));
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair( node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::bitstring_t &node)  {
            auto value = khaotica::algorithm::unpack(node.value);
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair( node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::integer_t &node)  {
            auto value = node.value;
            return std::make_shared<flavor::value_t>(flavor::value_t{std::make_pair( node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::identifier_t &node)  {
//            auto it = definitions.find(node.name);
//            if( it == definitions.end()){
//                auto definition = doc.definitions.find(node.name);
//                if(definition != doc.definitions.end()){
//                    definition->second->process(*this);
//                }
//            }
//
//            it = definitions.find(node.name);
//            if(it == definitions.end()){
//                // TODO: Default value
//                definitions[node.name] = std::make_shared<value_t>(value_t{uint64_t{0}});
//            }
//            values[knot] = definitions[node.name];
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::reference_t &node)  {
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::if_t &node)  {
//            node.condition->process(*this);
//            auto condition = std::visit(conditional_t(), (*values[node.condition]));
//            values[node.condition] = std::make_shared<value_t>(condition);
//
//            if(std::get<bool>(condition)){
//                node._then->process(*this);
//            } else if(node._else){
//                (*node._else)->process(*this);
//            }
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::for_t &node)  {

//            if(node.initializer){
//                (*node.initializer)->process(*this);
//            }
//
//            value_t condition{true};
//            if(node.condition){
//                (*node.condition)->process(*this);
//                condition = std::visit(conditional_t(), (*values[*node.condition]));
//            }
//
//            while(std::get<bool>(condition)){
//                node.body->process(*this);
//
//                (*node.modifier)->process(*this);
//                if(node.condition){
//                    (*node.condition)->process(*this);
//                    condition = std::visit(conditional_t(), (*values[*node.condition]));
//                }
//            }
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::compound_t &node)  {

            auto& compound = std::get<flavor::compound_t>(doc.definitions.at(node.name)->payload);

            std::list<std::shared_ptr<flavor::value_t>> value;
            for (auto &&item : compound.body) {
                value.push_back(on(item));
            }
            return std::make_shared<flavor::value_t>(flavor::value_t{ std::make_pair(node, value)});
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::assignment_t &node)  {
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::preincrement_t&node)  {
            return nullptr;
        };


        std::shared_ptr<flavor::value_t> operator()(const  flavor::postincrement_t&node)  {
            return nullptr;
        };


        std::shared_ptr<flavor::value_t> operator()(const  flavor::unary_expression_t&node)  {
            return nullptr;
        };

        std::shared_ptr<flavor::value_t> operator()(const  flavor::binary_expression_t&node)  {
            return nullptr;
        };

    private:
        bitreader_t &bitreader;
        const flavor::document_t &doc;
    };
}

void parser_t::parse(bitreader_t &in, const flavor::document_t &doc) {
    parse_t parse(in, doc);
    flavor::snapshot_t snapshot;
    for (auto &&entry : doc.structure) {
        snapshot[entry] = parse.on(entry);
    }

    int i = 0;
}
