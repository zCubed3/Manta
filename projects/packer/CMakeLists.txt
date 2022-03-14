set(SOURCES
    "${COMMON_DIR}/data/meshes/obj.cpp"
    "${COMMON_DIR}/data/meshes/bsm.cpp"

    "src/main.cpp"
)

set(INCLUDE_DIRS
    "${COMMON_DIR}"
)

add_executable(MantaPacker ${GLOB_SOURCES} ${SOURCES})
target_include_directories(MantaPacker PUBLIC ${INCLUDE_DIRS})
target_link_directories(MantaPacker PUBLIC ${LINK_DIRS})
target_link_libraries(MantaPacker PUBLIC ${LIBRARIES})
