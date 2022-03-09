#ifndef SILICA_MATERIAL_HPP
#define SILICA_MATERIAL_HPP

namespace Silica {
    class Shader;

    class Material {
    public:
        explicit Material(Shader* shader);

        void Use();

        Shader* shader;
        // TODO: Variants

        static Material* error_material;
    };
}

#endif
