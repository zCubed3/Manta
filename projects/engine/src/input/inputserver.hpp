#ifndef MANTA_INPUTSERVER_HPP
#define MANTA_INPUTSERVER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>

namespace Manta::Input {
    class Axis;
    class Button;

    class InputServer {
    public:
        std::unordered_map<SDL_Keycode, bool> bound_keys;
        std::unordered_map<std::string, Axis*> bound_axes;

        float mouse_delta_x, mouse_delta_y;

        void ProcessEvent(SDL_Event* event);
        void UpdateBinds();

        void AddAxis(const std::string& name, Axis* axis);
    };
}

#endif
