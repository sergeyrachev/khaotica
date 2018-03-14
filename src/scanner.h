#pragma once

#include "khaotica/mpeg2_sax.h"

#include "type_conversion.h"

#include <iostream>
#include <iomanip>

namespace khaotica {
    using namespace khaotica::syntax::mpeg2;
    using namespace khaotica::algorithm;

    class scanner_t : public sax_t{
        static const int32_t P = 10;
        static const int32_t S = 6;
        static const int32_t T = 50;
        static const int32_t V = 70;
        static const int32_t N = 30;
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
            indent++;
        }
        virtual void close(const while_t &node) final {
            indent--;
        }

        virtual void open(const do_t &node) final {
            indent++;
        }
        virtual void close(const do_t &node) final {
            indent--;
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

        virtual void on(const simsbf_t& node, const simsbf_v& field, const uint64_t position, const uint64_t length){
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const tcimsbf_t& node, const tcimsbf_v& field, const uint64_t position, const uint64_t length){
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << node.name;
            out << std::setw(V) << std::right << "0x"+khaotica::algorithm::to_hex(field.value);
            out << std::setw(N) << std::right << field.value;
            out << std::endl;
        }

        virtual void on(const collection_t&, const collection_v& node, const uint64_t position, const uint64_t length) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << "<collection>";
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const std::pair<bitstring_v, sparsed_v>& field, const uint64_t position, const uint64_t length, bslbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << "<sparsed>";
            out << std::setw(V) << std::right << " ";
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const std::pair<bitstring_v, sparsed_v>& field, const uint64_t position, const uint64_t length, uimsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << "<sparsed>";
            out << std::setw(V) << std::right << " ";
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const std::pair<bitstring_v, sparsed_v>& field, const uint64_t position, const uint64_t length, simsbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << "<sparsed>";
            out << std::setw(V) << std::right << " ";
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }
        virtual void on(const sparsed_t& node, const std::pair<bitstring_v, sparsed_v>& field, const uint64_t position, const uint64_t length, vlclbf_tag) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << std::string(indent, ' ') << std::setfill(' ') << std::left << std::setw(T - indent) << "<sparsed>";
            out << std::setw(V) << std::right << " ";
            out << std::setw(N) << std::right << " ";
            out << std::endl;
        }

        virtual void on(const expression_t&, const expression_v&node, const uint64_t position, const uint64_t length) final {
            out << std::setw(P) << std::to_string(position / 8) + ":" + std::to_string(position % 8);
            out << std::setw(S) << length;
            out << "<expr>";
            out << std::endl;
        }

    private:
        size_t indent{1};
        std::ostream& out{std::cout};
    };
}
