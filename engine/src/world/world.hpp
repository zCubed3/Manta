#ifndef SILICA_WORLD_HPP
#define SILICA_WORLD_HPP

#include <vector>

namespace Silica {
    class Actor;

    class World {
    public:
        std::vector<Actor> actors;
    };
}

#endif
