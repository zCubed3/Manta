#ifndef MANTA_MESH_HPP
#define MANTA_MESH_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <string>

#include <data/meshes/obj.hpp>
#include <data/meshes/bsm.hpp>

namespace Manta {
    class Shader;
    class Viewport;
    class EngineContext;

    class Mesh {
    public:
        // We have 1 vertex type in the engine for now, later more will come with different data configs allowed inside them!
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv0;
            glm::vec4 tangent;

            bool operator==(const Vertex& v);
        };

        std::string name = "Unnamed";
        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;

        void CreateBuffers();
        void UpdateBuffers();

        static Mesh* LoadFromFile(const std::string &path);

        // Meshes can draw themselves, though it is preferred to use renderers!
        void DrawNow(const glm::mat4& transform, Shader* shader, EngineContext* engine);
        void DrawNow(const glm::mat4& transform, const glm::mat4& transform_i, Shader* shader, EngineContext* engine);

    protected:
        uint32_t vao;
        uint32_t vbo, ibo;

        void ReadOBJ(std::istream& source);
        void ReadBSM(std::istream& source);
    };
}

#endif
