#include "camera.hpp"

#include <algorithm>

#include <world/actor.hpp>

#include <data/engine_context.hpp>

namespace Manta {
    void CameraBehavior::OnDisable(Actor *owner, EngineContext* engine) {
        auto iter = std::find(engine->active_viewports.begin(), engine->active_viewports.end(), &viewport);

        if (iter != engine->active_viewports.end())
            engine->active_viewports.erase(iter);
    }

    void CameraBehavior::OnEnable(Actor *owner, EngineContext* engine) {
        engine->active_viewports.emplace_back(&viewport);
    }

    void CameraBehavior::Update(Actor *owner, EngineContext *engine) {
        viewport.transform = owner->transform;
        viewport.width = width;
        viewport.height = height;
        viewport.fov = fov;
    }

    std::string CameraBehavior::get_TypeId() { return "camera"; }
}