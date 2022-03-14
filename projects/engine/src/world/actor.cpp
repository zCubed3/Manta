#include "actor.hpp"

#include "behavior.hpp"

#include <assets/shader.hpp>
#include <assets/mesh.hpp>

namespace Manta {
    Actor::Actor(const std::string &name) {
        this->name = name;
    }

    void Actor::Update() {
        // TODO: Safety
        for (auto behavior : behaviors) {
            if (behavior->get_IsNew())
                behavior->Start(this);

            behavior->Update(this);
        }

        transform.UpdateMatrices();
    }

    void Actor::Draw() {
        int m = 0;
        for (const auto& mesh : meshes) {
            Shader* shader = Shader::error_shader;

            if (shaders.size() < m) {
                shader = shaders[m];
            }

            mesh->DrawNow(transform.local_to_world, transform.world_to_local_t, shader);
            m += 1;
        }
    }

    //TODO: SAFETY
    void Actor::AddBehavior(Behavior *behavior) {
        behaviors.emplace_back(behavior);
    }
}