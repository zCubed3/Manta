#ifndef MANTA_ENGINE_CONTEXT_HPP
#define MANTA_ENGINE_CONTEXT_HPP

#include <vector>

// Stores dynamically allocated parts of the engine inside a single class that is passed into modules for usage
namespace Manta {
    class Renderer;
    class Viewport;
    class Timing;
    class Shader;
    class ImGuiContext;

    namespace Console {
        class Console;
    }

    class EngineContext {
    public:
        std::vector<Viewport*> active_viewports;
        Viewport* active_viewport;

        Timing* timing;
        Renderer* renderer;

        Console::Console* console;

        ImGuiContext* imgui;

        // TODO: Move stupid data like this elsewhere
        Shader* error_shader;
    };
}

#endif
