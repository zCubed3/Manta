#ifndef MANTA_COMMON_OBJ_HPP
#define MANTA_COMMON_OBJ_HPP

#include <iostream>
#include <string>
#include <vector>

namespace Manta::Data::Meshes {
    class WavefrontOBJ {
    public:
        struct OBJVertex {
            float position[3];
            float normal[3];
            float uv[2];

            bool operator==(const OBJVertex& rhs);
        };

        std::string name;
        std::vector<OBJVertex> vertices;
        std::vector<uint32_t> indices;

        static WavefrontOBJ* LoadFromStream(std::istream& stream);
    };
}

#endif
