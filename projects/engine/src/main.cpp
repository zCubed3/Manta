#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// Opengl
#include <GL/glew.h>

// Engine
#include "assets/mesh.hpp"
#include "assets/shader.hpp"

#include "rendering/renderer.hpp"
#include "rendering/viewport.hpp"

#include "world/timing.hpp"

#include "world/world.hpp"
#include "world/actor.hpp"
#include "world/behavior.hpp"

#include "world/behaviors/camera.hpp"

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

    SDL_Event sdl_event;
    bool keep_running = true;

    auto renderer = new Renderer();
    renderer->Initialize();

    Shader::CreateEngineShaders();

    Shader* shader = Shader::LoadFile("content/engine/shaders/debug.glsl");
    shader->Compile();

    //Mesh* mesh = Mesh::LoadFromFile("test.obj");
    Mesh* mesh = Mesh::LoadFromFile("test.bsm");

    auto world = new World();

    auto test_actor = new Actor("test");
    test_actor->meshes.emplace_back(mesh);
    test_actor->shaders.emplace_back(shader);

    world->AddActor(test_actor);

    int last_width = 0, last_height = 0;

    // Used to allow for an in-engine console!
    std::ostringstream engine_stream;
    BifurcatedStream engine_out(std::cout, engine_stream);

    // TODO: Differentiate between shipping and editor if we ever make an editor

    bool first_run = true;

    auto actor_camera = new Actor("camera");
    auto camera = actor_camera->AddBehavior<CameraBehavior>();

    world->AddActor(actor_camera);

    while (keep_running) {
        // Event polling
        bool resized = false;

        while (SDL_PollEvent(&sdl_event) != 0) {
            if (sdl_event.type == SDL_QUIT)
                keep_running = false;

            if (sdl_event.type == SDL_WINDOWEVENT) {
                if (sdl_event.window.event == SDL_WINDOWEVENT_RESIZED)
                    resized = true;
            }
        }

        if (first_run) {
            resized = true;
            first_run = false;
        }

        renderer->Update();
        Timing::UpdateTime();

        //
        // Camera
        //
        camera->viewport.fov = 60;
        camera->viewport.transform.position.z = 2;

        camera->viewport.width = renderer->width;
        camera->viewport.height = renderer->height;

        //
        // Test Actor
        //
        //test->position = glm::vec3(0.5f, 0, 0);
        test_actor->transform.euler += glm::vec3(Timing::delta_time, Timing::delta_time, Timing::delta_time) * 20.0f;

        world->Update();

        //
        // Drawing
        //
        if (resized)
            renderer->ClearScreen();

        for (auto target_viewport : Viewport::active_viewports) {
            Viewport::active_viewport = target_viewport;

            if (target_viewport)
                target_viewport->UpdateMatrices();
            else
                continue;

            glViewport(target_viewport->x, target_viewport->y,target_viewport->width, target_viewport->height);
            glScissor(target_viewport->x, target_viewport->y,target_viewport->width, target_viewport->height);

            glClearColor(target_viewport->clear_color.x, target_viewport->clear_color.y, target_viewport->clear_color.z, 1.0f);

            int clear = 0;

            if (target_viewport->clear)
                clear |= GL_COLOR_BUFFER_BIT;

            if (target_viewport->clear_depth)
                clear |= GL_DEPTH_BUFFER_BIT;

            glClear(clear);
            renderer->DrawWorld(world);
            //mesh->DrawNow(test->transform.local_to_world, test->transform.world_to_local_t, shader);
        }

        renderer->Present();
    }
}