#ifndef SILICA_WORLD_HPP
#define SILICA_WORLD_HPP

#include <vector>
#include <string>

namespace Silica {
    class Actor;

    class World final {
    public:
        static void AddActor(Actor* actor);
        static Actor* FindActor(const std::string& path);

        static std::vector<Actor*> actors;
    };
}

#endif
