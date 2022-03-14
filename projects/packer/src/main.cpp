#include <data/meshes/obj.hpp>
#include <data/meshes/bsm.hpp>

using namespace Manta::Data::Meshes;

#include <iostream>
#include <fstream>

#include <math.h>

int main(int argc, char** argv) {
    // TODO: Make do more than this
    if (argc < 3)
        throw std::runtime_error("Please provide 2 arguments!");

    auto file = std::ifstream(argv[1]);
    auto obj = WavefrontOBJ::LoadFromStream(file);

    auto bsm = new MantaBSM();

    for (auto idx : obj->indices)
        bsm->indices.emplace_back(idx);

    for (auto vert : obj->vertices) {
        MantaBSM::BSMVertex bsm_vert {};

        for (int p = 0; p < 3; p++)
            bsm_vert.position[p] = vert.position[p];

        for (int u = 0; u < 2; u++)
            bsm_vert.uv[u] = vert.uv[u];

        float yaw = atan2f(vert.normal[0], vert.normal[2]);
        float pitch = asinf(vert.normal[1]);

        bsm_vert.normal[0] = yaw;
        bsm_vert.normal[1] = pitch;

        bsm->vertices.emplace_back(bsm_vert);
    }

    bsm->name = obj->name;
    bsm->WriteToFile(argv[2]);
}