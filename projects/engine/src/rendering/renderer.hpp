#ifndef MANTA_RENDERER_HPP
#define MANTA_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Manta {
    class World;

    class Renderer {
    public:
        void Initialize();

        void Update();

        void ClearScreen();
        void DrawWorld(World* world);

        void Present();

        int width, height;

    protected:
        SDL_GLContext sdl_context = nullptr;
        SDL_Window* sdl_window = nullptr;
    };
}

#endif
