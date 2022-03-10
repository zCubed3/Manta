#ifndef SILICA_MESH_HPP
#define SILICA_MESH_HPP

#include <types/vector2.hpp>
#include <types/vector3.hpp>
#include <types/matrix4x4.hpp>

#include <vector>
#include <string>

namespace Silica {
    class Shader;
    class Camera;

    class Mesh {
    public:
        // We have 1 vertex type in the engine for now, later more will come with different data configs allowed inside them!
        struct Vertex {
            Vector3 position;
            Vector3 normal;
            Vector2 uv0;

            bool operator==(const Vertex& v);
        };

        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;

        void CreateBuffers();
        void UpdateBuffers();

        static Mesh * LoadFromFile(const std::string &path);

        // Meshes can draw themselves, though it is preferred to use renderers!
        void DrawNow(const Matrix4x4& transform, Camera* camera, Shader* shader);

    protected:
        uint32_t vao;
        uint32_t vbo, ibo;

        void ReadObj(const std::string& source);


    };
}

#endif
