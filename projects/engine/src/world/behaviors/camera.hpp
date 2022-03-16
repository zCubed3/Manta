#ifndef MANTA_CAMERA_HPP
#define MANTA_CAMERA_HPP

#include <rendering/viewport.hpp>

#include <world/behavior.hpp>

namespace Manta {
    class CameraBehavior : public Behavior {
    public:
        Viewport viewport;

        void OnDisable(Actor *owner) override;
        void OnEnable(Actor *owner) override;

        std::string get_TypeId() override;
    };
}

#endif
