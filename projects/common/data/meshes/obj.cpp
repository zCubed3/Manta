#include "obj.hpp"

#include <vector>
#include <memory>

namespace Manta::Data::Meshes {
    bool WavefrontOBJ::OBJVertex::operator==(const OBJVertex &rhs) {
        return  position[0] == rhs.position[0] &&
                position[1] == rhs.position[1] &&
                position[2] == rhs.position[2] &&
                normal[0] == rhs.normal[0] &&
                normal[1] == rhs.normal[1] &&
                normal[2] == rhs.normal[2] &&
                uv[0] == rhs.uv[0] &&
                uv[1] == rhs.uv[1];
    }

    WavefrontOBJ* WavefrontOBJ::LoadFromStream(std::istream& stream) {
        auto obj = new WavefrontOBJ();

        std::vector<std::shared_ptr<float[]>> positions;
        std::vector<std::shared_ptr<float[]>> normals;
        std::vector<std::shared_ptr<float[]>> uvs;
        std::vector<std::shared_ptr<uint32_t[]>> triangles;

        std::string line;
        while (std::getline(stream, line)) {
            // First two characters are a data id
            std::string id = line.substr(0, 2);

            if (id[0] == '#')
                continue;

            std::string contents = line.substr(2);

            if (id[0] == 'o')
                obj->name = contents;

            if (id[0] == 'v') {
                if (id[1] == 't') {
                    float* uv = new float[2];
                    sscanf(contents.c_str(), "%f %f", &uv[0], &uv[1]);
                    uvs.push_back(std::shared_ptr<float[]>(uv));
                    continue;
                }

                float* v3data = new float[3];
                sscanf(contents.c_str(), "%f %f %f", &v3data[0], &v3data[1], &v3data[2]);

                if (id[1] == 'n')
                    normals.push_back(std::shared_ptr<float[]>(v3data));
                else
                    positions.push_back(std::shared_ptr<float[]>(v3data));
            }

            // F = face, we either iterate 3 times to make a tri, or once to make it a vert, depends on the obj
            // Ugly method of loading faces but idc enough to fix it
            if (id[0] == 'f') {
                std::string face = contents;

                size_t pos;
                while (true) {
                    uint32_t* tri = new uint32_t[3];
                    pos = face.find(' ');

                    sscanf(face.c_str(), "%i/%i/%i",
                           &tri[0], &tri[1], &tri[2]
                    );

                    face.erase(0, pos + 1);

                    for (int i = 0; i < 3; i++)
                        tri[i] -= 1;

                    triangles.push_back(std::shared_ptr<uint32_t[]>(tri));

                    if (pos == std::string::npos)
                        break;
                }
            }
        }

        for (const auto& tri : triangles) {
            OBJVertex vert {};

            for (int p = 0; p < 3; p++)
                vert.position[p] = positions[tri[0]][p];

            for (int u = 0; u < 2; u++)
                vert.uv[u] = uvs[tri[1]][u];

            for (int n = 0; n < 3; n++)
                vert.normal[n] = normals[tri[2]][n];

            bool similarVert = false;
            for (int v = 0; v < obj->vertices.size(); v++)
                if (obj->vertices[v] == vert) {
                    obj->indices.emplace_back(v);
                    similarVert = true;
                    break;
                }

            if (!similarVert) {
                obj->indices.emplace_back(obj->vertices.size());
                obj->vertices.emplace_back(vert);
            }
        }

        return obj;
    }
}