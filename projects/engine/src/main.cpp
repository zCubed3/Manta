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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);

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

    Actor* test2 = new Actor("test2");
    World::AddActor(test);

    int last_width = 0, last_height = 0;

    // SMF Testing
    auto smf = SMF();

    for (auto idx : mesh->indices)
        smf.indices.emplace_back(idx);

    for (auto vert : mesh->vertices) {
        SMF::SMFVertex smf_vert {};

        for (int p = 0; p < 3; p++)
            smf_vert.position[p] = vert.position[p];

        for (int u = 0; u < 2; u++)
            smf_vert.uv[u] = vert.uv0[u];

        float yaw = atan2f(vert.normal.x, vert.normal.z);
        float pitch = asinf(vert.normal.y);

        smf_vert.normal[0] = yaw;
        smf_vert.normal[1] = pitch;

        smf.vertices.emplace_back(smf_vert);
    }

    smf.name = mesh->name;
    smf.WriteToFile("test.smf");

    //SMF* smf_test = SMF::LoadFromFile("test.smf");
    auto mesh2 = Mesh::LoadFromFile("test.smf");

    // Test SMF compression
    float total = 0;
    for (int i = 0; i < mesh->vertices.size(); i++) {
        total += glm::distance(mesh->vertices[i].normal, mesh2->vertices[i].normal);
    }

    total /= mesh->vertices.size();
    std::cout << "About '" << total << "' total compression error!" << std::endl;

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
        test->position = glm::vec3(0.5f, 0, 0);
        test->euler += glm::vec3(Timing::delta_time, Timing::delta_time, Timing::delta_time) * 20.0f;
        test->Update();

        test2->position = glm::vec3(-0.5f, 0, 0);
        test2->euler += glm::vec3(Timing::delta_time, Timing::delta_time, Timing::delta_time) * 20.0f;
        test2->Update();

        //
        // Drawing
        //
        if (width != last_width || height != last_height) {
            glViewport(0, 0, width, height);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh->DrawNow(test->model_matrix, test->model_it_matrix, &camera, shader);
        mesh2->DrawNow(test2->model_matrix, test2->model_it_matrix, &camera, shader);

        SDL_GL_SwapWindow(sdl_window);

        last_width = width;
        last_height = height;
    }

    SDL_DestroyWindow(sdl_window);
}