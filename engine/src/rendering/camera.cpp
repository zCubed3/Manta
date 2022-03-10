#include "camera.hpp"

namespace Silica {
    void Camera::UpdateMatrices() {
        if (lookAt)
            rotation = Vector3::EulerFromLookAt((position - target).Normalize());

        view = Matrix4x4::MakeTranslation(position);

        // TODO: Make this better

        perspective = Matrix4x4::MakePerspective(fov, aspect, 0.001f, 100.0f);

        eye = view * perspective;
    }
}