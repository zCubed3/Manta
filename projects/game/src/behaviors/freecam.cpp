#include "freecam.hpp"

#include <world/timing.hpp>
#include <world/actor.hpp>

#include <data/engine_context.hpp>

namespace Manta::Game {
    void FreecamBehavior::Update(Actor *owner, EngineContext *engine) {
        Behavior::Update(owner, engine);
    }

    std::string FreecamBehavior::get_TypeId() { return "freecamera"; }
}