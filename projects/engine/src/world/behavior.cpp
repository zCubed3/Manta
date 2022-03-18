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

    void Behavior::Update(Actor* actor, EngineContext* engine) {
        if (last_enabled != enabled) {
            if (enabled)
                OnEnable(actor, engine);
            else
                OnDisable(actor, engine);
        }

        last_enabled = enabled;

        if (!enabled || actor->enabled)
            return;
    }

    void Behavior::OnDisable(Actor *owner, EngineContext* engine) {}
    void Behavior::OnEnable(Actor *owner, EngineContext* engine) {}

    bool Behavior::IsNew() {
        return is_new;
    }
}