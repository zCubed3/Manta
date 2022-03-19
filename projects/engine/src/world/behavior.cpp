#include "behavior.hpp"

#include "actor.hpp"

namespace Manta {
    void Behavior::Start(Actor *owner, EngineContext* engine) {
        if (enabled)
            OnEnable(owner, engine);
        else
            OnDisable(owner, engine);

        is_new = false;
    }

    bool Behavior::Update(Actor* owner, EngineContext* engine) {
        if (last_enabled != enabled) {
            if (enabled)
                OnEnable(owner, engine);
            else
                OnDisable(owner, engine);
        }

        this->owner = owner;

        last_enabled = enabled;

        if (!enabled || !owner->enabled)
            return false;

        return true;
    }

    void Behavior::OnDisable(Actor *owner, EngineContext* engine) {}
    void Behavior::OnEnable(Actor *owner, EngineContext* engine) {}

    bool Behavior::IsNew() {
        return is_new;
    }
}