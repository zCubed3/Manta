#ifndef SILICA_MESH_HPP
#define SILICA_MESH_HPP

#include <types/vector2.hpp>
#include <types/vector3.hpp>
#include <types/matrix4x4.hpp>

#include <vector>

namespace Silica {
    class Material;

    class Mesh {
        // We have 1 vertex type in the engine for now, later more will come with different data configs allowed inside them!
        struct Vertex {
            Vector3 position;
            Vector3 normal;
            Vector2 uv0;
        };

        std::vector<uint32_t> indices;
        std::vector<Vertex> vertices;

        // Meshes can draw themselves, though it is preferred to use renderers!
        void DrawNow(const Matrix4x4& transform, Material* material);
    };
}

#endif
