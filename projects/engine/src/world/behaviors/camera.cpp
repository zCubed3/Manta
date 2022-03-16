#include "camera.hpp"

#include <algorithm>

namespace Manta {
    void CameraBehavior::OnDisable(Actor *owner) {
        auto iter = std::find(Viewport::active_viewports.begin(), Viewport::active_viewports.end(), &viewport);

        if (iter != Viewport::active_viewports.end())
            Viewport::active_viewports.erase(iter);
    }

    void CameraBehavior::OnEnable(Actor *owner) {
        Viewport::active_viewports.emplace_back(&viewport);
    }

    std::string CameraBehavior::get_TypeId() { return "camera"; }
}