set(SOURCES
        "${COMMON_DIR}/data/meshes/obj.cpp"
        "${COMMON_DIR}/data/meshes/mbsm.cpp"
        "${COMMON_DIR}/data/meshes/mmdl.cpp"

        "${ENGINE_DIR}/src/world/timing.cpp"
        "${ENGINE_DIR}/src/world/world.cpp"
        "${ENGINE_DIR}/src/world/actor.cpp"
        "${ENGINE_DIR}/src/world/behavior.cpp"
        "${ENGINE_DIR}/src/world/transform.cpp"

        "${ENGINE_DIR}/src/world/behaviors/camera.cpp"

        "${ENGINE_DIR}/src/rendering/renderer.cpp"
        "${ENGINE_DIR}/src/rendering/viewport.cpp"

        "${ENGINE_DIR}/src/assets/shader.cpp"
        "${ENGINE_DIR}/src/assets/mesh.cpp"

        "${ENGINE_DIR}/src/data/console/console.cpp"

        "${ENGINE_DIR}/src/data/engine_context.cpp"

        "${ENGINE_DIR}/src/input/axis.cpp"
        "${ENGINE_DIR}/src/input/inputserver.cpp"

        "src/behaviors/freecam.cpp"

        "src/game/gamegamemodule.cpp"

        "src/main.cpp"
)

set(INCLUDE_DIRS
    "${ENGINE_DIR}/src"
    "${GAME_DIR}/src"
    "${DEPENDS_DIR}/glm"
    "${DEPENDS_DIR}/imgui"
    "${DEPENDS_DIR}/MikkTSpace"
    "${COMMON_DIR}"
)

set(LINK_DIRS)

if (DEFINED WIN32)

else ()

endif ()

# TODO: Come up with a name for the damn game!
add_library(MantaGame SHARED ${SOURCES})
target_include_directories(MantaGame PUBLIC ${INCLUDE_DIRS})

set_target_properties(MantaGame PROPERTIES
        OUTPUT_NAME "game"
)
