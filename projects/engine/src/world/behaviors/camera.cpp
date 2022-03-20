#include "camera.hpp"

#include <algorithm>

#include <world/actor.hpp>
#include <world/world.hpp>

#include <rendering/render_target.hpp>

#include <data/engine_context.hpp>

namespace Manta {
    void CameraBehavior::OnDisable(World *world, Actor *owner, EngineContext* engine) {
        auto iter = std::find(world->viewports.begin(), world->viewports.end(), &viewport);

        if (iter != world->viewports.end())
            world->viewports.erase(iter);
    }

    void CameraBehavior::OnEnable(World *world, Actor *owner, EngineContext* engine) {
        world->viewports.emplace_back(&viewport);
    }

    bool CameraBehavior::Update(World *world, Actor *owner, EngineContext *engine) {
        if (!Behavior::Update(world, owner, engine))
            return false;

        if (render_target) {
            width = render_target->width;
            height = render_target->height;
        }

        owner->transform.gen_view = true;

        viewport.view = owner->transform.view;
        viewport.position = owner->transform.position;
        viewport.width = width;
        viewport.height = height;
        viewport.fov = fov;
        viewport.z_near = z_near;
        viewport.z_far = z_far;
        viewport.render_target = render_target;

        viewport.Update();

        return true;
    }

    std::string CameraBehavior::get_TypeId() { return "camera"; }
}