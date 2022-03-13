#ifndef MANTA_ACTOR_HPP
#define MANTA_ACTOR_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "transform.hpp"

#include <string>
#include <vector>

namespace Manta {
    class Behavior;

    class Actor {
    public:
        Transform transform = Transform();
        std::string name = "New Actor";
        bool enabled = true;

        std::vector<Behavior*> behaviors;

        Actor(const std::string& name);

        void Update();
        void Draw(); // Updates anything that implements draw, usually renderer components

        void AddBehavior(Behavior* behavior);
    };
}

#endif
