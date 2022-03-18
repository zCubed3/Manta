#include "gamegamemodule.hpp"

#include <iostream>

#include <assets/mesh.hpp>
#include <assets/shader.hpp>

#include <world/timing.hpp>
#include <world/world.hpp>
#include <world/actor.hpp>
#include <world/behaviors/camera.hpp>

#include <rendering/renderer.hpp>

#include <data/engine_context.hpp>

namespace Manta::Game {
    void GameGameModule::Initialize(EngineContext *engine) {
        Mesh* mesh = Mesh::LoadFromFile("test.bsm");

        Shader* shader = Shader::LoadFile("content/engine/shaders/debug.glsl");
        shader->Compile();

        world = new World();

        test_actor = new Actor("test");
        test_actor->meshes.emplace_back(mesh);
        test_actor->shaders.emplace_back(shader);

        world->AddActor(test_actor);

        camera_actor = new Actor("camera");
        camera = camera_actor->AddBehavior<CameraBehavior>();

        world->AddActor(camera_actor);
    }

    void GameGameModule::Update(EngineContext* engine) {
        camera->viewport.fov = 60;
        camera->viewport.transform.position.z = 2;

        camera->viewport.width = engine->renderer->width;
        camera->viewport.height = engine->renderer->height;

        test_actor->transform.euler += glm::vec3(engine->timing->delta_time, engine->timing->delta_time, engine->timing->delta_time) * 20.0f;

        world->Update(engine);
    }

    void GameGameModule::Draw(EngineContext* engine) {
        engine->renderer->DrawWorld(world, engine);
    }
}