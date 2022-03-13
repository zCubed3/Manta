#include "smf.hpp"

#include <fstream>

namespace Manta::Data::Meshes {
    const uint32_t SMF::SMF_IDENT = MAKE_32_IDENT("SMF#");

    SMF* SMF::LoadFromStream(std::istream& stream) {
        auto smf = new SMF();

        SMFHeader header {};
        stream.read(reinterpret_cast<char*>(&header), sizeof(SMFHeader));

        if (header.ident != SMF_IDENT)
            throw std::runtime_error("The file you're trying to load isn't an SMF file!");

        smf->name.resize(header.name_len);
        stream.read(smf->name.data(), header.name_len);

        uint32_t idx = 0;
        for (auto i = 0; i < header.indice_count; i++) {
            stream.read(reinterpret_cast<char*>(&idx), sizeof(uint32_t));
            smf->indices.emplace_back(idx);
        }

        SMFVertex vert{};
        for (auto v = 0; v < header.indice_count; v++) {
            stream.read(reinterpret_cast<char*>(&vert), sizeof(SMFVertex));
            smf->vertices.emplace_back(vert);
        }

        return smf;
    }

    SMF* SMF::LoadFromFile(const std::string& path) {
        std::ifstream file(path, std::ofstream::binary);
        auto smf = LoadFromStream(file);
        file.close();

        return smf;
    }

    void SMF::WriteToFile(const std::string &path) {
        std::ofstream file(path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

        SMFHeader header {};

        header.ident = SMF_IDENT;
        header.indice_count = indices.size();
        header.vertex_count = vertices.size();
        header.name_len = name.length();

        file.write(reinterpret_cast<char*>(&header), sizeof(SMFHeader));
        file << name;

        for (auto indice : indices) {
            file.write(reinterpret_cast<char*>(&indice), sizeof(uint32_t));
        }

        for (auto vertex : vertices) {
            file.write(reinterpret_cast<char*>(&vertex), sizeof(SMFVertex));
        }

        file.close();
    }
}