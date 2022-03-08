#include "shader.hpp"

#include <fstream>
#include <sstream>

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
        auto version_sec = source.find_first_of("\n");

        if (version_sec != std::string::npos) {
            auto version_line = source.substr(0, version_sec);

            for (char c : version_line)
                if (c != '\n')
                    break;
                    //TODO: CORRECT IT IF WE FIND ONE!

            int version_no = 0;
            sscanf(version_line.c_str(), "#version %i", &version_no);

            version = version_no;
        }

        analyzed = true;
    }

    std::string combine_keywords(const std::vector<std::string>& keywords) {
        std::stringstream keyword_buf;

        for (const auto& key : keywords)
            keyword_buf << key << " ";

        return keyword_buf.str();
    }

    // Compiles a variant with the following keywords then appends it to the list of variants
    // For performance reasons variant lookup doesn't use nested vectors!
    ShaderProgram* Shader::Compile(const std::vector<std::string>& keywords) {
        if (source.empty())
            throw std::runtime_error("Can't compile a shader with no source!");

        auto combo = combine_keywords(keywords);

        if (!analyzed)
            ProcessSource();

        return nullptr;
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
}