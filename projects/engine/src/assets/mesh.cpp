#include "mesh.hpp"

#include <rendering/viewport.hpp>

#include <assets/shader.hpp>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

#include <world/timing.hpp>

#include <math.h>

using namespace Manta::Data::Meshes;

namespace Manta {
    void Mesh::CreateBuffers() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);

        UpdateBuffers();
    }

    void Mesh::UpdateBuffers() {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Mesh::DrawNow(const glm::mat4 &transform, Shader *shader) {
        Mesh::DrawNow(transform, glm::inverseTranspose(transform), shader);
    }

    void Mesh::DrawNow(const glm::mat4& transform, const glm::mat4& transform_it, Shader* shader) {
        uint32_t handle = shader->Use();

        // TODO: Shader properties
        if (Viewport::active_viewport) {
            shader->SetMat4x4("MANTA_MVP", Viewport::active_viewport->eye * transform);
            shader->SetMat4x4("MANTA_M", transform);
            shader->SetMat4x4("MANTA_M_IT", transform_it);

            shader->SetVec3("MANTA_CAM_POS", Viewport::active_viewport->transform.position);
        }

        uint32_t sintime_uniform = glGetUniformLocation(handle, "MANTA_SINTIME");
        glUniform4fv(sintime_uniform, 1, glm::value_ptr(Timing::sin_time));

        //
        //
        //

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        int v_size = sizeof(Vertex);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, v_size, nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, v_size, (void *) (sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, v_size, (void *) (sizeof(float) * 6));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    Mesh* Mesh::LoadFromFile(const std::string& path) {
        std::ifstream file(path);

        if (!file.is_open())
            throw std::runtime_error("File wasn't found / couldn't be read!");

        // Try to grab the file extension
        // We look backwards in the path
        std::string extension;
        for (int c = path.size() - 1; c > 0; c--) {
            if (path.at(c) == '.')
                break;

            extension.insert(0, 1, path.at(c));
        }

        std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

        Mesh* mesh = new Mesh();

        auto start = std::chrono::high_resolution_clock::now();

        if (extension == "obj")
            mesh->ReadOBJ(file);

        if (extension == "bsm")
            mesh->ReadBSM(file);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Loading '" << mesh->name << "' (from '" << path << "') took " << duration.count() << "ms" << std::endl;

        mesh->CreateBuffers();

        return mesh;
    }

    void Mesh::ReadOBJ(std::istream& source) {
        auto obj = WavefrontOBJ::LoadFromStream(source);

        for (auto indice : obj->indices)
            indices.emplace_back(indice);

        for (auto obj_vert : obj->vertices) {
            Vertex vert {};

            for (int p = 0; p < 3; p++)
                vert.position[p] = obj_vert.position[p];

            for (int n = 0; n < 3; n++)
                vert.normal[n] = obj_vert.normal[n];

            for (int u = 0; u < 2; u++)
                vert.uv0[u] = obj_vert.uv[u];

            vertices.emplace_back(vert);
        }

        name = obj->name;

        delete obj;
    }

    void Mesh::ReadBSM(std::istream &source) {
        auto bsm = MantaBSM::LoadFromStream(source);

        name = bsm->name;

        for (auto indice : bsm->indices)
            indices.emplace_back(indice);

        for (auto bsm_vert : bsm->vertices) {
            Vertex vert {};

            for (int p = 0; p < 3; p++)
                vert.position[p] = bsm_vert.position[p];

            for (int u = 0; u < 2; u++)
                vert.uv0[u] = bsm_vert.uv[u];

            float yaw = bsm_vert.normal[0];
            float pitch = bsm_vert.normal[1];

            float cos_t = cosf(pitch);
            vert.normal = glm::vec3(sinf(yaw) * cos_t, sin(pitch), cosf(yaw) * cos_t);

            vertices.emplace_back(vert);
        }

        delete bsm;
    }

    bool Mesh::Vertex::operator==(const Vertex& v) {
        return position == v.position && normal == v.normal && uv0 == v.uv0;
    }
}
