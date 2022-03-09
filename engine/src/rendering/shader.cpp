#include "shader.hpp"

#include <fstream>
#include <sstream>

#include "material.hpp"

// TODO: Make this more generic for other APIs?
#include <GL/glew.h>

namespace Silica {
    ShaderProgram::ShaderProgram(uint32_t program, std::string keywords) {
        handle = program;
        this->keywords = keywords;
    }

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

    uint32_t compile_source(const std::string& source_str, int version, bool isVertex) {
        uint32_t shader = glCreateShader(isVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

        auto version_str = std::string("#version ") + std::to_string(version);
        auto type_str = std::string("#define ") + (isVertex ? "VERT" : "FRAG") + "\n#define" + (isVertex ? "VERTEX" : "FRAGMENT") + "\n";

        const char *sources[3] = { version_str.c_str(), type_str.c_str(), source_str.c_str() };

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
    ShaderProgram* Shader::Compile(const std::vector<std::string>& keywords) {
        if (source.empty())
            throw std::runtime_error("Can't compile a shader with no source!");

        auto combo = combine_keywords(keywords);

        if (!analyzed)
            ProcessSource();

        // TODO: Geometry shaders?
        auto vert = compile_source(source, version, true);
        auto frag = compile_source(source, version, false);

        auto prog = link_program(vert, frag);

        glDeleteShader(vert);
        glDeleteShader(frag);

        auto res = new ShaderProgram(prog, combo);
        compiled_variants.emplace(combo, res);

        return res;
    }

    // This does one of two things, it either compiles the variant on the spot and returns it or it returns a cached representation
    ShaderProgram* Shader::GetVariant(const std::vector<std::string>& keywords) {
        auto combo = combine_keywords(keywords);

        auto variant = compiled_variants.find(combo);

        // Did we stumble across an invalidated slot? If so we try to fill it
        bool invalidated = false;
        if (variant != compiled_variants.end()) {
            invalidated = variant->second == nullptr;
            compiled_variants.erase(variant);
        }

        // The variant wasn't found, let's try to compile it!
        // If a variant fails to compile we fill its spot with a redirection to the engine's error shader
        if (variant == compiled_variants.end() || invalidated) {
            ShaderProgram* result = Compile(keywords);

            compiled_variants.emplace(combo, result);
            return result;
        }


        return variant->second;
    }

    void Shader::CreateEngineShaders() {
        auto err_shader = LoadCode(R"(#version 330 core
        #ifdef VERT
            layout(location = 0) in vec3 _vertex;

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

        err_shader->Compile();

        Material::error_material = new Material(err_shader);
    }

    Shader* Shader::error_shader = nullptr;
}