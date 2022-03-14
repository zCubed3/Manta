#ifndef MANTA_RENDERER_HPP
#define MANTA_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Manta {
    class World;
    class Viewport;

    class Renderer {
    public:
        Viewport* active_viewport = nullptr;

        void Initialize();

        void Update();

        void ClearScreen();

        void DrawWorld(World* world);

        void Present();

        int width = 512, height = 512;

    protected:
        SDL_GLContext sdl_context = nullptr;
        SDL_Window* sdl_window = nullptr;
    };
}

#endif
