#include "behavior.hpp"

#include "actor.hpp"

namespace Silica {
    void Behavior::Start(Actor *owner) {
        is_new = false;
    }

    void Behavior::Update(Actor* actor) {
        last_enabled = enabled;

        if (!enabled || actor->enabled)
            return;
    }

    bool Behavior::IsNew() {
        return is_new;
    }
}