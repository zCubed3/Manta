#ifndef MANTA_RENDERER_HPP
#define MANTA_RENDERER_HPP

#include <SDL2/SDL.h>

namespace Manta {
    class World;
    class EngineContext;
}

namespace Manta::Rendering {
    class Viewport;
    class Lighting;
    class RenderTarget;

    class Renderer {
    public:
        Viewport* active_viewport = nullptr;

        void Initialize();

        void Update();

        void ClearScreen();

        void DrawWorld(World* world, EngineContext* engine);

        void BeginImGui();
        void EndImGui();

        // Pass nullptr to draw to the default framebuffer!
        void SetRenderTarget(RenderTarget* target);

        // Drawing properties
        enum class CullMode {
            Off, Back, Front
        };

        void SetCullMode(CullMode mode);

        enum class DepthTestFunc {
            Off, Less, Greater
        };

        void SetDepthTest(DepthTestFunc func);

        void Present();

        int width = 512, height = 512;

        SDL_GLContext sdl_context = nullptr;
        SDL_Window* sdl_window = nullptr;
    };
}

#endif
