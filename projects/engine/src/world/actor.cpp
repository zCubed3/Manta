#include "actor.hpp"

#include "behavior.hpp"

#include <assets/shader.hpp>
#include <assets/mesh.hpp>
#include <data/engine_context.hpp>

namespace Manta {
    Actor::Actor(const std::string &name) {
        this->name = name;
    }

    void Actor::Update(EngineContext* engine) {
        // TODO: Safety
        for (auto behavior : behaviors) {
            if (behavior->IsNew())
                behavior->Start(this, engine);

            behavior->Update(this, engine);
        }

        transform.UpdateMatrices();
    }

    void Actor::Draw(EngineContext* engine) {
        int m = 0;
        for (const auto& mesh : meshes) {
            Shader* shader = engine->error_shader;

            if (m < shaders.size()) {
                shader = shaders[m];
            }

            mesh->DrawNow(transform.local_to_world, transform.world_to_local, shader, engine);
            m += 1;
        }
    }
}