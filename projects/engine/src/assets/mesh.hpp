#ifndef MANTA_MESH_HPP
#define MANTA_MESH_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <string>

#include <data/mmdl.hpp>

namespace Manta {
    class Shader;
    class Camera;

    class Mesh {
    public:
        // We have 1 vertex type in the engine for now, later more will come with different data configs allowed inside them!
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv0;

            bool operator==(const Vertex& v);
        };

        std::string name = "Unnamed";
        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;

        void CreateBuffers();
        void UpdateBuffers();

        static Mesh* LoadFromFile(const std::string &path);

        // Meshes can draw themselves, though it is preferred to use renderers!
        void DrawNow(const glm::mat4& transform, Camera* camera, Shader* shader);
        void DrawNow(const glm::mat4& transform, const glm::mat4& transform_it, Camera* camera, Shader* shader);

    protected:
        uint32_t vao;
        uint32_t vbo, ibo;

        void ReadObj(std::stringstream& source);
    };
}

#endif
