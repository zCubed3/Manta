#ifndef MANTA_CAMERA_HPP
#define MANTA_CAMERA_HPP

#include <rendering/viewport.hpp>

#include <world/behavior.hpp>

namespace Manta {
    class CameraBehavior : public Behavior {
    public:
        float fov = 90;
        int width, height;

        void OnDisable(Actor *owner, EngineContext* engine) override;
        void OnEnable(Actor *owner, EngineContext* engine) override;

        void Update(Actor *owner, EngineContext *engine) override;

        std::string get_TypeId() override;

    protected:
        Viewport viewport;
    };
}

#endif
