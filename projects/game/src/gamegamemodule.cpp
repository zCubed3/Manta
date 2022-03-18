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
        //Mesh* mesh = Mesh::LoadFromFile("test.bsm");
        Mesh* obj_mesh = Mesh::LoadFromFile("test.obj");
        Mesh* bsm_mesh = Mesh::LoadFromFile("test.bsm");
        Mesh* mmdl_mesh = Mesh::LoadFromFile("test.mmdl");

        Shader* shader = Shader::LoadFile("content/engine/shaders/debug.glsl");
        shader->Compile();

        world = new World();

        test_actor = new Actor("test");
        test_actor->transform.position = glm::vec3(-1, 0, 0);
        test_actor->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        test_actor->meshes.emplace_back(obj_mesh);

        test_actor2 = new Actor("test2");
        test_actor2->transform.position = glm::vec3(0, 0, 0);
        test_actor2->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        test_actor2->meshes.emplace_back(bsm_mesh);

        test_actor3 = new Actor("test3");
        test_actor3->transform.position = glm::vec3(1, 0, 0);
        test_actor3->transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        test_actor3->meshes.emplace_back(mmdl_mesh);
        //test_actor->shaders.emplace_back(shader);

        world->AddActor(test_actor);
        world->AddActor(test_actor2);
        world->AddActor(test_actor3);

        camera_actor = new Actor("camera");
        camera = camera_actor->AddBehavior<CameraBehavior>();

        world->AddActor(camera_actor);
    }

    void GameGameModule::Update(EngineContext* engine) {
        camera->viewport.fov = 60;
        camera->viewport.transform.position.z = 2;

        camera->viewport.width = engine->renderer->width;
        camera->viewport.height = engine->renderer->height;

        auto spin = glm::vec3(engine->timing->delta_time, engine->timing->delta_time, engine->timing->delta_time) * 20.0f;
        test_actor->transform.euler += spin;
        test_actor2->transform.euler += spin;
        test_actor3->transform.euler += spin;

        world->Update(engine);
    }

    void GameGameModule::Draw(EngineContext* engine) {
        engine->renderer->DrawWorld(world, engine);
    }
}