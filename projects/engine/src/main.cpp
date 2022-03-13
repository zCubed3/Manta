#include <SDL2/SDL.h>

#include <string>
#include <iostream>
#include <sstream>

// Opengl
#include <GL/glew.h>

// Silica
#include <assets/mesh.hpp>

#include "rendering/shader.hpp"
#include "rendering/camera.hpp"

#include "world/timing.hpp"

#include "world/world.hpp"
#include "world/actor.hpp"
#include "world/behavior.hpp"

// Misc
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Manta;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    SDL_GLContext sdl_context = nullptr;
    SDL_Window* sdl_window = SDL_CreateWindow(
            "Manta",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280,
            720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    sdl_context = SDL_GL_CreateContext(sdl_window);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = true;
    if (glewInit() != 0) {
        throw std::runtime_error("GLEW failed to initialize!");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SDL_Event sdl_event;
    bool keep_running = true;

    Shader::CreateEngineShaders();

    Shader* shader = Shader::LoadFile("content/engine/shaders/lit.glsl");
    shader->Compile();

    Mesh* mesh = Mesh::LoadFromFile("test.obj");

    Camera camera;

    Actor* test = new Actor("test");
    World::AddActor(test);

    int last_width = 0, last_height = 0;

    // MMDL Testing
    auto desc = MMDL::DescriptorV1({
        MMDL::DescriptorV1::ChannelType::FLOAT3,
        MMDL::DescriptorV1::ChannelType::FLOAT3,
        MMDL::DescriptorV1::ChannelType::FLOAT2
    },
    {
        MMDL::DescriptorV1::ChannelSemantic::VERTEX,
        MMDL::DescriptorV1::ChannelSemantic::NORMAL,
        MMDL::DescriptorV1::ChannelSemantic::UV0,
    });

    while (keep_running) {
        // Event polling
        while (SDL_PollEvent(&sdl_event) != 0) {
            if (sdl_event.type == SDL_QUIT)
                keep_running = false;
        }

        Timing::UpdateTime();

        //
        // Camera
        //
        camera.position.z = 3;

        camera.lookAt = true;
        camera.target = glm::vec3(0, 0, 0);

        int width, height;
        SDL_GetWindowSize(sdl_window, &width, &height);

        camera.aspect = (float)width / (float)height;

        camera.UpdateMatrices();

        //
        // Test Actor
        //
        test->euler += glm::vec3(Timing::delta_time, Timing::delta_time, Timing::delta_time) * 20.0f;

        test->Update();

        //
        // Drawing
        //
        if (width != last_width || height != last_height) {
            glViewport(0, 0, width, height);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh->DrawNow(test->model_matrix, test->model_it_matrix, &camera, shader);

        SDL_GL_SwapWindow(sdl_window);

        last_width = width;
        last_height = height;
    }

    SDL_DestroyWindow(sdl_window);
}