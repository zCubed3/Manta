#include "timing.hpp"

#include <SDL2/SDL.h>

#include <iostream>

namespace Manta {
    Timing::Timing() {};

    float Timing::delta_time = 0.0f;
    float Timing::time = 0.0f;

    uint64_t Timing::last_tick = 0;

    void Timing::UpdateTime() {
        uint64_t tick = SDL_GetTicks64();
        delta_time = (tick - last_tick) / 1000.0f;

        time += delta_time;
        last_tick = tick;
    }
}