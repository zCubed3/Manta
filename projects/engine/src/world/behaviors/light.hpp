#ifndef MANTA_LIGHT_HPP
#define MANTA_LIGHT_HPP

#include <glm/vec3.hpp>

#include <world/behavior.hpp>

namespace Manta {
    class LightBehavior : public Behavior {
    public:
        enum LightType : int {
            Sun,
            Point,
            Spotlight
        };

        float intensity = 1;
        LightType light_type = LightType::Sun;
        glm::vec3 color = glm::vec3(1, 1, 1);

        void OnDisable(Actor *owner, EngineContext *engine) override;
        void OnEnable(Actor *owner, EngineContext *engine) override;

        std::string get_TypeId() override;
    };
}

#endif
