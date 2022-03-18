#ifndef MANTA_ENGINE_CONTEXT_HPP
#define MANTA_ENGINE_CONTEXT_HPP

// Stores dynamically allocated parts of the engine inside a single class that is passed into modules for usage
namespace Manta {
    class Renderer;
    class Viewport;
    class Timing;

    class EngineContext {
    public:
        std::vector<Viewport*> active_viewports;
        Viewport* active_viewport;

        Timing* timing;
        Renderer* renderer;
    };
}

#endif
