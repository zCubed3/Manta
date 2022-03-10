set(SOURCES
    "src/types/vector2.cpp"
    "src/types/vector3.cpp"
    "src/types/vector4.cpp"
    "src/types/matrix4x4.cpp"

    "src/rendering/mesh.cpp"
    "src/rendering/shader.cpp"
    "src/rendering/camera.cpp"

    "src/data/timing.cpp"

    "src/main.cpp"
)

set(INCLUDE_DIRS
    "${ENGINE_DIR}/src"
    "${DEPENDS_DIR}/openvr/headers"
)

set(LINK_DIRS)

if (DEFINED WIN32)
    set(LINK_DIRS "${DEPENDS_DIR}/openvr/lib/win64" "${DEPENDS_DIR}/vulkan/lib" "${DEPENDS_DIR}/SDL2/lib/x64" "${DEPENDS_DIR}/glew/lib/Release/x64" "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    set(LIBRARIES "SDL2" "SDL2main" "glew32" "opengl32" lua)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} "${DEPENDS_DIR}/vulkan/include" "${DEPENDS_DIR}/SDL2/include" "${DEPENDS_DIR}/lua/src" "${DEPENDS_DIR}/glew/include")
else ()
    set(LINK_DIRS "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    set(LIBRARIES "SDL2" "SDL2main" "GL" "GLEW" lua)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} "${DEPENDS_DIR}/lua/src")
endif ()

add_executable(Silica ${GLOB_SOURCES} ${SOURCES})
target_include_directories(Silica PUBLIC ${INCLUDE_DIRS})
target_link_directories(Silica PUBLIC ${LINK_DIRS})
target_link_libraries(Silica PUBLIC ${LIBRARIES})

set_target_properties(Silica PROPERTIES
    OUTPUT_NAME "Silica"
)

# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_target(copy-lua ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_DIR}/lua_src
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lua
)

add_custom_target(copy-glsl ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)

add_dependencies(Silica copy-lua)

