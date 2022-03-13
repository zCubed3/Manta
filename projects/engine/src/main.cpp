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
#include <glm/gtx/string_cast.hpp>

#include <data/bifurcated_ostream.hpp>

using namespace Manta;
using namespace Manta::Data::Meshes;

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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);

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

    // Used to allow for an in-engine console!
    std::ostringstream engine_stream;
    BifurcatedStream engine_out(std::cout, engine_stream);

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
        camera.fov = 60;
        camera.position.z = 2;

        camera.lookAt = true;
        camera.target = glm::vec3(0, 0, 0);

        int width, height;
        SDL_GetWindowSize(sdl_window, &width, &height);

        camera.aspect = (float)width / (float)height;

        camera.UpdateMatrices();

        //
        // Test Actor
        //
        //test->position = glm::vec3(0.5f, 0, 0);
        test->transform.euler += glm::vec3(Timing::delta_time, Timing::delta_time, Timing::delta_time) * 20.0f;
        test->Update();

        //
        // Drawing
        //
        if (width != last_width || height != last_height) {
            glViewport(0, 0, width, height);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh->DrawNow(test->transform.local_to_world, test->transform.world_to_local_t, &camera, shader);

        SDL_GL_SwapWindow(sdl_window);

        last_width = width;
        last_height = height;
    }

    SDL_DestroyWindow(sdl_window);
}