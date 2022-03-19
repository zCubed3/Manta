#include "lightbuffer.hpp"

#include <GL/glew.h>

// TODO: Don't rely entirely on OpenGL?
namespace Manta {
    void LightBuffer::CreateBuffer() {
        glGenBuffers(1, &handle);

        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferData(GL_UNIFORM_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        UpdateBuffer();
    }

    void LightBuffer::UpdateBuffer() {
        glBindBuffer(GL_UNIFORM_BUFFER, handle);

        uint32_t data_size = sizeof(LightData);
        for (int l = 0; l < MAX_LIGHT_COUNT; l++) {
            glBufferSubData(GL_UNIFORM_BUFFER, data_size * l, data_size, &data[l]);
        }

        glBufferSubData(GL_UNIFORM_BUFFER, data_size * MAX_LIGHT_COUNT, sizeof(float), &light_count);

        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}