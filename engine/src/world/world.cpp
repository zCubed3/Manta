#include "world.hpp"

#include "actor.hpp"

namespace Silica {
    std::vector<Actor*> World::actors = std::vector<Actor*>();

    void World::AddActor(Actor *actor) {
        actors.emplace_back(actor);
    }

    // TODO: Child actors and so on
    Actor *World::FindActor(const std::string &path) {
        for (Actor* actor : actors) {
            if (actor->name == path)
                return actor;
        }

        return nullptr;
    }
}