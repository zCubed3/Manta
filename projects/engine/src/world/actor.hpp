#ifndef MANTA_ACTOR_HPP
#define MANTA_ACTOR_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "transform.hpp"

#include <string>
#include <vector>

namespace Manta {
    class Behavior;
    class Mesh;
    class Shader;

    class Actor {
    public:
        Transform transform = Transform();
        std::string name = "New Actor";
        bool enabled = true;

        std::vector<Mesh*> meshes;
        std::vector<Shader*> shaders;
        std::vector<Behavior*> behaviors;

        Actor(const std::string& name);

        void Update();
        void Draw();

        void AddBehavior(Behavior* behavior);
    };
}

#endif
