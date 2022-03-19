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
#include "rendering/lighting.hpp"
#include "rendering/viewport.hpp"

#include "world/timing.hpp"
#include "world/world.hpp"
#include "world/actor.hpp"
#include "world/behavior.hpp"
#include "world/behaviors/camera.hpp"

#include "modularity/dynlib.hpp"
#include "modularity/gamemodule.hpp"

#include "data/console/console.hpp"
#include "data/console/cvar.hpp"
#include "data/console/cfunc.hpp"
#include "data/console/stdelems.hpp"

#include "data/engine_context.hpp"

#include "input/inputserver.hpp"

#include "ui/imguicontext.hpp"

// Misc
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Manta;
using namespace Manta::Data::Meshes;

typedef GameModule*(*module_init_fptr)();

// TODO: Differentiate between shipping and editor if we ever make an editor
int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    SDL_Event sdl_event;
    bool keep_running = true;

    // TODO: Renderer modularity?
    auto renderer = new Renderer();
    renderer->Initialize();

    auto dlib_game = DynLib::Open("./lib/game.so");

    auto module_init = dlib_game->GetFunction<module_init_fptr>("module_init");
    GameModule* game_module = module_init();

    auto imgui = new Manta::ImGuiContext(renderer->sdl_window, renderer->sdl_context);

    auto input = new Input::InputServer();

    auto engine = new EngineContext();

    engine->renderer = renderer;
    engine->timing = new Timing();
    engine->console = new Console::Console();
    engine->imgui = imgui;
    engine->input = input;
    engine->lighting = new Lighting();

    engine->lighting->CreateBuffer();

    Shader::CreateEngineShaders(engine);

    game_module->Initialize(engine);

    std::vector<std::string> safe_argv;

    for (int c = 1; c < argc; c++)
        safe_argv.emplace_back(argv[c]);

    Console::RegisterCommonCElems(engine->console);
    engine->console->DoCommandLine(safe_argv);

    safe_argv.clear();

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
            //imgui->Process(&sdl_event);
        }

        if (first_run) {
            resized = true;
            first_run = false;
        }

        renderer->Update();
        engine->timing->UpdateTime();

        input->UpdateBinds();

        game_module->Update(engine);

        //
        // Drawing
        //
        if (resized)
            renderer->ClearScreen();

        // TODO: Dynamic lighting
        engine->lighting->UpdateBuffer();

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

            // TODO: More generic draw loop
            game_module->Draw(engine);

            //mesh->DrawNow(test->transform.local_to_world, test->transform.world_to_local_t, shader);
        }

        // We have a special viewport change for the UI itself
        glViewport(0, 0, renderer->width, renderer->height);
        glScissor(0, 0,renderer->width, renderer->height);

        renderer->BeginImGui();

        game_module->DrawGUI(engine);

        renderer->EndImGui();


        renderer->Present();
    }

    // Clean up
    delete renderer;
    delete dlib_game;
}