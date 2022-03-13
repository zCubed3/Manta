#ifndef MANTA_WORLD_HPP
#define MANTA_WORLD_HPP

#include <vector>
#include <string>

namespace Manta {
    class Actor;

    class World final {
    public:
        static void AddActor(Actor* actor);
        static Actor* FindActor(const std::string& path);

        static std::vector<Actor*> actors;
    };
}

#endif
