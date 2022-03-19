#include "lighting.hpp"

#include <GL/glew.h>

#include <world/actor.hpp>
#include <world/behaviors/light.hpp>

// TODO: Don't rely entirely on OpenGL?
namespace Manta {
    void Lighting::CreateBuffer() {
        glGenBuffers(1, &handle);

        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferData(GL_UNIFORM_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        UpdateBuffer();
    }

    void Lighting::Update() {
        int actual_count = 0;
        for (auto light : lights) {
            if (!light->enabled || !light->owner->enabled)
                continue;

            data[actual_count].color_w_intensity = glm::vec4(light->color, light->intensity);
            data[actual_count].position_w_type = glm::vec4(light->owner->transform.position, light->light_type);
            actual_count++;
        }

        light_count = actual_count;
    }

    void Lighting::UpdateBuffer() {
        glBindBuffer(GL_UNIFORM_BUFFER, handle);

        uint32_t data_size = sizeof(LightData);
        for (int l = 0; l < MAX_LIGHT_COUNT; l++) {
            glBufferSubData(GL_UNIFORM_BUFFER, data_size * l, data_size, &data[l]);
        }

        glBufferSubData(GL_UNIFORM_BUFFER, data_size * MAX_LIGHT_COUNT, sizeof(float), &light_count);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}