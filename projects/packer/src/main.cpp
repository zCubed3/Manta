#include <iostream>

#include <data/engine_context.hpp>

#include <world/timing.hpp>
#include <world/world.hpp>
#include <world/actor.hpp>

#include <world/behaviors/camera.hpp>

#include <rendering/viewport.hpp>
#include <rendering/renderer.hpp>
#include <rendering/lighting.hpp>

#include <data/console/console.hpp>

#include <ui/imguicontext.hpp>

#include <input/input_server.hpp>

#include <GL/glew.h>

using namespace Manta;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    SDL_Event sdl_event;
    bool keep_running = true;

    // TODO: Renderer modularity?
    auto renderer = new Rendering::Renderer();
    renderer->Initialize();

    auto imgui = new Manta::ImGuiContext(renderer->sdl_window, renderer->sdl_context);

    auto input = new Input::InputServer();

    auto engine = new EngineContext();

    engine->renderer = renderer;
    engine->timing = new Timing();
    engine->console = new Console::Console();
    engine->imgui = imgui;
    engine->input = input;
    engine->lighting = new Rendering::Lighting();

    // The empty world is used for the blank scene!
    auto empty_world = new World();

    // By default we have one giant scene camera pointed at 0, 0, 0
    auto scene_camera_actor = new Actor("scene_camera");
    auto scene_camera = scene_camera_actor->AddBehavior<CameraBehavior>();

    empty_world->AddActor(scene_camera_actor);

    //
    // ImGui styling
    //
    imgui->style->WindowMenuButtonPosition = ImGuiDir_None;

    bool first_run = true;
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

            input->ProcessEvent(&sdl_event);
            imgui->Process(&sdl_event);
        }

        if (first_run) {
            resized = true;
            first_run = false;
        }

        renderer->Update();
        engine->timing->UpdateTime();

        input->UpdateBinds();

        // Empty world update
        scene_camera->width = renderer->width;
        scene_camera->height = renderer->height;

        empty_world->Update(engine);

        for (auto target_viewport : engine->active_viewports) {
            engine->active_viewport = target_viewport;

            if (!target_viewport)
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

            //mesh->DrawNow(test->transform.local_to_world, test->transform.world_to_local_t, shader);
        }

        //
        // Packer UI
        //
        glViewport(0, 0, renderer->width, renderer->height);
        glScissor(0, 0,renderer->width, renderer->height);

        renderer->BeginImGui();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(renderer->width, renderer->height));

        ImGui::Begin("Packer Operations");

        ImGui::End();

        renderer->EndImGui();


        renderer->Present();
    }

    return 0;
}