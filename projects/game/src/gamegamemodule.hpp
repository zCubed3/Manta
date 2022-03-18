#ifndef MANTA_GAMEGAMEMODULE_HPP
#define MANTA_GAMEGAMEMODULE_HPP

#include <modularity/gamemodule.hpp>

namespace Manta {
    class Actor;
    class World;

    class CameraBehavior;
}

namespace Manta::Game {
    class GameGameModule : public GameModule {
    public:
        void Initialize(EngineContext* engine) override;
        void Update(EngineContext* engine) override;
        void Draw(EngineContext* engine) override;

    protected:
        World *world;
        Actor *test_actor, *test_actor2, *test_actor3, *camera_actor;
        CameraBehavior *camera;
    };
}


#endif
