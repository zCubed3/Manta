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

#include <behaviors/freecam.hpp>

namespace Manta::Game {
    void GameGameModule::Initialize(EngineContext *engine) {
        //Mesh* mesh = Mesh::LoadFromFile("test.bsm");
        //Mesh* obj_mesh = Mesh::LoadFromFile("test.obj");
        //Mesh* bsm_mesh = Mesh::LoadFromFile("test.bsm");
        Mesh* mmdl_mesh = Mesh::LoadFromFile("test.mmdl");

        Shader* shader = Shader::LoadFile("content/engine/shaders/lit.glsl");
        shader->Compile();

        world = new World();

        test_actor = new Actor("test");
        test_actor->meshes.emplace_back(mmdl_mesh);
        test_actor->shaders.emplace_back(shader);

        world->AddActor(test_actor);

        camera_actor = new Actor("camera");
        camera_actor->transform.position = glm::vec3(0, 0, 1);
        camera = camera_actor->AddBehavior<CameraBehavior>();
        freecam = camera_actor->AddBehavior<FreecamBehavior>();

        world->AddActor(camera_actor);
    }

    void GameGameModule::Update(EngineContext* engine) {
        camera_actor->transform.position.y = 0.6;
        camera_actor->transform.position.z = 0.5;
        camera->fov = 60;
        camera->width = engine->renderer->width;
        camera->height = engine->renderer->height;

        auto spin = glm::vec3(0, engine->timing->delta_time, 0) * 20.0f;
        //test_actor->transform.euler += spin;

        world->Update(engine);
    }

    void GameGameModule::Draw(EngineContext* engine) {
        engine->renderer->DrawWorld(world, engine);
    }

    void GameGameModule::DrawGUI(EngineContext* engine) {

    }
}