#include "material.hpp"

namespace Silica {
    Material::Material(Shader *shader) {
        // TODO: Material safety
        this->shader = shader;
    }

    void Material::Use() {

    }

    Material* Material::error_material = nullptr;
}
