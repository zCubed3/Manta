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

    bool CameraBehavior::Update(Actor *owner, EngineContext *engine) {
        if (!Behavior::Update(owner, engine))
            return false;

        owner->transform.gen_view = true;

        viewport.view = owner->transform.view;
        viewport.position = owner->transform.position;
        viewport.width = width;
        viewport.height = height;
        viewport.fov = fov;
        viewport.z_near = z_near;
        viewport.z_far = z_far;

        viewport.UpdateMatrices();

        return true;
    }

    std::string CameraBehavior::get_TypeId() { return "camera"; }
}