#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

// TODO: Make this more generic for other APIs?
#include <GL/glew.h>

namespace Silica {
    Shader* Shader::LoadCode(std::string code) {
        auto shader = new Shader();

        shader->path = "#internal";
        shader->source = code;

        shader->ProcessSource();

        return shader;
    }

    Shader* Shader::LoadFile(std::string path) {
        auto shader = new Shader();

        std::ifstream shader_file(path);

        if (!shader_file.is_open())
            return nullptr; // We couldn't find a file!

        std::stringstream str;
        str << shader_file.rdbuf();
        shader_file.close();

        shader->path = path;
        shader->source = str.str();

        shader->ProcessSource();

        return shader;
    }

    void Shader::Reload() {
        if (path != "#internal") {
            std::ifstream shader_file(path);

            if (!shader_file.is_open()) // TODO: Error on deletion
                return; // We couldn't find a file!

            std::stringstream str;
            str << shader_file.rdbuf();
            shader_file.close();

            source = str.str();
            analyzed = false;
        }
    }

    void Shader::ProcessSource() {
        auto final_src = source;
        auto version_sec = final_src.find_first_of("\n");

        if (version_sec != std::string::npos) {
            auto version_line = final_src.substr(0, version_sec);

            for (char c : version_line)
                if (c != '\n')
                    break;
            //TODO: CORRECT IT IF WE FIND ONE!

            int version_no = 0;
            sscanf(version_line.c_str(), "#version %i", &version_no);

            version = version_no;
            final_src = final_src.erase(0, version_sec);
        }

        analyzed = true;
        source = final_src;
    }

    std::string combine_keywords(const std::vector<std::string>& keywords) {
        std::stringstream keyword_buf;

        for (const auto& key : keywords)
            keyword_buf << key << " ";

        return keyword_buf.str();
    }

    bool did_compile_shader(uint32_t shader) {
        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE) {
            int logLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

            char *log = new char[logLen];

            glGetShaderInfoLog(shader, logLen, nullptr, log);
            printf("Shader failed to compile with error:\n%s\n", log);

            delete[] log;
        }

        return status;
    }

    uint32_t compile_source(const std::string& source_str, int version, bool isVertex) {
        uint32_t shader = glCreateShader(isVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

        auto version_str = std::string("#version") + " " + std::to_string(version) + "\n";
        auto type_str = std::string("#define") + " " + (isVertex ? "VERT" : "FRAG") + "\n#define " + (isVertex ? "VERTEX" : "FRAGMENT") + "\n";

        const char *sources[3] = { version_str.c_str(), type_str.c_str(), source_str.c_str() };

        //std::cout << version_str << type_str << source_str << std::endl;

        glShaderSource(shader, 3, sources, nullptr);
        glCompileShader(shader);

        return shader;
    }

    uint32_t link_program(uint32_t vert, uint32_t frag) {
        uint32_t program = glCreateProgram();

        glAttachShader(program, vert);
        glAttachShader(program, frag);

        glLinkProgram(program);

        glDetachShader(program, vert);
        glDetachShader(program, frag);

        return program;
    }

    // Compiles a variant with the following keywords then appends it to the list of variants
    // For performance reasons variant lookup doesn't use nested vectors!
    bool Shader::Compile() {
        if (!analyzed)
            ProcessSource();

        // TODO: Geometry shaders?
        auto vert = compile_source(source, version, true);
        did_compile_shader(vert);

        auto frag = compile_source(source, version, false);
        did_compile_shader(frag);

        auto prog = link_program(vert, frag);

        glDeleteShader(vert);
        glDeleteShader(frag);

        handle = prog;
        return true;
    }

    void Shader::Use() {
        glUseProgram(handle);
    }

    void Shader::CreateEngineShaders() {
        error_shader = LoadCode(R"(#version 330 core
        #ifdef VERT
            layout(location = 0) in vec3 _vertex;

            uniform mat4 SILICA_MVP;

            void main() {
                gl_Position = SILICA_MVP * vec4(_vertex, 1.0);
            }
        #endif

        #ifdef FRAG
            out vec4 col;

            void main() {
                col = vec4(1.0, 0.0, 0.0, 1.0);
            }
        #endif
        )");

        error_shader->Compile();
    }

    Shader* Shader::error_shader = nullptr;
}