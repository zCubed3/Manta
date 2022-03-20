#ifndef MANTA_CAMERA_HPP
#define MANTA_CAMERA_HPP

#include <world/behavior.hpp>

#include <rendering/viewport.hpp>

namespace Manta::Rendering {
    class RenderTarget;
}

namespace Manta {
    class CameraBehavior : public Behavior {
    public:
        float fov = 90, z_near = 0.001f, z_far = 100.0f;
        int width, height;
        Rendering::RenderTarget* render_target;

        void OnDisable(World *world, Actor *owner, EngineContext* engine) override;
        void OnEnable(World *world, Actor *owner, EngineContext* engine) override;

        bool Update(World *world, Actor *owner, EngineContext *engine) override;

        std::string get_TypeId() override;

    protected:
        Rendering::Viewport viewport;
    };
}

#endif
