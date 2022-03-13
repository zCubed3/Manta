#include "behavior.hpp"

#include "actor.hpp"

namespace Manta {
    void Behavior::Start(Actor *owner) {
        is_new = false;
    }

    void Behavior::Update(Actor* actor) {
        last_enabled = enabled;

        if (!enabled || actor->enabled)
            return;
    }

    bool Behavior::get_IsNew() {
        return is_new;
    }
}