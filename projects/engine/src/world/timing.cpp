#include "timing.hpp"

#include <SDL2/SDL.h>

#include <iostream>

namespace Manta {
    void Timing::UpdateTime() {
        uint64_t tick = SDL_GetTicks64();
        delta_time = (tick - last_tick) / 1000.0f;

        time += delta_time;
        last_tick = tick;

        sin_time = glm::vec4(sinf(time), sinf(time * 2), sinf(time * 4), sinf(time * 8));
        cos_time = glm::vec4(cosf(time), cosf(time * 2), cosf(time * 4), cosf(time * 8));
        tan_time = glm::vec4(tanf(time), tanf(time * 2), tanf(time * 4), tanf(time * 8));
    }
}