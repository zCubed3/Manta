#ifndef MANTA_COMMON_SMF_HPP
#define MANTA_COMMON_SMF_HPP

#include <cstdint>
#include <vector>
#include <string>

#include "../ident.hpp"

namespace Manta::Data::Meshes {
    // SMF, a binary mesh format similar to OBJ!
    // One trade off is a bit of normal precision since we bake it as atan2(), asin() coords instead of a direction!
    class SMF {
    public:
        static const uint32_t SMF_IDENT;

        struct SMFHeader {
            uint32_t ident; // "SMF#"
            uint32_t indice_count;
            uint32_t vertex_count;
            uint16_t name_len;
        };

        // Name follows after the header!
        // Indice block follows after name!

        struct SMFVertex {
            float position[3];
            float normal[2]; // Yaw Pitch compressed normals
            float uv[2];
        };

        std::vector<SMFVertex> vertices;
        std::vector<uint32_t> indices;
        std::string name;

        static SMF* LoadFromStream(std::istream& stream);
        static SMF* LoadFromFile(const std::string &path);

        void WriteToFile(const std::string& path);
    };
}

#endif
