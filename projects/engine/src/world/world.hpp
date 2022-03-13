#ifndef MANTA_WORLD_HPP
#define MANTA_WORLD_HPP

#include <vector>
#include <string>

namespace Manta {
    class Actor;

    class World final {
    public:
        void AddActor(Actor* actor);
        Actor* FindActor(const std::string& path);

        void Update();

        std::vector<Actor*> actors;
    };
}

#endif
