#include <data/meshes/obj.hpp>
#include <data/meshes/mbsm.hpp>
#include <data/meshes/mmdl.hpp>

using namespace Manta::Data::Meshes;

#include <iostream>
#include <fstream>

#include <math.h>

#include <mikktspace.h>

#define GET_IDX() ((iFace * 3) + iVert)

static std::vector<WavefrontOBJ::OBJVector3> tangents;
static std::vector<float> tangents_w;

int get_num_faces(const SMikkTSpaceContext* context) {
    WavefrontOBJ* mesh = (WavefrontOBJ*)context->m_pUserData;

    auto len = mesh->unweld_indices.size() / 3;
    return len;
}

int get_num_vertices_of_face(const SMikkTSpaceContext* context, const int iFace) {
    return 3;
}

void get_position(const SMikkTSpaceContext *context, float *outdata, const int iFace, const int iVert) {
    WavefrontOBJ* mesh = (WavefrontOBJ*)context->m_pUserData;

    size_t idx = GET_IDX();

    auto position = mesh->unweld_positions[mesh->unweld_indices[idx].v];
    outdata[0] = position.x;
    outdata[1] = position.y;
    outdata[2] = position.z;
}

void get_normal(const SMikkTSpaceContext *context, float *outdata, const int iFace, const int iVert) {
    WavefrontOBJ* mesh = (WavefrontOBJ*)context->m_pUserData;

    size_t idx = GET_IDX();

    auto normal = mesh->unweld_normals[mesh->unweld_indices[idx].n];
    outdata[0] = normal.x;
    outdata[1] = normal.y;
    outdata[2] = normal.z;
}

void get_uv(const SMikkTSpaceContext *context, float *outdata, const int iFace, const int iVert) {
    WavefrontOBJ* mesh = (WavefrontOBJ*)context->m_pUserData;

    size_t idx = GET_IDX();

    auto uv = mesh->unweld_uvs[mesh->unweld_indices[idx].u];
    outdata[0] = uv.x;
    outdata[1] = uv.y;
}

void set_tspace(const SMikkTSpaceContext *context, const float *tangent, const float fSign, const int iFace, const int iVert) {
    WavefrontOBJ* mesh = (WavefrontOBJ*)context->m_pUserData;

    size_t idx = GET_IDX();

    WavefrontOBJ::OBJVector4 v4 = {tangent[0], tangent[1], tangent[2], fSign};
    mesh->unweld_tangents[mesh->unweld_indices[idx].v] = v4;
}

int main(int argc, char** argv) {
    // TODO: Make do more than this
    if (argc < 3)
        throw std::runtime_error("Please provide 2 arguments!");

    auto file = std::ifstream(argv[1]);
    auto obj = WavefrontOBJ::LoadFromStream(file, false);

    obj->unweld_tangents.resize(obj->unweld_positions.size());

    SMikkTSpaceContext context {};
    SMikkTSpaceInterface interface {};

    interface.m_getNumFaces = get_num_faces;
    interface.m_getNumVerticesOfFace = get_num_vertices_of_face;
    interface.m_getPosition = get_position;
    interface.m_getNormal = get_normal;
    interface.m_getTexCoord = get_uv;
    interface.m_setTSpaceBasic = set_tspace;

    context.m_pInterface = &interface;
    context.m_pUserData = obj;

    genTangSpaceDefault(&context);

    obj->WeldVertices();

    auto bsm = new MantaBSM();
    auto mmdl = new MantaMDL();

    auto channel0 = &mmdl->channels[0];

    channel0->type = MantaMDL::ChannelType::VEC3;
    channel0->hint = MantaMDL::ChannelHint::VERTEX;

    auto channel1 = &mmdl->channels[1];

    channel1->type = MantaMDL::ChannelType::VEC3;
    channel1->hint = MantaMDL::ChannelHint::NORMAL;

    auto channel2 = &mmdl->channels[2];

    channel2->type = MantaMDL::ChannelType::VEC2;
    channel2->hint = MantaMDL::ChannelHint::UV0;

    auto channel3 = &mmdl->channels[3];

    channel3->type = MantaMDL::ChannelType::VEC4;
    channel3->hint = MantaMDL::ChannelHint::TANGENT;

    auto channel4 = &mmdl->channels[4];

    channel4->type = MantaMDL::ChannelType::UINT32;
    channel4->hint = MantaMDL::ChannelHint::INDEXER;

    mmdl->name = obj->name;

    for (auto vert : obj->weld_vertices) {
        auto clone_pos = new MantaMDL::Vec3;
        *clone_pos = { vert.position[0], vert.position[1], vert.position[2] };

        channel0->data.push_back(clone_pos);

        auto clone_norm = new MantaMDL::Vec3;
        *clone_norm = { vert.normal[0], vert.normal[1], vert.normal[2] };

        channel1->data.push_back(clone_norm);

        auto clone_uv0 = new MantaMDL::Vec2;
        *clone_uv0 = { vert.uv[0], vert.uv[1] };

        channel2->data.push_back(clone_uv0);

        auto clone_tan = new MantaMDL::Vec4;
        *clone_tan = { vert.tangent[0], vert.tangent[1], vert.tangent[2], vert.tangent[3] };

        channel3->data.push_back(clone_tan);
    }

    for (auto idx : obj->weld_indices) {
        auto clone = new uint32_t;
        *clone = idx;

        channel4->data.push_back(clone);
    }

    mmdl->WriteToFile("test.mmdl");

    for (auto idx : obj->weld_indices)
        bsm->indices.emplace_back(idx);

    for (auto vert : obj->weld_vertices) {
        MantaBSM::BSMVertex bsm_vert {};

        for (int p = 0; p < 3; p++)
            bsm_vert.position[p] = vert.position[p];

        for (int u = 0; u < 2; u++)
            bsm_vert.uv[u] = vert.uv[u];

        float yaw = atan2f(vert.normal[0], vert.normal[2]);
        float pitch = asinf(vert.normal[1]);

        bsm_vert.normal[0] = yaw;
        bsm_vert.normal[1] = pitch;

        yaw = atan2f(vert.tangent[0], vert.tangent[2]);
        pitch = asinf(vert.tangent[1]);

        bsm_vert.tangent[0] = yaw;
        bsm_vert.tangent[1] = pitch;
        bsm_vert.tangent[2] = vert.tangent[3];

        bsm->vertices.emplace_back(bsm_vert);
    }

    bsm->name = obj->name;
    bsm->WriteToFile(argv[2]);
}