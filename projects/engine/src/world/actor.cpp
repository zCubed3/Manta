#include "actor.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "behavior.hpp"

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

        UpdateMatrices();
    }

    void Actor::UpdateMatrices() {
        model_matrix = glm::translate(glm::mat4(1.0f), position);
        model_matrix *= glm::toMat4(glm::quat(glm::radians(euler)));
        model_matrix = glm::scale(model_matrix, scale);

        model_i_matrix = glm::inverse(model_matrix);
        model_it_matrix = glm::transpose(model_i_matrix);
    }

    //TODO: SAFETY
    void Actor::AddBehavior(Behavior *behavior) {
        behaviors.emplace_back(behavior);
    }
}