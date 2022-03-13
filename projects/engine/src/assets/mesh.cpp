#include "mesh.hpp"

#include <rendering/shader.hpp>
#include <rendering/camera.hpp>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

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

    void Mesh::DrawNow(const glm::mat4 &transform, Camera *camera, Shader *shader) {
        Mesh::DrawNow(transform, glm::inverseTranspose(transform), camera, shader);
    }

    void Mesh::DrawNow(const glm::mat4& transform, const glm::mat4& transform_it, Camera* camera, Shader* shader) {
        shader->Use();

        //TODO: Make me not complete and utter shit!
        uint32_t mvp_uniform = glGetUniformLocation(shader->handle, "MANTA_MVP");
        glm::mat4 mvp_mat = camera->eye * transform;
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(mvp_mat));

        uint32_t m_uniform = glGetUniformLocation(shader->handle, "MANTA_M");
        glUniformMatrix4fv(m_uniform, 1, GL_FALSE, glm::value_ptr(transform));

        uint32_t mit_uniform = glGetUniformLocation(shader->handle, "MANTA_M_IT");
        glUniformMatrix4fv(mit_uniform, 1, GL_FALSE, glm::value_ptr(transform_it));

        uint32_t cam_pos_uniform = glGetUniformLocation(shader->handle, "MANTA_CAM_POS");
        glUniform3fv(cam_pos_uniform, 1, glm::value_ptr(camera->position));

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
        std::ifstream file(path); //TODO: SAFETY!
        std::stringstream source;
        source << file.rdbuf();

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
            mesh->ReadOBJ(source);

        if (extension == "smf")
            mesh->ReadBSM(source);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Loading '" << mesh->name << "' (from '" << path << "') took " << duration.count() << "ms" << std::endl;

        mesh->CreateBuffers();

        return mesh;
    }

    void Mesh::ReadOBJ(std::stringstream& source) {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv0s;
        std::vector<glm::u32vec3> triangles;

        std::string line;
        while (std::getline(source, line)) {
            if (line.size() <= 2)
                continue;

            // First two characters are a data id
            std::string id = line.substr(0, 2);
            std::string contents = line.substr(2);

            if (id[0] == '#')
                continue;

            if (id[0] == 'o')
                name = contents;

            if (id[0] == 'v') {
                glm::vec3 v3data;

                if (id[1] == 't') {
                    glm::vec2 uv;
                    sscanf(contents.c_str(), "%f %f", &uv.x, &uv.y);
                    uv0s.emplace_back(uv);
                    continue;
                }

                sscanf(contents.c_str(), "%f %f %f", &v3data.x, &v3data.y, &v3data.z);

                if (id[1] == 'n')
                    normals.emplace_back(v3data);
                else
                    positions.emplace_back(v3data);
            }

            // F = face, we either iterate 3 times to make a tri, or once to make it a vert, depends on the obj
            // Ugly method of loading faces but idc enough to fix it
            if (id[0] == 'f') {
                glm::u32vec3 tri;
                std::string face = contents;

                size_t pos;
                while (true) {
                    pos = face.find(" ");

                    sscanf(face.c_str(), "%i/%i/%i",
                           &tri.x, &tri.y, &tri.z
                    );

                    face.erase(0, pos + 1);

                    tri -= 1;
                    triangles.emplace_back(tri);

                    if (pos == std::string::npos)
                        break;
                }
            }
        }

        for (int t = 0; t < triangles.size(); t++) {
            auto tri = triangles[t];
            Vertex vert;

            vert.position = positions[tri.x];
            vert.uv0 = uv0s[tri.y];
            vert.normal = normals[tri.z];

            bool similarVert = false;
            for (int v = 0; v < vertices.size(); v++)
                if (vertices[v] == vert) {
                    indices.emplace_back(v);
                    similarVert = true;
                    break;
                }

            if (!similarVert) {
                indices.emplace_back(vertices.size());
                vertices.emplace_back(vert);
            }
        }
    }

    void Mesh::ReadBSM(std::stringstream &source) {
        auto bsm = BSM::LoadFromStream(source);

        name = bsm->name;

        for (auto indice : bsm->indices)
            indices.emplace_back(indice);

        for (auto bsf_vert : bsm->vertices) {
            Vertex vert {};

            for (int p = 0; p < 3; p++)
                vert.position[p] = bsf_vert.position[p];

            for (int u = 0; u < 2; u++)
                vert.uv0[u] = bsf_vert.uv[u];

            float yaw = bsf_vert.normal[0];
            float pitch = bsf_vert.normal[1];

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
