set(SOURCES
    "src/world/world.cpp"
    "src/world/actor.cpp"
    "src/world/behavior.cpp"
    "src/world/timing.cpp"

    "src/rendering/mesh.cpp"
    "src/rendering/shader.cpp"
    "src/rendering/camera.cpp"

    "src/main.cpp"
)

set(INCLUDE_DIRS
    "${ENGINE_DIR}/src"
    "${DEPENDS_DIR}/glm"
)

set(LINK_DIRS)

if (DEFINED WIN32)
    set(LINK_DIRS "${DEPENDS_DIR}/openvr/lib/win64" "${DEPENDS_DIR}/vulkan/lib" "${DEPENDS_DIR}/SDL2/lib/x64" "${DEPENDS_DIR}/glew/lib/Release/x64" "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    set(LIBRARIES "SDL2" "SDL2main" "glew32" "opengl32")

    set(INCLUDE_DIRS ${INCLUDE_DIRS} "${DEPENDS_DIR}/vulkan/include" "${DEPENDS_DIR}/SDL2/include" "${DEPENDS_DIR}/glew/include")
else ()
    set(LINK_DIRS "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    set(LIBRARIES "SDL2" "SDL2main" "GL" "GLEW")
    set(INCLUDE_DIRS ${INCLUDE_DIRS})
endif ()

add_executable(Silica ${GLOB_SOURCES} ${SOURCES})
target_include_directories(Silica PUBLIC ${INCLUDE_DIRS})
target_link_directories(Silica PUBLIC ${LINK_DIRS})
target_link_libraries(Silica PUBLIC ${LIBRARIES})

set_target_properties(Silica PROPERTIES
    OUTPUT_NAME "Silica"
)

# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_target(copy-content ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_DIR}/base_content
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/content/engine
)

add_dependencies(Silica copy-content)

