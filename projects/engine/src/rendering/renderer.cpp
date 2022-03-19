#include "renderer.hpp"

#include <GL/glew.h>

#include <stdexcept>

#include <world/world.hpp>
#include <world/actor.hpp>

#include "viewport.hpp"

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

namespace Manta {
    void Renderer::Initialize() {
        sdl_context = nullptr;
        sdl_window = SDL_CreateWindow(
                "Manta",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                1280,
                720,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        // TODO: MSAA config
        //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);

        sdl_context = SDL_GL_CreateContext(sdl_window);
        SDL_GL_SetSwapInterval(1);

        glewExperimental = true;
        if (glewInit() != 0) {
            throw std::runtime_error("GLEW failed to initialize!");
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_SCISSOR_TEST);
    }

    void Renderer::Update() {
        SDL_GetWindowSize(sdl_window, &width, &height);
    }

    void Renderer::DrawWorld(World *world, EngineContext* engine) {
        if (world == nullptr)
            throw std::runtime_error("world was nullptr!");

        for (const auto& actor : world->actors) {
            if (actor == nullptr)
                continue;

            actor->Draw(engine);
        }
    }

    void Renderer::BeginImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(sdl_window);
        ImGui::NewFrame();
    }

    void Renderer::EndImGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Renderer::ClearScreen() {
        glViewport(0, 0, width, height);
        glScissor(0, 0, width, height);
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Present() {
        SDL_GL_SwapWindow(sdl_window);
    }
}