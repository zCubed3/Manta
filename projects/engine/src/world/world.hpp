#ifndef MANTA_WORLD_HPP
#define MANTA_WORLD_HPP

#include <vector>
#include <string>

namespace Manta::Rendering {
    class Viewport;
}

namespace Manta {
    class Actor;
    class EngineContext;

    class World final {
    public:
        std::vector<Rendering::Viewport*> viewports;

        void AddActor(Actor* actor);
        Actor* FindActor(const std::string& path);

        void Update(EngineContext* engine);

        std::vector<Actor*> actors;
    };
}

#endif
