#ifndef SILICA_ACTOR_HPP
#define SILICA_ACTOR_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Silica {
    class Actor {
    public:
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 euler = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);

        glm::mat4 model_matrix;
        glm::mat4 model_i_matrix;
        glm::mat4 model_it_matrix;

        void Update();
        void Draw(); // Updates anything that implements draw, usually renderers

    protected:
        void UpdateMatrices();
    };
}

#endif
