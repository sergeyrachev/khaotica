#pragma once

#include "grammar_mpeg2_types.h"
#include "logging.h"

#include "type_conversion.h"

namespace khaotica{
    namespace mpeg2{
        class sax_t {
        public:
            virtual ~sax_t() = default;

            virtual void open(const compound_t& node){
                logging::debug() << "Open Compound " << node.name;
            };
            virtual void close(const compound_t& node){
                logging::debug() << "Close Compound " << node.name;
            };

            virtual void open(const if_t& node){
                logging::debug() << "Open If ";
            };
            virtual void close(const if_t& node){
                logging::debug() << "Close If ";
            };

            virtual void open(const for_t& node){
                logging::debug() << "Open For ";
            };
            virtual void close(const for_t& node){
                logging::debug() << "Close For ";
            };

            virtual void open(const while_t& node){
                logging::debug() << "Open While ";
            };
            virtual void close(const while_t& node){
                logging::debug() << "Close While ";
            };

            virtual void open(const do_t& node){
                logging::debug() << "Open Do ";
            };
            virtual void close(const do_t& node){
                logging::debug() << "Close Do ";
            };

            virtual void on(const std::pair<bslbf_t, bitstring_v> & node){
                logging::debug() << "On Bslbf " << node.first.name << " 0b" << algorithm::to_string(node.second.value);
            };

            virtual void on(const std::pair<uimsbf_t, uimsbf_v> & node){
                logging::debug() << "On Uimsbf " << node.first.name  << " 0x" << algorithm::to_hex(node.second.value);
            };

            virtual void on(const std::pair<collection_t, collection_v> & node){
                logging::debug() << "On Collection ";
            };

            virtual void on(const std::pair<sparsed_t, std::pair<bitstring_v, sparsed_v>> & node){
                logging::debug() << "On Sparsed ";
            };

            virtual void on(const std::pair<expression_t, expression_v> & node){
                logging::debug() << "On Expression ";
            };
        };
    }
}
