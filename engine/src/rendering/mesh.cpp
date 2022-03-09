#include "mesh.hpp"

#include "shader.hpp"
#include "material.hpp"

namespace Silica {
    void Mesh::DrawNow(const Matrix4x4 &transform, Material *material) {
        auto mat = material;
        if (mat == nullptr)
            mat = Material::error_material;

    }
}
