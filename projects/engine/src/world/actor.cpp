#include "actor.hpp"

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

        transform.UpdateMatrices();
    }

    //TODO: SAFETY
    void Actor::AddBehavior(Behavior *behavior) {
        behaviors.emplace_back(behavior);
    }
}