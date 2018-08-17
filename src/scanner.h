#pragma once

#include "khaotica/mpeg2_sax.h"

#include "type_conversion.h"

#include <iostream>
#include <iterator>
#include <iomanip>
#include <algorithm>

namespace khaotica {
    using namespace khaotica::syntax::mpeg2;
    using namespace khaotica::algorithm;

    class scanner_t : public sax_t{
        static const int32_t P = 10;
        static const int32_t S = 6;
        static const int32_t T = 40;
        static const int32_t V = 60;
        static const int32_t N = 40;
    public:

        scanner_t(){
            out << std::setw(P) << std::right << "Position";
            out << std::setw(S) << std::right << "Size";
            out << std::setw(T) << std::left << std::setfill(' ') << " Title";
            out << std::setw(V) << std::right << "Value";
            out << std::setw(N) << std::right << "Notes";

            out << std::endl;
        }

        virtual void open(const compound_t &node) final {
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << node.name << "(" << "){" << std::endl;
            indent++;
        }
        virtual void close(const compound_t &node) final {
            indent--;
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "}" << std::endl;
        }

        virtual void open(const if_t &node) final {
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "if" << "(" << "){" << std::endl;
            indent++;
        }
        virtual void close(const if_t &node) final {
            indent--;
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "}" << std::endl;
        }

        virtual void open(const for_t &node) final {
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "for" << "(" << "){" << std::endl;
            indent++;
        }
        virtual void close(const for_t &node) final {
            indent--;
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "}" << std::endl;
        }

        virtual void open(const while_t &node) final {
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "while" << "(" << "){" << std::endl;
            indent++;
        }
        virtual void close(const while_t &node) final {
            indent--;
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "}" << std::endl;
        }

        virtual void open(const do_t &node) final {
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "do" << "{" << std::endl;
            indent++;
        }
        virtual void close(const do_t &node) final {
            indent--;
            out << std::setw(P) << std::right << "-";
            out << std::setw(S) << std::right << "-";
            out << std::string(indent, ' ') << "} while()" << std::endl;
        }

        virtual void on(const bslbf_t& node, const bitstring_v& field, const uint64_t position, const uint64_t length) final {

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.value);
            out << std::setw(N) << std::right << khaotica::algorithm::to_hex(field.value);
            out << std::endl;

        }
        virtual void on(const uimsbf_t& node, const uimsbf_v& field, const uint64_t position, const uint64_t length) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const uilsbf_t& node, const uimsbf_v& field, const uint64_t position, const uint64_t length) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const simsbf_t& node, const simsbf_v& field, const uint64_t position, const uint64_t length) final{
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const tcimsbf_t& node, const tcimsbf_v& field, const uint64_t position, const uint64_t length) final{
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const vlclbf_t& node, const vlclbf_v& field, const uint64_t position, const uint64_t length) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.value);
            out << std::setw(N) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::endl;
        }

        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, bslbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;

            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, uimsbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, uilsbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, simsbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, vlclbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const collection_t& node, const collection_v& field, const uint64_t position, const uint64_t length, tcimsbf_tag){
            std::stringstream ss;
            std::for_each( field.value.cbegin(), field.value.cend(), [&ss](const auto& entry){
                ss << khaotica::algorithm::to_string(entry) << ";";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << ss.str();
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }

        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, bslbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, uimsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, uilsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, simsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, vlclbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const sparsed_v& field, const uint64_t position, const uint64_t length, tcimsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + "[" + std::to_string(node.range.front) + ".." + std::to_string(node.range.back) + "]";
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << khaotica::algorithm::to_string(field.value, field.mask);
            out << std::endl;
        }

        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, bslbf_tag) final{

            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, uimsbf_tag) final{
            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, uilsbf_tag) final{
            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, simsbf_tag) final{
            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, vlclbf_tag) final{
            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const slot_t& node, const slot_v& field, const uint64_t position, const uint64_t length, tcimsbf_tag) final{
            std::ostringstream ss;
            std::for_each(field.indices.begin(), field.indices.end(), [&ss](const auto & entry){
                ss << "[" << entry << "]";
            });

            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name + ss.str();
            out << std::setw(V) << std::right << khaotica::algorithm::to_string(field.field);
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }

        virtual void on(const expression_t&, const expression_v&node) final {
            out << "<expr>";
            out << std::endl;
        }

    private:
        size_t indent{1};
        std::ostream& out{std::cout};
    };
}
