#include "behavior.hpp"

#include "actor.hpp"

namespace Manta {
    void Behavior::Start(Actor *owner) {
        if (enabled)
            OnEnable(owner);
        else
            OnDisable(owner);

        is_new = false;
    }

    void Behavior::Update(Actor* actor) {
        if (last_enabled != enabled) {
            if (enabled)
                OnEnable(actor);
            else
                OnDisable(actor);
        }

        last_enabled = enabled;

        if (!enabled || actor->enabled)
            return;
    }

    void Behavior::OnDisable(Actor *owner) {}
    void Behavior::OnEnable(Actor *owner) {}

    bool Behavior::get_IsNew() {
        return is_new;
    }
}